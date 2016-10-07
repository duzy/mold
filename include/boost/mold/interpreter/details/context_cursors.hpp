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

  template<typename Iterator, typename Interface>
  struct context_cursor : Interface
  {
    Iterator beg, cur, end;
    const value *init(Iterator a, Iterator b) /*final*/
    {
      beg = cur = a, end = b;
      return Interface::get(cur);
    }
    virtual const value *next() override
    {
      if (cur != end && ++cur != end) {
        return Interface::get(cur);
      }
      return nullptr;
    }
    virtual bool is_valid() const override { return cur != end; }
    virtual bool is_first() const override { return cur == beg; }
    virtual bool is_last() const override 
    {
      auto i = cur; ++i;
      return cur != end && i == end; 
    }
  };
      
}}}} // namespace boost::mold::interpreter::details

#endif//_BOOST_MOLD_INTERPRETER_DETAILS_CONTEXT_CURSORS_HPP_
