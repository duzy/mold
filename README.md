# mold

A **C++ Template Engine Library** supports multiple domain languages. 

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

Examples
--------

```c++
#include <boost/mold/mold.hpp>
#include <boost/mold/format/mustache.hpp>
#include <iostream>

static auto MUSTACHE_EXAMPLE = std::string
{
  R"***(//"
  
  Dear {{FirstName}},

  It's been long time since we last met.

  {{Story}}
  
  Best Regards,
  {{MyName}}
  
  )***" + 4
};//";

namespace mold = boost::mold;
  
int main(int argc, char**argv)
{
  mold::object context
  {
    { "FirstName", "Tony" },
    { "MyName", "Mold" },
    { "Story", "Blah blah blah, here goes the long story..." },
  };
  
  auto const &s = MUSTACHE_EXAMPLE;
  auto i = s.begin();
  auto t = mold::load<mold::format::mustache_spec>(i, s.end());
  mold::generate(std::cout, t, context);
  return 0;
}
```

Expected result on `std::cout`:

```
  
  Dear Tony,

  It's been long time since we last met.

  Blah blah blah, here goes the long story...
  
  Best Regards,
  Mold
  
```

Supported Formats
-----------------

The **mold** engine supports multiple template domain languages. Currently it has:

  * [mustache][] - Logic-less templates.
  * [tildache][] - A variant of [mustache][] with [tild extensions][tild-ext]. (*)

[boostache]: https://github.com/cierelabs/boostache
[mustache]: http://mustache.github.io/mustache.5.html
[tildache]: https://github.com/duzy/mold/wiki/tildache
[tild-ext]: https://github.com/duzy/mold/wiki/tild-extensions
