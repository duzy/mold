/**
 *  \file boost/mold/format/tildache.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@extbit.io>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_TILDACHE_HPP_
#define _BOOST_MOLD_FORMAT_TILDACHE_HPP_ 1
# include <mold/domain/tildache/translater.hpp>
# include <mold/domain/tildache/parser.hpp>
namespace mold { namespace format
{

  using tildache = spec<
    domain::tildache::translater,
    domain::tildache::parser::tildache_type,
    domain::tildache::ast::node_list,
    domain::tildache::spec>;
  
}} // namespace mold::format
#endif//_BOOST_MOLD_FORMAT_TILDACHE_HPP_
