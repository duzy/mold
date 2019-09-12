/**
 *  \file boost/mold/domain/tildache/translater.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
ppa *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_TILDACHE_TRANSLATER_HPP_
#define _BOOST_MOLD_DOMAIN_TILDACHE_TRANSLATER_HPP_ 1
# include <mold/domain/mustache/translater.hpp>
# include <mold/domain/tildache/ast.hpp>
# include <limits>
# include <iostream>
namespace mold { namespace domain { namespace tildache
{
  struct translation_state : mustache::translation_state {};
  struct translation_visitor : mustache::translation_visitor
  {
    explicit translation_visitor(translation_state &state)
      : mustache::translation_visitor(state) {}

    using mustache::translation_visitor::operator();
    
    result_type operator()(const ast::node_list &l) const
    {
      vm::ops::op_list ops;
      for (auto const &n : l) {
        ops.push_back((*this)(n));
      }
      return ops;
    }

    result_type operator()(const ast::node &n) const
    {
      return boost::apply_visitor(*this, n);
    }

#if 1
    result_type operator()(const mustache::ast::node &n) const
    {
      /*
      if ( 0 < state.whitespace.size() ) {
        std::clog << state.inline_directives << std::endl;
        ops.push_back(vm::ops::render{ 
          vm::ops::kind::immediate, state.whitespace });
        state.whitespace.clear();
      }
      */
      using base_t = mustache::translation_visitor;
      auto base = static_cast<const base_t*>(this);
      return base->operator()(n);
    }
#endif

    result_type operator()(const ast::statement &n) const
    {
      std::clog << __PRETTY_FUNCTION__ << std::endl;
      return vm::ops::op{};
    }

    result_type operator()(const ast::tild_expr &t) const
    {
      vm::ops::op_list ops;
      
      state.inline_entities += 1;
      
      if ( 0 < state.whitespace.size() ) {
        ops.push_back(vm::ops::render{ 
          vm::ops::kind::immediate, state.whitespace });
        state.whitespace.clear();
      }
      
      if (t.expr) {
        ops.push_back(vm::ops::new_stack{});
        ops.push_back((*this)(*t.expr)); // construct new stack
        ops.push_back(vm::ops::render{ 
          vm::ops::kind::stack, "", 0, true });
        ops.push_back(vm::ops::pop_stack{});
      } else {
        // {{~}} renders reg #0
        ops.push_back(vm::ops::render{ 
          vm::ops::kind::reg, "", 0, false });
      }
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
          operand = vm::ops::op_list{
            operand, boost::apply_visitor(*this, operation.oper),
            get_binary_op(operation.op),
          };
          break;

        case ast::op_push:
          operand = vm::ops::op_list{
            operand, boost::apply_visitor(*this, operation.oper),
          };
          break;
          
        default:
          std::clog << __PRETTY_FUNCTION__ << std::endl;
          return vm::ops::op{ /* undefined*/ };
        }
      }
      return operand;
    }

    // same as mustache, but working with tildache nodes
    result_type operator()(const ast::mustache_section &sec) const
    {
      vm::ops::op_list ops;

      /*
      if ( 0 < state.whitespace.size() ) {
        ops.push_back(vm::ops::render{ 
          vm::ops::kind::immediate, state.whitespace });
        state.whitespace.clear();
      }
      */
      
      // Counting section begin directive.
      state.inline_directives += 1;
      
      vm::ops::op_list body;
      for (auto const &n : sec.nodes) {
        body.push_back((*this)(n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      
      ops.push_back(vm::ops::switch_context{ sec.name, sec.inverted, body });
      return ops;
    }

    result_type operator()(const ast::tild_see_section &ts) const
    {
      vm::ops::op_list ops{ 
        vm::ops::new_regs{},
        vm::ops::new_stack{},
        (*this)(ts.init_case.expr), // construct new stack for test
      };

      for (auto &sec : ts.expr_cases) {
        ops.push_back((*this)(sec, vm::ops::iterate_source::top_intersection));
      }

      if (ts.else_case) {
        ops.push_back(vm::ops::new_stack{});
        
        // The reg#0 has the value of the last iterated item. If it's empty,
        // the "else" block is executed.
        ops.push_back(vm::ops::push{ vm::ops::kind::reg, "", 0 });
        ops.push_back(vm::ops::if_then_else{{}, (*this)(*ts.else_case)});
        
        ops.push_back(vm::ops::pop_stack{});
      }

      ops.push_back(vm::ops::pop_stack{});
      ops.push_back(vm::ops::pop_regs{});
      return ops;
    }

    result_type operator()(const ast::tild_once_section &sec) const
    {
      vm::ops::op_list ops{ 
        vm::ops::new_stack{},
        (*this)(sec.expr_case.expr), // construct new stack for test
      };
      ops.push_back(vm::ops::if_then_else{
        (*this)(sec.expr_case.nodes), 
        (sec.else_case ? (*this)(*sec.else_case) : vm::ops::nop{})
      });
      ops.push_back(vm::ops::pop_stack{});
      return ops;
    }

    result_type operator()(const ast::tild_each_section &sec) const
    {
      vm::ops::op els;
      if (sec.else_case) els = (*this)(*sec.else_case);
      return (*this)(sec.expr_case, vm::ops::iterate_source::top_stack,
                     els);
    }
    
    result_type operator()(const ast::tild_expr_case &sec, 
                           vm::ops::iterate_source source,
                           vm::ops::op body_else = {}) const
    {
      // Counting section begin directive.
      state.inline_directives += 1;

      vm::ops::op_list ops{ 
        vm::ops::new_stack{},
        (*this)(sec.expr), // construct new stack
      };
      
      vm::ops::op_list body;
      for (auto const &n : sec.nodes) {
        body.push_back((*this)(n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      
      ops.push_back(vm::ops::for_each{ body, body_else, source });
      ops.push_back(vm::ops::pop_stack{});
      return ops;
    }

    result_type operator()(const ast::tild_else_case &sec) const
    {
      if (!sec) return vm::ops::nop{};

      // Counting section begin directive.
      state.inline_directives += 1;

      vm::ops::op_list ops;
      for (auto const &n : *sec) {
        ops.push_back((*this)(n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      return ops;
    }
    
    //
    // Expression Operands
    
    result_type operator()(ast::nil) const
    {
      return vm::ops::nop{};
    }

    result_type operator()(ast::predictor pred) const
    {
      using limit = std::numeric_limits<
        decltype(vm::ops::test_cursor::pos)>;
      switch (pred) {
      case ast::predictor::first:
        return vm::ops::test_cursor{ limit::min() };
      case ast::predictor::last:
        return vm::ops::test_cursor{ limit::max() };
      }
      return vm::ops::op{ /* undefined */ };
    }

    result_type operator()(ast::placeholder place) const
    {
      switch (place) {
      case ast::placeholder::underscore:
        return vm::ops::push{ vm::ops::kind::reg, "", 0 };
      }
      return vm::ops::op{ /* undefined */ };
    }

    result_type operator()(const ast::unary &op) const
    {
      switch (op.op) {
      case ast::op_positive:
        return vm::ops::op_list{
          boost::apply_visitor(*this, op.oper),
          vm::ops::unary::math_posit,
        };
      case ast::op_negative:
        return vm::ops::op_list{
          boost::apply_visitor(*this, op.oper),
          vm::ops::unary::math_negate,
        };
      case ast::op_not:
        return vm::ops::op_list{
          boost::apply_visitor(*this, op.oper),
          vm::ops::unary::test_negative,
        };
      default: break;
      }
      return vm::ops::op{ /* undefined */ };
    }

    result_type operator()(const ast::emit_text &text) const
    {
      return vm::ops::render{ vm::ops::kind::immediate, text };
    }

    result_type operator()(const std::string &s) const
    {
      return vm::ops::push{ vm::ops::kind::immediate, s };
    }

    result_type operator()(const ast::identifier &id) const
    {
      return vm::ops::push{ vm::ops::kind::variable, id };
    }
    
  private:
    static vm::ops::binary get_binary_op(ast::optoken op)
    {
      switch (op) {
      case ast::op_plus: return vm::ops::binary::math_plus;
      case ast::op_minus: return vm::ops::binary::math_minus;
      case ast::op_times: return vm::ops::binary::math_times;
      case ast::op_divide: return vm::ops::binary::math_divide;
      case ast::op_equal: return vm::ops::binary::test_equal;
      case ast::op_not_equal: return vm::ops::binary::test_not_equal;
      case ast::op_less: return vm::ops::binary::test_less;
      case ast::op_less_equal: return vm::ops::binary::test_less_equal;
      case ast::op_greater: return vm::ops::binary::test_greater;
      case ast::op_greater_equal: return vm::ops::binary::test_greater_equal;
      case ast::op_and: return vm::ops::binary::test_and;
      case ast::op_or: return vm::ops::binary::test_or;
      case ast::op_range: return vm::ops::binary::range;
      case ast::op_sel: return vm::ops::binary::select;
      default: break;
      }
      assert(false && "undefined binary operation");
    }
  }; // translation_visitor

  using translater = basic_translater<
    translation_state, translation_visitor>;

}}} // namespace mold::domain::tildache
#endif//_BOOST_MOLD_DOMAIN_TILDACHE_TRANSLATER_HPP_
