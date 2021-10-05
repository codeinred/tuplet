#ifndef TUPLET_PERMUTE_HPP_IMPLEMENTATION
#define TUPLET_PERMUTE_HPP_IMPLEMENTATION

#include "tuple.hpp"

#include <array>
#include <utility>

#define TUPLET_FWD(...) std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)
#define TUPLET_RET(...)                                                        \
    { return __VA_ARGS__; }
#define TUPLET_NOEX(...) noexcept(noexcept(__VA_ARGS__)) TUPLET_RET(__VA_ARGS__)
#define TUPLET_ARROW(...)                                                      \
    noexcept(noexcept(__VA_ARGS__))                                            \
        ->decltype(__VA_ARGS__)                                                \
        TUPLET_RET(__VA_ARGS__)
#define TUPLET_IMPLICIT(...) explicit(!(__VA_ARGS__))

namespace tuplet {
/**
 * In order to (1) sort types using std::sort and (2) create a structure packed
 * tuple that transparently sorts its elements, we need a mechanism to permute
 * and unpermute the elements of a tuple.
 */
namespace permute {

/**
 * A dense representation of Sn (permutation group of {0,1,... n-1})
 * σ: i ↦ σ[i]
 */
template <std::size_t N>
using permutation = std::array<std::size_t, N>;

/**
 * The inverse of a permutation
 */
template <std::size_t N>
constexpr permutation<N> inverse(permutation<N> const sigma) noexcept {
    //  σ⁻¹σ   = id
    //  σ⁻¹σ i = i
    permutation<N> inv;
    for (std::size_t i = 0; i < N; ++i)
        inv[sigma[i]] = i;
    return inv;
}


/*****************
 * σx[i]=x[σ⁻¹i] *
 *****************/
// σ ∈ Sn (permutation group for n elements)
// (σx)[σi] = x[i]
// (σx)[i] = x[σ⁻¹i] (what gets moved to position i)
// Best shown with an example:
// x = (a,b,c)
// σ = (0 → 1, 1 → 2, 2 → 0)  then σ⁻¹ = (0 → 2, 1 → 0, 2 → 1)
// σx = (c,a,b) = (x[2], x[0], x[1]) = (x[σ⁻¹ 0], x[σ⁻¹ 1], x[σ⁻¹ 2])
namespace detail {
template <auto sigma, auto... I>
constexpr auto unpermute_apply_(std::index_sequence<I...>, auto&& f, auto&& tup)
    TUPLET_ARROW(TUPLET_FWD(f)(get<sigma[I]>(TUPLET_FWD(tup))...))
}

/**
 * Passes elements of tup to f in the order unpermuted by σ
 * (permutes the elements by σ⁻¹)
 */
template <auto σ>
constexpr auto unpermute_apply(auto&& f, auto&& tup)
    TUPLET_ARROW(::tuplet::permute::detail::unpermute_apply_<σ>(
        std::make_index_sequence<
            std::tuple_size<std::decay_t<decltype(tup)>>::value> {},
        TUPLET_FWD(f),
        TUPLET_FWD(tup)))

    /**
     * Passes elements of tup to f in the order permuted by σ
     */
    template <auto σ>
    constexpr auto permute_apply(auto&& f, auto&& tup) TUPLET_ARROW(
        unpermute_apply<inverse(σ)>(TUPLET_FWD(f), TUPLET_FWD(tup)));

namespace detail {
inline constexpr auto mk_tup = [](auto&&... x)
    TUPLET_ARROW(tuplet::make_tuple(TUPLET_FWD(x)...));

} // namespace detail

/**
 * Permutes tup's elements by σ
 */
template <auto σ>
constexpr auto permute_tuple(auto&& tup) TUPLET_ARROW(
    permute_apply<σ>(tuplet::permute::detail::mk_tup, TUPLET_FWD(tup)))


    namespace detail {
    template <auto σ, class... Ts>
    using permute_tuple_t = decltype(permute_tuple<σ>(
        std::declval<tuple<Ts...>>()));

    template <class X, class Y, auto... I>
    constexpr auto equal_(
        std::index_sequence<I...>,
        X const& x,
        Y const& y) TUPLET_ARROW(((get<I>(x) == get<I>(y)) and ...))

        template <class X, class Y, auto... I>
        using spaceship_t = std::common_comparison_category_t<
            decltype(get_n<I>(std::declval<X>()) <=> get_n<I>(std::declval<Y>()))...>;

    template <class X, class Y, auto... I>
    constexpr spaceship_t<X, Y, I...> spaceship_(
        std::index_sequence<I...>,
        X const& x,
        Y const& y) noexcept((noexcept(get_n<I>(x) <=> get_n<I>(y)) and ...)) {
        using Ord = spaceship_t<X, Y, I...>;
        Ord ord = Ord::equivalent;
        (void)((ord = (get_n<I>(x) <=> get_n<I>(y)),
            std::is_eq(ord)) // while (std::is_eq(ord))
           and ...);
        return ord;
    }

    struct tuple_friends {
        template <class X, class Y>
        requires(std::tuple_size<X>::value == std::tuple_size<Y>::value) //
            friend constexpr bool
            operator==(X&& x, Y&& y) TUPLET_NOEX(equal_(
                std::make_index_sequence<std::tuple_size<X>::value> {},
                TUPLET_FWD(x),
                TUPLET_FWD(y)))

                template <class X, class Y>
        requires(std::tuple_size<X>::value == std::tuple_size<Y>::value) //
            friend constexpr auto
            operator<=>(X&& x, Y&& y) TUPLET_ARROW(spaceship_(
                std::make_index_sequence<std::tuple_size<X>::value> {},
                TUPLET_FWD(x),
                TUPLET_FWD(y)))
    };
} // namespace detail


/**
 * A tuple containing elements Ts... where the storage is *transparently
 * permuted* by σ: elements are stored in permuted order (by σ) but are
 * accessed in declaration order (the order they show up in Ts...)
 *
 * This is for storage optimization. For example, structure packing by
 * sorting members by descending size.
 */
template <auto σ_, class... Ts>
class storage_permuted_tuple : detail::tuple_friends {
   private:
    static constexpr auto σ = σ_;
    using underlying_t = tuplet::permute::detail::permute_tuple_t<σ, Ts...>;
    underlying_t underlying_;

   public:
    // TODO: right semantics for convert vs singleton?
    // convert seems more likely to be what a user meant
    // TODO: explicit?
    constexpr storage_permuted_tuple(tuple<Ts...> const& args) noexcept(
        noexcept(underlying_t {permute_tuple<σ>(args)}))
      : underlying_ {permute_tuple<σ>(args)} {}

    template <class... Args>
    constexpr TUPLET_IMPLICIT((std::is_convertible_v<Args&&, Ts> and ...))
        storage_permuted_tuple(Args&&... args) noexcept(
            noexcept(storage_permuted_tuple {tuple {TUPLET_FWD(args)...}}))
      : storage_permuted_tuple {tuple {TUPLET_FWD(args)...}} {}

#define TUPLET_DEF_OPERATOR_SUBSCRIPT(constref, move)                          \
    template <std::size_t i>                                                   \
    constexpr auto operator[](tuplet::tag<i>) constref TUPLET_ARROW(           \
        move(underlying_[tag_v<σ[i]>]))

    TUPLET_DEF_OPERATOR_SUBSCRIPT(&, )
    TUPLET_DEF_OPERATOR_SUBSCRIPT(const&, )
    TUPLET_DEF_OPERATOR_SUBSCRIPT(&&, std::move)
    TUPLET_DEF_OPERATOR_SUBSCRIPT(const&&, std::move)
#undef TUPLET_DEF_OPERATOR_SUBSCRIPT
};
template <auto σ, class... T>
constexpr auto make_storage_permuted_tuple(T&&... x)
    TUPLET_ARROW(storage_permuted_tuple<σ, T...>(TUPLET_FWD(x)...))


} // namespace permute
} // namespace tuplet

namespace std {
template <auto σ, class... T>
struct tuple_size<tuplet::permute::storage_permuted_tuple<σ, T...>>
  : std::integral_constant<size_t, sizeof...(T)> {};

template <size_t I, auto σ, class... T>
struct tuple_element<I, tuplet::permute::storage_permuted_tuple<σ, T...>>
  : tuple_element<I, tuplet::tuple<T...>> {};
} // namespace std

#endif // TUPLET_PERMUTE_HPP_IMPLEMENTATION
