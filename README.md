# mold

Mold is a **C++ Template Engine Library**, a header-only library supports
multiple domain languages.

The [mold][] engine has a stacked-based virtual machine implementation, it's
execution is operating on text to generate text output to a stream or produce
a value from the program. The goal of the virtual machine is text operations
instead of fast computation although it's still capable of doing mathmatical
and logical computation.

All domain languages are compiled into programs for the virtual machine. It's
possible to support many advanced languages.

Support: [https://www.bountysource.com/teams/mold] -- [Salt It Now!](https://salt.bountysource.com/checkout/amount?team=mold])

Introduction
------------

The **mold** engine is inspired by [boostache][]. Reasons why I started
**mold** as a new project and deprecated [boostache][] are:

  * [boostache][] is still buggy (e.g. not fully working with the [mustache][] spec).
  * [boostache][] source code seems to be immature so far and not updated.

The primary goal of **mold** is at least a fully working [mustache][] engine.
The long term goals are:

  * Performance improvement: avoid many string copy operations.
  * Support multiple domain languages after [mustache][].
  * Mordern C++ design and C++1z friendly.

Examples
--------

```c++
#include <boost/mold/mold.hpp>
#include <boost/mold/format/mustache.hpp>
#include <boost/mold/format/mustache.ipp>
#include <iostream>

namespace mold = boost::mold;

static std::string MUSTACHE_EXAMPLE
{
u8R"***(//"
  
  Dear {{FirstName}},

  It's been long time since we last met.

  {{Story}}
  
  Best Regards,
  {{MyName}}
  
)***" + 4
};//";
  
int main(int argc, char**argv)
{
  mold::object feed
  {
    { "FirstName", "Tony" },
    { "MyName", "Mold" },
    { "Story", "Blah blah blah... Here goes a very long story..." },
  };
  
  auto t = mold::load<mold::format::mustache>(MUSTACHE_EXAMPLE);
  mold::generate(std::cout, t, feed);
  return 0;
}
```

Expected result on `std::cout`:

```
  
  Dear Tony,

  It's been long time since we last met.

  Blah blah blah... Here goes a very long story...
  
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
