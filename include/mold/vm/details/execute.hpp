/**
 *  \file execute.hpp
 *
 *  Copyright 2016 Duzy Chan <code@extbit.io>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VM_DETAILS_EXECUTE_HPP_
#define _BOOST_MOLD_VM_DETAILS_EXECUTE_HPP_ 1
# include <mold/vm/details/context_cursors.hpp>
# include <mold/vm/ops.hpp>
# include <boost/spirit/home/x3/numeric/int.hpp>
# include <boost/spirit/home/x3/core/parse.hpp>
# include <boost/spirit/home/x3/char/char_class.hpp>
# include <algorithm>
# include <iterator>
# include <limits>
# include <sstream>
# if USE_EXTBIT_LOG
#   include <ext/log>
# endif
namespace mold { namespace vm
{
  using undefined_behavior = std::runtime_error;
  using exceeds_max_range = std::runtime_error;
  using unexpected_object_kind = std::runtime_error;
  using unexpected_operation = std::runtime_error;

  namespace details
  {
    
    template <typename Machine>
    struct execute
    {
      explicit execute(Machine& m) : machine(m) {}

#if 0
      void operator()(ops::op&& op) const { apply(op); }
#else
      void call(const ops::op& op) const { apply(op); }
#endif

      void operator()(const ops::op_list& ol) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("op_list = {}, stacks = {}", ol.size(), machine.caculate_size());
#endif
        for (const ops::op& op : ol) apply(op);
      }

      void operator()(ops::undefined op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("undefined, stacks = {}", machine.caculate_size());
#endif

#if 0
        throw undefined_behavior();
#else
        machine.render("<undefined>");
#endif
      }
    
      void operator()(ops::nop op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("nop, stacks = {}", machine.caculate_size());
#endif
      }

      void operator()(ops::end op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("end, stacks = {}", machine.caculate_size());
#endif

        // TODO: ends the machine
      }
      
      void operator()(const ops::load& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("load = {{{}, {}, {}, {}}} stacks = {}", op.k, op.s, op.i, op.incremental, machine.caculate_size());
#endif
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

      void operator()(const ops::clear& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("clear = {{{}}}, stacks = {}", op.k, machine.caculate_size());
#endif

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

      void operator()(const ops::edit& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("edit, stacks = {}", machine.caculate_size());
#endif
        machine.edit(op.f);
      }
    
      void operator()(const ops::render& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::printf("render = {{{}, \"{}\", {}, {}}}", op.k, op.s, op.i, op.flush);
#endif
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

      void operator()(const ops::for_each& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("for_each, stacks = {}", machine.caculate_size());
#endif
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
              apply(op.body_else);
              break;
            }
          
            apply(op.body);
            i += 1;
          }
        }
      }

      void operator()(const ops::if_then_else& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("if_then_else, stacks = {}", machine.caculate_size());
#endif
        auto success = false;
        for (auto &s : machine.top_stack()) if ((success = !s.empty())) break;
        
        if ( success ) {
          if (!boost::get<ops::undefined>(&op.body_then))
            apply(op.body_then);
        } else {
          if (!boost::get<ops::undefined>(&op.body_else))
            apply(op.body_else);
        }
      }

      void operator()(const ops::switch_context& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("switch_context, stacks = {}", machine.caculate_size());
#endif
        typename Machine::template scope<details::context_cursor>
          scope(machine, op.name/*, op.inverted*/); // FIXME: `inverted` unused
        if (scope.is_valid()) {
          do { apply(op.body); } while (scope.next());
        }
      }

      void operator()(const ops::test_cursor& op) const
      {
        using limit = std::numeric_limits<decltype(ops::test_cursor::pos)>;
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::printf("test_cursor = {{{}}}, [{}, {}]", op.pos, limit::min(), limit::max());
#endif
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

      void operator()(const ops::push& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::printf("push = {{{}}}", op.k);
#endif
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

      void operator()(ops::pop op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::print("pop");
#endif
        if (op.memorize /*&& !machine.empty()*/) {
          machine.load_text(machine.top());
        }
        machine.pop();
      }

      void operator()(const ops::new_stack& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::print("new_stack");
#endif
        machine.new_stack();
      }

      void operator()(const ops::pop_stack& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::print("pop_stack");
#endif
        machine.pop_stack();
      }

      void operator()(const ops::new_regs& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::print("new_regs");
#endif
        machine.new_regs();
      }

      void operator()(const ops::pop_regs& op) const
      {
#if USE_EXTBIT_LOG && ENABLE_TRANSLATION_LOG
        std::extbit::log::print("pop_regs");
#endif
        machine.pop_regs();
      }
      
      void operator()(ops::unary op) const
      {
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("unary = {}, stacks = {}", op, machine.caculate_size());
#endif
        switch (op) {
        case ops::unary::math_negate:
          machine.top() = to_string(-to_number<std::int64_t>(machine.top()));
          return;
        case ops::unary::math_posit:
          machine.top() = to_string(to_number<std::int64_t>(machine.top()));
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
#if USE_EXTBIT_LOG && ENABLE_EXECUTION_LOG
        std::extbit::log::printf("binary = {}, stacks = {}", op, machine.caculate_size());
#endif
        assert(!machine.empty());
        using value_t = std::remove_cvref_t<decltype(machine.top())>;
        value_t rhs = machine.top(); machine.pop();

#if 0
        assert(!machine.empty());
        value_t lhs = machine.top();
#else
        value_t lhs;
        if (!machine.empty()) {
          lhs = machine.top();
        }
#endif

        switch (op) {
        case ops::binary::test_equal:
          machine.top() = to_string(lhs == rhs);
          return;
        case ops::binary::test_not_equal:
          machine.top() = to_string(lhs != rhs);
          return;
        case ops::binary::test_less:
          machine.top() = to_string(lhs < rhs);
          return;
        case ops::binary::test_less_equal:
          machine.top() = to_string(lhs <= rhs);
          return;
        case ops::binary::test_greater:
          machine.top() = to_string(lhs > rhs);
          return;
        case ops::binary::test_greater_equal:
          machine.top() = to_string(lhs >= rhs);
          return;
        case ops::binary::test_and:
          machine.top() = to_string(!lhs.empty() && !rhs.empty());
          return;
        case ops::binary::test_or:
          machine.top() = to_string(!lhs.empty() || !rhs.empty());
          return;

        case ops::binary::math_plus: 
          machine.top() = to_string(to_number<std::int64_t>(lhs) + to_number<std::int64_t>(rhs));
          return;
        case ops::binary::math_minus: 
          machine.top() = to_string(to_number<std::int64_t>(lhs) - to_number<std::int64_t>(rhs));
          return;
        case ops::binary::math_times: 
          machine.top() = to_string(to_number<std::int64_t>(lhs) * to_number<std::int64_t>(rhs));
          return;
        case ops::binary::math_divide: 
          if (auto num = to_number<std::int64_t>(rhs); num == 0) {
            machine.top() = "NaN";
          } else {
            machine.top() = to_string(to_number<std::int64_t>(lhs) / num);
          }
          return;

        case ops::binary::range: 
          {
            auto i = lhs.begin();
            auto a = to_number<std::int64_t>(i, lhs.end());
            auto j = rhs.begin();
            auto b = to_number<std::int64_t>(j, rhs.end());
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
      Machine& machine;

#if 0
      void apply(ops::op&& op) const
      {
        boost::apply_visitor(execute(*this), std::forward<ops::op>(op));
      }
#endif

      void apply(const ops::op& op) const
      {
        boost::apply_visitor(execute(*this), op);
      }

      template<typename NumType, typename String>
      static NumType to_number(const String &s)
      {
        auto i = s.begin();
        return to_number<NumType>(i, s.end());
      }
      
      template<typename NumType, typename Iterator>
      static NumType to_number(Iterator &a, const Iterator &b)
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
    };

  } // namespace details
}} // namespace mold::vm

#endif//_BOOST_MOLD_VM_DETAILS_EXECUTE_HPP_
