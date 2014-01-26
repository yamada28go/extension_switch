//
//--[extension_switch match regex]--
//

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

#ifndef _EXTENSION_SWITCH_MATCH_REGEX_
#define _EXTENSION_SWITCH_MATCH_REGEX_

//c++ default
#include <memory>

// boost 
#include <boost/regex.hpp>

//extension_switch
#include <extension_switch.h>

namespace extension_switch
{

  template < typename CONDITION , typename lamba_type >
    struct match_regex_holder
    {

    public:
	
      typedef CONDITION CONDITION_TYPE;
  
    private:

      CONDITION_TYPE c_;
      lamba_type l_;

      typedef std::basic_string< typename CONDITION_TYPE::value_type > REGEX_STRING_TYPE;
      typedef boost::match_results< typename REGEX_STRING_TYPE::const_iterator > MATCH_RESULTS_TYPE;
      //std::unique_ptr< MATCH_RESULTS_TYPE > match_ret_;
      MATCH_RESULTS_TYPE match_ret_;
  
    public:
	
    match_regex_holder( const CONDITION_TYPE & c , const lamba_type & l )
    : c_(c) , l_(l) 
      {}
 
      auto do_func( const boost::any & r ) -> decltype( l_( match_ret_ ) )
      {
      	if( false == match_ret_.empty() )
      	  {
      	    return l_( match_ret_ );
      	  }
      	throw std::runtime_error("Determination process has not been performed.");
      }

      template < typename JUDGMENT_TYPE >
      bool is_match( boost::any r , typename std::enable_if< std::is_same< JUDGMENT_TYPE , REGEX_STRING_TYPE  >::value >::type* = 0) 
      {
	const REGEX_STRING_TYPE & str = *(boost::any_cast< const REGEX_STRING_TYPE * >( r ));
	if( true ==  boost::regex_match( str , match_ret_ , c_ ) )
	  {
	    return true;
	  }
	return false;

      }

      template < typename JUDGMENT_TYPE >
      bool is_match( boost::any r , typename std::enable_if< !std::is_same< JUDGMENT_TYPE , REGEX_STRING_TYPE >::value >::type* = 0) 
      {
	return false;
      }

    };

  namespace match
  {
    template < typename CONDITION , typename lamba_type >
      match_regex_holder< CONDITION , lamba_type > regex( const CONDITION & cond , const lamba_type & t )
      {
	return match_regex_holder< CONDITION , lamba_type >( cond , t );
      }
  }

}

#endif // end of _EXTENSION_SWITCH_MATCH_REGEX_
