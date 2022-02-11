#include "util/printing.hpp"
#include <catch2/catch_test_macros.hpp>
#include <iterator>
#include <memory>
#include <string>
#include <string_view>
#include <tuplet/tuple.hpp>

TEST_CASE("Test that for_each works", "[for_each]") {
    using tuplet::tuple;

    std::string str;

    auto ins = std::back_inserter(str);

    tuple tup {
        "Hello world!",
        std::string("This is a test..."),
        10,
        'x',
        20,
        3.141592};

    tup.for_each([&](auto& value) { fmt::format_to(ins, "{}\n", value); });

    REQUIRE(
        str
        == "Hello world!\n"      //
           "This is a test...\n" //
           "10\n"                //
           "x\n"                 //
           "20\n"                //
           "3.141592\n");
}

TEST_CASE(
    "Test that for_each moves values when given moved-from tuple",
    "[for_each]") {
    using tuplet::tuple;

    auto tup = tuple {
        std::make_unique<int>(10),
        std::make_unique<int>(20),
        std::make_unique<int>(30)};

    SECTION("Check that the tuple is valid") {
        REQUIRE(tup.all([](auto& val) { return bool(val); }));
    }
    int sum = 0;
    std::move(tup).for_each([&](auto value) { sum += *value; });

    SECTION("check that the tuple was moved from and the sum is 60") {
        REQUIRE(tup.all([](auto& val) { return bool(val) == false; }));
        REQUIRE(sum == 60);
    }
}
