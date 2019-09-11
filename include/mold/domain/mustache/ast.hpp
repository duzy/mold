/**
 *  \file ast/def.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_
#define _BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_ 1
# include <boost/spirit/home/x3/support/ast/variant.hpp>
# include <boost/fusion/include/adapt_struct.hpp>
# include <forward_list>
# include <string>
namespace boost { namespace spirit { namespace x3 { namespace traits
{
  template <typename T>
  struct push_back_container<std::forward_list<T>,void>
  {
    static bool call(std::forward_list<T>& list, T&& val)
    {
      auto before_end = list.before_begin();
      for (auto& _ : list) ++before_end; // TODO: it's O(n)!!!
      list.insert_after(before_end, std::forward<T>(val));
      return true;
    }
  };
}}}} // boost::spirit::x3::traits
namespace mold { namespace domain { namespace mustache { namespace ast
{

  struct undefined {};

  struct comment {};

  struct identifier : std::string
  {
    using std::string::string;
    using std::string::operator=;
  };
  
  struct literal_text : std::string 
  {
    using std::string::string;
    using std::string::operator=;
  };

  struct blank_text : std::string 
  {
    using std::string::string;
    using std::string::operator=;
  };

  struct eol : std::string
  {
    using std::string::string;
    using std::string::operator=;
  };

  struct eoi { /* end of input */ };
  
  struct variable
  {
    bool unescaped;
    identifier name;
  };

  struct partial : identifier 
  {
    using identifier::identifier;
    using identifier::operator=;
  };

  struct section;

  struct node : boost::spirit::x3::variant<
    undefined, comment, literal_text, blank_text,
    variable, partial, eol, eoi,
    boost::spirit::x3::forward_ast<section>>
  {
    node() : base_type() {}
    template<typename V> // use `explicit` to avoid ambiguous
    explicit node(V&& v) : base_type(std::forward<V>(v)) {}
    template<typename V> node& operator=(V&& v) {
      this->base_type::operator=(std::forward<V>(v));
      return *this; }
    //node& operator=(const node& other) {
    //  this->base_type::operator=(other);
    //  return *this; }
  };

  using node_list = std::vector<node>;

  static_assert(boost::spirit::x3::traits::is_container<node_list>::value,
                "node_list is not container");

  struct section
  {
    bool inverted;
    identifier name;
    node_list nodes;
  };
  
}}}} // namespace mold::domain::mustache::ast

BOOST_FUSION_ADAPT_STRUCT(mold::domain::mustache::ast::variable, unescaped, name)
BOOST_FUSION_ADAPT_STRUCT(mold::domain::mustache::ast::section, inverted, name, nodes)

#endif//_BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_
