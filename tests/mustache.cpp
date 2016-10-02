/**
 *  \file example.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
//#define BOOST_SPIRIT_X3_DEBUG
#include <boost/core/lightweight_test.hpp>
#include <boost/mold/mold.hpp>
#include <boost/mold/format/mustache.hpp>
#include <boost/mold/format/mustache/parser_def.hpp>
#include <map>
#include <sstream>
#include <iostream>

static auto MUSTACHE_EXAMPLE = std::string
{
  R"***(//"

  namespace {{&Namespace}}
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

  namespace Example
  {
    class Foo;
    class Bar;
    
    class Bar;
    class Foo;
  }

  )***" + 4
};//";

namespace
{
  struct value;
  using object = std::map<std::string, value>;
  using list = std::vector<value>;
  struct value : boost::spirit::x3::variant<std::string, object, list>
  {
    value();
    value(const char *s) : base_type(std::string{s}) {}
    value(const std::string &s) : base_type(s) {}
    value(const object &o) : base_type(o) {}
    value(const list &l) : base_type(l) {}
  };
}

//BOOST_MOLD_MUSTACHE_INSTANTIATE(std::string::iterator, object)

namespace mold = boost::mold;
  
int main(int argc, char**argv)
{
  //{ mold::format::mustache::ast::identifier id("test"); }
  
  std::stringstream ss;
  auto const &s = MUSTACHE_EXAMPLE;
  auto i = s.begin();
  auto t = mold::load<mold::format::mustache_spec>(i, s.end());
  mold::generate(ss, t, object{
      { "Namespace", "Example" },
      { "Classes", list{
          object{{ "Name", "Foo" }},
          object{{ "Name", "Bar" }},
      }},
  });

  BOOST_TEST(i == s.end());
  BOOST_TEST(ss.str() == MUSTACHE_EXAMPLE_EXPECT);

  std::cout << "-------------------------" << std::endl;
  std::cout << ss.str() << std::endl;
  return boost::report_errors();
}
