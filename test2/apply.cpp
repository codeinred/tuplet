#include <catch2/catch_test_macros.hpp>
#include <tuplet/tuple.hpp>

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
