#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <tuplet/tuple.hpp>

TEST_CASE("Test tup.all()", "[test-all]") {
    using tuplet::tuple;

    auto tup = tuple {1, 2, 3, 4.3, 5};
    REQUIRE(tup.all([](auto val) { return val < 10; }));
    REQUIRE_FALSE(tup.all([](auto val) { return val < 4; }));
}

TEST_CASE("Test tup.all() with move", "[test-all]") {
    using tuplet::tuple;

    auto tup = tuple {
        std::make_unique<int>(10),
        std::make_unique<int>(20),
        std::make_unique<int>(30)};

    auto is_valid_ref = [](auto& value) { return bool(value); };
    auto is_valid_move = [](auto value) { return bool(value); };

    REQUIRE(tup.all(is_valid_ref));
    REQUIRE(std::move(tup).all(is_valid_move));
    REQUIRE_FALSE(tup.all(is_valid_ref));
}
