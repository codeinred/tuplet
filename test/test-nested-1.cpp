#include <string>
#include <tuplet/tuple.hpp>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    using tuplet::pair;
    using tuplet::tuple;
    using namespace tuplet::literals;

    auto x = tuple {tuple {pair {1, 2}}, tuple {"Hello, world!"}};

    return !is_good(
        x[0_tag][0_tag][0_tag], x[0_tag][0_tag][1_tag], x[1_tag][0_tag]);
}
