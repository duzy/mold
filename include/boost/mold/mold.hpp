/**
 *  \file load.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_MOLD_HPP_
#define _BOOST_MOLD_MOLD_HPP_ 1
# include <boost/mold/load.hpp>
# include <boost/mold/generate.hpp>
namespace boost { namespace mold
{

  template<
    typename Translater,
    typename SpecType,
    typename AstType,
    const SpecType &(Ctor)()>
  struct spec
  {
    using translater = Translater;
    using spec_type = SpecType;
    using ast_type = AstType;
    static const spec_type &get() { return Ctor(); }
  };
  
}} // namespace boost::mold

#endif//_BOOST_MOLD_MOLD_HPP_
