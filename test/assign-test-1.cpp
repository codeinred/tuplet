#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tie(a, b, c).assign(1, 2, "Hello, world!");

    return !is_good(a, b, c);
}
