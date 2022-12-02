#include <cstdio>
#include <memory>
#include <string>
#include <tuplet/tuple.hpp>

bool is_good(int a, std::string b) {
    return a == 69420 && b == "Hello, world!";
}

int main() {
    using namespace tuplet::literals;
    using tuplet::tuple;

    tuple<std::unique_ptr<int>, std::string> t, q;

    t = {std::make_unique<int>(69420), "Hello, world!"};

    q = std::move(t);

    // If the unique_ptr still holds a value for t, return 1
    if (t[0_tag]) {
        return 1;
    }
    return !is_good(*q[0_tag], q[1_tag]);
}
