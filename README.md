# tuplet

**tuplet** is a one-header library that implements a fast and lightweight
tuple type `tuplet::tuple` that guarantees a sensible data layout.
`tuplet::tuple<T...>` is an aggregate type, and this means that when
it's members are trivial, `tuplet::tuple` is

- trivially copyable,
- trivially moveable,
- trivially assignable,
- trivially constructible,
- and trivially destructible

This results in better code generation by the compiler, faster compile
times, and smaller executables.

What's more, the implementation of `tuplet::tuple` is *less than one-tenth*
the size of `std::tuple`, both in terms of lines, and in terms of kilobytes
of code. `tuplet.hpp` clocks in at 157 lines, compared to 1724 lines for gcc 9's
implementation of `std::tuple`.


