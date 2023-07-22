#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_version_macros.hpp>
#include <memory>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <vector>

static_assert(CATCH_VERSION_MAJOR >= 3);
static_assert(CATCH_VERSION_MINOR >= 0);


TEST_CASE("Test get type for tuples containing unique types", "[core][get<T>]") {
    auto t1 = tuplet::tuple<int, double, std::string> {10, .2, "Hello"};
    auto t2 = tuplet::make_tuple(
        short(5),
        long(-129837),
        std::string("The quick brown fox jumps over the lazy dogs"));

    REQUIRE(tuplet::get<int>(t1) == 10);
    REQUIRE(tuplet::get<double>(t1) == .2);
    REQUIRE(tuplet::get<std::string>(t1) == "Hello");

    REQUIRE(tuplet::get<int>(t1) == tuplet::get<0>(t1));
    REQUIRE(tuplet::get<double>(t1) == tuplet::get<1>(t1));
    REQUIRE(tuplet::get<std::string>(t1) == tuplet::get<2>(t1));

    REQUIRE(tuplet::get<short>(t2) == 5);
    REQUIRE(tuplet::get<long>(t2) == -129837);
    REQUIRE(
        tuplet::get<std::string>(t2)
        == std::string("The quick brown fox jumps over the lazy dogs"));

    REQUIRE(tuplet::get<short>(t2) == tuplet::get<0>(t2));
    REQUIRE(tuplet::get<long>(t2) == tuplet::get<1>(t2));
    REQUIRE(tuplet::get<std::string>(t2) == tuplet::get<2>(t2));
}

TEST_CASE("Test handling of tuples containing refs", "[core]") {
    int a = 0;
    int b = 0;
    std::string c;

    SECTION("tuples can be created from refs") {
        auto tup = tuplet::tuple {std::ref(a), std::ref(b), std::ref(c)};

        static_assert(
            std::is_same_v<std::tuple_element_t<0, decltype(tup)>, int&>,
            "Expected int& at element 0");
        static_assert(
            std::is_same_v<std::tuple_element_t<1, decltype(tup)>, int&>,
            "Expected int& at element 1");
        static_assert(
            std::
                is_same_v<std::tuple_element_t<2, decltype(tup)>, std::string&>,
            "Expected std::string& at element 2");

        using tuplet::get;

        tup = tuplet::tuple {1, 2, "Hello, world!"};

        REQUIRE((a == 1 && b == 2 && c == "Hello, world!"));
    }

    SECTION("Tuples can be created with tuplet::tie") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        auto tup = tuplet::tie(a, b, c);

        static_assert(
            std::is_same_v<std::tuple_element_t<0, decltype(tup)>, int&>,
            "Expected int& at element 0");
        static_assert(
            std::is_same_v<std::tuple_element_t<1, decltype(tup)>, int&>,
            "Expected int& at element 1");
        static_assert(
            std::
                is_same_v<std::tuple_element_t<2, decltype(tup)>, std::string&>,
            "Expected std::string& at element 2");


        tup = tuplet::tuple {1, 2, "Hello, world!"};

        REQUIRE((a == 1 && b == 2 && c == "Hello, world!"));
    }

    SECTION("Tuples created with tuplet::tie are equality-comparable") {
        REQUIRE(tuplet::tie(a, b, c) == tuplet::tie(a, b, c));
    }

    SECTION("Tuples created with tuplet::tie are comparable to normal tuples") {
        REQUIRE(tuplet::tie(a, b, c) == tuplet::tuple {a, b, c});
    }
}



TEST_CASE("Empty tuple", "[core][empty]") {
    tuplet::tuple<> empty_tuple;
    static_assert(
        std::is_empty_v<tuplet::tuple<>>,
        "Empty tuple should be an empty type");
    static_assert(
        tuplet::stateless_v<tuplet::tuple<>>,
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
}


TEST_CASE("Swap tuples", "[core][swap]") {
    auto t1 = std::make_tuple(10, 20, std::string("Hello"));
    auto t2 = std::make_tuple(
        5,
        -129837,
        std::string("The quick brown fox jumps over the lazy dogs"));

    REQUIRE(std::get<0>(t1) == 10);
    REQUIRE(std::get<1>(t1) == 20);
    REQUIRE(std::get<2>(t1) == "Hello");

    REQUIRE(std::get<0>(t2) == 5);
    REQUIRE(std::get<1>(t2) == -129837);
    REQUIRE(
        std::get<2>(t2)
        == std::string("The quick brown fox jumps over the lazy dogs"));

    t1.swap(t2);

    REQUIRE(std::get<0>(t2) == 10);
    REQUIRE(std::get<1>(t2) == 20);
    REQUIRE(std::get<2>(t2) == "Hello");

    REQUIRE(std::get<0>(t1) == 5);
    REQUIRE(std::get<1>(t1) == -129837);
    REQUIRE(
        std::get<2>(t1)
        == std::string("The quick brown fox jumps over the lazy dogs"));

    swap(t1, t2);

    REQUIRE(std::get<0>(t1) == 10);
    REQUIRE(std::get<1>(t1) == 20);
    REQUIRE(std::get<2>(t1) == "Hello");

    REQUIRE(std::get<0>(t2) == 5);
    REQUIRE(std::get<1>(t2) == -129837);
    REQUIRE(
        std::get<2>(t2)
        == std::string("The quick brown fox jumps over the lazy dogs"));
}