/**
 *  \file ast/def.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_
#define _BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_ 1
# include <boost/spirit/home/x3/support/ast/variant.hpp>
# include <boost/fusion/include/adapt_struct.hpp>
# include <string>
# include <vector>
namespace boost { namespace mold { namespace domain { namespace mustache { namespace ast
{
  struct node_list;

  struct undefined {};

  struct comment {};

  struct identifier : std::string
  {
    //using std::string::string;
    //using std::string::operator=;
  };
  
  struct literal_text : std::string 
  {
    //using std::string::string;
    //using std::string::operator=;
  };

  struct blank_text : std::string 
  {
    //using std::string::string;
    //using std::string::operator=;
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
    using std::string::string;
    using std::string::operator=;
  };

  struct section;
  
  struct node : boost::spirit::x3::variant<
      undefined
    , comment
    , literal_text
    , blank_text
    , eol
    , eoi
    , variable
    , partial
    , boost::spirit::x3::forward_ast<section>
    >
  {
    node() : base_type() {}
    node(comment const & rhs) : base_type(rhs) {}
    node(literal_text const & rhs) : base_type(rhs) {}
    node(blank_text const & rhs) : base_type(rhs) {}
    node(eol const & rhs) : base_type(rhs) {}
    node(eoi const & rhs) : base_type(rhs) {}
    node(variable const & rhs) : base_type(rhs) {}
    node(partial const & rhs) : base_type(rhs) {}
    node(section const & rhs);
  };

  struct node_list : std::vector<node> {};

  struct section
  {
    bool inverted;
    identifier name;
    node_list nodes;
  };

  inline node::node(section const & rhs) : base_type(rhs) {}
  
}}}}} // namespace boost::mold::domain::mustache::ast

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::domain::mustache::ast::variable,
   (bool, unescaped)
   (boost::mold::domain::mustache::ast::identifier, name)
)

BOOST_FUSION_ADAPT_STRUCT(
   boost::mold::domain::mustache::ast::section,
   (bool, inverted)
   (boost::mold::domain::mustache::ast::identifier, name)
   (boost::mold::domain::mustache::ast::node_list, nodes)
)

#endif//_BOOST_MOLD_DOMAIN_MUSTACHE_AST_HPP_
