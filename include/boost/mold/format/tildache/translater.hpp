/**
 *  \file boost/mold/format/tildache/translater.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_TILDACHE_TRANSLATER_HPP_
#define _BOOST_MOLD_FORMAT_TILDACHE_TRANSLATER_HPP_ 1
# include <boost/mold/format/mustache/translater.hpp>
# include <boost/mold/format/tildache/ast.hpp>
namespace boost { namespace mold { namespace format { namespace tildache
{

  struct translation_visitor : mustache::translation_visitor
  {
    explicit translation_visitor(mustache::translation_state &state)
      : mustache::translation_visitor(state) {}
    
    result_type operator()(const ast::node_list &l) const
    {
      interpreter::ops::op_list ops;
      for (auto const &n : l) {
        ops.push_back(boost::apply_visitor(*this, n));
      }
      return ops;
    }

    result_type operator()(const ast::node &n) const
    {
      return boost::apply_visitor(*this, n);
    }

    result_type operator()(const ast::mustache_node &n) const
    {
      auto base = static_cast<const mustache::translation_visitor*>(this);
      return boost::apply_visitor(*base, n);
    }

    result_type operator()(const ast::statement &n) const
    {
      return interpreter::ops::nop{};
    }

    result_type operator()(const ast::expression &n) const
    {
      return interpreter::ops::nop{};
    }

    result_type operator()(const ast::mustache_section &n) const
    {
      return interpreter::ops::nop{};
    }

    result_type operator()(const ast::tild_section &n) const
    {
      return interpreter::ops::nop{};
    }
    
  }; // translation_visitor

  using translater = details::basic_translater<
    mustache::translation_state, translation_visitor>;

}}}} // namespace boost::mold::format::tildache
#endif//_BOOST_MOLD_FORMAT_TILDACHE_TRANSLATER_HPP_
