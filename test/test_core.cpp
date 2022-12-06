#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_version_macros.hpp>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <vector>
#include <memory>

static_assert(CATCH_VERSION_MAJOR >= 3);
static_assert(CATCH_VERSION_MINOR >= 0);

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


TEST_CASE("Tuple conversion to references", "[core][conversion]") {
    using namespace tuplet::literals;
    auto tup1 = tuplet::tuple {0, 0, std::string()};

    tuplet::tuple<int&, int&, std::string&> tup2 = tuplet::convert {tup1};

    tup1[0_tag] = 1;
    tup1[1_tag] = 2;
    tup1[2_tag] = "Hello, world!";

    REQUIRE(tup2 == tuplet::tuple {1, 2, "Hello, world!"});
}

TEST_CASE("Tuple conversion to struct", "[core][conversion]") {
    using namespace tuplet::literals;
    struct Vals {
        int a;
        int b;
        std::string c;
    };

    auto tup1 = tuplet::tuple {1, 2, std::string("Hello, world!")};

    Vals val = tuplet::convert {tup1};

    REQUIRE(val.a == 1);
    REQUIRE(val.b == 2);
    REQUIRE(val.c == "Hello, world!");
}

TEST_CASE("Tuple conversion to std::Tuple", "[core][conversion]") {
    using namespace tuplet::literals;

    auto tup1 = tuplet::tuple {1, 2, std::string("Hello, world!")};

    std::tuple<int, int, std::string> val = tuplet::convert {tup1};

    REQUIRE(std::get<0>(val) == 1);
    REQUIRE(std::get<1>(val) == 2);
    REQUIRE(std::get<2>(val) == "Hello, world!");
}


TEST_CASE("Tuple conversion to std::vector", "[core][conversion]") {
    using namespace tuplet::literals;

    auto tup1 = tuplet::tuple {1, 2, 3, 4, 5};

    std::vector<int> v = tuplet::convert {tup1};

    REQUIRE(v == std::vector<int> {1, 2, 3, 4, 5});
}

TEST_CASE("Tuple conversion results in move", "[core][conversion]") {
    using namespace tuplet::literals;

    auto tup1 = tuplet::tuple {1, 2, std::make_unique<int>(3)};

    std::tuple<int, int, std::unique_ptr<int>> t2 = tuplet::convert { std::move(tup1) };

    REQUIRE(std::get<0>(t2) == 1);
    REQUIRE(std::get<1>(t2) == 2);
    REQUIRE(std::get<2>(t2) != nullptr);
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
