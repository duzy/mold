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

      void clear()
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
        if ( auto o = boost::get<object>(current) ) {
          auto it = o->find(name);
          if ( it != o->end() ) {
            auto s = boost::get<std::string>(&it->second);
            if ( s ) {
              memory += *s;
              return true;
            }
          }
        }
        return false;
      }

      bool has(const std::string &name)
      {
        auto o = boost::get<object>(current);
        return o && o->find(name) != o->end();
      }

      template <typename Cursor>
      struct scope
      {
        const value *saved;
        execution_machine &m;
        Cursor cursor;
        scope(execution_machine &m, const std::string &name)
          : saved(m.current), m(m), cursor()
        {
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
      std::string memory;
    };
  
  } // namespace details
}}} // namespace boost::mold::interpreter

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_MACHINE_HPP_
