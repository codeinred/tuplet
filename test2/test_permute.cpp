#include <catch2/catch_test_macros.hpp>
#include <tuplet/permute.hpp>

using namespace tuplet::literals;

template <class To>
constexpr auto narrow(auto x) {
    return static_cast<To>(x);
}
constexpr auto make_permutation(auto... nums)
    TUPLET_ARROW(tuplet::permute::permutation<sizeof...(nums)> {
        narrow<std::size_t>(nums)...});

TEST_CASE("Composing a permutation with its inverse is the identity") {
    auto constexpr sigma = make_permutation(0, 2, 1);
    auto constexpr inv = tuplet::permute::inverse(sigma);

    STATIC_REQUIRE(sigma[inv[0]] == 0);
    STATIC_REQUIRE(sigma[inv[1]] == 1);
    STATIC_REQUIRE(sigma[inv[2]] == 2);
}

TEST_CASE("permute_tuple permutes the elements of a tuple") {
    {
        auto constexpr sigma = make_permutation(0, 2, 1);
        auto constexpr tup = tuplet::tuple {'a', 1, 'c'};
        auto constexpr permuted = tuplet::permute::permute_tuple<sigma>(tup);

        STATIC_REQUIRE(permuted[0_tag] == 'a');
        STATIC_REQUIRE(permuted[1_tag] == 'c');
        STATIC_REQUIRE(permuted[2_tag] == 1);
    }

    {
        auto constexpr sigma = make_permutation(3, 2, 0, 1);
        auto constexpr tup = tuplet::tuple {'a', 1, 'c', 3.0};
        auto constexpr permuted = tuplet::permute::permute_tuple<sigma>(tup);

        STATIC_REQUIRE(permuted[0_tag] == 'c');
        STATIC_REQUIRE(permuted[1_tag] == 3.0);
        STATIC_REQUIRE(permuted[2_tag] == 1);
        STATIC_REQUIRE(permuted[3_tag] == 'a');
    }
}

TEST_CASE("Indexing a storage_permuted_tuple accesses in declaration order") {
    auto constexpr sigma = make_permutation(0, 2, 1);
    auto constexpr tup = tuplet::permute::make_storage_permuted_tuple<sigma>(
        'a',
        1,
        2.0);
    STATIC_REQUIRE(tup[0_tag]=='a');
    STATIC_REQUIRE(tup[1_tag]==1);
    STATIC_REQUIRE(tup[2_tag]==2.0);
}
