/**
 *  \file boost/mold/domain/tildache/parser.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@extbit.io>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_TILDACHE_PARSER_HPP_
#define _BOOST_MOLD_DOMAIN_TILDACHE_PARSER_HPP_ 1
#include <mold/domain/tildache/ast.hpp>
#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/traits/tuple_traits.hpp>
#include <boost/spirit/home/x3/nonterminal/rule.hpp>
namespace mold { namespace domain { namespace tildache
{
  namespace parser
  {
    namespace x3 = boost::spirit::x3;
    using tildache_type = x3::rule<struct tildache_class, ast::node_list>;
    BOOST_SPIRIT_DECLARE(tildache_type)
  }

  const parser::tildache_type &spec();
  
}}} // namespace mold::domain::tildache

#define BOOST_MOLD_TILDACHE_INSTANTIATE(iterator_type)    \
  namespace mold { namespace domain { namespace tildache { namespace parser \
  { BOOST_SPIRIT_INSTANTIATE(tildache_type, iterator_type, ::mold::value) }}}}

#endif//_BOOST_MOLD_DOMAIN_TILDACHE_PARSER_HPP_
