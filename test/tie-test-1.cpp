#include <tuplet/tuplet.hpp>
#include <functional>
#include <cstdio>
#include <string>

int main() {
    int a = 0;
    int b = 0;
    std::string c;

    auto tup = tuplet::tie(a, b, c);

    static_assert(std::is_same_v<std::tuple_element_t<0 , decltype(tup)>, int&>, "Expected int& at element 0");
    static_assert(std::is_same_v<std::tuple_element_t<1 , decltype(tup)>, int&>, "Expected int& at element 1");
    static_assert(std::is_same_v<std::tuple_element_t<2 , decltype(tup)>, std::string&>, "Expected std::string& at element 2");

    get<0>(tup) = 1;
    get<1>(tup) = 2;
    get<2>(tup) = "Hello, world!";

    printf("  a = %i\n  b = %i\n  c = \"%s\"\n\n", a, b, c.c_str());
}
