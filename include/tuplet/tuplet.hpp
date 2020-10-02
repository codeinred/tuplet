#ifndef TUPLET_TUPLET_HPP_IMPLEMENTATION
#define TUPLET_TUPLET_HPP_IMPLEMENTATION

#include <cstddef>
#include <type_traits>
#include <utility>

namespace tuplet {
template <class T, class U>
concept other_than = !std::is_same_v<std::decay_t<T>, U>;

template <size_t I>
using index = std::integral_constant<size_t, I>;

template <size_t... I>
using indexes = std::index_sequence<I...>;

template <class Tuple>
constexpr auto indicies =
    std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>();

template <class T>
concept empty_type = std::is_empty_v<T>;

template <class T>
concept indexable = empty_type<T> || requires(T t) {
    t[index<0>()];
};

template <class T>
concept tuple_type = indexable<T> || requires(T t) {
    std::get<0>(t);
};

template <class U, class T>
concept assignable_to = requires(U u, T t) {
    t = u;
};

namespace detail {
template <size_t I, class T>
struct tuple_elem {
    static T decl_elem(index<I>);

    [[no_unique_address]] T elem;

    constexpr decltype(auto) operator[](index<I>) & { return (elem); }
    constexpr decltype(auto) operator[](index<I>) const& { return (elem); }
    constexpr decltype(auto) operator[](index<I>) && {
        return (std::move(*this).elem);
    }
};

template <size_t I, class... T>
struct partial_tuple {}; // Represents an empty tuple

template <size_t I, class T>
struct partial_tuple<I, T> : tuple_elem<I, T> {
    using tuple_elem<I, T>::decl_elem;
    using tuple_elem<I, T>::operator[];
};

template <size_t I, class T, class... Rest>
struct partial_tuple<I, T, Rest...> : tuple_elem<I, T>,
                                      partial_tuple<I + 1, Rest...> {
    using tuple_elem<I, T>::decl_elem;
    using tuple_elem<I, T>::operator[];
    using partial_tuple<I + 1, Rest...>::decl_elem;
    using partial_tuple<I + 1, Rest...>::operator[];
};

template <size_t... I, class Dest, class... T>
constexpr Dest& assign_impl(Dest& dest, indexes<I...>, T&&... elems) {
    return ((void)(dest[index<I>()] = std::forward<T>(elems)), ..., dest);
}
template <size_t... I, class Dest, class Tup>
constexpr Dest& assign_tuple(Dest& dest, Tup&& tup, indexes<I...>) {
    return (
        (void)(dest[index<I>()] = get<I>(std::forward<Tup>(tup))), ..., dest);
}
template <class Product, class Source, size_t... I>
constexpr Product convert_impl(Source&& source, indexes<I...>) {
    return Product{std::forward<Source>(source)[index<I>()]...};
}
template <class F, class Tuple, size_t... I>
constexpr decltype(auto) apply_impl(F&& func, Tuple&& tup, indexes<I...>) {
    return std::forward<F>(func)(std::forward<Tuple>(tup)[index<I>()]...);
}
} // namespace detail

template <class F, indexable Tuple>
constexpr decltype(auto) apply(F&& func, Tuple&& tup) {
    return detail::apply_impl(
        std::forward<F>(func), std::forward<Tuple>(tup), indicies<Tuple>);
}

template <class... T>
struct tuple : detail::partial_tuple<0, T...> {
    constexpr static auto indicies = std::make_index_sequence<sizeof...(T)>();
    using detail::partial_tuple<0, T...>::operator[];
    using detail::partial_tuple<0, T...>::decl_elem;

    template <other_than<tuple> Type> // Preserves default assignments
    constexpr auto& operator=(Type&& tup) {
        return assign_tuple(*this, std::forward<Type>(tup), indicies);
    }

    template <assignable_to<T>... U>
    constexpr auto& assign(U&&... values) {
        return detail::assign_impl(*this, indicies, std::forward<U>(values)...);
    }
    template <class Aggregate>
    constexpr operator Aggregate() & {
        return detail::convert_impl<Aggregate>(*this, indicies);
    }
    template <class Aggregate>
    constexpr operator Aggregate() const& {
        return detail::convert_impl<Aggregate>(*this, indicies);
    }
    template <class Aggregate>
    constexpr operator Aggregate() && {
        return detail::convert_impl<Aggregate>(std::move(*this), indicies);
    }
};
template <class... T>
tuple(T...) -> tuple<T...>;

template <class First, class Second>
struct pair {
    constexpr static indexes<0, 1> indicies{};
    [[no_unique_address]] First    first;
    [[no_unique_address]] Second   second;

    constexpr decltype(auto) operator[](index<0>) & { return (first); }
    constexpr decltype(auto) operator[](index<0>) const& { return (first); }
    constexpr decltype(auto) operator[](index<0>) && {
        return (std::move(*this).first);
    }
    constexpr decltype(auto) operator[](index<1>) & { return (second); }
    constexpr decltype(auto) operator[](index<1>) const& { return (second); }
    constexpr decltype(auto) operator[](index<1>) && {
        return (std::move(*this).second);
    }

    template <other_than<pair> Type> // Preserves default assignments
    constexpr auto& operator=(Type&& tup) {
        return assign_tuple(*this, std::forward<Type>(tup), indicies);
    }

    template<assignable_to<First> F2, assignable_to<Second> S2>
    constexpr auto& assign(F2&& f, S2&& s) {
        first = std::forward<F2>(f);
        second = std::forward<S2>(s);
        return *this;
    }
    template <class Aggregate>
    constexpr operator Aggregate() & {
        return detail::convert_impl<Aggregate>(*this, indicies);
    }
    template <class Aggregate>
    constexpr operator Aggregate() const& {
        return detail::convert_impl<Aggregate>(*this, indicies);
    }
    template <class Aggregate>
    constexpr operator Aggregate() && {
        return detail::convert_impl<Aggregate>(std::move(*this), indicies);
    }
};
template <class A, class B>
pair(A, B) -> pair<A, B>;

// clang-format off
template <size_t I, indexable Tup>
decltype(auto) get(Tup&& tup) { return std::forward<Tup>(tup)[index<I>()]; }

template <class... T> tuple<T&...> tie(T&... args) { return {args...}; }

namespace literals {
template <char... D>
constexpr size_t size_t_from_digits() {
    static_assert((('0' <= D && D <= '9') && ...), "Must be integral literal");
    size_t num = 0;
    return ((num = num * 10 + (D - '0')), ..., num);
}
template <char... D> using index_t = index<size_t_from_digits<D...>()>;
template <char... D> constexpr index_t<D...> operator""_idx() { return {}; }
template <char... D> constexpr index_t<D...> operator""_st() { return {}; }
template <char... D> constexpr index_t<D...> operator""_nd() { return {}; }
template <char... D> constexpr index_t<D...> operator""_rd() { return {}; }
template <char... D> constexpr index_t<D...> operator""_th() { return {}; }
// clang-format on
} // namespace literals
} // namespace tuplet

namespace std {
template <class... T>
struct tuple_size<tuplet::tuple<T...>>
  : std::integral_constant<size_t, sizeof...(T)> {};

template <size_t I, class... T>
struct tuple_element<I, tuplet::tuple<T...>> {
    using type = decltype(tuplet::tuple<T...>::decl_elem(tuplet::index<I>()));
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
