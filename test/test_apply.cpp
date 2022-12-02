#include <catch2/catch_test_macros.hpp>
#include <tuplet/tuple.hpp>

TEST_CASE("Check tuplet::apply with tuplet::tie", "[apply]") {
    int a = 0;
    int b = 0;
    std::string c;
    REQUIRE((a == 0 && b == 0 && c == ""));

    auto func = [](auto& x, auto& y, auto& z) {
        x = 1;
        y = 2;
        z = "Hello, world!";
    };

    tuplet::apply(func, tuplet::tie(a, b, c));

    REQUIRE(a == 1);
    REQUIRE(b == 2);
    REQUIRE(c == "Hello, world!");
}
