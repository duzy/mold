/**
 *  \file boost/mold/domain/details/basic_translater.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_DETAILS_BASIC_TRANSLATER_HPP_
#define _BOOST_MOLD_DOMAIN_DETAILS_BASIC_TRANSLATER_HPP_ 1
# include <boost/mold/interpreter/ops.hpp>
namespace boost { namespace mold { namespace domain { namespace details
{

  template<typename State, typename Worker>
  struct basic_translater
  {
    template <typename NodeType>
    interpreter::ops::op translate(const NodeType &ast)
    {
      State state;
      Worker trans(state);
      return trans(ast);
    }
  };
  
}}}} // namespace boost::mold::domain::details
#endif//_BOOST_MOLD_DOMAIN_DETAILS_BASIC_TRANSLATER_HPP_

