/**
 *  \file boost/mold/domain/tildache/ast.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_TILDACHE_AST_HPP_
#define _BOOST_MOLD_DOMAIN_TILDACHE_AST_HPP_ 1
# include <mold/domain/mustache/ast.hpp>
#if 0
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  template <typename Source, typename Dest>
  inline void move_to(boost::optional<Source>&& src, Dest& dest)
  {
    // For tild_else_case = boost::optional<node_list>
    if (src) move_to(*src, dest);
  }
}}}} // boost::spirit::x3::traits
#endif
namespace mold { namespace domain { namespace tildache { namespace ast
{
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
    nil, predictor, placeholder,
    std::string, identifier, emit_text,
    boost::spirit::x3::forward_ast<expression>,
    boost::spirit::x3::forward_ast<unary>>
  {
    operand() : base_type() {}
    template<typename V> // use `explicit` to avoid ambiguous
    explicit operand(V&& v) : base_type(std::forward<V>(v)) {}
    //explicit operand(const char *v) : base_type(std::string{v}) {}
    template<typename V> operand& operator=(V&& v) {
      this->base_type::operator=(std::forward<V>(v));
      return *this; }
    //operand& operator=(const operand& other) {
    //  this->base_type::operator=(other);
    //  return *this; }
    //operand& operator=(const char *v) {
    //  return base_type::operator=(std::string{v}); }
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
     op_push,  // ,
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

  using operations = std::vector<operation>;

  static_assert(boost::spirit::x3::traits::is_container<operations>::value,
                "operations is not container");
  
  struct expression //: boost::spirit::x3::position_tagged
  {
    operand first;
    operations rest;
  };

  using expression_opt = boost::optional<expression>;
  
  struct tild_expr
  {
    expression_opt expr;
  };
  
  struct statement
  {
    // TODO: tild statement
  };

  struct mustache_section;
  struct tild_see_section;
  struct tild_once_section;
  struct tild_each_section;

  struct node : boost::spirit::x3::variant<
#if 0
    mustache::ast::undefined,
    mustache::ast::comment,
    mustache::ast::literal_text,
    mustache::ast::blank_text,
    mustache::ast::variable,
    mustache::ast::partial,
    mustache::ast::eol,
    mustache::ast::eoi,
    mustache::ast::section,
#else
    mustache::ast::node,
#endif
    tild_expr, statement,
    boost::spirit::x3::forward_ast<mustache_section>,
    boost::spirit::x3::forward_ast<tild_see_section>,
    boost::spirit::x3::forward_ast<tild_once_section>,
    boost::spirit::x3::forward_ast<tild_each_section>>
  {
    node() : base_type() {}
    template<typename V> // use `explicit` to avoid ambiguous
    explicit node(V&& v) : base_type(std::forward<V>(v)) {}
    template<typename V> node& operator=(V&& v) {
      this->base_type::operator=(std::forward<V>(v));
      return *this; }
    //node& operator=(const node& other) {
    //  this->base_type::operator=(other);
    //  return *this; }
  };

  using node_list = std::vector<node>;

  static_assert(boost::spirit::x3::traits::is_container<node_list>::value,
                "node_list is not container");

  struct mustache_section
  {
    bool inverted;
    mustache::ast::identifier name;
    node_list nodes;
  };

  struct tild_expr_case
  {
    expression expr;
    node_list nodes;
  };
  
  using tild_expr_cases = std::vector<tild_expr_case>;
# if 0
  using tild_else_case = boost::optional<node_list>;
# else
  struct tild_else_case : node_list {
    using node_list::node_list;
    using node_list::operator=;
  };
# endif
  
  struct tild_see_section
  {
    tild_expr_case  init_case;
    tild_expr_cases expr_cases;
    boost::optional<tild_else_case> else_case;
  };

  struct tild_once_section
  {
    tild_expr_case expr_case;
    boost::optional<tild_else_case> else_case;
  };

  struct tild_each_section
  {
    tild_expr_case expr_case;
    boost::optional<tild_else_case> else_case;
  };

}}}} // namespace mold::domain::tildache::ast

BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::unary, op, oper)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::operation, op)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::expression, first, rest)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::tild_expr, expr)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::tild_expr_case, expr, nodes)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::tild_see_section, init_case, expr_cases, else_case)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::tild_once_section, expr_case, else_case)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::tildache::ast::tild_each_section, expr_case, else_case)

#endif//_BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_
