//
//--[extension_switch]--
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

#ifndef _EXTENSION_SWITCH_
#define _EXTENSION_SWITCH_

#include <typeinfo>
#include <stdexcept>
#include <type_traits>

#include <boost/any.hpp>

//デバッグモードを有効かするための定義
//#define __EXTENSION_SWITCH_DEBUG_MODE__

#ifdef __EXTENSION_SWITCH_DEBUG_MODE__

//デバッグログ出力マクロ
#include <cxxabi.h>
#include <iostream>
#include <boost/current_function.hpp>
#include <boost/format.hpp>

#endif  


namespace extension_switch
{

  //デバッグモード
#ifdef __EXTENSION_SWITCH_DEBUG_MODE__

  //! 型名を取得する
  template< typename DST_TYPE >
    std::string get_demangled_type_name (void)
    {
      DST_TYPE *dst = nullptr;
      int status;
      std::unique_ptr<char> ret
	( ( char * )(abi::__cxa_demangle (typeid ( *dst ).name (), 0, 0, &status) ));
      if ( nullptr != ret.get() )
	{
	  return std::string( ret.get() );
	}
      throw std::runtime_error(" demangle was fail ");
    }

  //ロギング用マクロ  
#define EXTENSION_SWITCH_LOG( __MSG__ )					\
  do{									\
    try									\
      {									\
	std::cout << std::endl;						\
	std::cout << ( boost::format( "%|| , %|| , %|| , msg [ %|| ] " ) \
		       % (__FILE__)					\
		       % (__LINE__)					\
		       % (BOOST_CURRENT_FUNCTION)			\
		       % (__MSG__)					\
		       ).str() << std::endl ;				\
      }									\
    catch( const std::exception & e )					\
      {									\
	std::cout << std::endl;						\
	std::cout << (__FILE__) << " , "				\
		  << (__LINE__) << " , "				\
		  << (BOOST_CURRENT_FUNCTION) << " , "			\
		  << "Logging failed , what [ " << e.what() << " ] "	\
		  << std::endl;						\
      }									\
  }while(false)								\
    
#else
  
#define EXTENSION_SWITCH_LOG( __MSG__ )	  
  
#endif  

    template < typename lamba_type >
      struct other_type_holder
      {
      private:
	lamba_type l_;

      public:  

      other_type_holder( const lamba_type & l )
      :l_(l)
	{}

	auto do_other_func( const boost::any & r ) -> decltype( l_( r  )  ) 
	{
	  return l_( r );
	}
  
      };

  
    template < typename CONDITION , typename lamba_type >
      struct match_type_holder
      {
      private:
	lamba_type l_;

      public:  
	typedef CONDITION CONDITION_TYPE;

      match_type_holder( const lamba_type & l )
      :l_(l)
	{}

	auto do_func( const boost::any & r ) -> decltype( l_( boost::any_cast< CONDITION_TYPE >( r ) )  ) 
	{
	  return l_( *boost::any_cast< CONDITION_TYPE *>( r ) );
	}

	template < typename JUDGMENT_TYPE >
	bool is_match( boost::any r )
	{
	  (void)(r);

	  if( typeid( CONDITION_TYPE ) == typeid( JUDGMENT_TYPE ) )
	    {
	      return true;
	    }
	  return false;
	}
  
      };

    template < typename CONDITION , typename lamba_type >
      struct match_data_holder
      {
      public:

	typedef CONDITION CONDITION_TYPE;
  
      private:
	CONDITION_TYPE c_;
	lamba_type l_;
  
      public:
      match_data_holder( const CONDITION_TYPE & c , const lamba_type & l )
      : c_(c) , l_(l)
	{}

	auto do_func( const boost::any & r ) -> decltype( l_( boost::any_cast< CONDITION_TYPE >( r ) ) ) 
	{
	  return l_( *boost::any_cast< CONDITION_TYPE *>( r ) );
	}

	typedef typename std::remove_const< CONDITION_TYPE >::type RC_CONDITION_TYPE;

	template < typename JUDGMENT_TYPE >
	bool is_match( boost::any r , typename std::enable_if< std::is_same< JUDGMENT_TYPE , RC_CONDITION_TYPE  >::value >::type* = 0) const
	{
	  if( typeid( CONDITION_TYPE ) == typeid( JUDGMENT_TYPE ) )
	    {
	      if( *boost::any_cast< CONDITION_TYPE *>( r ) == c_ )
		{
		  return true;
		}
	    }
	  return false;
	}

	template < typename JUDGMENT_TYPE >
	bool is_match( boost::any r , typename std::enable_if< !std::is_same< JUDGMENT_TYPE , RC_CONDITION_TYPE >::value >::type* = 0) const 
	{
	  return false;
	}

      };

    template < typename OBJECT_TYPE , typename CONDITION , typename lamba_type >
      struct match_dynamic_holder
      {

      public:

	typedef CONDITION CONDITION_TYPE;
  
      private:
	lamba_type l_;
  
      public:
      match_dynamic_holder( const lamba_type & l )
      :  l_(l)
	{}

	auto do_func( const boost::any & r ) -> decltype( l_( dynamic_cast< const OBJECT_TYPE & >( *boost::any_cast< const CONDITION_TYPE * >( r ) ) ) ) 
	{
	  return l_( dynamic_cast< const OBJECT_TYPE & >( *boost::any_cast< const CONDITION_TYPE * >( r )  ) );
	}


	template < typename JUDGMENT_TYPE >
	bool is_match( boost::any r , typename std::enable_if< std::is_base_of< JUDGMENT_TYPE , OBJECT_TYPE >::value >::type* = 0) const 
	{
	  try
	    {

	      //ログを保存
	      //以下関数を使うと例外が発生するのでここでは呼ばない
	      //get_demangled_type_name< JUDGMENT_TYPE >()
	      //% get_demangled_type_name< OBJECT_TYPE >()
	      EXTENSION_SWITCH_LOG( "judgment target! " );
	      
	      const JUDGMENT_TYPE * t = boost::any_cast< const JUDGMENT_TYPE * >( r );
	      const OBJECT_TYPE & ot = dynamic_cast< const OBJECT_TYPE & >(*t);
	      (void)(ot);
	      return true;
	      
	    }
	  catch( const std::exception & e )
	    {
	      EXTENSION_SWITCH_LOG
		( ( boost::format( "Cast failed , what : %||" )  
		    % e.what() ).str() );
	    }

	  return false;
	}

	template < typename JUDGMENT_TYPE >
	bool is_match( boost::any r , typename std::enable_if< !std::is_base_of< JUDGMENT_TYPE , OBJECT_TYPE >::value >::type* = 0) const 
	{

	  //ログを保存
	  EXTENSION_SWITCH_LOG
	    ( ( boost::format( "not judgment target! , JUDGMENT_TYPE : %|| , CONDITION : %||" )  
		% get_demangled_type_name< JUDGMENT_TYPE >()
		% get_demangled_type_name< CONDITION >() ).str() );
	  return false;
	}

      };

  namespace match
  {

    template < typename T >
      other_type_holder< T > other( const T & t )
      {
	return std::move( other_type_holder< T >( t ) );
      }
    
    template < typename CONDITION , typename T >
      match_type_holder< CONDITION , T > type( const T & t )
      {
	return std::move( match_type_holder< CONDITION , T >( t ) );
      }
    
    template < typename CONDITION , typename T >
      match_data_holder< CONDITION , T > data( const CONDITION & cond , const T & t )
      {
	return std::move( match_data_holder< CONDITION , T >( cond , t ) );
      }
    
    template < typename OBJECT_TYPE , typename CONDITION , typename lamba_type >
      match_dynamic_holder< OBJECT_TYPE , CONDITION , lamba_type > dynamic( const lamba_type & t )
      {
	return std::move( match_dynamic_holder< OBJECT_TYPE , CONDITION , lamba_type >( t ) );
      }
  }


  //! match default type
  template < typename COND , typename LAMBDA_T >
    auto _switch( const COND & c , LAMBDA_T t ) -> decltype( t.do_other_func( c ) ) 
  {
    return t.do_other_func( &c );
  }

  template < typename COND , typename LAMBDA_T , typename... Args >
    auto _switch( const COND & c , LAMBDA_T t , Args... args  ) -> decltype( _switch( c , args... ) ) 
  {

    if( true == t.LAMBDA_T::template is_match< COND >( &c ) )
      {
	return t.do_func( &c );
      }
    else
      {
	return _switch( c , args... );
      }
  }

}

#endif
