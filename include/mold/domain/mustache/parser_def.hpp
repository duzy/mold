/**
 *  \file boost/mold/domain/mustache/grammar.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@extbit.io>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_MUSTACHE_PARSER_DEF_HPP_
#define _BOOST_MOLD_DOMAIN_MUSTACHE_PARSER_DEF_HPP_ 1
#include <mold/domain/mustache/parser.hpp>
/*
#include <boost/spirit/home/x3/auxiliary.hpp>
#include <boost/spirit/home/x3/char/char_class.hpp>
#include <boost/spirit/home/x3/char/char.hpp>
#include <boost/spirit/home/x3/directive/no_skip.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/operator.hpp>
*/
#include <boost/spirit/home/x3.hpp>
#include <list>
namespace mold { namespace domain { namespace mustache 
{
  namespace parser // http://mustache.github.io/mustache.5.html
  {
    using x3::_attr;
    using x3::_pass;
    using x3::_val;
    using x3::alnum;
    using x3::alpha;
    using x3::attr;
    using x3::blank;
    using x3::char_;
    using x3::eoi;
    using x3::eol;
    using x3::eps;
    using x3::lexeme;
    using x3::lit;
    using x3::matches;
    using x3::no_skip;
    using x3::omit;
    using x3::skip;
    using x3::space;
    using x3::space_type;
    using x3::unused;
    
    using node_list_type = x3::rule<struct node_list_class, ast::node_list>;
    using node_type = x3::rule<struct node_class, ast::node>;
    using identifier_type = x3::rule<struct identifier_class, ast::identifier>;
    using comment_type = x3::rule<struct comment_class, ast::comment>;
    using literal_text_type = x3::rule<struct literal_text_class, ast::literal_text>;
    using blank_text_type = x3::rule<struct blank_text_class, ast::blank_text>;
    using end_of_line_type = x3::rule<struct end_of_line_class, ast::eol>;
    using variable_type = x3::rule<struct variable_class, ast::variable>;
    using variable_unescaped_type = x3::rule<struct variable_unescaped_class, ast::variable>;
    using section_type = x3::rule<struct section_class, ast::section>;
    using section_begin_type = x3::rule<struct section_begin_class, ast::identifier>;
    using section_end_type = x3::rule<struct section_end_class, ast::identifier>;
    using partial_type = x3::rule<struct partial_class, ast::partial>;
    
    const mustache_type spec = "mustache";
    const node_list_type node_list = "node_list";
    const node_type node = "node";
    const identifier_type identifier = "identifier";
    const comment_type comment = "comment";
    const literal_text_type literal_text = "literal_text";
    const blank_text_type blank_text = "blank_text";
    const end_of_line_type end_of_line = "end_of_line";
    const variable_type variable = "variable";
    const variable_unescaped_type variable_unescaped = "variable_unescaped";
    const section_type section = "section";
    const section_begin_type section_begin = "section_begin";
    const section_end_type section_end = "section_end";
    const partial_type partial = "partial";

    auto const spec_def =
      *node >> attr(ast::eoi{})
      ;
    
    auto const node_def =
         end_of_line
      |  blank_text
      |  no_skip[literal_text]
      |  comment
      |  variable
      |  variable_unescaped
      |  section
      |  partial
      ;

    auto const node_list_def =
      *node
      ;

    auto const literal_text_def =
      +(char_ - (eol|"{{"))
      ;

    auto const blank_text_def =
      no_skip[+blank]
      ;

#if 0
    auto const end_of_line_def =
      char_('\n')
      >> ( (char_("\r") >> attr(ast::eol{"\n\r"}))
         | attr(ast::eol{"\n"}) )
      ;
#else
    auto const end_of_line_def =
      char_('\n') >> -char_("\r")
      ;
#endif

    auto const comment_def =
      lit("{{")
      >> '!'
      >> omit[*(char_ - "}}")]
      >> "}}"
      ;

    auto const identifier_def =
      lexeme[alpha >> *(alnum | char_('_'))]
      ;

    auto const variable_def =
      lit("{{")
      >> skip(space)
      [
       matches['&']
       >> identifier
       >> "}}"
      ]
      ;

    auto const variable_unescaped_def =
      lit("{{{")
      >> skip(space)
      [
       attr(true)
       >> identifier
       >> "}}}"
      ]
      ;

    auto set_tag = [](auto &c){ _val(c).name = _attr(c); };
    auto see_tag = [](auto &c){ _pass(c) = _attr(c) == _val(c).name; };
    auto const section_def =
      matches[&(lit("{{") >> '^')][( [](auto &c){ _val(c).inverted = _attr(c); } )]
      >> section_begin [ set_tag ]
      >> node_list     [( [](auto &c){ _val(c).nodes = _attr(c); } )]
      >> section_end   [ see_tag ]
      ;

    auto const section_begin_def =
      lit("{{")
      >> skip(space)
      [
      (lit('#') | '^')
      >> identifier
      >> "}}"
      //>> omit[ no_skip[ (*char_(" ") >> eol) ] ]
      ]
      ;

    auto const section_end_def =
      lit("{{")
      >> skip(space)
      [
       '/'
       >> identifier
       >> lit("}}")
      ]
      //>> omit[ no_skip[ (*char_(" ") >> eol) ] ]
      ;

    auto const partial_def =
      lit("{{")
      >> skip(space)
      [
       '>'
       >> identifier
       >> "}}"
      ]
      ;

    BOOST_SPIRIT_DEFINE(
        spec,
        node_list,
        node,
        identifier,
        comment,
        literal_text,
        blank_text,
        end_of_line,
        variable,
        variable_unescaped,
        section,
        section_begin,
        section_end,
        partial
    )
  } // namespace parser
  
  const parser::mustache_type &spec()
  {
    return parser::spec;
  }

}}} // namespace mold::domain::mustache
#endif//_BOOST_MOLD_DOMAIN_MUSTACHE_PARSER_DEF_HPP_
