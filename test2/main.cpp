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

TEST_CASE("Test tuplet::apply") {
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
