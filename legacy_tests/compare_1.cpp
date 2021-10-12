#include <cstdio>
#include <functional>
#include <tuplet/tuple.hpp>

int main() {
    using tuplet::tuple;

    constexpr tuple t1 {10, 1};
    constexpr tuple t2 {10, 2};
    static_assert(t1 == t1, "Tuple equality broken");
    static_assert(t1 != t2, "Tuple equality broken");
    static_assert(t1 < t2, "Tuple compare broken");

    int a = 10, b = 1, c = 2;

    std::reference_wrapper<int> ra = std::ref(a);
    static_assert(
        std::is_same_v<tuplet::unwrap_ref_decay_t<decltype(ra)>, int&>,
        "unwrap_ref_decay_t broken");
    static_assert(
        std::is_same_v<tuplet::unwrap_ref_decay_t<decltype(std::ref(a))>, int&>,
        "unwrap_ref_decay_t broken");
    tuple<const int&, int&> t3 = tuple {std::cref(a), std::ref(b)};
    tuple t4 = {std::cref(a), std::ref(c)};
    static_assert(std::is_same_v<decltype(t4), tuple<const int&, int&>>);

    if (t3 > t4) {
        return 1;
    }
    b = 2;
    c = 1;
    if (t3 < t4) {
        return 1;
    }
}
