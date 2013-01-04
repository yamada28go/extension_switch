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

#include <iostream>

#include "extension_switch.h"
#include "match_regex.h"

//! test3 : match regex

namespace {

  void verification( const std::string & line )
  { 
    using namespace extension_switch;
    _switch
      ( line ,
	match::regex(boost::regex( "^[0-9]+$" ), 
		     []( const boost::smatch & result )
		     { 
		       std::cout << "This line is digits only." << std::endl;
		     }) ,
	match::regex( boost::regex("^[a-z]+$") , 
		      []( const boost::smatch & result )
		      { 
			std::cout << "This line is lower case." << std::endl;
		      }),
	match::regex( boost::regex("^[A-Z]+$") , 
		      []( const boost::smatch & result )
		      { 
			std::cout << "This line is upper case." << std::endl;
		      }),
	match::regex( boost::regex("^This year is (\\d+).$") , 
		      []( const boost::smatch & result )
		      { 
			std::cout << "Year number is " << result.str(1) << "."  << std::endl;
		      }),
	match::other( []( const boost::any & ref)
		      {
			std::cout << "default type" << std::endl;
		      })
	);
  }
}

int main()
{
  try{
    const std::vector< std:: string > line = 
      {"123",
       "abc",
       "ABC",
       "This year is 2013.",
       "1234 hoge orz"
      };

    for(const auto & l : line)
      {
	std::cout << "src  : " << l  << std::endl;
	verification(l);
      }
  }
  catch( const std::exception & e)
    {
      std::cout << e.what() << std::endl;
    }
}


