#include <tuplet/tuplet.hpp>
#include <cstdio>
#include <string>

// Included to ensure there's no conflict with std::apply
#include <tuple>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}
int main() {
    int a = 0;
    int b = 0;
    std::string c;

    auto func = [](auto& x, auto& y, auto& z) {
        x = 1;
        y = 2;
        z = "Hello, world!";
    };

    apply(func, tuplet::tie(a, b, c));

    return !is_good(a, b, c);
}
