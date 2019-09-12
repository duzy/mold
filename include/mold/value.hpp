/**
 *  \file value.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_VALUE_HPP_
#define _BOOST_MOLD_VALUE_HPP_ 1
# include <boost/spirit/home/x3/support/ast/variant.hpp>
# include <string>
# include <vector>
# include <unordered_map>
namespace mold
{

  //template <typename Type>
  //using forward = boost::spirit::x3::forward_ast<Type>;

  //template <typename ...Types>
  //using variant = boost::spirit::x3::variant<Types...>;
  
  struct value;

  using object = std::unordered_map<std::string, value>;
  using array = std::vector<value>;
  using nil_t = std::monostate;

  struct value : boost::spirit::x3::variant<
      nil_t
    , std::string
    , object
    , array
    >
  {
    value() : base_type() {}
    value(const nil_t &v) : base_type(v) {}
    value(const char *s) : base_type(std::string{s}) {}
    value(const std::string &v) : base_type(v) {}
    value(const object &v) : base_type(v) {}
    value(const array &v) : base_type(v) {}
  };

} // namespace mold

#endif//_BOOST_MOLD_VALUE_HPP_
