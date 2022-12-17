#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>

using namespace tuplet::literals;
using tuplet::tuple;

TEST_CASE("Check assignment with tuplet::tie", "[assign]") {
    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tie(a, b, c).assign(1, 2, "Hello, world!");

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}

TEST_CASE("Check = operator for tuples", "[assign]") {
    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tie(a, b, c) = tuplet::tuple {1, 2, std::string("Hello, world!")};

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}

TEST_CASE("Operator = with heterogenous element types", "[assign]") {
    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tie(a, b, c) = tuplet::tuple {1, 2, "Hello, world!"};

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}

TEST_CASE("check assignment results in moves", "[assign]") {
    tuplet::tuple<std::unique_ptr<int>, std::string> t, q;

    t = {std::make_unique<int>(69420), "Hello, world!"};

    q = std::move(t);

    REQUIRE(t[0_tag].get() == nullptr);
    REQUIRE(*q[0_tag] == 69420);
    REQUIRE(q[1_tag] == "Hello, world!");
}

TEST_CASE("check assignment results in moves with tuplet::tie", "[assign]") {
    std::shared_ptr<int> p;
    std::string s;
    tuplet::tuple<std::shared_ptr<int>, std::string> t, q;

    t = {std::make_shared<int>(69420), "Hello, world!"};

    tuplet::tie(p, s) = std::move(t);

    REQUIRE(t[0_tag].get() == nullptr);
    REQUIRE(*p == 69420);
    REQUIRE(s == "Hello, world!");
}

TEST_CASE("check tuple assignment", "[assign]") {
    using namespace tuplet::literals;
    tuplet::tuple<int, int, std::string> t1 {1, 2, "Hello, world!"}, t2;

    t2 = t1;

    REQUIRE(t2[0_tag] == 1);
    REQUIRE(t2[1_tag] == 2);
    REQUIRE(t2[2_tag] == "Hello, world!");
}

TEST_CASE("Assignment to empty tuple", "[assign]") {
    tuplet::tuple<> empty_tuple;

    // A tuple should be assignable from a stateless type
    empty_tuple = [] {};

    // A empty tuple should be reference assignable from a stateless type
    auto empty_lambda = [] {};
    empty_tuple = empty_lambda;

    // Checks that self-assignment compiles
    empty_tuple = empty_tuple;
}

TEST_CASE("Check assignment of tuple with nested tuples", "[assign]") {
    tuplet::tuple<int, tuplet::tuple<double>, tuplet::tuple<int>> t;

    t.assign(1, tuplet::make_tuple(2.0), tuplet::make_tuple(3));

    REQUIRE(t[0_tag] == 1);
    REQUIRE(t[1_tag][0_tag] == 2.0);
    REQUIRE(t[2_tag][0_tag] == 3);
}