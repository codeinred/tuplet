#include "util/reflection.hpp"
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <fmt/format.h>
#include <tuplet/tuple.hpp>

// To get clang-cl: cmake -T ClangCL ...

template <typename Tuple, size_t... Indexes, typename Fn>
constexpr decltype(auto) tuple_for_each_impl(
    Tuple&& tuple,
    std::integer_sequence<size_t, Indexes...>,
    Fn&& fn) {
    using tuplet::get;

    (fn(get<Indexes>(std::forward<Tuple>(tuple))), ...);

    return std::forward<Fn>(fn);
}
template <typename Tuple, typename Fn>
constexpr decltype(auto) tuple_for_each(Tuple&& tuple, Fn&& fn) {
    using indexes = std::make_index_sequence<
        std::tuple_size<std::decay_t<Tuple>>::value>;

    return tuple_for_each_impl(
        std::forward<Tuple>(tuple),
        indexes {},
        std::forward<Fn>(fn));
}

template <size_t Alignment, typename T>
constexpr T align_value(T value) {
    constexpr auto alignment {static_cast<T>(Alignment)};

    value += alignment - 1;
    value &= ~(alignment - 1);

    return value;
}

#if (defined _MSC_VER)
constexpr bool is_cl_or_clang_cl = true;
#else
constexpr bool is_cl_or_clang_cl = false;
#endif

constexpr bool has_no_unique_address = TUPLET_HAS_NO_UNIQUE_ADDRESS;

struct empty {};

struct struct_with_empty {
    alignas(sizeof(int) * 2) int a;
    TUPLET_NO_UNIQUE_ADDRESS empty b;
    int c;
};

#if TUPLET_HAS_NO_UNIQUE_ADDRESS && !(defined _MSC_VER)
static_assert(offsetof(struct_with_empty, b) == 0);
static_assert(sizeof(struct_with_empty) == sizeof(int) * 2);

// c's offset is different with cl + [[msvc::no_unique_address]] than gcc/clang
// with [[no_unique_address]].
static_assert(
    offsetof(struct_with_empty, c) == sizeof(int) || is_cl_or_clang_cl);
#endif

template <class T>
uintptr_t pointer_to_int(T const* pointer) {
    uintptr_t dest {};
    static_assert(sizeof(T const*) == sizeof(dest));
    memcpy(&dest, &pointer, sizeof(T const*));
    return dest;
}

template <typename Tuple>
void test_tuple_alignment() {
    Tuple t;
    const auto base_addr = pointer_to_int(&t);
    size_t offset {0}, index {0};

    INFO(fmt::format("{}", refl::name_of_type<Tuple>));

    tuple_for_each(t, [&](auto& element) {
        using element_type = std::decay_t<decltype(element)>;

        INFO(fmt::format(
            "Element index {}, sizeof {}, alignof {}",
            index++,
            sizeof(element_type),
            alignof(element_type)));

        const auto addr {reinterpret_cast<uintptr_t>(&element)};
        const auto element_offset {addr - base_addr};
        constexpr auto element_size {sizeof(element_type)};

        offset = align_value<alignof(element_type)>(offset);

        if (has_no_unique_address) {
            // cl with [[msvc::no_unique_address]] will not optimize out empty
            // tuple elements.
            if (is_cl_or_clang_cl || (element_type::s_size != 0)) {
                CHECK(element_offset == offset);

                if (!is_cl_or_clang_cl && !std::is_aggregate_v<element_type>) {
                    // Only non-aggregates allow their padding to be used for
                    // the next element.
                    // cl doesn't do this either.
                    offset += element_type::s_size;
                } else {
                    offset += element_size;
                }
            } else {
                // gcc and clang will optimize away and the offset is 0, not the
                // offset of the previous element.
                CHECK(element_offset == 0);
            }
        } else {
            // Simple case; no [[no_unique_address]] affecting things.
            CHECK(element_offset == offset);
            offset += element_size;
        }
    });

    offset = align_value<alignof(Tuple)>(offset);

    CHECK(offset == sizeof(Tuple));
}

template <size_t Size, size_t Alignment>
struct alignas(Alignment) aligned_buffer_aggregate {
    static constexpr size_t s_size {Size};

    uint8_t m_buff[s_size];
};

template <size_t Alignment>
struct alignas(Alignment) aligned_buffer_aggregate<0, Alignment> {
    static constexpr size_t s_size {0};
};

template <size_t Size, size_t Alignment>
struct alignas(Alignment) aligned_buffer {
    static constexpr size_t s_size {Size};

    uint8_t m_buff[s_size];

    aligned_buffer() {}
};

template <size_t Alignment>
struct alignas(Alignment) aligned_buffer<0, Alignment> {
    static constexpr size_t s_size {0};

    aligned_buffer() {}
};

TEST_CASE("Check alignment", "[alignment]") {
    using buff40_64_a = aligned_buffer_aggregate<40, 64>;
    using buff10_16_a = aligned_buffer_aggregate<10, 16>;
    using buff15_32_a = aligned_buffer_aggregate<15, 32>;
    using buff0_16_a = aligned_buffer_aggregate<0, 16>;
    using buff13_8_a = aligned_buffer_aggregate<13, 8>;

    using buff40_64 = aligned_buffer<40, 64>;
    using buff10_16 = aligned_buffer<10, 16>;
    using buff15_32 = aligned_buffer<15, 32>;
    using buff0_16 = aligned_buffer<0, 16>;
    using buff13_8 = aligned_buffer<13, 8>;

    test_tuple_alignment<tuplet::tuple<
        buff40_64_a,
        buff10_16_a,
        buff15_32_a,
        buff0_16_a,
        buff13_8_a>>();

    test_tuple_alignment<
        tuplet::tuple<buff40_64, buff10_16, buff15_32, buff0_16, buff13_8>>();

    test_tuple_alignment<
        tuplet::
            tuple<buff40_64_a, buff10_16, buff15_32_a, buff0_16, buff13_8_a>>();

    test_tuple_alignment<
        tuplet::
            tuple<buff40_64, buff10_16_a, buff15_32, buff0_16_a, buff13_8>>();
}
