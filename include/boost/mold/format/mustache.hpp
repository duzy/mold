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
# include <boost/mold/format/mustache/parser.hpp>
# include <boost/mold/format/mustache/translater.hpp>
namespace boost { namespace mold { namespace format
{
  
  struct mustache_spec
  {
    using translater = mustache::translater;
    using spec_type = mustache::parser::mustache_type;
    using ast_type = mustache::ast::node_list;
    static const spec_type &spec() { return mustache::spec(); }
  };
  
}}} // namespace boost::mold::format
#endif//_BOOST_MOLD_FORMAT_MUSTACHE_HPP_
