# mold
C++ Template Engine Library

Introduction
------------

The **mold** engine is inspired by [boostache][]. Reasons why I started
**mold** as a new project and deprecated [boostache][] are:

  * [boostache][] is still buggy (e.g. not fully working with the [mustache][] spec).
  * [boostache][] source code seems to be immature so far and not updated.

The primary goal of **mold** is at least a fully working [mustache][] engine without a issue.
The long term goals are:

  * Performance improvement: avoid many string copy operations.
  * Support multiple domain languages after [mustache][].
  * Mordern C++ design and C++1z friendly.

Supported Formats
-----------------

The **mold** engine is possible to support multiple template domain languages. But
only [mustache][] is available so far.

Examples
--------

```c++
#include <boost/mold/load.hpp>
#include <boost/mold/generate.hpp>
#include <boost/mold/format/mustache.hpp>
#include <boost/mold/format/mustache/parser_def.hpp>
#include <iostream>

static auto MUSTACHE_EXAMPLE = std::string
{
  R"***(//"
  
  namespace {{ &Namespace }}
  {
    {{#Classes}}
    class {{Name}};
    {{/Classes}}
  }
  
  )***" + 4
};//";

namespace mold = boost::mold;
  
int main(int argc, char**argv)
{
  mold::object context
  {
    { "Namespace", "Example" },
    { "Classes", mold::array{
        mold::object{{ "Name", "Foo" }},
        mold::object{{ "Name", "Bar" }},
        }
    },
  };
  
  auto const &s = MUSTACHE_EXAMPLE;
  auto i = s.begin();
  auto t = mold::load<mold::format::mustache_spec>(i, s.end());
  mold::generate(std::cout, t, context);
  return 0;
}
```

Expected result on `std::cout`:

```c++
  
  namespace Example
  {
    class Foo;
    class Bar;
  }
  
```

[boostache]: https://github.com/cierelabs/boostache
[mustache]: http://mustache.github.io/mustache.5.html
