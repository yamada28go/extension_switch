#include <iostream>
#include <memory>

#include "extension_switch.h"

//! test2 dynamic type match
//! Grammar : 
//! match::dynamic< Basic type , Derived type > ( ***define lamba function*** )

namespace {

  //! three node struct
  struct tree { virtual ~tree(){}};
  struct empty : public tree{};
  struct leaf : public tree{};
  struct node : public tree
  {
    node( tree *l , tree *r )
      :l_(l),r_(r)
    {
    }

    std::unique_ptr< tree >  l_;
    std::unique_ptr< tree >  r_;

  };

  //! get three depth
  int depth( const tree & t )
  { 
    using namespace extension_switch;
    return _switch
      ( t ,
	match::dynamic<  empty , tree >( []( const empty & ref )
					 { 
					   std::cout << "empty" << std::endl;
					   return 0;
					 }),
	match::dynamic< leaf , tree  >( []( const leaf & ref )
					{ 
					  std::cout << "leaf" << std::endl;
					  return 1;
					}),
	match::dynamic< node , tree  >( []( const node & ref )
					{ 
					  std::cout << "node" << std::endl;
					  return 1 + std::max( depth(*ref.r_) , depth(*ref.l_) );
					})
	);
  }

}

int main()
{
  node n( new leaf() , new node( new leaf() ,  new node( new leaf() , new leaf() ) ) );
  std::cout << "depth is " << depth( n ) << std::endl;
}


