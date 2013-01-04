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
					}),
	//default type
	match::other( []( const boost::any & ref)
		      {
			std::cout << "default type";
			return 0;
		      })
	);
  }

}

int main()
{
  const node n( new leaf() , new node( new leaf() ,  new node( new leaf() , new leaf() ) ) );

  while(true)
    {
      depth( n );
    }
  
  std::cout << "depth is " << depth( n ) << std::endl;
}


