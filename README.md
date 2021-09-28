# tuplet: A Lightweight Tuple Library for Modern C++

**tuplet** is a one-header library that implements a fast and lightweight tuple
type, `tuplet::tuple`, that guarantees performance, fast compile times, and a
sensible and efficent data layout. A `tuplet::tuple` is implemented as an
aggregate containing it's elements, and this ensures that it's

- trivially copyable,
- trivially moveable,
- trivially assignable,
- trivially constructible,
- and trivially destructible.

This results in better code generation by the compiler, allowing `tuplet::tuple`
to be passed in registers, and to be serialized and deserialized via `memcpy`.

What's more, the implementation of `tuplet::tuple` is less than one fifth the
size of `std::tuple`, both in terms of lines, and in terms of kilobytes of code.
`tuplet.hpp` clocks in at 300 odd lines, compared to 1724 lines for gcc 9's
implementation of `std::tuple`.

If you'd like a further discussion of how `tuplet::tuple` compares to
`std::tuple` and why you should use it, see the [Motivation](#Motivation)
section below!

## Usage

Creating a tuple is as simple as `1, 2, "Hello, world"`! Writing

```cpp
tuplet::tuple tup = {1, 2, std::string("Hello, world!")};
```

Will create a tuple of type `tuple<int, int, std::string>`, just like you'd
expect it to. This is all you need to get started, but the following sections
will expand upon the functionality provided by **tuplet** in greater depth.

### Access members with _get()_ or the index operator

You can access members via `get`:

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

### Use Index Literals for Clean, Easy Access

You can access elements of a tuple very cleanly by using the literals provided
in `tuplet::literals`! This namespace defines the literal operators `_st`,`_nd`,
`_rd`, and `_th`, which take number and produce a `tuplet::index` templated on
that number, so `0_th` evaluates to `tuplet::index<0>()`, `1_st` evaluates to
`tuplet::index<1>()`, and so on!

```cpp
using namespace tuplet::literals;

tuplet::tuple tup = {1, 2, std::string("Hello, world!")};

std::cout << tup[0_th] << std::endl; // Prints 1
std::cout << tup[1_st] << std::endl; // Prints 2
std::cout << tup[2_nd] << std::endl; // Prints Hello World
```

You don't need to match the ending. These literals are defined on a numeric
literal operator template, so any number works for them. `999999_th` will give
you `tuplet::index<999999>`.

### Decompose tuples via Structured Bindings

The tuple can also be accessed via a structured binding:

```cpp
// a binds to get<0>(tup),
// b binds to get<1>(tup), and
// c binds to get<2>(tup)
auto& [a, b, c] = tup;

std::cout << c << std::endl; // Print "Hello, world!"
```

### Tie values together with _tuplet::tie()_

You can create a tuple of references with `tuplet::tie`! This function acts just
like `std::tie` does:

```cpp
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

### Assign Values via _tuple.assign()_

It's possible to easily and efficently assign values to a tuple using the
`.assign()` method:

```cpp
tuplet::tuple<int, int, std::string> tup;

tup.assign(1, 2, "Hello, world!");
```

### Store references using _std::ref()_

You can use `std::ref` to store references inside a tuple!

```cpp
std::string message;

// t has type tuple<int, int, std::string&>
tuplet::tuple t = {1, 2, std::ref(message)};

message = "Hello, world!";

std::cout << get<2>(t) << std::endl; // Prints Hello, world!
```

You can also store a reference by specifying it as part of the type of the
tuple:

```cpp
// Stores a reference to message
tuplet::tuple<int, int, std::string&> t = {1, 2, message};
```

These methods are equivilant, but the one with `std::ref` can result in cleaner
and shorter code, so the template deduction guide accounts for it.

### Use elements as function args with _tuplet::apply()_

As with `std::apply`, you can use `tuplet::apply` to use the elements of a tuple
as arguments of a function, like so:

```cpp
// Prints arguments on successive lines
auto print = [](auto&... args) {
    ((std::cout << args << '\n') , ...);
};

apply(print, tuplet::tuple{1, 2, "Hello, world!"});
```

## Motivation

This section intends to address a single fundamental question: _Why would I use
this instead of `std::tuple`?_

It is my hope that by addressing this question, I might explain my purpose for
writing this library, as well as providing a clearer overview of what it
provides.

`std::tuple` is _not_ a zero-cost abstraction, and using it introduces a runtime
penalty in comparison to traditional aggregate datatypes, such as structs.
`std::tuple` also compiles slowly, introducing a penalty on libraries that make
extensive use of it.

`tuplet::tuple` has none of these problems.

- `tuplet::tuple` an aggregate type.
  - When the elements are trivially constructible, `tuplet::tuple` is trivially
    constructible
  - When elements are trivially destructible, `tuplet::tuple` is trivially
    destructible
- `tuplet::tuple` can be passed in the registers. This means that there's's no
  overhead compared to a struct
- Compilation is much faster, especially for larger or more complex tuples.

  This occurs because `tuplet::tuple` is an aggregate type, and also because
  indexing was specifically designed in a way that allowed for faster lookp of
  elements.

- `tuplet::tuple` takes advantage of empty-base-optimization and
  `[[no_unique_address]]`. This means that empty types don't contribute to the
  size of the tuple.
