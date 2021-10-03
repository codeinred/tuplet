#include <catch2/catch_test_macros.hpp>
#include <memory>
#include "util/printing.hpp"


TEST_CASE("Test Assignment") {
    using namespace tuplet::literals;
    using tuplet::tuple;

    int a = 0;
    int b = 0;
    std::string c;

    SECTION("Check assignment with tuplet::tie", "[assign]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c).assign(1, 2, "Hello, world!");

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("Check = operator for tuples", "[assign]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c) = tuplet::tuple {
            1,
            2,
            std::string("Hello, world!")};

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("Operator = with heterogenous element types", "[assign]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c) = tuplet::tuple {1, 2, "Hello, world!"};

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("check assignment results in moves", "[assign]") {
        tuplet::tuple<std::unique_ptr<int>, std::string> t, q;

        t = {std::make_unique<int>(69420), "Hello, world!"};

        q = std::move(t);

        REQUIRE(t[0_tag].get() == nullptr);
        REQUIRE(*q[0_tag] == 69420);
        REQUIRE(q[1_tag] == "Hello, world!");
    }

    SECTION("check tuple assignment", "[assign]") {
        using namespace tuplet::literals;
        tuplet::tuple<int, int, std::string> t1 {1, 2, "Hello, world!"}, t2;

        t2 = t1;

        REQUIRE(t2[0_tag] == 1);
        REQUIRE(t2[1_tag] == 2);
        REQUIRE(t2[2_tag] == "Hello, world!");
    }
}
