/**
 *  \file boost/mold/format/tildache/ast.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_TILDACHE_AST_HPP_
#define _BOOST_MOLD_FORMAT_TILDACHE_AST_HPP_ 1
# include <boost/mold/format/mustache/ast.hpp>
# include <list>
namespace boost { namespace mold { namespace format { namespace tildache { namespace ast
{
  using mustache_node = mustache::ast::node;

  struct unary;
  struct expression;
  struct statement;

  struct nil {};

  struct identifier : std::string
  {
    using std::string::string;
    using std::string::operator=;
  };

  struct emit_text : std::string
  {
    using std::string::string;
    using std::string::operator=;
  };

  enum class predictor
  {
    first,
    last,
  };

  enum class placeholder
  {
    underscore,
  };
  
  struct operand : boost::spirit::x3::variant<
      nil
    , predictor
    , placeholder
    , std::string
    , identifier
    , emit_text
    , boost::spirit::x3::forward_ast<expression>
    , boost::spirit::x3::forward_ast<unary>
    >
  {
    operand() : base_type() {}
    operand(predictor v) : base_type(v) {}
    operand(placeholder v) : base_type(v) {}
    operand(const char *v) : base_type(std::string{v}) {}
    operand(const std::string &v) : base_type(v) {}
    operand(const identifier &v) : base_type(v) {}
    operand(const expression &v);
    operand(const unary &v);
  };

  enum optoken
  {
    op_plus,
    op_minus,
    op_times,
    op_divide,
    op_positive,
    op_negative,
    op_not,
    op_equal,
    op_not_equal,
    op_less,
    op_less_equal,
    op_greater,
    op_greater_equal,
    op_and,
    op_or,
    op_range, // ..
    op_sel,   // :
  };

  struct unary
  {
    optoken op;
    operand oper;
  };

  struct operation //: boost::spirit::x3::position_tagged
  {
    optoken op;
    operand oper;
  };

  using operations = std::list<operation>;
  
  struct expression //: boost::spirit::x3::position_tagged
  {
    operand first;
    operations rest;
  };

  struct tild
  {
    expression expr;
  };
  
  struct statement
  {
    // TODO: tild statement
  };

  struct mustache_section;
  struct tild_section;
  
  struct node : boost::spirit::x3::variant<
      mustache_node
    , tild
    , statement
    , boost::spirit::x3::forward_ast<mustache_section>
    , boost::spirit::x3::forward_ast<tild_section>
    >
  {
    node() : base_type() {}
    node(mustache_node const & rhs) : base_type(rhs) {}
    node(tild const & rhs) : base_type(rhs) {}
    node(statement const & rhs) : base_type(rhs) {}
    node(mustache_section const & rhs);
    node(tild_section const & rhs);
  };

  struct node_list : std::vector<node> {};

  struct mustache_section
  {
    bool inverted;
    mustache::ast::identifier name;
    node_list nodes;
  };

  struct tild_section
  {
    expression expr;
    node_list nodes;
  };
  
  inline operand::operand(const unary &v) : base_type(v) {}
  inline operand::operand(const expression &v) : base_type(v) {}
  inline node::node(mustache_section const & rhs) : base_type(rhs) {}
  inline node::node(tild_section const & rhs) : base_type(rhs) {}
  
}}}}} // namespace boost::mold::format::tildache::ast

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::format::tildache::ast::unary,
   (boost::mold::format::tildache::ast::optoken, op)
   (boost::mold::format::tildache::ast::operand, oper)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::format::tildache::ast::operation,
   (boost::mold::format::tildache::ast::optoken, op)
   (boost::mold::format::tildache::ast::operand, oper)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::format::tildache::ast::expression,
   (boost::mold::format::tildache::ast::operand, first)
   (boost::mold::format::tildache::ast::operations, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::format::tildache::ast::tild,
   (boost::mold::format::tildache::ast::expression, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::format::tildache::ast::tild_section,
   (boost::mold::format::tildache::ast::expression, expr)
   (boost::mold::format::tildache::ast::node_list, nodes)
)

#endif//_BOOST_MOLD_FORMAT_MUSTACHE_AST_HPP_
