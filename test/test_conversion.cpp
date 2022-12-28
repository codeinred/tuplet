#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_version_macros.hpp>
#include <memory>
#include <tuple>
#include <tuplet/tuple.hpp>
#include <vector>

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

    std::tuple<int, int, std::unique_ptr<int>> t2 = tuplet::convert {
        std::move(tup1)};

    REQUIRE(std::get<0>(t2) == 1);
    REQUIRE(std::get<1>(t2) == 2);
    REQUIRE(std::get<2>(t2) != nullptr);
}

TEST_CASE("Tuple::as conversion to references", "[core][conversion]") {
    using namespace tuplet::literals;
    auto tup1 = tuplet::tuple {0, 0, std::string()};

    auto tup2 = tup1.as<tuplet::tuple<int&, int&, std::string&>>();

    tup1[0_tag] = 1;
    tup1[1_tag] = 2;
    tup1[2_tag] = "Hello, world!";

    REQUIRE(tup2 == tuplet::tuple {1, 2, "Hello, world!"});
}

TEST_CASE("Tuple::as conversion to struct", "[core][conversion]") {
    using namespace tuplet::literals;
    struct Vals {
        int a;
        int b;
        std::string c;
    };

    auto tup1 = tuplet::tuple {1, 2, std::string("Hello, world!")};

    Vals val = tup1.as<Vals>();

    REQUIRE(val.a == 1);
    REQUIRE(val.b == 2);
    REQUIRE(val.c == "Hello, world!");
}

TEST_CASE("Tuple::as conversion to std::Tuple", "[core][conversion]") {
    using namespace tuplet::literals;

    auto tup1 = tuplet::tuple {1, 2, std::string("Hello, world!")};

    auto val = tup1.as<std::tuple<int, int, std::string>>();

    REQUIRE(std::get<0>(val) == 1);
    REQUIRE(std::get<1>(val) == 2);
    REQUIRE(std::get<2>(val) == "Hello, world!");
}


TEST_CASE("Tuple::as conversion to std::vector", "[core][conversion]") {
    using namespace tuplet::literals;

    auto tup1 = tuplet::tuple {1, 2, 3, 4, 5};

    auto v = tup1.as<std::vector<int>>();

    REQUIRE(v == std::vector<int> {1, 2, 3, 4, 5});
}

TEST_CASE("Tuple::as conversion results in move", "[core][conversion]") {
    using namespace tuplet::literals;

    auto tup1 = tuplet::tuple {1, 2, std::make_unique<int>(3)};

    auto t2 = std::move(tup1).as<std::tuple<int, int, std::unique_ptr<int>>>();

    REQUIRE(std::get<0>(t2) == 1);
    REQUIRE(std::get<1>(t2) == 2);
    REQUIRE(std::get<2>(t2) != nullptr);
}

TEST_CASE(
    "Tuple explicit conversion to and from references",
    "[core][conversion]") {
    using namespace tuplet::literals;
    auto tup1 = tuplet::tuple {0, 0, std::string()};

    tuplet::tuple<int&, int&, std::string&> tup2(tup1);

    tup1[0_tag] = 1;
    tup1[1_tag] = 2;
    tup1[2_tag] = "Hello, world!";

    REQUIRE(tup2 == tuplet::tuple {1, 2, "Hello, world!"});

    tuplet::tuple<int, int, std::string> tup3(tup2);

    REQUIRE(tup3 == tuplet::tuple {1, 2, "Hello, world!"});
}
