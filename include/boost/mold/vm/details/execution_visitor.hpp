/**
 *  \file execution_visitor.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VM_DETAILS_EXECUTION_VISITOR_HPP_
#define _BOOST_MOLD_VM_DETAILS_EXECUTION_VISITOR_HPP_ 1
# include <boost/mold/vm/details/context_cursors.hpp>
# include <boost/mold/vm/ops.hpp>
# include <boost/spirit/home/x3/numeric/int.hpp>
# include <boost/spirit/home/x3/core/parse.hpp>
# include <boost/spirit/home/x3/char/char_class.hpp>
# include <algorithm>
# include <iterator>
# include <limits>
# include <sstream>
namespace boost { namespace mold { namespace vm
{
  using undefined_behavior = std::runtime_error;
  using exceeds_max_range = std::runtime_error;
  using unexpected_object_kind = std::runtime_error;
  using unexpected_operation = std::runtime_error;

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
          machine.clear_memory();
        }

        switch (op.k) {
        case ops::kind::immediate:
          machine.load_text(op.s);
          break;
        case ops::kind::variable:
          machine.load_var(op.s);
          break;
        case ops::kind::stack:
          machine.load_stack();
          break;
        case ops::kind::reg:
          machine.load_reg(op.i);
          break;
        default:
          throw unexpected_object_kind("unexpected object kind");
        }
      }

      void operator()(const ops::clear &op) const
      {
        switch (op.k) {
        case ops::kind::memory:
          machine.clear_memory();
          break;
        case ops::kind::stack:
          machine.clear_stack();
          break;
        case ops::kind::reg:
          machine.reg(op.i).clear();
          break;
        default:
          throw unexpected_object_kind("unexpected object kind");
        }
      }

      void operator()(const ops::edit &op) const
      {
        machine.edit(op.f);
      }
    
      void operator()(const ops::render &op) const
      {
        switch (op.k) {
        case ops::kind::immediate:
          machine.render(op.s);
          break;
        case ops::kind::memory:
          machine.render();
          if (op.flush) {
            machine.clear_memory();
          }
          break;
        case ops::kind::variable:
          machine.render_var(op.s);
          break;
        case ops::kind::stack:
          machine.render_stack();
          if (op.flush) {
            machine.clear_stack();
          }
          break;
        case ops::kind::reg:
          machine.render_reg(op.i);
          if (op.flush) {
            machine.reg(op.i).clear();
          }
          break;
        default:
          throw unexpected_object_kind("unexpected object kind");
        }
      }

      void operator()(const ops::for_each &op) const
      {
        switch (op.source) {
        case ops::iterate_source::top_stack: break;
        case ops::iterate_source::top_intersection:
          {
            std::list<std::string> set;
            auto t = machine.top_stack(); machine.pop_stack();
            auto o = machine.top_stack();
            t.sort(), o.sort();
            std::set_intersection(t.begin(), t.end(), o.begin(), o.end(),
              std::back_inserter(set));
            machine.push_stack(set);
          } break;
        }
        {
          unsigned i = 0;
          while (!machine.empty()) {
            auto &r0 = machine.reg(0);
            r0 = machine.front();
            machine.unshift();
          
            // Skip if there's only one item and empty.
            if (i == 0 && machine.empty() && r0.empty()) {
              boost::apply_visitor(*this, op.body_else);
              break;
            }
          
            boost::apply_visitor(*this, op.body);
            i += 1;
          }
        }
      }

      void operator()(const ops::if_then_else &op) const
      {
        auto success = false;
        for (auto &s : machine.top_stack()) if ((success = !s.empty())) break;
        
        if ( success ) {
          if (!boost::get<ops::undefined>(&op.body_then))
            boost::apply_visitor(*this, op.body_then);
        } else {
          if (!boost::get<ops::undefined>(&op.body_else))
            boost::apply_visitor(*this, op.body_else);
        }
      }

      void operator()(const ops::switch_context &op) const
      {
        typename Machine::template scope<details::context_cursor>
          scope(machine, op.name, op.inverted);
        if (scope.is_valid()) {
          do { boost::apply_visitor(*this, op.body); } while (scope.next());
        }
      }

      void operator()(const ops::test_cursor &op) const
      {
        using limit = std::numeric_limits<decltype(ops::test_cursor::pos)>;
        auto success = false;
        switch (op.pos) {
        case limit::min():
          success = machine.is_cursor_first();
          break;
        case limit::max():
          success = machine.is_cursor_last();
          break;
        }
        machine.push(success ? "true" : "");
      }

      void operator()(const ops::push &op) const
      {
        switch (op.k) {
        case ops::kind::immediate:
          machine.push(op.s);
          break;
        case ops::kind::memory:
          machine.push();
          break;
        case ops::kind::variable:
          machine.push_var(op.s);
          break;
        case ops::kind::reg:
          machine.push_reg(op.i);
          break;
        default:
          throw unexpected_object_kind("unexpected object kind");
        }
      }

      void operator()(const ops::pop &op) const
      {
        if (op.memorize /*&& machine.empty()*/) {
          machine.load_text(machine.top());
        }
        machine.pop();
      }

      void operator()(const ops::new_stack &op) const
      {
        machine.new_stack();
      }

      void operator()(const ops::pop_stack &op) const
      {
        machine.pop_stack();
      }

      void operator()(const ops::new_regs &op) const
      {
        machine.new_regs();
      }

      void operator()(const ops::pop_regs &op) const
      {
        machine.pop_regs();
      }
      
      void operator()(ops::unary op) const
      {
        switch (op) {
        case ops::unary::math_negate:
          machine.top() = to_string(-parse_num<std::int64_t>(machine.top()));
          return;
        case ops::unary::math_posit:
          machine.top() = to_string(parse_num<std::int64_t>(machine.top()));
          return;
        case ops::unary::test_negative:
          if (machine.empty()) {
            machine.push("true");
          } else {
            auto success = machine.top().empty();
            machine.top() = success ? "true" : "";
          }
          return;
        case ops::unary::test_positive:
          if (machine.empty()) {
            machine.push("");
          } else {
            auto success = !machine.top().empty();
            machine.top() = success ? "true" : "";
          }
          return;
        }
      }

      void operator()(ops::binary op) const
      {
        //std::clog << "binary: " << int(op) << ", " << machine.size() << std::endl;
        assert(!machine.empty());
        auto rhs = machine.top(); machine.pop();
        
        assert(!machine.empty());
        auto lhs = machine.top();
        
        if (binary_test(op, lhs, rhs)) return;
        if (binary_math(op, lhs, rhs)) return;

        switch (op) {
        case ops::binary::range: 
          {
            auto i = lhs.begin();
            auto a = parse_num<std::int64_t>(i, lhs.end());
            auto j = rhs.begin();
            auto b = parse_num<std::int64_t>(j, rhs.end());
            machine.pop(); // remove lhs from stack
            if (lhs.begin() < i || rhs.begin() < j) {
              if (1024 < b - a) throw exceeds_max_range("exceeds max range");
              for (auto n = a; n <= b; ++n) {
                machine.push(to_string(n));
              }
            }
          }
          return;

        case ops::binary::select:
          if (!lhs.empty()) {
            machine.top() = rhs;
          }
          return;

        default: break;
        }
        
        assert(false && "unhandled binary operation");
      }

    private:
      Machine &machine;

      template<typename NumType, typename String>
      static NumType parse_num(const String &s)
      {
        auto i = s.begin();
        return parse_num<NumType>(i, s.end());
      }
      
      template<typename NumType, typename Iterator>
      static NumType parse_num(Iterator &a, const Iterator &b)
      {
        using boost::spirit::x3::phrase_parse;
        using boost::spirit::x3::space;
        using boost::spirit::x3::int64;
        auto num = NumType();
        if (phrase_parse(a, b, int64, space, num)) {
          // disregarded
        }
        return num;
      }

      template<typename T>
      static std::string to_string(const T &v)
      {
        std::ostringstream ss;
        ss << v;
        return ss.str();
      }
      
      static std::string to_string(bool v)
      {
        std::ostringstream ss;
        ss << (v ? "true" : "");
        return ss.str();
      }

      bool binary_test(ops::binary op, const std::string &lhs, const std::string &rhs) const
      {
        switch (op) {
        case ops::binary::test_equal:
          machine.top() = to_string(lhs == rhs);
          return true;
        case ops::binary::test_not_equal:
          machine.top() = to_string(lhs != rhs);
          return true;
        case ops::binary::test_less:
          machine.top() = to_string(lhs < rhs);
          return true;
        case ops::binary::test_less_equal:
          machine.top() = to_string(lhs <= rhs);
          return true;
        case ops::binary::test_greater:
          machine.top() = to_string(lhs > rhs);
          return true;
        case ops::binary::test_greater_equal:
          machine.top() = to_string(lhs >= rhs);
          return true;
        case ops::binary::test_and:
          machine.top() = to_string(!lhs.empty() && !rhs.empty());
          return true;
        case ops::binary::test_or:
          machine.top() = to_string(!lhs.empty() || !rhs.empty());
          return true;

        default:
          break; //throw unexpected_operation("unexpected operation");
        }
        return false;
      }
      
      bool binary_math(ops::binary op, const std::string &lhss, const std::string &rhss) const
      {
        auto lhs = parse_num<std::int64_t>(lhss);
        auto rhs = parse_num<std::int64_t>(rhss);
        switch (op) {
        case ops::binary::math_plus: 
          machine.top() = to_string(lhs + rhs);
          return true;
        case ops::binary::math_minus: 
          machine.top() = to_string(lhs - rhs);
          return true;
        case ops::binary::math_times: 
          machine.top() = to_string(lhs * rhs);
          return true;
        case ops::binary::math_divide: 
          if (rhs == 0) {
            machine.top() = "NaN";
          } else {
            machine.top() = to_string(lhs / rhs);
          }
          return true;

        default:
          break; //throw unexpected_operation("unexpected operation");
        }
        return false;
      }
    };

  } // namespace details
}}} // namespace boost::mold::vm

#endif//_BOOST_MOLD_VM_DETAILS_EXECUTION_VISITOR_HPP_
