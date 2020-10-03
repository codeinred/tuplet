#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    auto tup1 = tuplet::tuple {1, 2, std::string("Hello, world!")};

    auto tup = tup1;

    static_assert(
        std::is_same_v<std::tuple_element_t<0, decltype(tup)>, int>,
        "Expected int& at element 0");
    static_assert(
        std::is_same_v<std::tuple_element_t<1, decltype(tup)>, int>,
        "Expected int& at element 1");
    static_assert(
        std::is_same_v<std::tuple_element_t<2, decltype(tup)>, std::string>,
        "Expected std::string& at element 2");

    return !is_good(get<0>(tup), get<1>(tup), get<2>(tup));
}
