/**
 *  \file mold.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_MOLD_HPP_
#define _BOOST_MOLD_MOLD_HPP_ 1
# include <boost/mold/interpreter/compile.hpp>
# include <boost/mold/interpreter/execute.hpp>
namespace boost { namespace mold
{

  template <typename Format, typename Iterator>
  interpreter::ops::op load(Iterator &begin, const Iterator &end)
  {
    return interpreter::compile<Format>(begin, end);
  }

  template <typename Stream, typename Context>
  void generate(Stream &stream, interpreter::ops::op const &op,
      Context const &context)
  {
    interpreter::execute(stream, op, context);
  }
  
}} // namespace boost::mold

#endif//_BOOST_MOLD_MOLD_HPP_
