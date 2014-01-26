//
//--[extension_switch match xpressive regex]--
//

//Copyright [2014] [yamada28go]
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

#ifndef _EXTENSION_SWITCH_MATCH_XPRESSIVE_REGEX_
#define _EXTENSION_SWITCH_MATCH_XPRESSIVE_REGEX_

//c++ default
#include <memory>

// boost 
#include <boost/xpressive/xpressive.hpp>

//extension_switch
#include "extension_switch.h"


namespace extension_switch
{
  namespace xpressive
  {
    //! 正規表現判定対象文字型
    namespace target
    {
      //! std::string型をマッチターゲットとする
      struct string
      {
	typedef std::string TARGET_STRING_VALUE_T;
	typedef boost::xpressive::smatch MATCH_T;
	
	//データ判定を行う
	template< typename T_REGEX >
	static	
	bool is_match( const boost::any & r , const T_REGEX & c_
		       , MATCH_T & match_ret_ )
	{
	  if( true ==  boost::xpressive::regex_match
	      (  *( boost::any_cast< const TARGET_STRING_VALUE_T * >( r ) ) , match_ret_ , c_ ) )
	    {
	      return true;
	    }
	  return false;
	}

      };
      
      //const char型
      struct c_char
      {
	typedef char const * TARGET_STRING_VALUE_T;
	typedef boost::xpressive::cmatch MATCH_T;

	template< typename T_REGEX >
	static 
	bool is_match( const boost::any & r , const T_REGEX  & c_
		       , MATCH_T & match_ret_ )
	{
	  if( true ==  boost::xpressive::regex_match
	      (  *( boost::any_cast< const TARGET_STRING_VALUE_T * >( r ) ) , match_ret_ , c_  ) )
	    {
	      return true;
	    }
	  return false;
	}
	
      };

    }

    //xpressive正規表現　情報保持オブジェクト
    template < typename MATCH_TARGET , typename CONDITION , typename lamba_type >
      struct match_xpressive_regex_holder
      {

      public:
	
	typedef CONDITION CONDITION_TYPE;
  
      private:

	CONDITION_TYPE c_;
	lamba_type l_;

	typedef typename MATCH_TARGET::TARGET_STRING_VALUE_T REGEX_STRING_TYPE;
	typedef typename MATCH_TARGET::MATCH_T MATCH_RESULTS_TYPE;

	MATCH_RESULTS_TYPE match_ret_;
  
      public:
	
      match_xpressive_regex_holder( const CONDITION_TYPE & c , const lamba_type & l )
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
	bool is_match
	( boost::any r ,
	  typename std::enable_if< std::is_same< JUDGMENT_TYPE , REGEX_STRING_TYPE  >::value >::type* = 0) 
	{
	  //ログを保存
	  EXTENSION_SWITCH_LOG
	    ( ( boost::format( "judgment target! , JUDGMENT_TYPE : %|| , REGEX_STRING_TYPE : %||" )  
		% get_demangled_type_name< JUDGMENT_TYPE >()
		% get_demangled_type_name< REGEX_STRING_TYPE >() ).str() );

	  //判定処理を実施。
	  //正規表現判定対象の文字型により、判定処理が異なるため外部に委任する
	  return MATCH_TARGET::template is_match( r , c_ ,  match_ret_ );

	}

	template < typename JUDGMENT_TYPE >
	bool is_match
	( boost::any r ,
	  typename std::enable_if< !std::is_same< JUDGMENT_TYPE , REGEX_STRING_TYPE >::value >::type* = 0 ) 
	{
	  //ログを保存
	  EXTENSION_SWITCH_LOG
	    ( ( boost::format( "not judgment target! , JUDGMENT_TYPE : %|| , REGEX_STRING_TYPE : %||" )  
		% get_demangled_type_name< JUDGMENT_TYPE >()
		% get_demangled_type_name< REGEX_STRING_TYPE >() ).str() );
	  
	  //正規表現判定対象の型では無いのでfalseを返す
	  return false;
	}

      };

    namespace match
    {

      //正規表現判定ホルダを生成する
      template < typename MATCH_TARGET , typename CONDITION , typename lamba_type >
	extension_switch::xpressive::match_xpressive_regex_holder
	< MATCH_TARGET , CONDITION , lamba_type > regex
	( const CONDITION & cond , const lamba_type & t )
	{
	  return extension_switch::xpressive::match_xpressive_regex_holder
	    < MATCH_TARGET , CONDITION , lamba_type >( cond , t );
	}

    }

  }

}

#endif // end of _EXTENSION_SWITCH_MATCH_XPRESSIVE_REGEX_
