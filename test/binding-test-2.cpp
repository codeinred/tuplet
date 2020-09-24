#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

int main() {
    auto tup = tuplet::tuple{0, 0, std::string()};

    auto& [a, b, c] = tup;

    a = 1;
    b = 2;
    c = "Hello, world!";

    printf("tup = (%i, %i, \"%s\")\n\n",
        get<0>(tup),
        get<1>(tup),
        get<2>(tup).c_str());
}
