#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <tuplet/tuple.hpp>

TEST_CASE("Invoke map on tuple", "[test-map]") {
    using tuplet::tuple;
    using std::string_literals::operator""s;

    auto tup = tuple {10, 20.4, "Hello, world"};

    auto expected = tuple {"10"s, "20.4"s, "Hello, world"s};

    auto to_str = [](auto val) { return fmt::format("{}", val); };
    REQUIRE(tup.map(to_str) == expected);
}

TEST_CASE("Move elements with tuple.map", "[test-map]") {
    using tuplet::tuple;
    using std::string_view_literals::operator""sv;

    auto tup = tuple {10, 20.4, "Hello, world"sv, std::make_unique<int>(10)};

    auto expected = tuple {
        10,
        20.4,
        "Hello, world"sv,
        std::make_unique<int>(10)};

    auto fucking_yeet = [](auto val) { return val; };

    auto result = std::move(tup).map(fucking_yeet);

    INFO("Check that the tuple was moved-from");
    REQUIRE(bool(get<3>(tup)) == false);
    REQUIRE(*get<3>(result) == *get<3>(expected));
}
