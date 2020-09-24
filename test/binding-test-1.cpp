#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

int main() {
    auto tup = tuplet::tuple{1, 2, std::string("Hello, world!")};

    auto [a, b, c] = tup;

    printf("In auto [a, b, c] = tup,\n  a = %i\n  b = %i\n  c = \"%s\"\n\n", a, b, c.c_str());
}
