/**
 *  \file execution_machine.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_MACHINE_HPP_
#define _BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_MACHINE_HPP_ 1
# include <boost/mold/interpreter/ops.hpp>
# include <boost/mold/value.hpp>
# include <stack>
# include <list>
namespace boost { namespace mold { namespace interpreter
{
  namespace details
  {
    template <typename Stream>
    struct execution_machine
    {
      execution_machine(Stream &stream, const value &v)
        : stream(stream)
        , context(v)
        , current(&context)
      {
        regs.resize(8);
      }

      const std::string *get_var_text(const std::string &name) const
      {
        if ( auto o = boost::get<object>(current) ) {
          auto it = o->find(name);
          if ( it != o->end() ) {
            return boost::get<std::string>(&it->second);
          }
        }
        return nullptr;
      }
      
      template<typename T>
      void render(const T &v) const
      {
        stream << v ;
      }

      void render() const
      {
        stream << memory ;
      }

      bool render_var(const std::string &name) const
      {
        if ( auto s = get_var_text(name) ) {
          stream << *s ;
          return true;
        }
        return false;
      }

      bool render_reg(unsigned int index) const
      {
        if (index < regs.size()) {
          stream << regs[index];
          return true;
        }
        return false;
      }

      bool render_stack() const
      {
        for (auto &s : stack.top()) stream << s;
        return !stack.top().empty();
      }
      
      void clear_memory()
      {
        memory.clear();
      }

      void edit(const std::function<void(std::string &)> &edit)
      {
        edit(memory);
      }

      void load_text(const std::string &s)
      {
        memory += s;
      }
      
      bool load_var(const std::string &name)
      {
        if ( auto s = get_var_text(name) ) {
          memory += *s;
          return true;
        }
        return false;
      }

      void load_stack()
      {
        for (auto &s : stack.top()) memory += s;
      }

      bool load_reg(unsigned int index)
      {
        if (index < regs.size()) {
          memory += regs[index];
          return true;
        }
        return false;
      }

      bool has(const std::string &name)
      {
        auto o = boost::get<object>(current);
        return o && o->find(name) != o->end();
      }

      unsigned int get_cursor_position() const
      {
        return cursor_position;
      }

      unsigned int get_cursor_stop() const
      {
        return cursor_stop;
      }

      void new_stack() { stack.push(std::list<std::string>{}); }
      void pop_stack() { stack.pop(); }
      
      void clear_stack() { stack.top().clear(); }
      
      std::string &front() { return stack.top().front(); }
      void shift(const std::string &s) { stack.top().push_front(s); }
      void unshift() { stack.top().pop_front(); }

      std::string &top() { return stack.top().back(); }
      void pop() { stack.top().pop_back(); }
      void push() { stack.top().push_back(memory); }
      void push(const std::string &s) { stack.top().push_back(s); }
      void push_reg(unsigned int i) { stack.top().push_back(reg(i)); }
      void push_var(const std::string &name) 
      {
        if ( auto s = get_var_text(name) ) {
          stack.top().push_back(*s);
        }
      }

      auto size() const { return stack.top().size(); } // expensive with list
      auto empty() const { return stack.top().empty(); }
      
      std::string &reg(unsigned n) { return regs[0]; }
      
      template <typename Cursor>
      struct scope
      {
        const value *saved;
        execution_machine &m;
        Cursor cursor;
        scope(execution_machine &m, const std::string &name)
          : saved(m.current), m(m), cursor()
        {
          m.cursor_position = m.cursor_stop = 0;
          if ( name == "." ) {
            // TODO: ...
            return;
          }
          if ( auto o = boost::get<object>(m.current) ) {
            auto it = o->find(name);
            if ( it != o->end() ) {
              if ( auto a = boost::get<array>(&it->second) ) {
                if (!a->empty()) {
                  m.current = cursor.init(a);
                  m.cursor_position = 0;
                  m.cursor_stop = a->size();
                  // std::clog
                  //   << "scope: enter: "
                  //   << name << ", "
                  //   << m.current << ", " << saved
                  //   << std::endl;
                }
              }
            }
          }
        }
        
        ~scope()
        {
          // std::clog
          //   << "scope: exit: "
          //   << m.current << ", " << saved
          //   << std::endl;
          m.current = saved;
        }

        bool is_valid() const
        {
          return cursor.is_valid();
        }
        
        bool next()
        {
          if ( auto c = cursor.next() ) {
            m.cursor_position += 1;
            m.current = c;
            return true;
          }
          return false;
        }
        
      private:
        scope(const scope &) = delete;
        void operator=(const scope &) = delete;
      }; // struct scope

    private:
      Stream &stream;
      const value context;
      const value*current;
      unsigned int cursor_position;
      unsigned int cursor_stop;
      std::string memory;
      std::vector<std::string> regs;
      std::stack<std::list<std::string>> stack;
    };
  
  } // namespace details
}}} // namespace boost::mold::interpreter

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_MACHINE_HPP_
