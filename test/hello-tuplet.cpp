#include <cstdio>
#include <string>
#include <tuplet/tuplet.hpp>

using namespace tuplet;
int main() {
    auto tup = tuple{1, 2, std::string("Hello, world!")};

    printf("(%i, %i, \"%s\")\n\n", 
        tup[index<0>()], 
        tup[index<1>()], 
        tup[index<2>()].c_str());

    auto& [a, b, c] = tup;

    printf("In auto [a, b, c] = tup,\n  a = %i\n  b = %i\n  c = \"%s\"\n\n", a, b, c.c_str()); 
}
