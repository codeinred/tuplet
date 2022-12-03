#include <catch2/catch_test_macros.hpp>
#include <tuplet/tuple.hpp>
#include <memory>

using tuplet::tuple;
using namespace tuplet::literals;

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

TEST_CASE("test tuple_cat", "[tuple_cat]") {
    tuple<std::unique_ptr<int>, std::string, char, char, char>
        tup = tuplet::tuple_cat(
            tuple {std::make_unique<int>(69420)},
            tuple {std::string {"Hello, world!"}},
            tuple {'a', 'b', 'c'});

    REQUIRE(*tup[0_tag] == 69420);
    REQUIRE(tup[1_tag] == "Hello, world!");
    REQUIRE(tup[2_tag] == 'a');
    REQUIRE(tup[3_tag] == 'b');
    REQUIRE(tup[4_tag] == 'c');
}
