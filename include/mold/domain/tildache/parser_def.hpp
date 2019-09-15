/**
 *  \file boost/mold/domain/tildache/parser_def.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@extbit.io>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_TILDACHE_PARSER_DEF_HPP_
#define _BOOST_MOLD_DOMAIN_TILDACHE_PARSER_DEF_HPP_ 1
#include <mold/domain/tildache/parser.hpp>
#include <mold/domain/mustache/parser_def.hpp>
namespace mold { namespace domain { namespace tildache
{
  namespace parser
  {
    using x3::_attr;
    using x3::_pass;
    using x3::_val;
    using x3::alnum;
    using x3::alpha;
    using x3::attr;
    using x3::blank;
    using x3::bool_;
    using x3::char_;
    using x3::confix;
    using x3::eoi;
    using x3::eol;
    using x3::eps;
    using x3::expect;
    using x3::lexeme;
    using x3::lit;
    using x3::matches;
    using x3::no_skip;
    using x3::omit;
    using x3::raw;
    using x3::skip;
    using x3::space;
    using x3::space_type;
    using x3::symbols;
    using x3::uint_;
    using x3::unused;
    using x3::with;
    using x3::skipper_tag;
    
    using node_list_type = x3::rule<struct node_list_class, ast::node_list>;
    using node_type = x3::rule<struct node_class, ast::node>;
    using tild_expr_type = x3::rule<struct tild_class, ast::tild_expr>;
    using tild_once_section_type = x3::rule<struct tild_once_section_class, ast::tild_once_section>;
    using tild_each_section_type = x3::rule<struct tild_each_section_class, ast::tild_each_section>;
    using tild_see_section_type = x3::rule<struct tild_see_section_class, ast::tild_see_section>;
    using tild_see_case_type = x3::rule<struct tild_see_case_class, ast::tild_expr_case>;
    using tild_once_case_type = x3::rule<struct tild_once_case_class, ast::tild_expr_case>;
    using tild_each_case_type = x3::rule<struct tild_each_case_class, ast::tild_expr_case>;
    using tild_expr_case_type = x3::rule<struct tild_expr_case_class, ast::tild_expr_case>;
    using tild_else_case_type = x3::rule<struct tild_else_case_class, ast::tild_else_case>;
    using mustache_section_type = x3::rule<struct mustache_section_class, ast::mustache_section>;
    using expression_type = x3::rule<struct expression_class, ast::expression>;
    using list_expr_type = x3::rule<struct list_expr_class, ast::expression>;
    using infix_expr_type = x3::rule<struct infix_expr_class, ast::expression>;
    using equality_expr_type = x3::rule<struct equality_expr_class, ast::expression>;
    using relational_expr_type = x3::rule<struct relational_expr_class, ast::expression>;
    using logical_expr_type = x3::rule<struct logical_expr_class, ast::expression>;
    using additive_expr_type = x3::rule<struct additive_expr_class, ast::expression>;
    using multiplicative_expr_type = x3::rule<struct multiplicative_expr_class, ast::expression>;
    using unary_expr_type = x3::rule<struct unary_expr_class, ast::operand>;
    using primary_expr_type = x3::rule<struct primary_expr_class, ast::operand>;
    using identifier_type = x3::rule<struct identifier_class, ast::identifier>;
    using emit_text_type = x3::rule<struct emit_text_class, ast::emit_text>;
    using single_quoted_type = x3::rule<struct single_quoted_class, std::string>;
    using double_quoted_type = x3::rule<struct double_quoted_class, std::string>;
    
    const tildache_type spec = "tildache";
    const tild_expr_type tild_expr = "tild_expr";
    const tild_once_section_type tild_once_section = "tild_once_section";
    const tild_each_section_type tild_each_section = "tild_each_section";
    const tild_see_section_type tild_see_section = "tild_see_section";
    const tild_see_case_type tild_see_case = "tild_see_case";
    const tild_once_case_type tild_once_case = "tild_once_case";
    const tild_each_case_type tild_each_case = "tild_each_case";
    const tild_expr_case_type tild_expr_case = "tild_expr_case";
    const tild_else_case_type tild_else_case = "tild_else_case";
    const mustache_section_type mustache_section = "mustache_section";
    const node_type node = "node";
    const node_list_type node_list = "node_list";
    const expression_type expression = "expression";
    const list_expr_type list_expr = "list_expr";
    const infix_expr_type infix_expr = "infix_expr";
    const equality_expr_type equality_expr = "equality_expr";
    const relational_expr_type relational_expr = "relational_expr";
    const logical_expr_type logical_expr = "logical_expr";
    const additive_expr_type additive_expr = "additive_expr";
    const multiplicative_expr_type multiplicative_expr = "multiplicative_expr";
    const unary_expr_type unary_expr = "unary_expr";
    const primary_expr_type primary_expr = "primary_expr";
    const identifier_type identifier = "identifier";
    const emit_text_type emit_text = "emit_text";
    const single_quoted_type single_quoted = "single_quoted";
    const double_quoted_type double_quoted = "double_quoted";

    symbols<ast::optoken> list_op;
    symbols<ast::optoken> infix_op;
    symbols<ast::optoken> equality_op;
    symbols<ast::optoken> relational_op;
    symbols<ast::optoken> logical_op;
    symbols<ast::optoken> additive_op;
    symbols<ast::optoken> multiplicative_op;
    symbols<ast::optoken> unary_op;
    symbols<ast::predictor> predictors;
    symbols<ast::placeholder> placeholders;
    symbols<> keywords;
    
    auto const sk = skip(space);
    auto const ss = sk[eps];
    
    auto const preserved = 
        keywords
      | predictors
      | placeholders
      ;

    auto const spec_def =
      *node >> attr(mustache::ast::node(mustache::ast::eoi{}))
      ;

    auto const node_def =
        tild_see_section
      | tild_once_section
      | tild_each_section
      | tild_expr
      | mustache_section
      | mustache::parser::node
      ;

    auto const node_list_def =
      *node
      ;

    auto const tild_expr_def =
      confix(lit("{{") >> sk['~'], sk["}}"])[ -expression ]
      ;

    auto const tild_once_section_def =
      tild_once_case
      >> -tild_else_case
      >> omit[sk[lit("{{") >> '~' >> "end" >> -lit("once") >> "}}"]]
      ;

    auto const tild_each_section_def =
      tild_each_case
      >> -tild_else_case
      >> omit[sk[lit("{{") >> '~' >> "end" >> -lit("each") >> "}}"]]
      ;
    
    auto const tild_see_section_def =
      tild_see_case
      >> +tild_expr_case
      >> -tild_else_case
      >> omit[sk[lit("{{") >> '~' >> "end" >> -lit("see") >> "}}"]]
      ;

    auto const tild_see_case_def =
      confix(lit("{{") >> sk[char_('~') >> "see"], sk["}}"])[ expression ]
      >> node_list
      ;

    auto const tild_once_case_def =
      confix(lit("{{") >> sk[char_('~') >> "once"], sk["}}"])[ expression ]
      >> node_list
      ;

    auto const tild_each_case_def =
      confix(lit("{{") >> sk[char_('~') >> "each"], sk["}}"])[ expression ]
      >> node_list
      ;
    
    auto const tild_expr_case_def =
      confix(lit("{{") >> sk[char_('~') >> "case"], sk["}}"])[ expression ]
      >> node_list
      ;
    
    auto const tild_else_case_def =
      omit[sk[lit("{{") >> '~' >> "else" >> "}}"]]
      >> node_list
      ;
    
    auto const mustache_section_def =
      matches[&(lit("{{") >> '^')][( [](auto &c){ _val(c).inverted = _attr(c); } )]
      >> mustache::parser::section_begin [ mustache::parser::set_tag ]
      >> node_list     [( [](auto &c){ _val(c).nodes = _attr(c); } )]
      >> mustache::parser::section_end   [ mustache::parser::see_tag ]
      ;

    auto const list_expr_def =
      infix_expr 
      >> *( sk[list_op] > infix_expr )
      ;
    
    auto const infix_expr_def =
      logical_expr
      >> *( sk[infix_op] > logical_expr )
      ;

    auto const logical_expr_def =
      equality_expr
      >> *( sk[logical_op] > equality_expr )
      ;

    auto const equality_expr_def =
      relational_expr
      >> *( sk[equality_op] > relational_expr )
      ;

    auto const relational_expr_def =
      additive_expr
      >> *( sk[relational_op] > additive_expr )
      ;

    auto const additive_expr_def =
      multiplicative_expr
      >> *( sk[additive_op] > multiplicative_expr )
      ;

    auto const multiplicative_expr_def =
      unary_expr
      >> *( sk[multiplicative_op] > unary_expr )
      ;

    auto const unary_expr_def =
      primary_expr
      | ( unary_op > primary_expr )
      ;

    auto const primary_expr_def =
        sk[predictors]
      | sk[placeholders]
      | (ss >> emit_text)
      | (ss >> single_quoted)
      | (ss >> double_quoted)
      | (sk['('] > expression > sk[')'])
      | sk[!preserved >> identifier]
      ;

    auto const identifier_def =
      raw[lexeme[(alpha | '_') >> *(alnum | '_')]]
      ;

    auto const single_quoted_def =
      confix('\'', '\'')[*(char_ - char_('\''))]
      ;

    auto const double_quoted_def =
      confix('"', '"')[*(char_ - char_('"'))]
      ;

    auto const emit_text_def =
      confix('{', '}')[*(char_ - '}')]
      ;

    auto const expression_def = 
      ss >> list_expr
      ;
    
    BOOST_SPIRIT_DEFINE(
        spec,
        node,
        node_list,
        tild_expr,
        tild_each_case,
        tild_each_section,
        tild_once_case,
        tild_once_section,
        tild_see_case,
        tild_see_section,
        tild_else_case,
        tild_expr_case,
        mustache_section,
        list_expr,
        infix_expr,
        equality_expr,
        relational_expr,
        logical_expr,
        additive_expr,
        multiplicative_expr,
        unary_expr,
        primary_expr,
        identifier,
        single_quoted,
        double_quoted,
        emit_text,
        expression
    )

    void init_symbols_once()
    {
      static bool done = false;
      if (done) return;
      done = true;

      list_op.add
        (",", ast::op_push)
        ;
      
      infix_op.add
        (":", ast::op_sel)
        ("..", ast::op_range)
        ;
      
      logical_op.add
        ("&&", ast::op_and)
        ("||", ast::op_or)
        ;

      equality_op.add
        ("==", ast::op_equal)
        ("!=", ast::op_not_equal)
        ;

      relational_op.add
        ("<", ast::op_less)
        ("<=", ast::op_less_equal)
        (">", ast::op_greater)
        (">=", ast::op_greater_equal)
        ;

      additive_op.add
        ("+", ast::op_plus)
        ("-", ast::op_minus)
        ;

      multiplicative_op.add
        ("*", ast::op_times)
        ("/", ast::op_divide)
        ;
      
      unary_op.add
        ("+", ast::op_positive)
        ("-", ast::op_negative)
        ("!", ast::op_not)
        ;

      predictors.add
        ("first", ast::predictor::first)
        ("last", ast::predictor::last)
        ;

      placeholders.add
        ("_", ast::placeholder::underscore)
        ;

      keywords.add
        ("once")
        ("each")
        ("else")
        ("see")
        ("end")
        ("$")
        ;
    }

    //struct unary_expr_class : x3::annotate_on_success {};
    //struct primary_expr_class : x3::annotate_on_success {};
  } // namespace parser
  
  const parser::tildache_type &spec()
  {
    parser::init_symbols_once();
    return parser::spec;
  }

}}} // namespace mold::domain::mustache
#endif//_BOOST_MOLD_DOMAIN_MUSTACHE_PARSER_DEF_HPP_
