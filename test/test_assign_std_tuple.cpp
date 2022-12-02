#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <tuple>

using namespace tuplet::literals;
using tuplet::tuple;


TEST_CASE("Check = operator for tuples", "[assign-std-tuple]") {
    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tie(a, b, c) = std::tuple {1, 2, std::string("Hello, world!")};

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}

TEST_CASE("Operator = with heterogenous element types", "[assign-std-tuple]") {
    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tie(a, b, c) = std::tuple {1, 2, "Hello, world!"};

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}

TEST_CASE(
    "check assignment results in moves with tuplet::tie",
    "[assign-std-tuple]") {
    std::shared_ptr<int> p;
    std::string s;
    std::tuple<std::shared_ptr<int>, std::string> t, q;

    t = {std::make_shared<int>(69420), "Hello, world!"};

    tuplet::tie(p, s) = std::move(t);

    REQUIRE(std::get<0>(t) == nullptr);
    REQUIRE(*p == 69420);
    REQUIRE(s == "Hello, world!");
}

TEST_CASE("check tuple assignment", "[assign-std-tuple]") {
    using namespace tuplet::literals;
    std::tuple<int, int, std::string> t1 {1, 2, "Hello, world!"};
    tuplet::tuple<int, int, std::string> t2;

    t2 = t1;

    REQUIRE(t2[0_tag] == 1);
    REQUIRE(t2[1_tag] == 2);
    REQUIRE(t2[2_tag] == "Hello, world!");
}
