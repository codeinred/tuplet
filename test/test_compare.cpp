#include "util/printing.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <tuplet/format.hpp>
#include <tuplet/tuple.hpp>


using Catch::operator<<;

TEST_CASE("Lexiconographic ordering", "[compare]") {
    using tuplet::tuple;

    tuple t1 {0, 0};
    tuple t2 {0, 1};
    tuple t3 {1, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}


TEST_CASE("Lexiconographic ordering", "[compare-str]") {
    using tuplet::tuple;
    using std::string_view_literals::operator""sv;

    static_assert(tuplet::sfinae::detail::
                      _has_compare_with<std::string_view, std::string_view>(0));

    auto t1 = tuple {"0"sv, "0"sv};
    auto t2 = tuple {"0"sv, "00"sv};
    auto t3 = tuple {"00"sv, "0"sv};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}

TEST_CASE("Lexiconographic ordering pairs", "[compare]") {
    using tuplet::pair;

    pair t1 {0, 0};
    pair t2 {0, 1};
    pair t3 {1, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}

TEST_CASE("Lexiconographic ordering, different types", "[compare]") {
    using tuplet::tuple;

    tuple t1 {0, 0};
    tuple t2 {0ul, 1};
    tuple t3 {1.0, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}

TEST_CASE("Tuples with different types", "[compare]") {
    using tuplet::tuple;

    tuple t1 {1, 2};
    tuple t2 {'\1', '\2'};
    tuple t3 {1ul, 2.0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 == t2);
    REQUIRE(t1 == t3);
    REQUIRE(t2 == t3);
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
