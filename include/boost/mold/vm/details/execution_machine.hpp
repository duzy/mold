/**
 *  \file execution_machine.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VM_DETAILS_EXECUTION_MACHINE_HPP_
#define _BOOST_MOLD_VM_DETAILS_EXECUTION_MACHINE_HPP_ 1
# include <boost/mold/vm/ops.hpp>
# include <boost/mold/value.hpp>
# include <stack>
# include <list>
# include <iostream>
namespace boost { namespace mold { namespace vm
{
  namespace details
  {
    struct cursor_interface
    {
      virtual const value *next() = 0;
      virtual bool is_valid() const = 0;
      virtual bool is_first() const = 0;
      virtual bool is_last() const = 0;
      virtual ~cursor_interface() {}
      const value *get(array::const_reverse_iterator i) { return &(*i); }
      const value *get(array::const_iterator i) { return &(*i); }
      const value *get(object::const_reverse_iterator i) { return &i->second; }
      const value *get(object::const_iterator i) { return &i->second; }
    };

    struct local final
    {
      const value *context = nullptr;
      cursor_interface *cursor = nullptr;
      unsigned position = 0;
      bool forward()
      {
        if ( cursor && (context = cursor->next()) ) {
          position += 1;
          return true;
        }
        return false;
      }
    };

    template<template<typename Iterator, typename Interface> class Cursor>
    struct local_initialize
    {
      template<typename Iterator>
      using cursor_type = Cursor<Iterator, cursor_interface>;
      
      local &l;
      bool reverse;
      explicit local_initialize(local &l, bool b) : l(l), reverse(b)
      {
        assert(l.cursor == nullptr && "cursor already initialized");
        if (l.context == nullptr) return;
        boost::apply_visitor(*this, *l.context);
      }
      
      void operator()(const nil_t &s) {}
      void operator()(const std::string &s) {}
      void operator()(const object &o) { this->create_cursor(o); }
      void operator()(const array &a) { this->create_cursor(a); }

      template<typename T>
      void create_cursor(const T &v)
      {
        if (reverse) {
          using ct = cursor_type<typename T::const_reverse_iterator>;
          std::unique_ptr<ct> cursor(new ct());
          l.context = cursor->init(v.rbegin(), v.rend());
          l.cursor = cursor.release();
        } else {
          using ct = cursor_type<typename T::const_iterator>;
          std::unique_ptr<ct> cursor(new ct());
          l.context = cursor->init(v.begin(), v.end());
          l.cursor = cursor.release();
        }
      }
    };

    template <typename Stream>
    struct execution_machine
    {
      execution_machine(Stream &stream, const value &v)
        : stream(stream)
        , root_context(v)
        , lookup({{ &root_context }})
      {
        regs.resize(8);
      }

      const value *find_var(const std::string &name) const
      {
        for (auto lit = lookup.rbegin(); lit != lookup.rend(); ++lit) {
          if ( auto o = boost::get<object>(lit->context) ) {
            auto it = o->find(name);
            if ( it != o->end() ) {
              return &it->second;
            }
          }
        }
        return nullptr;
      }
      
      const std::string *get_var_text(const std::string &name) const
      {
        if ( auto v = find_var(name) ) {
          return boost::get<std::string>(v);
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
        return find_var(name) != nullptr;
      }

      bool get_cursor_position() const
      {
        return lookup.back().position;
      }
      
      bool is_cursor_first() const
      {
        auto &top = lookup.back();
        return top.cursor && top.cursor->is_first();
      }

      bool is_cursor_last() const
      {
        auto &top = lookup.back();
        return top.cursor && top.cursor->is_last();
      }

      std::list<std::string> &top_stack() { return stack.top(); }
      void new_stack() { stack.push(std::list<std::string>{}); }
      void push_stack(const std::list<std::string> &s) { stack.push(s); }
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

      template<template<typename Iterator, typename Interface> class Cursor>
      struct scope
      {
        execution_machine &m;
        local &lookup;
        
        scope(execution_machine &m, const std::string &name, bool reverse)
          : m(m), lookup(m.new_lookup(name))
        {
          local_initialize<Cursor> init(lookup, reverse);
          (void) init;
        }
        
        ~scope()
        {
          delete lookup.cursor;
          lookup.cursor = nullptr;
          m.pop_lookup();
        }

        bool is_valid() const
        {
          return lookup.cursor && lookup.cursor->is_valid();
        }
        
        bool next()
        {
          return lookup.forward();
        }
        
      private:
        scope(const scope &) = delete;
        void operator=(const scope &) = delete;
      }; // struct scope

    private:
      Stream &stream;
      const value root_context;
      std::list<local> lookup;
      std::stack<std::list<std::string>> stack;
      std::vector<std::string> regs;
      std::string memory;

      local &new_lookup(const std::string &name)
      {
        // Create new lookup.
        lookup.push_back({ find_var(name) });
        return lookup.back();
      }

      void pop_lookup() { lookup.pop_back(); }
    };
  
  } // namespace details
}}} // namespace boost::mold::vm

#endif//_BOOST_MOLD_VM_DETAILS_EXECUTION_MACHINE_HPP_
