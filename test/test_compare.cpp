#include "util/printing.hpp"
#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <tuplet/format.hpp>
#include <tuplet/tuple.hpp>



TEST_CASE("Constexpr compare", "[compare]") {

    constexpr tuplet::tuple t1 {10, 1};
    constexpr tuplet::tuple t2 {10, 2};
    constexpr tuplet::tuple t3 {10, 2ul};
    static_assert(t1 == t1, "Tuple equality broken");
    static_assert(t1 != t2, "Tuple equality broken");
    static_assert(t1 < t2, "Tuple compare broken");
    static_assert(t2 == t3, "Tuple equality broken");
    static_assert(t1 != t3, "Tuple equality broken");
    static_assert(t1 < t3, "Tuple compare broken");
}


TEST_CASE("Lexiconographic ordering", "[compare]") {
    using tuplet::tuple;

    tuple t1 {0, 0};
    tuple t2 {0, 1};
    tuple t3 {1, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}


TEST_CASE("Lexiconographic ordering", "[compare-str]") {
    using tuplet::tuple;
    using namespace std::string_view_literals;

    static_assert(tuplet::sfinae::detail::
                      _test_m_compare<std::string_view, std::string_view>(0));

    auto t1 = tuple {"0"sv, "0"sv};
    auto t2 = tuple {"0"sv, "00"sv};
    auto t3 = tuple {"00"sv, "0"sv};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}

TEST_CASE("Lexiconographic ordering pairs", "[compare]") {
    using tuplet::pair;

    pair t1 {0, 0};
    pair t2 {0, 1};
    pair t3 {1, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}

TEST_CASE("Lexiconographic ordering, different types", "[compare]") {
    using tuplet::tuple;

    tuple t1 {0, 0};
    tuple t2 {0ul, 1};
    tuple t3 {1.0, 0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 < t2);
    REQUIRE(t2 < t3);
    REQUIRE(t1 < t3);
    REQUIRE(t2 > t1);
    REQUIRE(t3 > t2);
    REQUIRE(t3 > t1);

    REQUIRE(t1 <= t2);
    REQUIRE(t2 <= t3);
    REQUIRE(t3 >= t1);
    REQUIRE(t2 >= t1);
    REQUIRE(t3 >= t2);
    REQUIRE(t3 >= t1);
}

TEST_CASE("Tuples with different types", "[compare]") {
    using tuplet::tuple;

    tuple t1 {1, 2};
    tuple t2 {'\1', '\2'};
    tuple t3 {1ul, 2.0};

    REQUIRE(t1 == t1);
    REQUIRE(t1 == t2);
    REQUIRE(t1 == t3);
    REQUIRE(t2 == t3);
}

TEST_CASE("Empty tuple equals itself", "[compare]") {
    REQUIRE(tuplet::tuple {} == tuplet::tuple {});
    REQUIRE_FALSE(tuplet::tuple {} != tuplet::tuple {});
}

SCENARIO("We have tuples created with references", "[compare]") {
    using tuplet::tuple;

    int a = 0, b = 1, c = 2;
    tuple t1 = {std::ref(a), std::ref(b)};
    tuple t2 = {std::ref(a), std::ref(c)};
    REQUIRE(std::is_same_v<decltype(t1), tuple<int&, int&>>);

    REQUIRE(t1 < t2);
    REQUIRE(t1 != t2);
    REQUIRE_FALSE(t1 == t2);
    REQUIRE_FALSE(t1 > t2);

    WHEN("The values in b and c are swapped") {
        std::swap(b, c);
        THEN("The relative ordering of the tuples changes") {
            REQUIRE(t2 < t1);
            REQUIRE(t1 != t2);
            REQUIRE_FALSE(t1 == t2);
            REQUIRE_FALSE(t2 > t1);
        }
    }
}


template <
    class A,
    class B,
    class = decltype(std::declval<A>() == std::declval<B>())>
constexpr bool _sfinae_test_eq(int) {
    return true;
}

template <
    class A,
    class B,
    class = decltype(std::declval<A>() < std::declval<B>())>
constexpr bool _sfinae_test_less(int) {
    return true;
}

template <class A, class B>
constexpr bool _sfinae_test_eq(long long) {
    return false;
}

template <class A, class B>
constexpr bool _sfinae_test_less(long long) {
    return false;
}

TEST_CASE("Tuple Comparison Supports SFINAE", "[compare][sfinae]") {
    using tuplet::tuple;
    using tuplet::type_list;
    struct Uncomparable {};

    auto t1 = tuple {1, 2, 3};
    auto t2 = tuple {std::string {"Hello"}, 2, 3};
    auto t3 = tuple {Uncomparable {}, 2, 3};

    constexpr bool result = _sfinae_test_eq<decltype(t1), decltype(t2)>(0);

    static_assert(tuplet::sfinae::detail::_test_eq<int, int>(0) == true);
    static_assert(
        tuplet::sfinae::detail::_test_eq<int, std::string>(0) == false);
    static_assert(
        tuplet::sfinae::detail::_test_eq<int, const char*>(0) == false);

    static_assert(_sfinae_test_eq<tuple<int>, tuple<const char*>>(0) == false);

    STATIC_REQUIRE(_sfinae_test_eq<decltype(t1), decltype(t1)>(0));
    STATIC_REQUIRE(_sfinae_test_less<decltype(t1), decltype(t1)>(0));
    STATIC_REQUIRE(_sfinae_test_eq<decltype(t2), decltype(t2)>(0));
    STATIC_REQUIRE(_sfinae_test_less<decltype(t2), decltype(t2)>(0));

    STATIC_REQUIRE_FALSE(_sfinae_test_eq<decltype(t1), int>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_less<decltype(t1), int>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_eq<int, decltype(t1)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_less<int, decltype(t1)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_eq<decltype(t1), decltype(t2)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_less<decltype(t1), decltype(t2)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_eq<decltype(t1), decltype(t3)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_less<decltype(t1), decltype(t3)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_eq<decltype(t3), decltype(t2)>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_less<decltype(t3), decltype(t2)>(0));

    auto t_short = tuple {1};
    auto t_long = tuple {1, 2};

    STATIC_REQUIRE_FALSE(
        _sfinae_test_less<decltype(t_short), decltype(t_long)>(0));


    STATIC_REQUIRE_FALSE(_sfinae_test_eq<decltype(t1), int>(0));
    STATIC_REQUIRE_FALSE(_sfinae_test_less<decltype(t1), int>(0));
}
