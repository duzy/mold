/**
 *  \file boost/mold/format/tildache.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_TILDACHE_HPP_
#define _BOOST_MOLD_FORMAT_TILDACHE_HPP_ 1
# include <boost/mold/format/tildache/translater.hpp>
# include <boost/mold/format/tildache/parser_def.hpp>
namespace boost { namespace mold { namespace format
{

  using tildache_spec = spec<
    tildache::translater,
    tildache::parser::tildache_type,
    tildache::ast::node_list,
    tildache::spec>;
  
}}} // namespace boost::mold::format
#endif//_BOOST_MOLD_FORMAT_TILDACHE_HPP_
