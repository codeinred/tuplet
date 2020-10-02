#include <tuplet/tuplet.hpp>
#include <string>

bool is_good(int a, int b, std::string c) {
    return a == 1 && b == 2 && c == "Hello, world!";
}

int main() {
    using tuplet::tuple;
    using tuplet::pair;

    auto x = tuple {
        tuple { pair { 1, 2 }},
        tuple { "Hello, world!" }
    };
}
