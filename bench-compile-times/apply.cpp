#if USE_STD_TUPLE
#include <tuple>
using std::tuple;
#else
#include <tuplet/tuple.hpp>
using tuplet::tuple;
#endif

// Defined in print_ans.cpp
void print_ans(int);

// Function givet to apply
constexpr auto sum = [](auto... values) {
    return (values + ...);
};

int main() {
    tuple tup { VALUES };
    int result = apply(sum, tup);
    print_ans(result);
}
