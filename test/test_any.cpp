#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <string>
#include <string_view>
#include <tuplet/tuple.hpp>

TEST_CASE("Test tup.any()", "[test-any]") {
    using tuplet::tuple;

    auto tup = tuple {1, 2, 3, 4.3, 5};
    REQUIRE(tup.any([](auto val) { return val < 10; }));
    REQUIRE(tup.any([](auto val) { return val < 4; }));
    REQUIRE_FALSE(tup.any([](auto val) { return val < 0; }));
}

TEST_CASE("Test tup.any() with move", "[test-any]") {
    using tuplet::tuple;

    auto tup = tuple {std::make_unique<int>(10), std::make_unique<int>(20)};

    auto is_valid_ref = [](auto& value) { return bool(value); };
    auto is_valid_move = [](auto value) { return bool(value); };

    INFO("Only the first unique_ptr should be moved, so one should still "
         "be a valid ref the first go-around.");
    REQUIRE(tup.all(is_valid_ref));
    REQUIRE(std::move(tup).any(is_valid_move));
    REQUIRE(tup.any(is_valid_ref));
    REQUIRE(std::move(tup).any(is_valid_move));
    REQUIRE_FALSE(tup.any(is_valid_ref));
}
