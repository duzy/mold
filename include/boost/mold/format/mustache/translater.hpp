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

  struct translation_state
  {
    int inline_directives; // counting directives (inline) section/conditional tags
    int inline_entities; // counting entities that renders, e.g. text/variables.
    std::string whitespace; // whitespace cached
  };
  
  struct translation_visitor
  {
    using result_type = interpreter::ops::op;

    explicit translation_visitor(translater &tr, translation_state &state)
      : trans(tr), state(state) {}
    
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
      // This should never happen!
      throw std::runtime_error("undefined ast");
    }

    result_type operator()(const ast::comment &) const
    {
      return interpreter::ops::nop{};
    }

    result_type operator()(const ast::blank_text &blank) const
    {
      // Caches whitespaces to decide rendering later.
      state.whitespace += blank;

      return interpreter::ops::nop{};
    }

    result_type operator()(const ast::eol &eol) const
    {
      // If there's only a standalone directive in the line.
      if (state.inline_directives == 1 && state.inline_entities == 0) {
        state.inline_directives = 0;
        state.whitespace.clear();
        return interpreter::ops::nop(); // Do nothing.
      }
      
      state.whitespace += eol;
      state.inline_directives = 0;
      state.inline_entities = 0;
      
      auto op = interpreter::ops::render_text{ state.whitespace };

      state.whitespace.clear();
      
      return op;
    }

    result_type operator()(const ast::eoi & /*unused*/) const
    {
      state.inline_directives = 0;
      state.inline_entities = 0;

      if (state.whitespace.empty()) {
        return interpreter::ops::end{};
      }

      auto ops = interpreter::ops::op_list{
        interpreter::ops::render_text{ state.whitespace },
        interpreter::ops::end{},
      };
      
      state.whitespace.clear();
      
      return ops;
    }

    result_type operator()(const ast::literal_text &lit) const
    {
      interpreter::ops::op_list ops;

      state.inline_entities += 1;

      if (0 < state.whitespace.size()) {
        ops.push_back(interpreter::ops::render_text{ state.whitespace });
        state.whitespace.clear();
      }
      
      ops.push_back(interpreter::ops::render_text{ lit });
      return ops;
    }

    result_type operator()(const ast::variable &var) const
    {
      interpreter::ops::op_list ops;

      state.inline_entities += 1;
      
      auto has_spaces = 0 < state.whitespace.size();
      if ( has_spaces ) {
        ops.push_back(interpreter::ops::load_text{ state.whitespace, false });
        state.whitespace.clear();
      }

      ops.push_back(interpreter::ops::load{ var.name, has_spaces });
      
      if (var.unescaped) {
        ops.push_back(interpreter::ops::edit{ edit::unescape_html() });
      }
      
      ops.push_back(interpreter::ops::render{ true });
      return ops;
    }

    result_type operator()(const ast::section &sec) const
    {
      // Counting section begin directive.
      state.inline_directives += 1;
      
      interpreter::ops::op_list body;
      for (auto const &n : sec.nodes) {
        body.push_back(boost::apply_visitor(*this, n));
      }

      // Counting section end directive.
      state.inline_directives += 1;
      
      return interpreter::ops::switch_context{ sec.name, sec.inverted, body };
    }

    result_type operator()(const ast::partial &par) const
    {
      // TODO: load file and compile it into a `body`
      return interpreter::ops::render_text{ "<partial:"+par+">" };
    }

  private:
    translater &trans;
    translation_state &state;
  };

  template <typename NodeType>
  interpreter::ops::op translater::translate(const NodeType &ast)
  {
    translation_state state{ 0, 0 };
    translation_visitor trans(*this, state);
    return trans(ast);
  }
  
}}}} // namespace boost::mold::format::mustache
#endif//_BOOST_MOLD_FORMAT_MUSTACHE_TRANSLATER_HPP_
