#include <catch2/catch_test_macros.hpp>
#include <tuplet/tuple.hpp>

static_assert(
    std::is_empty_v<tuplet::tuple<>>,
    "Empty tuple should be an empty type");
static_assert(
    tuplet::stateless_v<tuplet::tuple<>>,
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
    REQUIRE(tuplet::stateless_v<tuplet::tuple<>>);
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



struct A {
    int value = 0;
};
struct B {
    int value = 0;
    B& operator=(B const& b) {
        value = b.value;
        return *this;
    }
    B& operator=(B&& b) {
        value = b.value;
        return *this;
    }
};
struct C {
    int value;
    template <class U>
    C& operator=(U&& other) {
        value = other.value;
        return *this;
    }
};

TEST_CASE("Other traits", "[traits]") {
    using tuple_A = tuplet::tuple<int, int, int>;
    using tuple_B = tuplet::tuple<int, int, long>;

    tuple_A a {0, 0, 0};
    tuple_B b {0, 0, 10};
    a = b;

    static_assert(!std::is_trivially_assignable_v<B, B>);
    static_assert(std::is_trivially_assignable_v<C, C>);
    static_assert(!std::is_trivially_assignable_v<C, B>);

    static_assert(
        std::is_trivially_copyable_v<tuple_A>,
        "Expected tuple<int, int, int> to be trivially copyable");
    static_assert(
        std::is_default_constructible_v<tuple_A>,
        "Expected tuple<int, int, int> to be default constructible");
    static_assert(
        std::is_aggregate_v<tuple_A>,
        "Expected tuple<int, int, int> to be an aggregate");

    static_assert(
        noexcept(a = a),
        "Assigning a tuple to itself must be noexcept for trivial types");
    static_assert(
        std::is_trivially_copy_assignable_v<tuple_A>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        std::is_trivially_copy_assignable_v<tuple_B>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        std::is_trivially_assignable_v<tuple_A, tuple_A>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        std::is_trivially_assignable_v<tuple_B, tuple_B>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        !std::is_trivially_assignable_v<tuple_A, tuple_B>,
        "A tuple should not be trivially assignable to converted tuples");
    static_assert(
        std::is_trivially_move_assignable_v<tuple_A>,
        "A tuple should be trivially move assignable if the members are");
    static_assert(
        std::is_trivially_move_assignable_v<tuple_B>,
        "A tuple should be trivially move assignable if the members are");

    static_assert(
        std::is_trivial_v<tuple_A>,
        "A tuple should be trivial if it's members are.");
    static_assert(
        std::is_trivial_v<tuple_B>,
        "A tuple should be trivial if it's members are.");
}
