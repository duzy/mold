/**
 *  \file mold.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_INTERPRETER_OPS_HPP_
#define _BOOST_MOLD_INTERPRETER_OPS_HPP_ 1
# include <boost/spirit/home/x3/support/ast/variant.hpp>
# include <vector>
# include <string>
namespace boost { namespace mold { namespace interpreter { namespace ops 
{
  /*
  template <typename Type>
  using forward = boost::spirit::x3::forward_ast<Type>;

  template <typename ...Types>
  using variant = boost::spirit::x3::variant<Types...>;
  */
  
  struct for_each;
  struct if_then_else;
  struct switch_context;
  struct iterate_context;
  struct op_list;

  struct undefined {};
  
  struct nop {};

  struct load
  {
    std::string name;
    bool incremental;
  };

  struct load_text
  {
    std::string text;
    bool incremental;
  };

  struct load_io
  {
    std::string name;
    bool incremental;
  };

  struct clear {};

  struct unescape
  {
    int format; // HTML, JSON, etc.
  };

  struct render
  {
    bool flush; // flush memory
  };

  struct render_text
  {
    std::string text;
  };
  
  struct op : boost::spirit::x3::variant<
    undefined, nop, 
    load, load_text, load_io, 
    clear, unescape, render, render_text,
    boost::spirit::x3::forward_ast<for_each>, 
    boost::spirit::x3::forward_ast<if_then_else>,
    boost::spirit::x3::forward_ast<switch_context>,
    boost::spirit::x3::forward_ast<iterate_context>,
    boost::spirit::x3::forward_ast<op_list>>
  {
    op() : base_type() {}
    op(const nop &o) : base_type(o) {}
    op(const load &o) : base_type(o) {}
    op(const load_text &o) : base_type(o) {}
    op(const load_io &o) : base_type(o) {}
    op(const clear &o) : base_type(o) {}
    op(const unescape &o) : base_type(o) {}
    op(const render &o) : base_type(o) {}
    op(const render_text &o) : base_type(o) {}
    op(const for_each &o);
    op(const if_then_else &o);
    op(const switch_context &o);
    op(const iterate_context &o);
    op(const op_list &l);
  };

  struct condition
  {
    std::string name;
    
    // TODO: using expression
  };
  
  struct for_each
  {
    std::string name;
    op value;
  };

  struct if_then_else
  {
    condition value;
    op body_then;
    op body_else;
  };

  struct switch_context
  {
    std::string name;
    bool inverted;
    op body_else;
  };

  struct iterate_context {};
  
  struct op_list : std::vector<op>
  {
    using std::vector<op>::vector;
    using std::vector<op>::operator=;
  };

  inline op::op(const for_each &o) : base_type(o) {}
  inline op::op(const if_then_else &o) : base_type(o) {}
  inline op::op(const switch_context &o) : base_type(o) {}
  inline op::op(const iterate_context &o) : base_type(o) {}
  inline op::op(const op_list &l) : base_type(l) {}
  
}}}} // namespace boost::mold::interpreter::ops

#endif//_BOOST_MOLD_INTERPRETER_OPS_HPP_
