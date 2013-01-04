**extension_switch**


extension_switch is template library that extend the C++ 11's switch.  
This library is able to enable the match type , match data , match types , match regular expressions in the inheritance relationship.  
Simple sample are as follows.

    const std::string line = "hoge".
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

**Dependency**

extension_switch is dependent on boost::any library.

**How to use**

See test directory.
(Simple sample is there.)

**License**

Apache License Version 2.0

**Development environment**

OS  : Free bsd 9.0 on AMD64
Compiler : clang version 3.1 C++11 mode

