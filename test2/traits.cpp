#include <catch2/catch_test_macros.hpp>
#include <tuplet/tuple.hpp>

static_assert(
    std::is_empty_v<tuplet::tuple<>>,
    "Empty tuple should be an empty type");
static_assert(
    tuplet::stateless<tuplet::tuple<>>,
    "Empty tuple should be stateless");

static_assert(
    apply([] { return 0; }, tuplet::tuple<> {}) == 0,
    "Apply not working for empty tuple");

static_assert(
    std::tuple_size<tuplet::tuple<>>::value == 0,
    "An empty tuple should have a tuple size of 0");

static_assert(
    std::is_trivial_v<tuplet::tuple<>>,
    "An empty tuple should be trivial");
static_assert(
    std::is_trivially_assignable_v<tuplet::tuple<>, tuplet::tuple<>>,
    "An empty tuple should be trivially assignable.");
static_assert(
    std::is_trivially_copy_assignable_v<tuplet::tuple<>>,
    "An empty tuple should be trivially assignable.");
static_assert(
    std::is_trivially_move_assignable_v<tuplet::tuple<>>,
    "An empty tuple should be trivially assignable.");

TEST_CASE("Empty tuple should be an empty type", "[traits]") {
    REQUIRE(std::is_empty_v<tuplet::tuple<>>);
}
TEST_CASE("Empty tuple should be stateless", "[traits]") {
    REQUIRE(tuplet::stateless<tuplet::tuple<>>);
}

TEST_CASE("Calling apply on an empty tuple requires no arguments", "[traits]") {
    REQUIRE(apply([] { return 0; }, tuplet::tuple {}) == 0);
}

TEST_CASE("An empty tuple should have a tuple size of 0", "[traits]") {
    REQUIRE(std::tuple_size<tuplet::tuple<>>::value == 0);
}

TEST_CASE("An empty tuple should be trivial", "[traits]") {
    REQUIRE(std::is_trivial_v<tuplet::tuple<>>);
}
TEST_CASE("An empty tuple should be trivially assignable.", "[traits]") {
    REQUIRE(std::is_trivially_assignable_v<tuplet::tuple<>, tuplet::tuple<>>);
}
TEST_CASE("An empty tuple should be trivially copy assignable.", "[traits]") {
    REQUIRE(std::is_trivially_copy_assignable_v<tuplet::tuple<>>);
}
TEST_CASE("An empty tuple should be trivially move assignable.", "[traits]") {
    REQUIRE(std::is_trivially_move_assignable_v<tuplet::tuple<>>);
}
