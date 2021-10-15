#include "util/printing.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
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
    REQUIRE_FALSE(t1 != t1);
    REQUIRE_FALSE(t1 > t2);
    REQUIRE_FALSE(t2 > t3);
}

TEST_CASE("Empty tuple equals itself", "[compare]") {
    REQUIRE(tuplet::tuple {} == tuplet::tuple {});
    REQUIRE_FALSE(tuplet::tuple {} != tuplet::tuple {});
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

SCENARIO("We have tuplet with different but comparable types", "[compare]") {
    using tuplet::tuple;

    tuple t0 {0, 0};
    tuple t1 {0, 0.0};
    tuple t2 {0l, 1};
    tuple t3 {1.0f, (unsigned short)(0)};

    REQUIRE(t0 == t1);
    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE_FALSE(t1 != t1);
    REQUIRE_FALSE(t1 > t2);
    REQUIRE_FALSE(t2 > t3);
}

SCENARIO(
    "We're comparing tuples containing string_views to ones containing string "
    "literals",
    "[compare]") {
    using tuplet::tuple;
    tuple t1 {1, 2, std::string("Hello, world!")};
    tuple t2 {1, 2, "Hello, world!"};
    STATIC_REQUIRE_FALSE(std::is_same_v<decltype(t1), decltype(t2)>);
    REQUIRE(t1 == t2);
}
