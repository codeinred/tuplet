#include "util/printing.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <tuplet/format.hpp>
#include <tuplet/tuple.hpp>

TEST_CASE("Lexiconographic ordering", "[compare]") {
    using tuplet::tuple;

    tuple t1 {0, 0};
    tuple t2 {0, 1};
    tuple t3 {1, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
}

TEST_CASE("Empty tuple equals itself", "[compare]") {
    REQUIRE(tuplet::tuple{} == tuplet::tuple{});
    REQUIRE_FALSE(tuplet::tuple{} != tuplet::tuple{});
}

SCENARIO("We have tuples created with references", "[compare]") {
    using tuplet::tuple;

    int a = 0, b = 1, c = 2;
    tuple t1 = {std::ref(a), std::ref(b)};
    tuple t2 = {std::ref(a), std::ref(c)};
    REQUIRE(std::is_same_v<decltype(t1), tuple<int&, int&>>);

    REQUIRE(t1 < t2);
    REQUIRE(t1 != t2);
    REQUIRE_FALSE(t1 == t2);
    REQUIRE_FALSE(t1 > t2);

    WHEN("The values in b and c are swapped") {
        std::swap(b, c);
        THEN("The relative ordering of the tuples changes") {
            REQUIRE(t2 < t1);
            REQUIRE(t1 != t2);
            REQUIRE_FALSE(t1 == t2);
            REQUIRE_FALSE(t2 > t1);
        }
    }
}
