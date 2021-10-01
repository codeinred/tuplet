#include <tuplet/tuple.hpp>

struct A {
    int value = 0;
};
struct B {
    int value = 0;
    B& operator=(B const& b) {
        value = b.value;
        return *this;
    }
    B& operator=(B&& b) {
        value = b.value;
        return *this;
    }
};
struct C {
    int value;
    template <class U>
    C& operator=(U&& other) {
        value = other.value;
        return *this;
    }
};
int main() {
    using tuple_A = tuplet::tuple<int, int, int>;
    using tuple_B = tuplet::tuple<int, int, long>;

    tuple_A a {0, 0, 0};
    tuple_B b {0, 0, 10};
    a = b;

    static_assert(!std::is_trivially_assignable_v<B, B>);
    static_assert(std::is_trivially_assignable_v<C, C>);
    static_assert(!std::is_trivially_assignable_v<C, B>);

    static_assert(
        std::is_trivially_copyable_v<tuple_A>,
        "Expected tuple<int, int, int> to be trivially copyable");
    static_assert(
        std::is_default_constructible_v<tuple_A>,
        "Expected tuple<int, int, int> to be default constructible");
    static_assert(
        std::is_aggregate_v<tuple_A>,
        "Expected tuple<int, int, int> to be an aggregate");

    static_assert(
        noexcept(a = a),
        "Assigning a tuple to itself must be noexcept for trivial types");
    static_assert(
        std::is_trivially_copy_assignable_v<tuple_A>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        std::is_trivially_copy_assignable_v<tuple_B>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        std::is_trivially_assignable_v<tuple_A, tuple_A>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        std::is_trivially_assignable_v<tuple_B, tuple_B>,
        "A tuple should be trivially assignable to itself if the members are");
    static_assert(
        !std::is_trivially_assignable_v<tuple_A, tuple_B>,
        "A tuple should not be trivially assignable to converted tuples");
    static_assert(
        std::is_trivially_move_assignable_v<tuple_A>,
        "A tuple should be trivially move assignable if the members are");
    static_assert(
        std::is_trivially_move_assignable_v<tuple_B>,
        "A tuple should be trivially move assignable if the members are");

    static_assert(
        std::is_trivial_v<tuple_A>,
        "A tuple should be trivial if it's members are.");
    static_assert(
        std::is_trivial_v<tuple_B>,
        "A tuple should be trivial if it's members are.");

    return 0;
}
