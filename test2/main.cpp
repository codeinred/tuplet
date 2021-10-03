#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <tuplet/format.hpp>
#include <tuplet/tuple.hpp>

namespace Catch {
template <class... T>
std::ostream& operator<<(std::ostream& cout, tuplet::tuple<T...> const& tup) {
    return cout << fmt::format("{:[]}", tup);
}
} // namespace Catch

TEST_CASE("tuplet::apply") {
    using namespace tuplet::literals;
    using tuplet::tuple;

    int a = 0;
    int b = 0;
    std::string c;

    SECTION("Check tuplet::apply with tuplet::tie", "[apply]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        auto func = [](auto& x, auto& y, auto& z) {
            x = 1;
            y = 2;
            z = "Hello, world!";
        };

        apply(func, tuplet::tie(a, b, c));

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }
}
TEST_CASE("Test Assignment") {
    using namespace tuplet::literals;
    using tuplet::tuple;

    int a = 0;
    int b = 0;
    std::string c;

    SECTION("Check assignment with tuplet::tie", "[assign]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c).assign(1, 2, "Hello, world!");

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("Check = operator for tuples", "[assign]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c) = tuplet::tuple {
            1,
            2,
            std::string("Hello, world!")};

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("Operator = with heterogenous element types", "[assign]") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c) = tuplet::tuple {1, 2, "Hello, world!"};

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("check assignment results in moves", "[assign]") {
        tuplet::tuple<std::unique_ptr<int>, std::string> t, q;

        t = {std::make_unique<int>(69420), "Hello, world!"};

        q = std::move(t);

        REQUIRE(t[0_tag].get() == nullptr);
        REQUIRE(*q[0_tag] == 69420);
        REQUIRE(q[1_tag] == "Hello, world!");
    }

    SECTION("check tuple assignment", "[assign]") {
        using namespace tuplet::literals;
        tuplet::tuple<int, int, std::string> t1 {1, 2, "Hello, world!"}, t2;

        t2 = t1;

        REQUIRE(t2[0_tag] == 1);
        REQUIRE(t2[1_tag] == 2);
        REQUIRE(t2[2_tag] == "Hello, world!");
    }
}
TEST_CASE("Test structured bindings") {
    SECTION("Check tuple decomposition", "[binding]") {
        auto tup = tuplet::tuple {1, 2, std::string("Hello, world!")};

        auto [a, b, c] = tup;

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("Check tuple decomposition by reference", "[binding]") {
        auto tup = tuplet::tuple {0, 0, std::string()};

        auto& [a, b, c] = tup;

        a = 1;
        b = 2;
        c = "Hello, world!";

        REQUIRE(get<0>(tup) == 1);
        REQUIRE(get<1>(tup) == 2);
        REQUIRE(get<2>(tup) == "Hello, world!");
    }

    SECTION("Check tuple decomposition by move", "[binding]") {
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
}

TEST_CASE("Test comparisons", "[compare]") {
    using tuplet::tuple;

    SECTION("lexiconographic ordering", "[compare]") {
        tuple t1 {0, 0};
        tuple t2 {0, 1};
        tuple t3 {1, 0};

        REQUIRE(t1 == t1);
        REQUIRE(t1 < t2);
        REQUIRE(t2 < t3);
    }
}

SCENARIO("We have tuples created with references", "[compare]") {
    using tuplet::tuple;

    int a = 0, b = 1, c = 2;
    tuple t1 = {std::ref(a), std::ref(b)};
    tuple t2 = {std::ref(a), std::ref(c)};
    REQUIRE(std::is_same_v<decltype(t1), tuple<int&, int&>>);

    REQUIRE(t1 < t2);
    REQUIRE(t1 != t2);
    REQUIRE(!(t1 == t2));
    REQUIRE(!(t1 > t2));

    WHEN("The values in b and c are swapped") {
        std::swap(b, c);
        THEN("The relative ordering of the tuples changes") {
            REQUIRE(t2 < t1);
            REQUIRE(t1 != t2);
            REQUIRE(!(t1 == t2));
            REQUIRE(!(t2 > t1));
        }
    }
}
