/**
 *  \file encode.hpp
 *
 *  Copyright 2016~2019 Duzy Chan <code@duzy.info>, ExtBit Limited
 *  
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */ 
#ifndef _BOOST_MOLD_EDIT_ENCODER_HPP_
#define _BOOST_MOLD_EDIT_ENCODER_HPP_ 1
#include <boost/config.hpp>
#include <boost/assert.hpp>
#include <boost/scoped_array.hpp>
#include <cstring>
#include <locale>
#include <string>
namespace mold { namespace edit
{
  namespace details
  {

    ///////////////////////////////////////////////////////////////////////////
    template <typename CharT>
    struct string_lit;

    template <>
    struct string_lit<char>
    {
      static char get(char c) { return c; }
      static std::string get(char const* str = "") { return str; }
    };

    template <>
    struct string_lit<wchar_t>
    {
      static wchar_t get(char c)
      {
        using ct = std::ctype<wchar_t>;
        return std::use_facet<ct>(std::locale()).widen(c);
      }
      static std::basic_string<wchar_t> get(char const* source = "")
      {
        using namespace std; // some systems have size_t in ns std
        size_t len = strlen(source);
        boost::scoped_array<wchar_t> result(new wchar_t[len+1]);
        result.get()[len] = '\0';

        // working with wide character streams is supported only if the
        // platform provides the std::ctype<wchar_t> facet
        BOOST_ASSERT(std::has_facet<std::ctype<wchar_t> >(std::locale()));

        std::use_facet<std::ctype<wchar_t> >(std::locale())
          .widen(source, source + len, result.get());
        return result.get();
      }
    };
  } // namespace details
  
  struct encoder
  {
    // Only UTF8 string.
    void encode(std::string &str, char s, const std::string &r, int len)
    {
      std::string::size_type pos = 0;
      while ((pos = str.find_first_of(s, pos)) != str.npos) {
        str.replace(pos, 1, r);
        pos += len;
      }
    }
  };
}}
#endif//_BOOST_MOLD_EDIT_ENCODER_HPP_
