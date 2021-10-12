#include <cstdio>
#include <memory>
#include <string>
#include <tuplet/tuple.hpp>

using tuplet::tuple;
using tuplet::literals::operator""_tag;

static_assert(
    std::is_same_v<decltype(tuplet::tuple_cat()), tuple<>>,
    "tuplet::tuple_cat broken");

static_assert(
    std::is_same_v<decltype(tuplet::tuple_cat(tuple<>())), tuple<>>,
    "tuplet::tuple_cat broken");

static_assert(
    std::is_same_v<decltype(tuplet::tuple_cat(tuple<int>())), tuple<int>>,
    "tuplet::tuple_cat broken");

static_assert(
    std::is_same_v<
        decltype(tuplet::tuple_cat(tuple<int, char>())),
        tuple<int, char>>,
    "tuplet::tuple_cat broken");

static_assert(
    std::is_same_v<
        decltype(tuplet::tuple_cat(tuple<char>(), tuple<int>())),
        tuple<char, int>>,
    "tuplet::tuple_cat broken");

static_assert(
    std::is_same_v<
        decltype(tuplet::tuple_cat(
            tuple<char>(),
            tuple<int>(),
            tuple<std::string>())),
        tuple<char, int, std::string>>,
    "tuplet::tuple_cat broken");

int main() {

    tuple<std::unique_ptr<int>, std::string, char, char, char>
        tup = tuplet::tuple_cat(
            tuple {std::make_unique<int>(69420)},
            tuple {std::string {"Hello, world!"}},
            tuple {'a', 'b', 'c'});

    if (*tup[0_tag] == 69420 && tup[1_tag] == "Hello, world!"
        && tup[2_tag] == 'a' && tup[3_tag] == 'b' && tup[4_tag] == 'c') {
        return 0;
    } else {
        return 1;
    }
}
