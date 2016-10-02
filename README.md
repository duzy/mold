# mold
C++ Template Engine Library

Introduction
------------

The **mold** engine is inspired by [boostache][]. Reasons why I started
**mold** as a new project and deprecated [boostache][] are:

  * [boostache][] is still buggy (e.g. not fully working with the [mustache][] spec).
  * [boostache][] source code seems to be immature so far and not updated.

The primary goal of **mold** is at least being a [mustache][] engine without issue.
The long term goals are:

  * Performance improvement: avoid many string copy operations.
  * Support multiple domain languages after [mustache][].
  * Mordern C++ design and C++1z friendly.

Supported Formats
-----------------

The **mold** engine is possible to support multiple template domain languages. But
only [mustache][] is available so far.

[boostache]: https://github.com/cierelabs/boostache
[mustache]: http://mustache.github.io/mustache.5.html
