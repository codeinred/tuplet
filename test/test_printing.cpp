#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <fmt/format.h>
#include <memory>
#include <tuplet/format.hpp>

TEST_CASE("Test printing with fmt", "[printing]") {
    REQUIRE(fmt::format("{}", tuplet::tuple {1, 2, 3}) == "(1, 2, 3)");
    REQUIRE(fmt::format("{:[]}", tuplet::tuple {1, 2, 3}) == "[1, 2, 3]");
    REQUIRE(fmt::format("{:<>}", tuplet::tuple {1, 2, 3}) == "<1, 2, 3>");
    REQUIRE(fmt::format("{:()}", tuplet::tuple {1, 2, 3}) == "(1, 2, 3)");
    REQUIRE(fmt::format("{:{}}", tuplet::tuple {1, 2, 3}) == "{1, 2, 3}");

    REQUIRE(fmt::format("{:[>}", tuplet::tuple {1, 2, 3}) == "[1, 2, 3>");
    REQUIRE(fmt::format("{:<)}", tuplet::tuple {1, 2, 3}) == "<1, 2, 3)");
    REQUIRE(fmt::format("{:(}}", tuplet::tuple {1, 2, 3}) == "(1, 2, 3}");
    REQUIRE(fmt::format("{:{]}", tuplet::tuple {1, 2, 3}) == "{1, 2, 3]");
}
