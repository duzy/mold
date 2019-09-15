# mold

Mold is a **C++ Text Engine**. It's a header-only library for generating
text from a template and a set of data (feed).

Support: [Salt It!](https://salt.bountysource.com/checkout/amount?team=mold])

Introduction
------------

Templates can be defined in different domain languages, here now we already
have [Mustache][mustache] and [Tildache][tildache], could implement more
text domain languages. And the data set (feeds the template) is contained
by `mold::object`. Text templates written in a domain language
(such as [Tildache][tildache]) are compiled into a [Mold][mold] **program**
(a sequence of operations) and further executed by the virtual
machine to create the text output.

The engine is running on a stacked-based virtual machine. Which executes a
sequence of text operations (see `ops.hpp`) and renders desired text
(defined by the template) to a `std::ostream`. The virtual machine is not
for generic computing. Instead, it focuses on text processing. So the
registers are `std::string` based, rather then `int` or `float`. But it do
supports some basic arithmatic and logical computation.

Notes
-----

The **mold** engine is inspired by [Boostache][boostache]. Reasons why I started
**mold** as a new project and deprecated [Boostache][boostache] are:

  * [Boostache][boostache] is still buggy (e.g. not fully working with the [Mustache][mustache] spec).
  * [Boostache][boostache] source code seems to be immature so far and not updated.

The primary goal of **mold** is at least a fully working [Mustache][mustache] engine.
The long term (possible) goals are:

  * Performance improvement: avoid many string copy operations.
  * Support multiple domain languages after [Mustache][mustache] is done.
  * Mordern C++ design and C++1z friendly.
  * Persistant byte code representation.

Examples
--------

```c++
#include <mold/mold.hpp>
#include <mold/format/mustache.hpp>
#include <mold/format/mustache.ipp>
#include <iostream>

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

  * [Mustache][mustache] - Logic-less templates.
  * [Tildache][tildache] - A variant of [Mustache][mustache] with [tild extensions][tild-ext]. (*)

[boostache]: https://github.com/cierelabs/boostache
[mustache]: http://mustache.github.io/mustache.5.html
[tildache]: https://github.com/duzy/mold/wiki/tildache
[tild-ext]: https://github.com/duzy/mold/wiki/tild-extensions
[mold]: https://github.com/extbit/mold
