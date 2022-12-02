#ifndef TUPLET_TUPLET_HPP_IMPLEMENTATION
#define TUPLET_TUPLET_HPP_IMPLEMENTATION

#include <cstddef>
#include <type_traits>
#include <utility>





///////////////////////////////////////////
////  Tuplet Preprocessor Definitions  ////
///////////////////////////////////////////

#define _TUPLET_COMPARISON_OPERATOR_1(type, member, op)                        \
    constexpr auto operator op(type const& other)                              \
        const noexcept(noexcept(member op other.member)) {                     \
        return member op other.member;                                         \
    }


#if __cpp_concepts
#define TUPLET_OTHER_THAN(Self, Other) tuplet::other_than<Self> Other
#else
#define TUPLET_OTHER_THAN(Self, Other)                                         \
    class Other, class = ::tuplet::sfinae::other_than<Self, Other>
#endif

#if __cpp_impl_three_way_comparison && !defined(TUPLET_DEFAULTED_COMPARISON)
#define TUPLET_DEFAULTED_COMPARISON 1
#include <compare>
#else
#define TUPLET_DEFAULTED_COMPARISON 0
#endif

#if __cpp_concepts
#define TUPLET_WEAK_CONCEPT(...) __VA_ARGS__
#define TUPLET_WEAK_REQUIRES(...) requires __VA_ARGS__
#else
#define TUPLET_WEAK_CONCEPT(...) class
#define TUPLET_WEAK_REQUIRES(...)
#endif

#if (__has_cpp_attribute(no_unique_address))
#define TUPLET_HAS_NO_UNIQUE_ADDRESS 1
#define TUPLET_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif (__has_cpp_attribute(msvc::no_unique_address))                           \
    || ((defined _MSC_VER) && (!defined __clang__))
// Note __has_cpp_attribute(msvc::no_unique_address) itself doesn't work as
// of 19.30.30709, even though the attribute itself is supported. See
// https://github.com/llvm/llvm-project/issues/49358#issuecomment-981041089
#define TUPLET_HAS_NO_UNIQUE_ADDRESS 1
#define TUPLET_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
// no_unique_address is not available.
#define TUPLET_HAS_NO_UNIQUE_ADDRESS 0
#define TUPLET_NO_UNIQUE_ADDRESS
#endif





/////////////////////////////////////////////
////  SFINAE, Concepts, and Type Traits  ////
/////////////////////////////////////////////

namespace tuplet::sfinae::detail {
    template <class Tup, class = typename Tup::base_list>
    constexpr bool _has_base_list(int) {
        return true;
    }
    template <class Tup>
    constexpr bool _has_base_list(long long) {
        return false;
    }
} // namespace tuplet::sfinae::detail
namespace tuplet::sfinae {
    /// Implement assignment but preserve default assignment
    template <class A, class B>
    using other_than = std::enable_if_t<!std::is_same_v<A&, B>>;
} // namespace tuplet::sfinae


// tuplet concepts and traits
namespace tuplet {
    template <class T>
    struct unwrap_reference {
        using type = T;
    };
    template <class U>
    struct unwrap_reference<std::reference_wrapper<U>> {
        using type = U&;
    };

    template <class T>
    struct unwrap_ref_decay : unwrap_reference<std::decay_t<T>> {};

    template <class T>
    using unwrap_ref_decay_t = typename unwrap_ref_decay<T>::type;

    template <class T>
    using identity_t = T;

    template <class First, class...>
    using first_t = First;

    // Obtains T::type
    template <class T>
    using type_t = typename T::type;

    template <size_t I>
    using tag = std::integral_constant<size_t, I>;

    template <size_t I>
    constexpr tag<I> tag_v {};

    template <size_t N>
    using tag_range = std::make_index_sequence<N>;


    template <class Tup>
    using base_list_t = typename std::decay_t<Tup>::base_list;
    template <class Tup>
    using element_list_t = typename std::decay_t<Tup>::element_list;

    template <class T>
    constexpr bool stateless_v = std::is_empty_v<std::decay_t<T>>;

#if __cpp_concepts
    template <class T, class U>
    concept same_as = std::is_same_v<T, U> && std::is_same_v<U, T>;

    template <class T, class U>
    concept other_than = !
    std::is_same_v<T&, U>;

    template <class Tuple>
    concept base_list_tuple = requires() {
                                  typename std::decay_t<Tuple>::base_list;
                              };

    template <class T>
    concept stateless = std::is_empty_v<std::decay_t<T>>;

    template <class T>
    concept indexable = stateless<T> || requires(T t) { t[tag<0>()]; };

    template <class U, class T>
    concept assignable_to = requires(U u, T t) { t = u; };

    template <class T>
    concept ordered = requires(T const& t) {
                          { t <=> t };
                      };
    template <class T>
    concept equality_comparable = requires(T const& t) {
                                      { t == t } -> same_as<bool>;
                                  };
#endif

    template <class Tuple>
    constexpr auto base_list_tuple_v =
#if __cpp_concepts
        base_list_tuple<Tuple>;
#else
        tuplet::sfinae::detail::_has_base_list<Tuple>(0);
#endif
} // namespace tuplet





///////////////////////////////////////////////////////
////  tuplet::detail: Comparison Operator Helpers  ////
///////////////////////////////////////////////////////

namespace tuplet {
    /// Represents a list of types
    template <class... T>
    struct type_list {};

    /// Convinience + operator for catenating type lists
    template <class... Ls, class... Rs>
    constexpr auto operator+(type_list<Ls...>, type_list<Rs...>) {
        return type_list<Ls..., Rs...> {};
    }
} // namespace tuplet


namespace tuplet::detail {
    /// Computes a partial comparison. Returns true iff a == b. Otherwise,
    /// sets less to true if a < b
    template <class T, class U>
    constexpr bool _partial_cmp(T const& a, U const& b, bool& less) {
        if (a < b) {
            less = true;
            return false;
        } else {
            return a == b;
        }
    }

    template <class Tup, class... Bases>
    constexpr inline bool _equals(
        Tup const& t1,
        Tup const& t2,
        type_list<Bases...>) {
        return (
            (t1.identity_t<Bases>::value == t2.identity_t<Bases>::value)
            && ...);
    }

    template <class Tup, class... Bases>
    constexpr inline bool _less(
        Tup const& t1,
        Tup const& t2,
        type_list<Bases...>) {
        bool is_less = false;
        (_partial_cmp(
             t1.identity_t<Bases>::value,
             t2.identity_t<Bases>::value,
             is_less)
         && ...);
        return is_less;
    }

    template <class Tup, class... Bases>
    constexpr inline bool _less_eq(
        Tup const& t1,
        Tup const& t2,
        type_list<Bases...>) {
        bool is_less = false;
        bool is_eq =
            (_partial_cmp(
                 t1.identity_t<Bases>::value,
                 t2.identity_t<Bases>::value,
                 is_less)
             && ... && true);
        return is_less || is_eq;
    }


    template <class Tup1, class Tup2, class... B1, class... B2>
    constexpr inline bool _equals(
        Tup1 const& t1,
        Tup2 const& t2,
        type_list<B1...>,
        type_list<B2...>) {
        return ((t1.identity_t<B1>::value == t2.identity_t<B2>::value) && ...);
    }

    template <class Tup1, class Tup2, class... B1, class... B2>
    constexpr inline bool _less(
        Tup1 const& t1,
        Tup2 const& t2,
        type_list<B1...>,
        type_list<B2...>) {
        bool is_less = false;
        (_partial_cmp(
             t1.identity_t<B1>::value,
             t2.identity_t<B2>::value,
             is_less)
         && ... && true);
        return is_less;
    }

    template <class Tup1, class Tup2, class... B1, class... B2>
    constexpr inline bool _less_eq(
        Tup1 const& t1,
        Tup2 const& t2,
        type_list<B1...>,
        type_list<B2...>) {
        bool is_less = false;
        bool is_eq =
            (_partial_cmp(
                 t1.identity_t<B1>::value,
                 t2.identity_t<B2>::value,
                 is_less)
             && ... && true);
        return is_less || is_eq;
    }
} // namespace tuplet::detail





////////////////////////////
////  tuplet::type_map  ////
////////////////////////////

namespace tuplet {
    template <class... Bases>
    struct type_map : Bases... {
        using base_list = type_list<Bases...>;
        using Bases::operator[]...;
        using Bases::decl_elem...;

#if TUPLET_DEFAULTED_COMPARISON
        auto operator<=>(type_map const&) const = default;
        bool operator==(type_map const&) const = default;
#else
        constexpr auto operator==(type_map const& other) const {
            return detail::_equals(*this, other, base_list {});
        }
        constexpr auto operator!=(type_map const& other) const {
            return !(*this == other);
        }
        constexpr auto operator<(type_map const& other) const {
            return detail::_less(*this, other, base_list {});
        }
        constexpr auto operator<=(type_map const& other) const {
            return detail::_less_eq(*this, other, base_list {});
        }
        constexpr auto operator>(type_map const& other) const {
            return detail::_less(other, *this, base_list {});
        }
        constexpr auto operator>=(type_map const& other) const {
            return detail::_less_eq(other, *this, base_list {});
        }
#endif
    };
} // namespace tuplet





//////////////////////////////
////  tuplet::tuple_elem  ////
//////////////////////////////

namespace tuplet {
    template <size_t I, class T>
    struct tuple_elem {
        // Like declval, but with the element
        static T decl_elem(tag<I>);
        using type = T;

        TUPLET_NO_UNIQUE_ADDRESS T value;

        constexpr decltype(auto) operator[](tag<I>) & { return (value); }
        constexpr decltype(auto) operator[](tag<I>) const& { return (value); }
        constexpr decltype(auto) operator[](tag<I>) && {
            return (static_cast<tuple_elem&&>(*this).value);
        }
#if TUPLET_DEFAULTED_COMPARISON
        auto operator<=>(tuple_elem const&) const = default;
        bool operator==(tuple_elem const&) const = default;
        // Implements comparison for tuples containing reference types
        constexpr auto operator<=>(tuple_elem const& other) const
            noexcept(noexcept(value <=> other.value))
            requires(std::is_reference_v<T> && ordered<T>)
        {
            return value <=> other.value;
        }
        constexpr bool operator==(tuple_elem const& other) const
            noexcept(noexcept(value == other.value))
            requires(std::is_reference_v<T> && equality_comparable<T>)
        {
            return value == other.value;
        }
#else
        _TUPLET_COMPARISON_OPERATOR_1(tuple_elem, value, ==)
        _TUPLET_COMPARISON_OPERATOR_1(tuple_elem, value, !=)
        _TUPLET_COMPARISON_OPERATOR_1(tuple_elem, value, <)
        _TUPLET_COMPARISON_OPERATOR_1(tuple_elem, value, <=)
        _TUPLET_COMPARISON_OPERATOR_1(tuple_elem, value, >)
        _TUPLET_COMPARISON_OPERATOR_1(tuple_elem, value, >=)
#endif
    };
} // namespace tuplet





///////////////////////////////////////////////////////////////
////  tuplet::tuple_base_t - Base class for tuplet::tuple  ////
///////////////////////////////////////////////////////////////

namespace tuplet::detail {
    template <class IndexSequence, class... T>
    struct _get_tuple_base;

    template <size_t... I, class... T>
    struct _get_tuple_base<std::index_sequence<I...>, T...> {
        using type = type_map<tuple_elem<I, T>...>;
    };
} // namespace tuplet::detail

namespace tuplet {
    /// Obtains a tuplet::type_map whose bases correspond to
    /// tuplet::tuple_elem<I, T>. Used as the base class for tuplet::tuple.
    template <class... T>
    using tuple_base_t = typename detail::
        _get_tuple_base<tag_range<sizeof...(T)>, T...>::type;
} // namespace tuplet





////////////////////////////////////////////////
////  tuplet::tuple Primary Implementation  ////
////////////////////////////////////////////////

namespace tuplet {
    template <class... T>
    struct tuple : tuple_base_t<T...> {
        constexpr static size_t N = sizeof...(T);
        constexpr static bool
            nothrow_swappable = (std::is_nothrow_swappable_v<T> && ...);
        using super = tuple_base_t<T...>;
        using super::operator[];
        using base_list = typename super::base_list;
        using element_list = type_list<T...>;
        using super::decl_elem;

        template <TUPLET_OTHER_THAN(tuple, U)> // Preserves default assignments
        constexpr auto& operator=(U&& tup) {
            using tuple2 = std::decay_t<U>;
            if constexpr (base_list_tuple_v<tuple2>) {
                _assign_tup(
                    static_cast<U&&>(tup),
                    base_list(),
                    typename tuple2::base_list());
            } else {
                _assign_index_tup(static_cast<U&&>(tup), tag_range<N>());
            }
            return *this;
        }

        template <TUPLET_WEAK_CONCEPT(assignable_to<T>)... U>
        constexpr auto& assign(U&&... values) {
            _assign(base_list(), static_cast<U&&>(values)...);
            return *this;
        }

#if TUPLET_DEFAULTED_COMPARISON
        auto operator<=>(tuple const&) const = default;
        bool operator==(tuple const&) const = default;
        bool operator!=(tuple const&) const = default;
        bool operator<(tuple const&) const = default;
        bool operator>(tuple const&) const = default;
        bool operator<=(tuple const&) const = default;
        bool operator>=(tuple const&) const = default;
#else
        auto operator==(tuple const& other) const {
            return detail::_equals(*this, other, base_list {});
        }
        auto operator!=(tuple const& other) const { return !(*this == other); }
        auto operator<(tuple const& other) const {
            return detail::_less(*this, other, base_list {});
        }
        auto operator<=(tuple const& other) const {
            return detail::_less_eq(*this, other, base_list {});
        }
        auto operator>(tuple const& other) const {
            return detail::_less(other, *this, base_list {});
        }
        auto operator>=(tuple const& other) const {
            return detail::_less_eq(other, *this, base_list {});
        }
#endif
        template <class... U>
        auto operator==(tuple<U...> const& other) const {
            using other_base_list = typename tuple<U...>::base_list;
            return detail::_equals(
                *this,
                other,
                base_list {},
                other_base_list {});
        }
        template <class... U>
        auto operator!=(tuple<U...> const& other) const {
            return !(*this == other);
        }
        template <class... U>
        auto operator<(tuple<U...> const& other) const {
            using other_base_list = typename tuple<U...>::base_list;
            return detail::_less(
                *this,
                other,
                base_list {},
                other_base_list {});
        }
        template <class... U>
        auto operator<=(tuple<U...> const& other) const {
            using other_base_list = typename tuple<U...>::base_list;
            return detail::_less_eq(
                *this,
                other,
                base_list {},
                other_base_list {});
        }
        template <class... U>
        auto operator>(tuple<U...> const& other) const {
            using other_base_list = typename tuple<U...>::base_list;
            return detail::_less(
                other,
                *this,
                other_base_list {},
                base_list {});
        }
        template <class... U>
        auto operator>=(tuple<U...> const& other) const {
            using other_base_list = typename tuple<U...>::base_list;
            return detail::_less_eq(
                other,
                *this,
                other_base_list {},
                base_list {});
        }

        void swap(tuple& other) noexcept(nothrow_swappable) {
            _swap(other, base_list {});
        }

        // Applies a function to every element of the tuple. The order is the
        // declaration order, so first the function will be applied to element
        // 0, then element 1, then element 2, and so on, where element N is
        // identified by get<N>
        template <class F>
        constexpr void for_each(F&& func) & {
            _for_each(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr void for_each(F&& func) const& {
            _for_each(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr void for_each(F&& func) && {
            static_cast<tuple&&>(*this)._for_each(
                base_list(),
                static_cast<F&&>(func));
        }

        // Applies a function to each element successively, until one returns a
        // truthy value. Returns true if any application returned a truthy
        // value, and false otherwise
        template <class F>
        constexpr bool any(F&& func) & {
            return _any(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr bool any(F&& func) const& {
            return _any(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr bool any(F&& func) && {
            return static_cast<tuple&&>(*this)._any(
                base_list(),
                static_cast<F&&>(func));
        }

        // Applies a function to each element successively, until one returns a
        // falsy value. Returns true if every application returned a truthy
        // value, and false otherwise
        template <class F>
        constexpr bool all(F&& func) & {
            return _all(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr bool all(F&& func) const& {
            return _all(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr bool all(F&& func) && {
            return static_cast<tuple&&>(*this)._all(
                base_list(),
                static_cast<F&&>(func));
        }

        // Map a function over every element in the tuple, using the values to
        // construct a new tuple
        template <class F>
        constexpr auto map(F&& func) & {
            return _map(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr auto map(F&& func) const& {
            return _map(base_list(), static_cast<F&&>(func));
        }
        template <class F>
        constexpr auto map(F&& func) && {
            return static_cast<tuple&&>(*this)._map(
                base_list(),
                static_cast<F&&>(func));
        }

       private:
        template <class... B>
        constexpr void _swap(tuple& other, type_list<B...>) noexcept(
            nothrow_swappable) {
            using std::swap;
            (swap(B::value, other.identity_t<B>::value), ...);
        }

        template <class U, class... B1, class... B2>
        constexpr void _assign_tup(
            U&& u,
            type_list<B1...>,
            type_list<B2...>) {
            // See:
            // https://developercommunity.visualstudio.com/t/fold-expressions-unreliable-in-171-with-c20/1676476
            (void(B1::value = static_cast<U&&>(u).B2::value), ...);
        }
        template <class U, size_t... I>
        constexpr void _assign_index_tup(
            U&& u,
            std::index_sequence<I...>) {
            using std::get;
            (void(tuple_elem<I, T>::value = get<I>(static_cast<U&&>(u))), ...);
        }
        template <class... U, class... B>
        constexpr void _assign(type_list<B...>, U&&... u) {
            (void(B::value = static_cast<U&&>(u)), ...);
        }

        template <class F, class... B>
        constexpr void _for_each(type_list<B...>, F&& func) & {
            (void(func(B::value)), ...);
        }
        template <class F, class... B>
        constexpr void _for_each(type_list<B...>, F&& func) const& {
            (void(func(B::value)), ...);
        }
        template <class F, class... B>
        constexpr void _for_each(type_list<B...>, F&& func) && {
            (void(func(static_cast<T&&>(B::value))), ...);
        }

        template <class F, class... B>
        constexpr bool _any(type_list<B...>, F&& func) & {
            return (bool(func(B::value)) || ...);
        }
        template <class F, class... B>
        constexpr bool _any(type_list<B...>, F&& func) const& {
            return (bool(func(B::value)) || ...);
        }
        template <class F, class... B>
        constexpr bool _any(type_list<B...>, F&& func) && {
            return (bool(func(static_cast<T&&>(B::value))) || ...);
        }

        template <class F, class... B>
        constexpr bool _all(type_list<B...>, F&& func) & {
            return (bool(func(B::value)) && ...);
        }
        template <class F, class... B>
        constexpr bool _all(type_list<B...>, F&& func) const& {
            return (bool(func(B::value)) && ...);
        }
        template <class F, class... B>
        constexpr bool _all(type_list<B...>, F&& func) && {
            return (bool(func(static_cast<T&&>(B::value))) && ...);
        }

        template <class F, class... B>
        constexpr auto _map(type_list<B...>, F&& func) & -> tuple<
            unwrap_ref_decay_t<decltype(func(B::value))>...> {
            return {func(B::value)...};
        }
        template <class F, class... B>
        constexpr auto _map(type_list<B...>, F&& func)
            const& -> tuple<unwrap_ref_decay_t<decltype(func(B::value))>...> {
            return {func(B::value)...};
        }
        template <class F, class... B>
        constexpr auto _map(type_list<B...>, F&& func) && -> tuple<
            unwrap_ref_decay_t<decltype(func(static_cast<T&&>(B::value)))>...> {
            return {func(static_cast<T&&>(B::value))...};
        }
    };
} // namespace tuplet





////////////////////////////////////////////////////
////  tuplet::tuple Empty Tuple Specialization  ////
////////////////////////////////////////////////////

namespace tuplet {
    template <>
    struct tuple<> : tuple_base_t<> {
        constexpr static size_t N = 0;
        constexpr static bool nothrow_swappable = true;
        using super = tuple_base_t<>;
        using base_list = type_list<>;
        using element_list = type_list<>;

        template <TUPLET_OTHER_THAN(tuple, U)> // Preserves default assignments
        TUPLET_WEAK_REQUIRES(
            stateless<U>) // Check that U is similarly stateless
            constexpr auto&
            operator=(U&&) noexcept {
            return *this;
        }

        void swap(tuple) noexcept {}
        constexpr auto& assign() noexcept { return *this; }
#if __cpp_impl_three_way_comparison
        auto operator<=>(tuple const&) const = default;
        bool operator==(tuple const&) const = default;
#else
        constexpr bool operator==(tuple const&) const noexcept { return true; }
        constexpr bool operator<=(tuple const&) const noexcept { return true; }
        constexpr bool operator>=(tuple const&) const noexcept { return true; }
        constexpr bool operator!=(tuple const&) const noexcept { return false; }
        constexpr bool operator<(tuple const&) const noexcept { return false; }
        constexpr bool operator>(tuple const&) const noexcept { return false; }
#endif
        // Applies a function to every element of the tuple. The order is the
        // declaration order, so first the function will be applied to element
        // 0, then element 1, then element 2, and so on, where element N is
        // identified by get<N>
        //
        // (Does nothing when invoked on empty tuple)
        template <class F>
        constexpr void for_each(F&&) const noexcept {}

        // Applies a function to each element successively, until one returns a
        // truthy value. Returns true if any application returned a truthy
        // value, and false otherwise
        //
        // (Returns false for empty tuple)
        template <class F>
        constexpr bool any(F&&) const noexcept {
            return false;
        }

        // Applies a function to each element successively, until one returns a
        // falsy value. Returns true if every application returned a truthy
        // value, and false otherwise
        //
        // (Returns true for empty tuple)
        template <class F>
        constexpr bool all(F&&) const noexcept {
            return true;
        }

        // Map a function over every element in the tuple, using the values to
        // construct a new tuple
        //
        // (Returns empty tuple when invoked on empty tuple)
        template <class F>
        constexpr auto map(F&&) const noexcept {
            return tuple {};
        }
    };
    template <class... Ts>
    tuple(Ts...) -> tuple<unwrap_ref_decay_t<Ts>...>;
} // namespace tuplet





///////////////////////////////////////
////  tuplet::pair Implementation  ////
///////////////////////////////////////

namespace tuplet {
    template <class First, class Second>
    struct pair {
        constexpr static size_t N = 2;
        constexpr static bool
            nothrow_swappable = std::is_nothrow_swappable_v<First>
                             && std::is_nothrow_swappable_v<Second>;
        TUPLET_NO_UNIQUE_ADDRESS First first;
        TUPLET_NO_UNIQUE_ADDRESS Second second;

        constexpr decltype(auto) operator[](tag<0>) & { return (first); }
        constexpr decltype(auto) operator[](tag<0>) const& { return (first); }
        constexpr decltype(auto) operator[](tag<0>) && {
            return (static_cast<pair&&>(*this).first);
        }
        constexpr decltype(auto) operator[](tag<1>) & { return (second); }
        constexpr decltype(auto) operator[](tag<1>) const& { return (second); }
        constexpr decltype(auto) operator[](tag<1>) && {
            return (static_cast<pair&&>(*this).second);
        }

        void swap(pair& other) noexcept(nothrow_swappable) {
            using std::swap;
            swap(first, other.first);
            swap(second, other.second);
        }

        template <
            TUPLET_OTHER_THAN(pair, Type)> // Preserves default assignments
        constexpr auto& operator=(Type&& tup) {
            auto&& [a, b] = static_cast<Type&&>(tup);
            first = static_cast<decltype(a)&&>(a);
            second = static_cast<decltype(b)&&>(b);
            return *this;
        }

        template <
            TUPLET_WEAK_CONCEPT(assignable_to<First>) F2,
            TUPLET_WEAK_CONCEPT(assignable_to<Second>) S2>
        constexpr auto& assign(F2&& f, S2&& s) {
            first = static_cast<F2&&>(f);
            second = static_cast<S2&&>(s);
            return *this;
        }

#if __cpp_impl_three_way_comparison
        auto operator<=>(pair const&) const = default;
        bool operator==(pair const&) const = default;
#else
        constexpr bool operator==(pair const& other) const {
            return first == other.first && second == other.second;
        }
        constexpr bool operator!=(pair const& other) const {
            return !(*this == other);
        }
        constexpr bool operator<(pair const& other) const {
            bool result = false;
            detail::_partial_cmp(first, other.first, result)
                && detail::_partial_cmp(second, other.second, result);
            return result;
        }
        constexpr bool operator<=(pair const& other) const {
            bool result = false;
            bool is_eq = detail::_partial_cmp(first, other.first, result)
                      && detail::_partial_cmp(second, other.second, result);
            return result || is_eq;
        }
        constexpr bool operator>(pair const& other) const {
            bool result = false;
            detail::_partial_cmp(other.first, first, result)
                && detail::_partial_cmp(other.second, second, result);
            return result;
        }
        constexpr bool operator>=(pair const& other) const {
            bool result = false;
            bool is_eq = detail::_partial_cmp(other.first, first, result)
                      && detail::_partial_cmp(other.second, second, result);
            return result || is_eq;
        }
#endif
    };
    template <class A, class B>
    pair(A, B) -> pair<unwrap_ref_decay_t<A>, unwrap_ref_decay_t<B>>;
} // namespace tuplet





//////////////////////////////////////////////////////
////  tuplet::convert: tuple conversion facility  ////
//////////////////////////////////////////////////////

namespace tuplet {
    // Converts from one tuple type to any other tuple or U
    template <class Tuple>
    struct convert {
        using base_list = typename std::decay_t<Tuple>::base_list;
        Tuple tuple;
        template <class U>
        constexpr operator U() && {
            return _convert<U>(base_list {});
        }

       private:
        template <class U, class... Bases>
        constexpr U _convert(type_list<Bases...>) {
            return U {static_cast<Tuple&&>(tuple).identity_t<Bases>::value...};
        }
    };
    template <class Tuple>
    convert(Tuple&) -> convert<Tuple&>;
    template <class Tuple>
    convert(Tuple const&) -> convert<Tuple const&>;
    template <class Tuple>
    convert(Tuple&&) -> convert<Tuple>;
} // namespace tuplet





/////////////////////////////////////////////////////////
////  tuplet Appendix 1: Small non-member functions  ////
/////////////////////////////////////////////////////////

namespace tuplet::detail {
    template <class F, class T, class... Bases>
    constexpr decltype(auto) _apply(F&& f, T&& t, type_list<Bases...>) {
        return static_cast<F&&>(f)(
            static_cast<T&&>(t).identity_t<Bases>::value...);
    }
} // namespace tuplet::detail

// tuplet::get implementation
// tuplet::tie implementation
// tuplet::apply implementation
// tuplet::swap
// tuplet::make_tuple
// tuplet::forward_as_tuple
namespace tuplet {
    template <size_t I, TUPLET_WEAK_CONCEPT(indexable) Tup>
    constexpr decltype(auto) get(Tup&& tup) {
        return static_cast<Tup&&>(tup)[tag<I>()];
    }

    template <class... T>
    constexpr tuple<T&...> tie(T&... t) {
        return {t...};
    }

    template <class F, TUPLET_WEAK_CONCEPT(base_list_tuple) Tup>
    constexpr decltype(auto) apply(F&& func, Tup&& tup) {
        return detail::_apply(
            static_cast<F&&>(func),
            static_cast<Tup&&>(tup),
            typename std::decay_t<Tup>::base_list());
    }
    template <class F, class A, class B>
    constexpr decltype(auto) apply(F&& func, tuplet::pair<A, B>& pair) {
        return static_cast<F&&>(func)(pair.first, pair.second);
    }
    template <class F, class A, class B>
    constexpr decltype(auto) apply(F&& func, tuplet::pair<A, B> const& pair) {
        return static_cast<F&&>(func)(pair.first, pair.second);
    }
    template <class F, class A, class B>
    constexpr decltype(auto) apply(F&& func, tuplet::pair<A, B>&& pair) {
        using P = tuplet::pair<A, B>&&;
        return static_cast<F&&>(
            func)(static_cast<P>(pair).first, static_cast<P>(pair).second);
    }

    template <class... T>
    void swap(tuple<T...>& a, tuple<T...>& b) noexcept(
        tuple<T...>::nothrow_swappable) {
        a.swap(b);
    }

    template <class A, class B>
    void swap(pair<A, B>& a, pair<A, B>& b) noexcept(
        pair<A, B>::nothrow_swappable) {
        a.swap(b);
    }

    template <typename... Ts>
    constexpr auto make_tuple(Ts&&... args) {
        return tuple<unwrap_ref_decay_t<Ts>...> {static_cast<Ts&&>(args)...};
    }

    template <typename... T>
    constexpr auto forward_as_tuple(T&&... a) noexcept {
        return tuple<T&&...> {static_cast<T&&>(a)...};
    }
} // namespace tuplet





///////////////////////////////////////////////////////////
////  tuplet Appendix 2: The Horror that is tuple_cat  ////
///////////////////////////////////////////////////////////

namespace tuplet::detail {
    template <class T, class... Q>
    constexpr auto _repeat_type(type_list<Q...>) {
        return type_list<first_t<T, Q>...> {};
    }
    template <class... Outer>
    constexpr auto _get_outer_bases(type_list<Outer...>) {
        return (_repeat_type<Outer>(base_list_t<type_t<Outer>> {}) + ...);
    }
    template <class... Outer>
    constexpr auto _get_inner_bases(type_list<Outer...>) {
        return (base_list_t<type_t<Outer>> {} + ...);
    }

    // This takes a forwarding tuple as a parameter. The forwarding tuple only
    // contains references, so it should just be taken by value.
    template <class T, class... Outer, class... Inner>
    constexpr auto _tuple_cat(T tup, type_list<Outer...>, type_list<Inner...>)
        -> tuple<type_t<Inner>...> {
        return {static_cast<type_t<Outer>&&>(tup.identity_t<Outer>::value)
                    .identity_t<Inner>::value...};
    }
} // namespace tuplet::detail

namespace tuplet {
    template <TUPLET_WEAK_CONCEPT(base_list_tuple)... T>
    constexpr auto tuple_cat(T&&... ts) {
        if constexpr (sizeof...(T) == 0) {
            return tuple<>();
        } else {

/**
 * It appears that Clang produces better assembly when
 * TUPLET_CAT_BY_FORWARDING_TUPLE == 0, while GCC produces better assembly when
 * TUPLET_CAT_BY_FORWARDING_TUPLE == 1. MSVC always produces terrible assembly
 * in either case. This will set TUPLET_CAT_BY_FORWARDING_TUPLE to the correct
 * value (0 for clang, 1 for everyone else)
 *
 * See: https://github.com/codeinred/tuplet/discussions/14
 */
#if !defined(TUPLET_CAT_BY_FORWARDING_TUPLE)
#if defined(__clang__)
#define TUPLET_CAT_BY_FORWARDING_TUPLE 0
#else
#define TUPLET_CAT_BY_FORWARDING_TUPLE 1
#endif
#endif
#if TUPLET_CAT_BY_FORWARDING_TUPLE
            using big_tuple = tuple<T&&...>;
#else
            using big_tuple = tuple<std::decay_t<T>...>;
#endif
            using outer_bases = base_list_t<big_tuple>;
            constexpr auto outer = detail::_get_outer_bases(outer_bases {});
            constexpr auto inner = detail::_get_inner_bases(outer_bases {});
            return detail::_tuple_cat(
                big_tuple {static_cast<T&&>(ts)...},
                outer,
                inner);
        }
    }


} // namespace tuplet





///////////////////////////////////////////
////  tuplet Appendix 3: tag literals  ////
///////////////////////////////////////////

namespace tuplet::literals::detail {
    template <char... D>
    constexpr size_t _size_t_from_digits() {
        static_assert(
            (('0' <= D && D <= '9') && ...),
            "Must be integral literal");
        size_t num = 0;
        return ((num = num * 10 + (D - '0')), ..., num);
    }
} // namespace tuplet::literals::detail

namespace tuplet::literals {
    template <char... D>
    constexpr auto operator""_tag() noexcept
        -> tag<detail::_size_t_from_digits<D...>()> {
        return {};
    }
} // namespace tuplet::literals





//////////////////////////////////////////////////////////////////
////  tuplet Appendix 4: std::tuple_size, std::tuple_element  ////
//////////////////////////////////////////////////////////////////

namespace std {
    template <class... T>
    struct tuple_size<tuplet::tuple<T...>>
      : std::integral_constant<size_t, sizeof...(T)> {};

    template <size_t I, class... T>
    struct tuple_element<I, tuplet::tuple<T...>> {
        using type = decltype(tuplet::tuple<T...>::decl_elem(tuplet::tag<I>()));
    };
    template <class A, class B>
    struct tuple_size<tuplet::pair<A, B>>
      : std::integral_constant<size_t, 2> {};

    template <size_t I, class A, class B>
    struct tuple_element<I, tuplet::pair<A, B>> {
        static_assert(I < 2, "tuplet::pair only has 2 elements");
        using type = std::conditional_t<I == 0, A, B>;
    };
} // namespace std





#endif
