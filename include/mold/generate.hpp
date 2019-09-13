/**
 *  \file generate.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_GENERATE_HPP_
#define _BOOST_MOLD_GENERATE_HPP_ 1
# include <mold/vm/execute.hpp>
namespace mold
{

  template <typename Stream, typename Context>
  inline void
  generate(Stream& out, vm::ops::op&& op, const Context& ctx)
  {
    vm::execute(out, std::move(op), ctx);
  }
  
} // namespace mold

#endif//_BOOST_MOLD_GENERATE_HPP_
