**extension_switch**


extension_switch is template library that extend the C++ 11's switch.
This library is able to enable the match type , match data , match types in the inheritance relationship.
( Match regular expressions is under development. )
Simple sample are as follows.

      using namespace extension_switch;
      const auto a = _switch
	( t , 
	  match::type< const double >( [](const double & ref) -> const char *
	    { 
	      std::cout << "double:" << ref << std::endl;
	      return "double";
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
	  );

**Dependency**

extension_switch is dependent on boost::any library.

**How to use **

See test directory.

**License**

Apache License Version 2.0

**Development environment**

OS  : Free bsd 9.0 on AMD64
Compiler : clang version 3.1 C++11 mode

