#include <cstdio>
#include <memory>
#include <string>
#include <tuple>
#include <tuplet/tuple.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    using namespace tuplet::literals;
    auto tup = tuplet::tuple {
        1,
        std::make_unique<int>(2),
        std::string("Hello, world!")};

    static_assert(
        std::is_same_v<std::tuple_element_t<0, decltype(tup)>, int>,
        "Expected int& at element 0");
    static_assert(
        std::is_same_v<
            std::tuple_element_t<1, decltype(tup)>,
            std::unique_ptr<int>>,
        "Expected int& at element 1");
    static_assert(
        std::is_same_v<std::tuple_element_t<2, decltype(tup)>, std::string>,
        "Expected std::string& at element 2");

    auto a = get<0>(tup);
    auto b = get<1>(std::move(tup)); // Check that when a tuple is moved, the
                                     // indicies are moved too
    auto c = get<2>(tup);
    return !is_good(a, *b, c);
}
