// This test exhibits the issue of parsing to boost::optional<ast::foo> node.
// It works well in previous version of spirit::x3.
//
// Duzy Chan, 2019-09-11, ExtBit Limited

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace extbit
{
  namespace ast
  {
    struct bar
    {
      int a;
      int b;
    };

    using foo = boost::optional<bar>;

    struct top
    {
      bar a;
      foo b;
      bar c;
    };
  }
}

BOOST_FUSION_ADAPT_STRUCT(extbit::ast::top, a, b, c)
BOOST_FUSION_ADAPT_STRUCT(extbit::ast::bar, a, b)

namespace extbit
{
  namespace parser
  {
    namespace x3 = boost::spirit::x3;

    const x3::rule<class top, ast::top> top = "top";
    const x3::rule<class foo, ast::foo> foo = "foo";
    using x3::int_;

    auto const top_def = foo >> -foo >> foo;
    auto const foo_def = int_ >> int_;

    BOOST_SPIRIT_DEFINE(top, foo);
  }
}

int main(int ac, char** av, char** ev)
{
  using boost::spirit::x3::ascii::space;

  extbit::ast::top node;
  extbit::parser::top foobar;

  std::string nothing; // nothing is really needed
  if (phrase_parse(nothing.begin(), nothing.end(), space, node)) {
    // ...
  }
}
