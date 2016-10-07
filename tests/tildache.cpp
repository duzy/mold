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
#include <boost/mold/format/tildache/parser_def.hpp>
#include <map>
#include <sstream>
#include <iostream>

static auto TILDACHE_EXAMPLE = std::string
{
  R"***(//"

  ‘{{ ~ { immediate }}}’ <- ‘ immediate ’
  {{~  "yes" }} <- yes
  '{{~  !  "yes" }}' <- ''
  {{~  !  "" }} <- true
  {{~ + "-1" }} <- -1
  {{~ + "abc" }} <- 0
  {{~ + " 1" }} <- 1
  {{~ - " 2 " }} <- -2
  {{~ - "-1" }} <- 1
  {{~ "1" + "2" }} <- 3
  {{~ "2" - "3" }} <- -1
  {{~ "2" * "3" }} <- 6
  {{~ "3" / "2" }} <- 1
  {{~ "6" / "2" }} <- 3
  {{~ "3" / "0" }} <- NaN
  {{~ "foobar" }} <- foobar
  {{~ 'foobar' }} <- foobar
  {{~"yes":"foobar"}} <- foobar
  {{~ !'' : "foobar" }} <- foobar
  '{{~ '' : "nothing" }}' <- ''
  '{{~ "foobar" : _ }}' <- ''
  {{~ !""}} <- true
  {{~ "1".."3" }} <- 123
  {{~ "1".."4" : _ }} <- 123
  {{#Members}}{{Name}}{{~ !last : {, }}}{{/Members}}
  {{#Members}}
  {{Name}}
  {{/Members}}
  {{~ "1".."3" ~}}
  Item #{{a}}{{~_}}
  {{~}}
  -
  )***" + 4
};//";

static auto TILDACHE_EXAMPLE_EXPECT = std::string
{
  R"***(//"

  ‘ immediate ’ <- ‘ immediate ’
  yes <- yes
  '' <- ''
  true <- true
  -1 <- -1
  0 <- 0
  1 <- 1
  -2 <- -2
  1 <- 1
  3 <- 3
  -1 <- -1
  6 <- 6
  1 <- 1
  3 <- 3
  NaN <- NaN
  foobar <- foobar
  foobar <- foobar
  foobar <- foobar
  foobar <- foobar
  '' <- ''
  '' <- ''
  true <- true
  123 <- 123
  123 <- 123
  Foo,   Bar,   Foobar
  Foo
  Bar
  Foobar
  Item #1
  Item #2
  Item #3
  -
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
  std::cout << ss.str() << std::endl;
  std::cout << "-------------------------" << std::endl;
  std::cout << TILDACHE_EXAMPLE_EXPECT << std::endl;
  std::cout << "-------------------------" << std::endl;
#endif
  
  return boost::report_errors();
}
