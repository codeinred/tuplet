#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    auto tup = tuplet::tuple{1, 2, std::string("Hello, world!")};

    auto [a, b, c] = tup;

    return !is_good(a, b, c);
}
