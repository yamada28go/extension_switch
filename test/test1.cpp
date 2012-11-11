#include <iostream>
#include <vector>

#include "extension_switch.h"

template < typename T >
void match_test( const T & t )
{
  try
    {
      using namespace extension_switch;
      const auto a = _switch
	( t , 
	  match::type< const double >( [](const double & ref) -> const char *
	    { 
	      std::cout << "double:" << ref << std::endl;
	      return "double";
	    }),
	  match::type< const char * >( [](const char * ref) -> const char * 
	    { 
	      std::cout << "char:" << ref << std::endl;
	      return "char";
	    }),
	  match::type< const std::string >( [](const std::string & ref) -> const char * 
	    { 
	      std::cout << "string:" << ref << std::endl;
	      return "string";
	    }),
	  match::data< const std::vector< int > >( {1,2,3} ,
						   []( const  std::vector< int > & ref ) -> const char *
	    { 
	      std::cout << "vector 123" << std::endl;
	      return "vector 123";
	    }),
	  match::type< const int >( []( const int & ref ) -> const char *
	    { 
	      std::cout << "int:" << ref << std::endl;
	      return "int";
	    })
	  );

      std::cout << a << std::endl;

    }
  catch( const std::exception & e )
    {
      std::cout << e.what() << std::endl;
    }
}


int main()
{

  match_test( 10 );
  match_test( 10.0 );
  match_test( std::string("hoge") );
  
  {
    const std::vector< int > t = {1,2,3};
    match_test(t);
  }

  {
    const std::vector< int > t = {2,3};
    match_test(t);
  }

}


