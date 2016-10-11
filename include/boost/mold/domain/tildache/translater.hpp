/**
 *  \file boost/mold/domain/tildache/translater.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_TILDACHE_TRANSLATER_HPP_
#define _BOOST_MOLD_DOMAIN_TILDACHE_TRANSLATER_HPP_ 1
# include <boost/mold/domain/mustache/translater.hpp>
# include <boost/mold/domain/tildache/ast.hpp>
# include <limits>
# include <iostream>
namespace boost { namespace mold { namespace domain { namespace tildache
{
  struct translation_visitor : mustache::translation_visitor
  {
    explicit translation_visitor(mustache::translation_state &state)
      : mustache::translation_visitor(state) {}
    
    result_type operator()(const ast::node_list &l) const
    {
      interpreter::ops::op_list ops;
      for (auto const &n : l) {
        ops.push_back(boost::apply_visitor(*this, n));
      }
      return ops;
    }

    result_type operator()(const ast::node &n) const
    {
      return boost::apply_visitor(*this, n);
    }

    result_type operator()(const ast::mustache_node &n) const
    {
      /*
      if ( 0 < state.whitespace.size() ) {
        std::clog << state.inline_directives << std::endl;
        ops.push_back(interpreter::ops::render{ 
          interpreter::ops::kind::immediate, state.whitespace });
        state.whitespace.clear();
      }
      */
      auto base = static_cast<const mustache::translation_visitor*>(this);
      return boost::apply_visitor(*base, n);
    }

    result_type operator()(const ast::statement &n) const
    {
      std::clog << __PRETTY_FUNCTION__ << std::endl;
      return interpreter::ops::op{};
    }

    result_type operator()(const ast::tild &t) const
    {
      //std::clog << __PRETTY_FUNCTION__ << std::endl;

      interpreter::ops::op_list ops;
      
      state.inline_entities += 1;
      
      if ( 0 < state.whitespace.size() ) {
        ops.push_back(interpreter::ops::render{ 
          interpreter::ops::kind::immediate, state.whitespace });
        state.whitespace.clear();
      }
      
      ops.push_back(interpreter::ops::new_stack{});
      
      if (t.expr) {
        ops.push_back((*this)(*t.expr)); // construct new stack
        ops.push_back(interpreter::ops::render{ 
          interpreter::ops::kind::stack, "", 0, true });
      } else {
        // {{~}} renders reg #0
        ops.push_back(interpreter::ops::render{ 
          interpreter::ops::kind::reg, "", 0, true });
      }
      
      ops.push_back(interpreter::ops::pop_stack{});
      return ops;
    }

    result_type operator()(const ast::expression &expr) const
    {
      auto operand = boost::apply_visitor(*this, expr.first);
      for (auto const &operation : expr.rest) {
        switch (operation.op) {
        case ast::op_plus:
        case ast::op_minus:
        case ast::op_times:
        case ast::op_divide:
        case ast::op_equal:
        case ast::op_not_equal:
        case ast::op_less:
        case ast::op_less_equal:
        case ast::op_greater:
        case ast::op_greater_equal:
        case ast::op_and:
        case ast::op_or:
        case ast::op_range:
        case ast::op_sel:
          operand = interpreter::ops::op_list{
            operand, boost::apply_visitor(*this, operation.oper),
            get_binary_op(operation.op),
          };
          break;

        case ast::op_push:
          operand = interpreter::ops::op_list{
            operand, boost::apply_visitor(*this, operation.oper),
          };
          break;
          
        default:
          std::clog << __PRETTY_FUNCTION__ << std::endl;
          return interpreter::ops::op{ /* undefined*/ };
        }
      }
      return operand;
    }

    // same as mustache, but working with tildache nodes
    result_type operator()(const ast::mustache_section &sec) const
    {
      interpreter::ops::op_list ops;

      /*
      if ( 0 < state.whitespace.size() ) {
        ops.push_back(interpreter::ops::render{ 
          interpreter::ops::kind::immediate, state.whitespace });
        state.whitespace.clear();
      }
      */
      
      // Counting section begin directive.
      state.inline_directives += 1;
      
      interpreter::ops::op_list body;
      for (auto const &n : sec.nodes) {
        body.push_back(boost::apply_visitor(*this, n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      
      ops.push_back(interpreter::ops::switch_context{ sec.name, sec.inverted, body });
      return ops;
    }

    result_type operator()(const ast::tild_see_section &ts) const
    {
      interpreter::ops::op_list ops{ 
        interpreter::ops::new_stack{},
        (*this)(ts.init_case.expr), // construct new stack for test
      };

      for (auto &sec : ts.expr_cases) {
        ops.push_back((*this)(sec, interpreter::ops::iterate_source::top_intersection));
      }

      if (ts.else_case) {
        ops.push_back(interpreter::ops::if_then_else{
          {}, (*this)(*ts.else_case) });
      }
      ops.push_back(interpreter::ops::pop_stack{});
      return ops;
    }

    result_type operator()(const ast::tild_once_section &sec) const
    {
      interpreter::ops::op_list ops{ 
        interpreter::ops::new_stack{},
        (*this)(sec.expr_case.expr), // construct new stack for test
      };
      ops.push_back(interpreter::ops::if_then_else{
        (*this)(sec.expr_case.nodes), 
        (sec.else_case ? (*this)(*sec.else_case) : interpreter::ops::nop{})
      });
      ops.push_back(interpreter::ops::pop_stack{});
      return ops;
    }

    result_type operator()(const ast::tild_each_section &sec) const
    {
      interpreter::ops::op els;
      if (sec.else_case) els = (*this)(*sec.else_case);
      return (*this)(sec.expr_case, interpreter::ops::iterate_source::top_stack,
                     els);
    }
    
    result_type operator()(const ast::tild_expr_case &sec, 
                           interpreter::ops::iterate_source source,
                           interpreter::ops::op body_else = {}) const
    {
      // Counting section begin directive.
      state.inline_directives += 1;

      interpreter::ops::op_list ops{ 
        interpreter::ops::new_stack{},
        (*this)(sec.expr), // construct new stack
      };
      
      interpreter::ops::op_list body;
      for (auto const &n : sec.nodes) {
        body.push_back(boost::apply_visitor(*this, n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      
      ops.push_back(interpreter::ops::for_each{ body, body_else, source });
      ops.push_back(interpreter::ops::pop_stack{});
      return ops;
    }

    result_type operator()(const ast::tild_else_case &sec) const
    {
      if (!sec) return interpreter::ops::nop{};
      
      // Counting section begin directive.
      state.inline_directives += 1;

      interpreter::ops::op_list ops;
      for (auto const &n : *sec) {
        ops.push_back(boost::apply_visitor(*this, n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      return ops;
    }
    
    //
    // Expression Operands
    
    result_type operator()(ast::nil) const
    {
      return interpreter::ops::nop{};
    }

    result_type operator()(ast::predictor pred) const
    {
      using limit = std::numeric_limits<
        decltype(interpreter::ops::test_cursor::pos)>;
      switch (pred) {
      case ast::predictor::first:
        return interpreter::ops::test_cursor{ limit::min() };
      case ast::predictor::last:
        return interpreter::ops::test_cursor{ limit::max() };
      }
      std::clog << __PRETTY_FUNCTION__ << std::endl;
      return interpreter::ops::op{ /* undefined */ };
    }

    result_type operator()(ast::placeholder place) const
    {
      switch (place) {
      case ast::placeholder::underscore:
        return interpreter::ops::push{ interpreter::ops::kind::reg, "", 0 };
      }
      std::clog << __PRETTY_FUNCTION__ << std::endl;
      return interpreter::ops::op{ /* undefined */ };
    }

    result_type operator()(const ast::unary &op) const
    {
      switch (op.op) {
      case ast::op_positive:
        return interpreter::ops::op_list{
          boost::apply_visitor(*this, op.oper),
          interpreter::ops::unary::math_posit,
        };
      case ast::op_negative:
        return interpreter::ops::op_list{
          boost::apply_visitor(*this, op.oper),
          interpreter::ops::unary::math_negate,
        };
      case ast::op_not:
        return interpreter::ops::op_list{
          boost::apply_visitor(*this, op.oper),
          interpreter::ops::unary::test_negative,
        };
        
      default: break;
      }
      std::clog << __PRETTY_FUNCTION__ << std::endl;
      return interpreter::ops::op{ /* undefined */ };
    }

    result_type operator()(const ast::emit_text &text) const
    {
      return interpreter::ops::render{
        interpreter::ops::kind::immediate, text };
    }

    result_type operator()(const std::string &s) const
    {
      return interpreter::ops::push{ interpreter::ops::kind::immediate, s };
    }

    result_type operator()(const ast::identifier &id) const
    {
      return interpreter::ops::push{ interpreter::ops::kind::variable, id };
    }
    
  private:
    static interpreter::ops::binary get_binary_op(ast::optoken op)
    {
      switch (op) {
      case ast::op_plus: return interpreter::ops::binary::math_plus;
      case ast::op_minus: return interpreter::ops::binary::math_minus;
      case ast::op_times: return interpreter::ops::binary::math_times;
      case ast::op_divide: return interpreter::ops::binary::math_divide;
      case ast::op_equal: return interpreter::ops::binary::test_equal;
      case ast::op_not_equal: return interpreter::ops::binary::test_not_equal;
      case ast::op_less: return interpreter::ops::binary::test_less;
      case ast::op_less_equal: return interpreter::ops::binary::test_less_equal;
      case ast::op_greater: return interpreter::ops::binary::test_greater;
      case ast::op_greater_equal: return interpreter::ops::binary::test_greater_equal;
      case ast::op_and: return interpreter::ops::binary::test_and;
      case ast::op_or: return interpreter::ops::binary::test_or;
      case ast::op_range: return interpreter::ops::binary::range;
      case ast::op_sel: return interpreter::ops::binary::select;
      default: break;
      }
      assert(false && "undefined binary operation");
    }

    
  }; // translation_visitor

  using translater = details::basic_translater<
    mustache::translation_state, translation_visitor>;

}}}} // namespace boost::mold::domain::tildache
#endif//_BOOST_MOLD_DOMAIN_TILDACHE_TRANSLATER_HPP_
