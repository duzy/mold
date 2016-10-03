/**
 *  \file boost/mold/format/mustache/grammar.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_MUSTACHE_HPP_
#define _BOOST_MOLD_FORMAT_MUSTACHE_HPP_ 1
# include <boost/mold/format/mustache/translater.hpp>
# include <boost/mold/format/mustache/parser_def.hpp>
namespace boost { namespace mold { namespace format
{

  using mustache_spec = spec<
    mustache::translater,
    mustache::parser::mustache_type,
    mustache::ast::node_list,
    mustache::spec>;
  
}}} // namespace boost::mold::format
#endif//_BOOST_MOLD_FORMAT_MUSTACHE_HPP_
