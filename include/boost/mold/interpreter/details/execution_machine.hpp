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

namespace boost { namespace mold { namespace interpreter
{
  namespace details
  {
    
    template <typename Stream, typename Context>
    struct execution_machine
    {
      execution_machine(Stream &stream, const Context &context)
        : stream(stream)
        , context(context)
      {
      }

      template<typename T>
      void render(const T &v) const
      {
        stream << v ;
      }

      void render_memory() const
      {
        stream << memory ;
      }

      void clear_memory()
      {
        memory.clear();
      }
    
    private:
      Stream &stream;
      const Context &context;
      std::string memory;
    };
  
  } // namespace details
}}} // namespace boost::mold::interpreter

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_EXECUTION_MACHINE_HPP_
