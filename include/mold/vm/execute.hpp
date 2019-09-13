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
# include <mold/vm/details/machine.hpp>
# include <mold/vm/details/execute.hpp>

namespace mold { namespace vm
{

  template <typename Stream, typename Template, typename Context>
  void execute(Stream& stream, Template&& t, const Context& context)
  {
    using machine = details::machine<Stream>;
    using executor = details::execute<machine>;
    machine mach(stream, context);
    executor(mach).call(t);
  }

}} // namespace mold::vm

#endif//_BOOST_MOLD_VM_EXECUTE_HPP_
