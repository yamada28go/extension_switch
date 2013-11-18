#include "stdafx.h"

//Copyright [2013] [yamada28go]
//
//Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0

//Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#define BOOST_TEST_MAIN    // Define main function
#include <boost/test/included/unit_test.hpp>

#include <vector>
#include <iostream>
#include <string>
#include <algorithm> 

#include "extension_switch.h"
#include "match_regex.h"

BOOST_AUTO_TEST_SUITE(extension_switch_test_suite)

namespace
{
	//! test1 match type , match data
	namespace case1{
		//test match
		template < typename T >
		std::string match_test(const T & t)
		{
			using namespace extension_switch;
			return _switch
				(t,
				match::type< const double >([](const double & ref) -> const char *
			{
				std::cout << "double:" << ref << std::endl;
				return "double";
			}),
				match::type< const char * >([](const char * ref) -> const char *
			{
				std::cout << "char:" << ref << std::endl;
				return "char";
			}),
				match::type< const std::string >([](const std::string & ref) -> const char *
			{
				std::cout << "string:" << ref << std::endl;
				return "string";
			}),
				match::data< const std::vector< int > >({ 1, 2, 3 },
				[](const  std::vector< int > & ref) -> const char *
			{
				std::cout << "vector 123" << std::endl;
				return "vector 123";
			}),
				match::type< const std::vector< int > >([](const  std::vector< int > & ref) -> const char *
			{
				std::cout << "vector" << std::endl;
				return "vector";
			}),
				match::type< const int >([](const int & ref) -> const char *
			{
				std::cout << "int:" << ref << std::endl;
				return "int";
			}),
				//default type
				match::other([](const boost::any & ref)
			{
				std::cout << "default type" << std::endl;;
				return "default";
			})
				);
		}


		BOOST_AUTO_TEST_CASE(test_case_1)
		{
			BOOST_CHECK_EQUAL("int", case1::match_test(1).c_str());
			BOOST_CHECK_EQUAL("double", case1::match_test(1.0).c_str());

			// Visual Studio 2013 can't compile this ocde.
			//{
			//	const char * s = "hoge";
			//	BOOST_CHECK_EQUAL("char", match_test(s).c_str());
			//}

			BOOST_CHECK_EQUAL("string", case1::match_test(std::string("string")).c_str());

			{
				const std::vector< int > t = { 3 };
				BOOST_CHECK_EQUAL("vector", case1::match_test(t).c_str());
			}

			{
				const std::vector< int > t = { 1, 2, 3 };
				BOOST_CHECK_EQUAL("vector 123", case1::match_test(t).c_str());
			}


			{
				struct tt{};
				tt t;
				BOOST_CHECK_EQUAL("default", case1::match_test(t).c_str());
			}
		}
	}

	//! test2 dynamic type match
	//! Grammar : 
	//! match::dynamic< Basic type , Derived type > ( ***define lamba function*** )
	//! three node struct
	namespace case2{


		struct tree { virtual ~tree(){} };
		struct empty : public tree{};
		struct leaf : public tree{};
		struct node : public tree
		{
			node(tree *l, tree *r)
			:l_(l), r_(r)
			{
			}

			std::unique_ptr< tree >  l_;
			std::unique_ptr< tree >  r_;

		};

		//! get three depth
		int depth(const tree & t)
		{
			using namespace extension_switch;
			return _switch
				(t,
				match::dynamic<  empty, tree >([](const empty & ref)
			{
				std::cout << "empty" << std::endl;
				return 0;
			}),
				match::dynamic< leaf, tree  >([](const leaf & ref)
			{
				std::cout << "leaf" << std::endl;
				return 1;
			}),
				match::dynamic< node, tree  >([](const node & ref)
			{
				std::cout << "node" << std::endl;
				return 1 + std::max<int>(depth(*ref.r_), depth(*ref.l_));
			}),
				//default type
				match::other([](const boost::any & ref)
			{
				std::cout << "default type";
				return 0;
			})
				);
		}

		BOOST_AUTO_TEST_CASE(test_case_2)
		{
			const node n(new leaf(), new node(new leaf(), new node(new leaf(), new leaf())));
			BOOST_CHECK_EQUAL(4, case2::depth(n));
		}
	}

	//! test3 : match regex
	namespace case3{


		enum result{
			digits_only,
			lower_case,
			upper_case,
			get_yesr_number,
			other
		};

		result verification(const std::string & line)
		{
			using namespace extension_switch;
			return _switch
				(line,
				match::regex(boost::regex("^[0-9]+$"),
				[](const boost::smatch & result)
			{
				std::cout << "This line is digits only." << std::endl;
				return result::digits_only;
			}),
				match::regex(boost::regex("^[a-z]+$"),
				[](const boost::smatch & result)
			{
				std::cout << "This line is lower case." << std::endl;
				return result::lower_case;
			}),
				match::regex(boost::regex("^[A-Z]+$"),
				[](const boost::smatch & result)
			{
				std::cout << "This line is upper case." << std::endl;
				return result::upper_case;
			}),
				match::regex(boost::regex("^This year is (\\d+).$"),
				[](const boost::smatch & result)
			{
				std::cout << "Year number is " << result.str(1) << "." << std::endl;
				return result::get_yesr_number;
			}),
				match::other([](const boost::any & ref)
			{
				std::cout << "default type" << std::endl;
				return result::other;
			})
				);
		}

		BOOST_AUTO_TEST_CASE(test_case_3)
		{
			BOOST_CHECK_EQUAL(case3::digits_only, case3::verification("123"));
			BOOST_CHECK_EQUAL(case3::lower_case, case3::verification("abc"));
			BOOST_CHECK_EQUAL(case3::upper_case, case3::verification("ABC"));
			BOOST_CHECK_EQUAL(case3::get_yesr_number, case3::verification("This year is 2013."));
			BOOST_CHECK_EQUAL(case3::other, case3::verification("hoge orz"));
		}

	}
	
}

BOOST_AUTO_TEST_SUITE_END()