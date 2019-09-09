/**
 *  \file mold/domain/basic_translater.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_BASIC_TRANSLATER_HPP_
#define _BOOST_MOLD_DOMAIN_BASIC_TRANSLATER_HPP_ 1
# include <mold/vm/ops.hpp>
namespace mold { namespace domain
{

  template<typename State, typename Worker>
  struct basic_translater
  {
    template <typename NodeType>
    vm::ops::op translate(const NodeType &node)
    {
      State state;
      return Worker(state)(node);
    }
  };
  
}} // namespace mold::domain
#endif//_BOOST_MOLD_DOMAIN_BASIC_TRANSLATER_HPP_
