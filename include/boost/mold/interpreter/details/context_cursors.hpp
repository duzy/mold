/**
 *  \file context_cursors.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_INTERPRETER_DETAILS_CONTEXT_CURSORS_HPP_
#define _BOOST_MOLD_INTERPRETER_DETAILS_CONTEXT_CURSORS_HPP_ 1
# include <boost/mold/value.hpp>
namespace boost { namespace mold { namespace interpreter { namespace details
{

  template<typename Iterator, typename Base, typename Derived>
  struct cursor : Base
  {
    Iterator cur, end;
    virtual const value *init(const array *a) override
    {
      auto that = static_cast<Derived*>(this);
      cur = that->begin(a);
      end = that->end(a);
      return &(*cur);
    }
    virtual bool is_valid() const override
    {
      return cur != end;
    }
    virtual const value *next() override
    {
      if (cur != end && ++cur != end) {
        return &( *cur );
      }
      return nullptr;
    }
  };
  
  template<typename Base>
  struct context_cursor : cursor<array::const_iterator, Base, context_cursor<Base>>
  {
    auto begin(const array *a) { return a->begin(); }
    auto end(const array *a) { return a->end(); }
  };

  template<typename Base>
  struct context_reverse_cursor : cursor<array::const_reverse_iterator, Base, context_reverse_cursor<Base>>
  {
    auto begin(const array *a) { return a->rbegin(); }
    auto end(const array *a) { return a->rend(); }
  };
      
}}}} // namespace boost::mold::interpreter::details

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_CONTEXT_CURSORS_HPP_
