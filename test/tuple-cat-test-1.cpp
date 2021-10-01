#include <cstdio>
#include <tuplet/tuple.hpp>

using tuplet::tuple;
using tuplet::tuple_cat;

static_assert(
    std::is_same_v<decltype(tuple_cat()), tuple<>>,
    "tuple_cat broken");

static_assert(
    std::is_same_v<decltype(tuple_cat(tuple<>())), tuple<>>,
    "tuple_cat broken");

static_assert(
    std::is_same_v<decltype(tuple_cat(tuple<int>())), tuple<int>>,
    "tuple_cat broken");

static_assert(
    std::is_same_v<decltype(tuple_cat(tuple<int, char>())), tuple<int, char>>,
    "tuple_cat broken");

static_assert(
    std::is_same_v<
        decltype(tuple_cat(tuple<char>(), tuple<int>())),
        tuple<char, int> >,
    "tuple_cat broken");

int main(){}
