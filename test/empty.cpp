#include <cstdio>
#include <string>
#include <tuplet/tuple.hpp>

int main(int argc, char** argv) {
    tuplet::tuple<> empty_tuple;
    static_assert(
        std::is_empty_v<tuplet::tuple<>>,
        "Empty tuple should be an empty type");
    static_assert(
        tuplet::stateless<tuplet::tuple<>>,
        "Empty tuple should be stateless");

    static_assert(
        apply([] { return 0; }, empty_tuple) == 0,
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

    // A tuple should be assignable from a stateless type
    empty_tuple = []() {};

    // A empty tuple should be reference assignable from a stateless type
    auto empty_lambda = []() {};
    empty_tuple = empty_lambda;

    // Checks that self-assignment compiles
    empty_tuple = empty_tuple;

    return 0;
}
