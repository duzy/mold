/**
 *  \file compile.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VM_COMPILE_HPP_
#define _BOOST_MOLD_VM_COMPILE_HPP_ 1
# include <mold/vm/ops.hpp>
# include <boost/spirit/home/x3/core/parse.hpp>

namespace mold { namespace vm
{

  template <typename Format, typename Iterator>
  ops::op compile(Iterator &it, const Iterator &end)
  {
    typename Format::ast_type ast;
    if (boost::spirit::x3::parse(it, end, Format::get(), ast)
        && it == end) {
      typename Format::translater t;
      return t.translate(ast);
    }
    // TODO: error handling...
    return ops::op{};
  }
  
}} // namespace mold::vm

#endif//_BOOST_MOLD_VM_COMPILE_HPP_
