#include <catch2/catch_test_macros.hpp>
#include <tuplet/size_sorted_tuple.hpp>

using namespace tuplet::literals;

TEST_CASE("attribute_sorted_indices sorts the indices in place of types "
          "according to the specified attribute and less.") {
    {
        auto constexpr sorted_indices = tuplet::size_sorted::
            attribute_sorted_indices<
                []<class T>() { return T::value; },
                std::less<>,
                tuplet::tag<5>,  // 0
                tuplet::tag<2>,  // 1
                tuplet::tag<3>,  // 2
                tuplet::tag<4>,  // 3
                tuplet::tag<1>,  // 4
                tuplet::tag<0>>; // 5

        STATIC_REQUIRE(sorted_indices[0] == 5);
        STATIC_REQUIRE(sorted_indices[1] == 4);
        STATIC_REQUIRE(sorted_indices[2] == 1);
        STATIC_REQUIRE(sorted_indices[3] == 2);
        STATIC_REQUIRE(sorted_indices[4] == 3);
        STATIC_REQUIRE(sorted_indices[5] == 0);
    }
    {
        auto constexpr sorted_indices = tuplet::size_sorted::
            attribute_sorted_indices<
                []<class T>() { return T::value; },
                std::less<>,
                tuplet::tag<3>,  // 0
                tuplet::tag<4>,  // 1
                tuplet::tag<2>,  // 2
                tuplet::tag<1>>; // 3

        STATIC_REQUIRE(sorted_indices[0] == 3);
        STATIC_REQUIRE(sorted_indices[1] == 2);
        STATIC_REQUIRE(sorted_indices[2] == 0);
        STATIC_REQUIRE(sorted_indices[3] == 1);
    }
}


TEST_CASE("attribute_sorting_permutation gives the permutation that would move "
          "the types into sorted position.") {
    constexpr auto
        sorting_perm = tuplet::size_sorted::attribute_sorting_permutation<
            []<class T>() { return T::value; },
            std::less<>,
            tuplet::tag<3>,
            tuplet::tag<4>,
            tuplet::tag<2>,
            tuplet::tag<1>>;

    STATIC_REQUIRE(std::is_same_v<
                   decltype(tuplet::permute::permute_tuple<sorting_perm>(
                       tuplet::tuple<
                           tuplet::tag<3>,
                           tuplet::tag<4>,
                           tuplet::tag<2>,
                           tuplet::tag<1>> {})),
                   tuplet::tuple<
                       tuplet::tag<1>,
                       tuplet::tag<2>,
                       tuplet::tag<3>,
                       tuplet::tag<4>>>);
}

TEST_CASE("sorting the size/struct packing shrinks the tuple") {
    STATIC_REQUIRE(
        sizeof(tuplet::size_sorted::size_sorted_tuple<char, int, char>)
        < sizeof(tuplet::tuple<char, int, char>));
}

TEST_CASE("Indexing a size_sorted_tuple accesses in declaration order") {
    auto constexpr tup = tuplet::size_sorted::size_sorted_tuple {'a', 1, 2.0};
    STATIC_REQUIRE(tup[0_tag] == 'a');
    STATIC_REQUIRE(tup[1_tag] == 1);
    STATIC_REQUIRE(tup[2_tag] == 2.0);
}
