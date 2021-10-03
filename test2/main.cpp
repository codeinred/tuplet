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

    SECTION("apply-1: Check tuplet::apply with tuplet::tie") {
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

    SECTION("assign-1: Check assignment with tuplet::tie") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c).assign(1, 2, "Hello, world!");

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("assign-2: Check = operator for tuples") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c) = tuplet::tuple {
            1,
            2,
            std::string("Hello, world!")};

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("assign-3: Operator = with heterogenous element types") {
        REQUIRE((a == 0 && b == 0 && c == ""));

        tuplet::tie(a, b, c) = tuplet::tuple {1, 2, "Hello, world!"};

        REQUIRE(a == 1);
        REQUIRE(b == 2);
        REQUIRE(c == "Hello, world!");
    }

    SECTION("assign-4: check assignment results in moves") {
        tuplet::tuple<std::unique_ptr<int>, std::string> t, q;

        t = {std::make_unique<int>(69420), "Hello, world!"};

        q = std::move(t);

        REQUIRE(t[0_tag].get() == nullptr);
        REQUIRE(*q[0_tag] == 69420);
        REQUIRE(q[1_tag] == "Hello, world!");
    }

    SECTION("assign-5: check tuple assignment") {
        using namespace tuplet::literals;
        tuplet::tuple<int, int, std::string> t1 {1, 2, "Hello, world!"}, t2;

        t2 = t1;

        REQUIRE(t2[0_tag] == 1);
        REQUIRE(t2[1_tag] == 2);
        REQUIRE(t2[2_tag] == "Hello, world!");
    }
}
