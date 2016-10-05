/**
 *  \file tildache.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//#define BOOST_SPIRIT_X3_DEBUG
#include <boost/core/lightweight_test.hpp>
#include <boost/mold/mold.hpp>
#include <boost/mold/format/tildache.hpp>
#include <map>
#include <sstream>
#include <iostream>

static auto TILDACHE_EXAMPLE = std::string
{
  R"***(//"

  {{~  !  "yes" }}
  {{ ~ { , }}}
  {{~ "foobar" }}
  {{~ 'foobar' }}
  {{~ + "1" }}
  {{~ "1" + "2" }}
  {{~"yes":"foobar"}}
  {{~ "foobar" : _ }}
  {{~ 'foobar' : _ }}
  {{~ !""}}
  {{~ "1".."3" : _ }}
  {{#Members}}{{Name}}{{~ !last : {, }}}{{/Members}}
  {{~ "1".."3" ~}}
  Item #{{~_}}
  {{~}}
  
  )***" + 4
};//";

static auto TILDACHE_EXAMPLE_EXPECT = std::string
{
  R"***(//"

   , 
  foobar
  foobar
  Foo, Bar, Foobar
  123
  Item #1
  Item #2
  Item #3
  
  )***" + 4
};//";

namespace mold = boost::mold;
  
int main(int argc, char**argv)
{
  mold::object context
  {
    { "Members", mold::array{
        mold::object{{ "Name", "Foo" }},
        mold::object{{ "Name", "Bar" }},
        mold::object{{ "Name", "Foobar" }},
        }
    },
  };
  
  std::stringstream ss;
  auto const &s = TILDACHE_EXAMPLE;
  auto i = s.begin();
  auto t = mold::load<mold::format::tildache_spec>(i, s.end());
  mold::generate(ss, t, context);

  BOOST_TEST(i == s.end());
  BOOST_TEST(ss.str() == TILDACHE_EXAMPLE_EXPECT);

#if 1
  std::cout << "-------------------------" << std::endl;
  std::cout << &(*i) << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << ss.str() << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << TILDACHE_EXAMPLE_EXPECT << std::endl;
  std::cout << "-------------------------" << std::endl;
#endif
  
  return boost::report_errors();
}
