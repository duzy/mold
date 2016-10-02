/**
 *  \file execution_visitor.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_VISITOR_HPP_
#define _BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_VISITOR_HPP_ 1
# include <boost/mold/interpreter/ops.hpp>

namespace boost { namespace mold { namespace interpreter
{
  using undefined_behavior = std::runtime_error;

  namespace details
  {
    
    template <typename Machine>
    struct execution_visitor
    {
      execution_visitor(Machine &machine) : machine(machine)
      {
      }

      void operator()(const ops::op_list &ol) const
      {
        for (auto const &op : ol) {
          boost::apply_visitor(*this, op);
        }
      }
    
      void operator()(const ops::op &op) const
      {
        boost::apply_visitor(*this, op);
      }

      void operator()(const ops::undefined &op) const
      {
#if 0
        throw undefined_behavior();
#else
        machine.render("<undefined>");
#endif
      }
    
      void operator()(const ops::nop &op) const
      {
      }

      void operator()(const ops::load &op) const
      {
        machine.render("<load>");
      }

      void operator()(const ops::load_text &op) const
      {
        machine.render("<load_text>");
      }

      void operator()(const ops::load_io &op) const
      {
        machine.render("<load_io>");
      }

      void operator()(const ops::clear &op) const
      {
        machine.render("<clear>");
      }

      void operator()(const ops::unescape &op) const
      {
        machine.render("<unescape>");
      }
    
      void operator()(const ops::render &op) const
      {
        machine.render_memory();
        if (op.flush) machine.clear_memory();
      }

      void operator()(const ops::render_text &op) const
      {
        machine.render(op.text);
      }
      
      void operator()(const ops::for_each &op) const
      {
        machine.render("<foreach>");
      }

      void operator()(const ops::if_then_else &op) const
      {
        machine.render("<if_then_else>");
      }

      void operator()(const ops::switch_context &op) const
      {
        machine.render("<switch_context>");
      }

      void operator()(const ops::iterate_context &op) const
      {
        machine.render("<restore_context>");
      }
    
    private:
      Machine &machine;
    };

  } // namespace details
}}} // namespace boost::mold::interpreter

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_VISITOR_HPP_
