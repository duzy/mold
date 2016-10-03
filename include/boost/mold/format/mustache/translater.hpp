/**
 *  \file boost/mold/format/mustache/grammar.hpp
 *
 *  Copyright 2016 Duzy Chan <code@duzy.info>
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_FORMAT_MUSTACHE_TRANSLATER_HPP_
#define _BOOST_MOLD_FORMAT_MUSTACHE_TRANSLATER_HPP_ 1
# include <boost/mold/format/mustache/ast.hpp>
# include <boost/mold/interpreter/ops.hpp>
# include <boost/mold/edit/unescape_html.hpp>
namespace boost { namespace mold { namespace format { namespace mustache
{

  struct translation_visitor;
  
  struct translater
  {
    template <typename NodeType>
    interpreter::ops::op translate(const NodeType &);
  };

  struct translation_visitor
  {
    using result_type = interpreter::ops::op;

    explicit translation_visitor(translater &tr, std::string &whitespace)
      : trans(tr), whitespace(whitespace) {}
    
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

    result_type operator()(const ast::undefined &) const
    {
      throw std::runtime_error("undefined ast");
    }

    result_type operator()(const ast::comment &) const
    {
      return interpreter::ops::nop{};
    }

    result_type operator()(const ast::literal_text &lit) const
    {
      //std::clog << "literal_text: " << lit << std::endl;
      
      interpreter::ops::op_list ops{
        interpreter::ops::render_text{ lit },
      };
      return ops;
    }

    result_type operator()(const ast::blank_text &blank) const
    {
      //std::clog << "blank_text: " << blank.size() << std::endl;
      
      whitespace += blank;

      return interpreter::ops::render_text{ blank };
    }

    result_type operator()(const ast::eol &eol) const
    {
      //std::clog << "eol: " << v.size() << std::endl;

      whitespace += eol;
      whitespace.clear();
      
      return interpreter::ops::render_text{ eol };
    }

    result_type operator()(const ast::variable &var) const
    {
      //std::clog << "variable: " << var.name << std::endl;
      
      interpreter::ops::op_list ops{
        interpreter::ops::load{ var.name, false },
      };
      if (var.unescaped) {
        ops.push_back(interpreter::ops::edit{ edit::unescape_html() });
      }
      ops.push_back(interpreter::ops::render{ true });
      return ops;
    }

    result_type operator()(const ast::partial &par) const
    {
      interpreter::ops::op_list ops{
        interpreter::ops::load_io{ par, false },
        interpreter::ops::render{ true },
      };
      return ops;
    }

    result_type operator()(const ast::section &sec) const
    {
      //std::clog << "section: " << sec.name << ", " << sec.inverted << ", " << sec.nodes.size() << std::endl;
      
      interpreter::ops::op_list body;
      for (auto const &n : sec.nodes) {
        body.push_back(boost::apply_visitor(*this, n));
      }
      return interpreter::ops::switch_context{ sec.name, sec.inverted, body };
    }
   
  private:
    translater &trans;
    std::string &whitespace; // per line whitespace cache
  };

  template <typename NodeType>
  interpreter::ops::op translater::translate(const NodeType &node)
  {
    std::string whitespace;
    translation_visitor trans(*this, whitespace);
    return trans(node);
  }
  
}}}} // namespace boost::mold::format::mustache
#endif//_BOOST_MOLD_FORMAT_MUSTACHE_TRANSLATER_HPP_
