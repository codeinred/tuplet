#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    auto tup = tuplet::tuple {0, 0, std::string()};

    auto& [a, b, c] = tup;

    a = 1;
    b = 2;
    c = "Hello, world!";

    return !is_good(get<0>(tup), get<1>(tup), get<2>(tup));
}
