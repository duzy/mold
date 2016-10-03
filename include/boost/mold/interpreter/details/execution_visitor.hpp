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
# include <boost/mold/interpreter/details/context_cursors.hpp>
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
        //std::clog << "op_list: " << ol.size() << std::endl;
        
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

      void operator()(const ops::end &op) const
      {
        // FIXME: Do something for ending?
      }
      
      void operator()(const ops::load &op) const
      {
        //std::clog << "load: " << op.name << std::endl;

        if (!op.incremental) {
          machine.clear();
        }
        
        machine.load_var(op.name);
      }

      void operator()(const ops::load_text &op) const
      {
        //std::clog << "load_text: " << op.text << std::endl;
        
        if (!op.incremental) {
          machine.clear();
        }
        
        machine.load_text(op.text);
      }

      void operator()(const ops::clear &op) const
      {
        machine.clear();
      }

      void operator()(const ops::edit &op) const
      {
        machine.edit(op.f);
      }
    
      void operator()(const ops::render &op) const
      {
        machine.render();
        if (op.flush) machine.clear();
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
        if (machine.has(op.value.name)) {
          boost::apply_visitor(*this, op.body_then);
        } else {
          boost::apply_visitor(*this, op.body_else);
        }
      }

      void operator()(const ops::switch_context &op) const
      {
        //std::clog << "switch_context: " << op.name << ", " << op.inverted << std::endl;

        if (op.inverted) {
          reverse_iterate_context(op.name, op.body);
        } else {
          iterate_context(op.name, op.body);
        }
      }

    private:
      Machine &machine;

      void reverse_iterate_context(const std::string &name, const ops::op &body) const
      {
        typename Machine::template scope<details::context_reverse_cursor>
          scope(machine, name);
        iterate_scope(scope, body);
      }
      
      void iterate_context(const std::string &name, const ops::op &body) const
      {
        typename Machine::template scope<details::context_cursor>
          scope(machine, name);
        iterate_scope(scope, body);
      }

      template<typename Cursor>
      void iterate_scope(typename Machine::template scope<Cursor> &scope, const ops::op &body) const
      {
        do { boost::apply_visitor(*this, body); } while (scope.next());
      }
    };

  } // namespace details
}}} // namespace boost::mold::interpreter

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_VISITOR_HPP_
