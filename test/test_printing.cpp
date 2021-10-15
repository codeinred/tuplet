#include <fmt/core.h>
#include <fmt/format.h>
#include "util/printing.hpp"
#include <tuplet/format.hpp>
#include <catch2/catch_test_macros.hpp>
#include <memory>

TEST_CASE("Test printing with fmt", "[printing]") {
    REQUIRE(fmt::format("{}", tuplet::tuple{1, 2, 3}) == "{1, 2, 3}");
    REQUIRE(fmt::format("{:[]}", tuplet::tuple{1, 2, 3}) == "[1, 2, 3]");
    REQUIRE(fmt::format("{:<>}", tuplet::tuple{1, 2, 3}) == "<1, 2, 3>");
    REQUIRE(fmt::format("{:()}", tuplet::tuple{1, 2, 3}) == "(1, 2, 3)");
    REQUIRE(fmt::format("{:{}}", tuplet::tuple{1, 2, 3}) == "{1, 2, 3}");

    REQUIRE(fmt::format("{:[>}", tuplet::tuple{1, 2, 3}) == "[1, 2, 3>");
    REQUIRE(fmt::format("{:<)}", tuplet::tuple{1, 2, 3}) == "<1, 2, 3)");
    REQUIRE(fmt::format("{:(}}", tuplet::tuple{1, 2, 3}) == "(1, 2, 3}");
    REQUIRE(fmt::format("{:{]}", tuplet::tuple{1, 2, 3}) == "{1, 2, 3]");
}
