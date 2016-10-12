/**
 *  \file load.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_LOAD_HPP_
#define _BOOST_MOLD_LOAD_HPP_ 1
# include <boost/mold/vm/compile.hpp>
namespace boost { namespace mold
{
  using template_data = vm::ops::op;
  
  template <typename Format, typename Iterator>
  template_data load(Iterator &begin, const Iterator &end)
  {
    return vm::compile<Format>(begin, end);
  }

  template <typename Format, typename Raw>
  template_data load(const Raw &s)
  {
    auto i = s.begin();
    return vm::compile<Format>(i, s.end());
  }
  
}} // namespace boost::mold

#endif//_BOOST_MOLD_LOAD_HPP_
