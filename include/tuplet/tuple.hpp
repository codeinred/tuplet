#ifndef TUPLET_TUPLET_HPP_IMPLEMENTATION
#define TUPLET_TUPLET_HPP_IMPLEMENTATION

#include <compare>
#include <cstddef>
#include <type_traits>
#include <utility>

// tuplet convenience types
namespace tuplet {
template <class T>
using identity_t = T;

template <size_t I>
using tag = std::integral_constant<size_t, I>;

template <size_t I>
constexpr tag<I> tag_v {};

template <size_t N>
using tag_range = std::make_index_sequence<N>;

// tuple needs to be forward-declared for use in tuplet_plus_impl
template <class... T>
struct tuple;
} // namespace tuplet

// tuplet concepts
namespace tuplet {
// clang-format off
template <class T, class U>
concept same_as = std::is_same_v<T, U>&& std::is_same_v<U, T>;

template <class T, class U>
concept other_than = !std::is_same_v<std::decay_t<T>, U>;

template <class Tup>
using base_list_t = typename std::decay_t<Tup>::base_list;
template <class Tup>
using element_list_t = typename std::decay_t<Tup>::element_list;

template <class Tuple>
concept base_list_tuple = requires() {
    typename std::decay_t<Tuple>::base_list;
};

template <class T>
concept stateless = std::is_empty_v<std::decay_t<T>>;

template <class T>
concept indexable = stateless<T> || requires(T t) {
    t[tag<0>()];
};

template <class T>
concept tuple_type = indexable<T> || requires(T t) {
    get<0>(t);
};

template <class U, class T>
concept assignable_to = requires(U u, T t) {
    t = u;
};

template <class T>
concept ordered = requires(T const& t) {
    {t <=> t};
};
template <class T>
concept equality_comparable = requires(T const& t) {
    { t == t } -> same_as<bool>;
};
// clang-format on
} // namespace tuplet

// tuplet::type_list implementation
// tuplet::type_map implementation
// tuplet::tuple_elem implementation
// tuplet::deduce_elems
namespace tuplet {
template <class... T>
struct type_list {};

template <class... Bases>
struct type_map : Bases... {
    using base_list = type_list<Bases...>;
    using Bases::operator[]...;
    using Bases::decl_elem...;
    auto operator<=>(type_map const&) const = default;
    bool operator==(type_map const&) const = default;
};

template <size_t I, class T>
struct tuple_elem {
    // Like declval, but with the element
    static T decl_elem(tag<I>);
    using type = T;

    [[no_unique_address]] T value;

    constexpr decltype(auto) operator[](tag<I>) & { return (value); }
    constexpr decltype(auto) operator[](tag<I>) const& { return (value); }
    constexpr decltype(auto) operator[](tag<I>) && {
        return (std::move(*this).value);
    }
    auto operator<=>(tuple_elem const&) const = default;
    bool operator==(tuple_elem const&) const = default;
    // Implements comparison for tuples containing reference types
    constexpr auto operator<=>(tuple_elem const& other) const
        noexcept(noexcept(value <=> other.value))
        requires(std::is_reference_v<T> && ordered<T>) {
        return value <=> other.value;
    }
    constexpr bool operator==(tuple_elem const& other) const
        noexcept(noexcept(value == other.value))
        requires(std::is_reference_v<T> && equality_comparable<T>) {
        return value == other.value;
    }
};
template <class T>
using unwrap_ref_decay_t = typename std::unwrap_ref_decay<T>::type;
} // namespace tuplet

// tuplet::detail::get_tuple_base implementation
// tuplet::detail::apply_impl
// tuplet::detail::cat2_impl
// tuplet::detail::size_t_from_digits
namespace tuplet::detail {
template <class A, class... T>
struct get_tuple_base;

template <size_t... I, class... T>
struct get_tuple_base<std::index_sequence<I...>, T...> {
    using type = type_map<tuple_elem<I, T>...>;
};

template <class F, class Tup, class... Bases>
constexpr decltype(auto) apply_impl(F&& f, Tup&& t, type_list<Bases...>) {
    return std::forward<F>(f)(std::forward<Tup>(t).identity_t<Bases>::value...);
}
template <class T, class U, class... E1, class... E2, class... B1, class... B2>
constexpr auto cat2_impl(
    T&& t1,
    U&& t2,
    type_list<E1...>,
    type_list<E2...>,
    type_list<B1...>,
    type_list<B2...>) -> tuple<E1..., E2...> {
    return {
        std::forward<T>(t1).identity_t<B1>::value...,
        std::forward<U>(t2).identity_t<B2>::value...};
}
template <char... D>
constexpr size_t size_t_from_digits() {
    static_assert((('0' <= D && D <= '9') && ...), "Must be integral literal");
    size_t num = 0;
    return ((num = num * 10 + (D - '0')), ..., num);
}
} // namespace tuplet::detail

// tuplet::tuple implementation
namespace tuplet {
template <class... T>
using tuple_base_t = typename detail::
    get_tuple_base<tag_range<sizeof...(T)>, T...>::type;

template <class... T>
struct tuple : tuple_base_t<T...> {
    constexpr static size_t N = sizeof...(T);
    using super = tuple_base_t<T...>;
    using super::operator[];
    using base_list = typename super::base_list;
    using element_list = type_list<T...>;
    using super::decl_elem;

    template <other_than<tuple> U> // Preserves default assignments
    constexpr auto& operator=(U&& tup) {
        using tuple2 = std::decay_t<U>;
        if (base_list_tuple<tuple2>) {
            eq_impl(
                std::forward<U>(tup),
                base_list(),
                typename tuple2::base_list());
        } else {
            eq_impl(std::forward<U>(tup), tag_range<N>());
        }
        return *this;
    }

    template <assignable_to<T>... U>
    constexpr auto& assign(U&&... values) {
        assign_impl(base_list(), std::forward<U>(values)...);
        return *this;
    }

    auto operator<=>(tuple const&) const = default;
    bool operator==(tuple const&) const = default;

   private:
    template <class U, class... B1, class... B2>
    constexpr void eq_impl(U&& u, type_list<B1...>, type_list<B2...>) {
        (void(B1::value = std::forward<U>(u).identity_t<B2>::value), ...);
    }
    template <class U, size_t... I>
    constexpr void eq_impl(U&& u, std::index_sequence<I...>) {
        (void(tuple_elem<I, T>::value = get<I>(std::forward<U>(u))), ...);
    }
    template <class... U, class... B>
    constexpr void assign_impl(type_list<B...>, U&&... u) {
        (void(B::value = std::forward<U>(u)), ...);
    }
};
template <>
struct tuple<> : tuple_base_t<> {
    constexpr static size_t N = 0;
    using super = tuple_base_t<>;
    using base_list = type_list<>;
    using element_list = type_list<>;

    template <other_than<tuple> U> // Preserves default assignments
    requires stateless<U>          // Check that U is similarly stateless
    constexpr auto& operator=(U&& tup) noexcept { return *this; }

    constexpr auto& assign() noexcept { return *this; }
    auto operator<=>(tuple const&) const = default;
    bool operator==(tuple const&) const = default;
};
template <class... Ts>
tuple(Ts...) -> tuple<unwrap_ref_decay_t<Ts>...>;
} // namespace tuplet

// tuplet::pair implementation
namespace tuplet {
template <class First, class Second>
struct pair {
    constexpr static size_t N = 2;
    [[no_unique_address]] First first;
    [[no_unique_address]] Second second;

    constexpr decltype(auto) operator[](tag<0>) & { return (first); }
    constexpr decltype(auto) operator[](tag<0>) const& { return (first); }
    constexpr decltype(auto) operator[](tag<0>) && {
        return (std::move(*this).first);
    }
    constexpr decltype(auto) operator[](tag<1>) & { return (second); }
    constexpr decltype(auto) operator[](tag<1>) const& { return (second); }
    constexpr decltype(auto) operator[](tag<1>) && {
        return (std::move(*this).second);
    }

    template <other_than<pair> Type> // Preserves default assignments
    constexpr auto& operator=(Type&& tup) {
        auto&& [a, b] = std::forward<Type>(tup);
        first = std::forward<decltype(a)>(a);
        second = std::forward<decltype(b)>(b);
        return *this;
    }

    template <assignable_to<First> F2, assignable_to<Second> S2>
    constexpr auto& assign(F2&& f, S2&& s) {
        first = std::forward<F2>(f);
        second = std::forward<S2>(s);
        return *this;
    }
    auto operator<=>(pair const&) const = default;
    bool operator==(pair const&) const = default;
};
template <class A, class B>
pair(A, B) -> pair<unwrap_ref_decay_t<A>, unwrap_ref_decay_t<B>>;
} // namespace tuplet

// tuplet::convert implementation
namespace tuplet {
// Converts from one tuple type to any other tuple or U
template <class Tuple>
struct convert {
    using base_list = typename std::decay_t<Tuple>::base_list;
    Tuple tuple;
    template <class U>
    constexpr operator U() && {
        return convert_impl<U>(base_list {});
    }

   private:
    template <class U, class... Bases>
    constexpr U convert_impl(type_list<Bases...>) {
        return U {std::forward<Tuple>(tuple).identity_t<Bases>::value...};
    }
};
template <class Tuple>
convert(Tuple&) -> convert<Tuple&>;
template <class Tuple>
convert(Tuple const&) -> convert<Tuple const&>;
template <class Tuple>
convert(Tuple&&) -> convert<Tuple>;
} // namespace tuplet

// tuplet::get implementation
// tuplet::tie implementation
// tuplet::apply implementation
namespace tuplet {
template <size_t I, indexable Tup>
constexpr decltype(auto) get(Tup&& tup) {
    return std::forward<Tup>(tup)[tag<I>()];
}

template <class... T>
constexpr tuple<T&...> tie(T&... t) {
    return {t...};
}

template <class F, base_list_tuple Tup>
constexpr decltype(auto) apply(F&& func, Tup&& tup) {
    return detail::apply_impl(
        std::forward<F>(func),
        std::forward<Tup>(tup),
        typename std::decay_t<Tup>::base_list());
}
template <class F, class A, class B>
constexpr decltype(auto) apply(F&& func, tuplet::pair<A, B>& pair) {
    return std::forward<F>(func)(pair.first, pair.second);
}
template <class F, class A, class B>
constexpr decltype(auto) apply(F&& func, tuplet::pair<A, B> const& pair) {
    return std::forward<F>(func)(pair.first, pair.second);
}
template <class F, class A, class B>
constexpr decltype(auto) apply(F&& func, tuplet::pair<A, B>&& pair) {
    return std::forward<F>(func)(std::move(pair).first, std::move(pair).second);
}
} // namespace tuplet

namespace tuplet {
constexpr tuple<> tuple_cat() { return {}; }
template <base_list_tuple T>
constexpr auto tuple_cat(T&& t) {
    return std::forward<T>(t);
}
template <base_list_tuple T1, base_list_tuple T2>
constexpr auto tuple_cat(T1&& t1, T2&& t2) {
    return detail::cat2_impl(
        std::forward<T1>(t1),
        std::forward<T2>(t2),
        element_list_t<T1>(),
        element_list_t<T2>(),
        base_list_t<T1>(),
        base_list_t<T2>());
}
template <base_list_tuple T1, base_list_tuple... T2>
constexpr auto tuple_cat(T1&& t1, T2&&... t2) {
    return tuplet::tuple_cat(
        std::forward<T1>(t1),
        tuplet::tuple_cat(std::forward<T2>(t2)...));
}
} // namespace tuplet

// tuplet::make_tuple implementation
// tuplet::forward_as_tuple implementation
namespace tuplet {
template <typename... Ts>
constexpr auto make_tuple(Ts&&... args) {
    return tuplet::tuple<unwrap_ref_decay_t<Ts>...> {std::forward<Ts>(args)...};
}

template <typename... T>
constexpr auto forward_as_tuple(T&&... a) noexcept {
    return tuple<T&&...> {std::forward<T>(a)...};
}
} // namespace tuplet

// tuplet literals
namespace tuplet::literals {
template <char... D>
constexpr auto operator""_tag() noexcept
    -> tag<detail::size_t_from_digits<D...>()> {
    return {};
}
} // namespace tuplet::literals

// std::tuple_size specialization
// std::tuple_element specialization
namespace std {
template <class... T>
struct tuple_size<tuplet::tuple<T...>>
  : std::integral_constant<size_t, sizeof...(T)> {};

template <size_t I, class... T>
struct tuple_element<I, tuplet::tuple<T...>> {
    using type = decltype(tuplet::tuple<T...>::decl_elem(tuplet::tag<I>()));
};
template <class A, class B>
struct tuple_size<tuplet::pair<A, B>> : std::integral_constant<size_t, 2> {};

template <size_t I, class A, class B>
struct tuple_element<I, tuplet::pair<A, B>> {
    static_assert(I < 2, "tuplet::pair only has 2 elements");
    using type = std::conditional_t<I == 0, A, B>;
};
} // namespace std
#endif
