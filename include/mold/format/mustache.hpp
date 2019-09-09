/**
 *  \file boost/mold/format/mustache.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_MUSTACHE_HPP_
#define _BOOST_MOLD_FORMAT_MUSTACHE_HPP_ 1
# include <mold/domain/mustache/translater.hpp>
# include <mold/domain/mustache/parser_def.hpp>
namespace mold { namespace format
{

  using mustache = domain::spec<
    domain::mustache::translater,
    domain::mustache::parser::mustache_type,
    domain::mustache::ast::node_list,
    domain::mustache::spec>;
  
}} // namespace mold::format
#endif//_BOOST_MOLD_FORMAT_MUSTACHE_HPP_
