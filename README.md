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

## Usage

Creating a tuple is pretty straight-forward. Writing
```cpp
tuplet::tuple tup = {1, 2, std::string("Hello, world!")};
```
Will create a tuple of type `tuple<int, int, std::string>`, just like you'd
expect it to. This is all you need to get started, but the following
sections will expand upon the functionality provided by **tuplet** in greater
depth.

### Access values via `get<i>(tuple)` or `tup[tuplet::inedx<i>()]`

You can access members via `get` or via `operator[]` with an index:
```cpp
std::cout << get<2>(tup) << std::endl; // Prints "Hello, world!"
```
Or via `operator[]`:
```cpp
std::cout << tup[tuplet::index<2>()] << std::endl; // Prints "Hello, world!"
```

Something that's important to note is that `index` is just an alias for
`std::integral_constant`:

```cpp
template <size_t I>
using index = std::integral_constant<size_t, I>;
```

#### Neat feature: `tuplet::literals` for shorthand access!
You can access elements of a tuple very cleanly by using the literals
provided in `tuplet::literals`! This namespace defines the literal
operators `_st`,`_nd`, `_rd`, and `_th`, which take
number and produce a `tuplet::index` templated on that number,
so `0_th` evaluates to `tuplet::index<0>()`, `1_st` evaluates
to `tuplet::index<1>()`, and so on!

```cpp
using namespace tuplet::literals;

tuplet::tuple tup = {1, 2, std::string("Hello, world!")};

std::cout << tup[0_th] << std::endl; // Prints 1
std::cout << tup[1_st] << std::endl; // Prints 2
std::cout << tup[2_nd] << std::endl; // Prints Hello World
```
You don't need to match the ending. These literals are defined on a
numeric literal operator template, so any number works for them. `999999_th`
will give you `tuplet::index<999999>`.

### Decompose a tuple via structured binding

The tuple can also be accessed via a structured binding:
```cpp
// a binds to get<0>(tup),
// b binds to get<1>(tup), and
// c binds to get<2>(tup)
auto& [a, b, c] = tup;

std::cout << c << std::endl; // Print "Hello, world!"
```

### Tie values together with `tuplet::tie`

You can create a tuple of references with `tuplet::tie`! This function
acts just like `std::tie` does:
```
int a;
int b;
std::string s;

// Creates a tuplet::tuple<int&, int&, std::string&>
tuplet::tuple tup = tuplet::tie(a, b, s);

// a will be set to 1,
// b will be set to 2, and
// s will be set to "Hello, world!"
tup = tuplet::tuple{1, 2, "Hello, world!"};

std::cout << s << std::endl; // Prints Hello World
```

### Assign values via `tuple.assign(values...)`

It's possible to easily and efficently assign values to a tuple using
the `.assign()` method:
```
tuplet::tuple<int, int, std::string> tup;

tup.assign(1, 2, "Hello, world!");
```
