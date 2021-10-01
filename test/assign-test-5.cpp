#include <cstdio>
#include <string>
#include <tuplet/tuple.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    using namespace tuplet::literals;

    int a = 0;
    int b = 0;
    std::string c;

    tuplet::tuple<int, int, std::string> t1 {1, 2, "Hello, world!"}, t2;

    t2 = t1;

    return !is_good(t2[0_tag], t2[1_tag], t2[2_tag]);
}
