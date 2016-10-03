/**
 *  \file example.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#define BOOST_SPIRIT_X3_DEBUG
#include <boost/core/lightweight_test.hpp>
#include <boost/mold/load.hpp>
#include <boost/mold/generate.hpp>
#include <boost/mold/format/mustache.hpp>
#include <boost/mold/format/mustache/parser_def.hpp>
#include <map>
#include <sstream>
#include <iostream>

static auto MUSTACHE_EXAMPLE = std::string
{
  R"***(//"
  
  // blah blah blah 
  // blah blah blah  
  // blah blah blah   
  
  namespace {{ &Namespace }} // {{{HTML}}} {{&HTML}}
  {
    {{#Classes}}
    class {{Name}};
    {{/Classes}}
    
    {{^Classes}}
    class {{{Name}}};
    {{/Classes}}
  }
  
  )***" + 4
};//";

static auto MUSTACHE_EXAMPLE_EXPECT = std::string
{
  R"***(//"
  
  namespace Example // &lt;b&gt;bold&lt;/b&gt; &lt;b&gt;bold&lt;/b&gt;
  {
    class Foo;
    class Bar;
    
    class Bar;
    class Foo;
  }
  
  )***" + 4
};//";

//BOOST_MOLD_MUSTACHE_INSTANTIATE(std::string::iterator, object)

namespace mold = boost::mold;
  
int main(int argc, char**argv)
{
  //{ mold::format::mustache::ast::identifier id("test"); }

  mold::object context
  {
    { "HTML", "<b>bold</b>" },
    { "Namespace", "Example" },
    { "Classes", mold::array{
        mold::object{{ "Name", "Foo" }},
        mold::object{{ "Name", "Bar" }},
        }
    },
  };
  
  std::stringstream ss;
  auto const &s = MUSTACHE_EXAMPLE;
  auto i = s.begin();
  auto t = mold::load<mold::format::mustache_spec>(i, s.end());
  mold::generate(ss, t, context);

  BOOST_TEST(i == s.end());
  BOOST_TEST(ss.str() == MUSTACHE_EXAMPLE_EXPECT);

  std::cout << "-------------------------" << std::endl;
  std::cout << ss.str() << std::endl;
  return boost::report_errors();
}
