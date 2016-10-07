/**
 *  \file boost/mold/format/mustache/grammar.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_MUSTACHE_PARSER_HPP_
#define _BOOST_MOLD_FORMAT_MUSTACHE_PARSER_HPP_ 1
#include <boost/mold/format/mustache/ast.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/nonterminal/rule.hpp>
namespace boost { namespace mold { namespace format { namespace mustache
{
  namespace parser
  {
    namespace x3 = boost::spirit::x3;
    using mustache_type = x3::rule<struct mustache_class, ast::node_list>;
    BOOST_SPIRIT_DECLARE(mustache_type)
  }

  const parser::mustache_type &spec();
  
}}}} // namespace boost::mold::format::mustache

#define BOOST_MOLD_MUSTACHE_INSTANTIATE(iterator_type)    \
  namespace boost { namespace mold { namespace format { namespace mustache { namespace parser \
  { BOOST_SPIRIT_INSTANTIATE(mustache_type, iterator_type, mold::value) }}}}}

#endif//_BOOST_MOLD_FORMAT_MUSTACHE_PARSER_HPP_
