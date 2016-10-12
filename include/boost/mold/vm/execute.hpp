/**
 *  \file execute.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VM_EXECUTE_HPP_
#define _BOOST_MOLD_VM_EXECUTE_HPP_ 1
# include <boost/mold/vm/details/execution_machine.hpp>
# include <boost/mold/vm/details/execution_visitor.hpp>

namespace boost { namespace mold { namespace vm
{

  template <typename Stream, typename Template, typename Context>
  void execute(Stream &stream, Template const &t, Context const &context)
  {
    using machine_type = details::execution_machine<Stream>;
    machine_type machine(stream, context);
    details::execution_visitor<machine_type> exec(machine);
    exec(t);
  }
  
}}} // namespace boost::mold::vm

#endif//_BOOST_MOLD_VM_EXECUTE_HPP_
