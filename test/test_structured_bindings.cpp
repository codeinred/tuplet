#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <tuplet/tuple.hpp>

TEST_CASE("Check tuple decomposition", "[binding]") {
    auto tup = tuplet::tuple {1, 2, std::string("Hello, world!")};

    auto [a, b, c] = tup;

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}

TEST_CASE("Check tuple decomposition by reference", "[binding]") {
    auto tup = tuplet::tuple {0, 0, std::string()};

    auto& [a, b, c] = tup;

    a = 1;
    b = 2;
    c = "Hello, world!";

    REQUIRE(get<0>(tup) == 1);
    REQUIRE(get<1>(tup) == 2);
    REQUIRE(get<2>(tup) == "Hello, world!");
}

TEST_CASE("Check tuple decomposition by move", "[binding]") {
    auto tup = tuplet::tuple {
        1,
        std::make_unique<int>(2),
        std::string("Hello, world!")};

    // Check that moving a tuple moves the elements when
    // doing a structured bind
    auto [a, b, c] = std::move(tup);

    REQUIRE(a == 1);
    REQUIRE(*b == 2);
    REQUIRE(c == "Hello, world!");
}
