/**
 *  \file mold.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VM_OPS_HPP_
#define _BOOST_MOLD_VM_OPS_HPP_ 1
# include <boost/spirit/home/x3/support/ast/variant.hpp>
# include <vector>
# include <string>
# if USE_EXTBIT_LOG
#   include <__formatter>
# endif
namespace mold { namespace vm { namespace ops 
{
  struct for_each;
  struct if_then_else;
  struct switch_context;
  struct op_list;

  struct undefined {};
  
  struct nop {};
  
  struct end {};

  enum class kind // object kind
  {
    undefined,
    immediate,
    variable,
    memory,
    stack,
    reg,
  };

  inline const std::string_view as_name(kind op) noexcept
  {
    // TODO: using static reflection for names
    static const char* names[] = {
      "undefined",
      "immediate",
      "variable",
      "memory",
      "stack",
      "reg",
    };
    return { names[static_cast<int>(op)] };
  }

  struct load
  {
    kind k = kind::undefined;
    std::string s; //!< immediate, variable
    unsigned int i = 0; //!< reg
    bool incremental = false;
  };

  struct clear
  {
    kind k = kind::undefined;
    unsigned int i = 0; //!< reg
  };

  struct edit
  {
    std::function<void(std::string &)> f; // HTML, JSON, etc.
  };

  struct render
  {
    kind k = kind::undefined;
    std::string s; //!< immediate, memory
    unsigned int i = 0; //!< reg
    bool flush; // flush memory, stack or reg
  };

  struct test_cursor
  {
    // std::numeric_limits<>::min() is the first
    // std::numeric_limits<>::max() is the last
    unsigned int pos;
  };
  
  struct push
  {
    kind k = kind::immediate;
    std::string s;
    unsigned int i = 0;
  };
  
  struct pop
  {
    bool memorize;
  };

  struct new_regs {};
  struct pop_regs {};

  struct new_stack {};
  struct pop_stack {};

  enum class unary : unsigned
  {
    math_posit,
    math_negate,
    test_negative,
    test_positive,
  };
  
  enum class binary : unsigned
  {
    math_plus,
    math_minus,
    math_times,
    math_divide,
    test_equal,
    test_not_equal,
    test_less,
    test_less_equal,
    test_greater,
    test_greater_equal,
    test_and,
    test_or,
    range,
    select,
  };

  inline const std::string_view as_name(unary op) noexcept
  {
    // TODO: using static reflection for names
    static const char* names[] = {
      "math_posit",
      "math_negate",
      "test_negative",
      "test_positive",
    };
    return { names[static_cast<int>(op)] };
  }

  inline const std::string_view as_name(binary op) noexcept
  {
    // TODO: using static reflection for names
    static const char* names[] = {
      "math_plus",
      "math_minus",
      "math_times",
      "math_divide",
      "test_equal",
      "test_not_equal",
      "test_less",
      "test_less_equal",
      "test_greater",
      "test_greater_equal",
      "test_and",
      "test_or",
      "range",
      "select",
    };
    return { names[static_cast<int>(op)] };
  }
  
  struct op : boost::spirit::x3::variant<
    undefined, nop, end, load,
    clear, edit, render,
    push, pop, new_stack, pop_stack, new_regs, pop_regs,
    test_cursor, unary, binary,
    boost::spirit::x3::forward_ast<for_each>, 
    boost::spirit::x3::forward_ast<if_then_else>,
    boost::spirit::x3::forward_ast<switch_context>,
    boost::spirit::x3::forward_ast<op_list>>
  {
    op() : base_type() {}
    template<typename V> op(V&& v) : base_type(std::forward<V>(v)) {}
    template<typename V> op& operator=(V&& v) {
      base_type::operator=(std::forward<V>(v));
      return *this; }
  };

  enum class iterate_source
  {
    top_stack,
    top_intersection,
  };
  
  struct for_each
  {
    op body, body_else;
    iterate_source source = iterate_source::top_stack;
  };

  struct if_then_else
  {
    op body_then;
    op body_else;
  };

  struct switch_context
  {
    std::string name;
    bool inverted;
    op body;
  };

  struct op_list : std::vector<op>
  {
    using std::vector<op>::vector;
    using std::vector<op>::operator=;
  };
  
}}} // namespace mold::vm::ops

#if USE_EXTBIT_LOG
namespace std
{
  template<typename Char>
  struct formatter<mold::vm::ops::kind,Char> : formatter<string_view,Char>
  {
    template <typename FormatContext>
    constexpr typename FormatContext::iterator
    format(mold::vm::ops::kind op, FormatContext& context) {
      return formatter<string_view,Char>
        ::format(mold::vm::ops::as_name(op), context);
    }
  };

  template<typename Char>
  struct formatter<mold::vm::ops::unary,Char> : formatter<string_view,Char>
  {
    template <typename FormatContext>
    constexpr typename FormatContext::iterator
    format(mold::vm::ops::unary op, FormatContext& context) {
      return formatter<string_view,Char>
        ::format(mold::vm::ops::as_name(op), context);
    }
  };

  template<typename Char>
  struct formatter<mold::vm::ops::binary,Char> : formatter<string_view,Char>
  {
    template <typename FormatContext>
    constexpr typename FormatContext::iterator
    format(mold::vm::ops::binary op, FormatContext& context) {
      return formatter<string_view,Char>
        ::format(mold::vm::ops::as_name(op), context);
    }
  };
}
#endif// USE_EXTBIT_LOG
#endif//_BOOST_MOLD_VM_OPS_HPP_
