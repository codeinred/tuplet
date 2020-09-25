#include <cstdio>
#include <memory>
#include <string>
#include <tuplet/tuplet.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    auto tup = tuplet::tuple{
        1, std::make_unique<int>(2), std::string("Hello, world!")};

    // Check that moving a tuple moves the elements when
    // doing a structured bind
    auto [a, b, c] = std::move(tup);

    return !is_good(a, *b, c);
}
