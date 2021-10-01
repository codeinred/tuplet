#include <cstdio>
#include <tuplet/tuple.hpp>

int main() {
    using tuplet::tuple;

    constexpr tuple t1 {10, 1};
    constexpr tuple t2 {10, 2};
    static_assert(t1 == t1, "Tuple equality broken");
    static_assert(t1 != t2, "Tuple equality broken");
    static_assert(t1 < t2, "Tuple compare broken");
}
