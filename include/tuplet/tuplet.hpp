#include <cstddef>
#include <type_traits>
#include <utility>

namespace std {
template <class T>
class reference_wrapper;
}

namespace tuplet {

template <size_t I>
using index = std::integral_constant<size_t, I>;

namespace detail {
template <size_t I, class T>
struct tuple_elem {
    static T decl_elem(index<I>);

    [[no_unique_address]] T elem;

    decltype(auto) operator[](index<I>) & { return (elem); }
    decltype(auto) operator[](index<I>) const& { return (elem); }
    decltype(auto) operator[](index<I>) && { return (std::move(*this).elem); }
};

template <size_t I, class... T>
struct partial_tuple;

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

template <class T>
struct unwrap_type {
    using type = T;
};
template <class T>
struct unwrap_type<std::reference_wrapper<T>> {
    using type = T&;
};
template <class T>
using unwrap_t = typename unwrap_type<T>::type;

template <size_t... I, class Dest, class... T>
void assign(Dest& dest, std::index_sequence<I...>, T&&... elems) {
    ((void)(dest[index<I>()] = std::forward<T>(elems)), ...);
}
template <class Product, class Source, size_t... I>
Product convert(Source&& source, std::index_sequence<I...>) {
    return Product{std::forward<Source>(source)[index<I>()]...};
}
template <class Func, class Tuple, size_t... I>
decltype(auto) apply(Tuple&& t, Func&& func, std::index_sequence<I...>) {
    return std::forward<Func>(func)(t[index<I>()]...);
}
} // namespace detail

template <class... T>
struct tuple : detail::partial_tuple<0, T...> {
    constexpr static auto indicies = std::make_index_sequence<sizeof...(T)>();
    using detail::partial_tuple<0, T...>::operator[];
    using detail::partial_tuple<0, T...>::decl_elem;

    template <class... U>
    void assign(U&&... values) {
        detail::assign(*this, indicies, std::forward<U>(values)...);
    }
    template <class F>
    decltype(auto) apply(F&& func) & {
        return detail::apply(*this, std::forward<F>(func), indicies);
    }
    template <class F>
    decltype(auto) apply(F&& func) const& {
        return detail::apply(*this, std::forward<F>(func), indicies);
    }
    template <class F>
    decltype(auto) apply(F&& func) && {
        return detail::apply(std::move(*this), std::forward<F>(func), indicies);
    }
    template <class... U>
    operator tuple<U...>() & {
        return detail::convert<tuple<U...>>(*this, indicies);
    }
    template <class... U>
    operator tuple<U...>() const& {
        return detail::convert<tuple<U...>>(*this, indicies);
    }
    template <class... U>
    operator tuple<U...>() && {
        return detail::convert<tuple<U...>>(std::move(*this), indicies);
    }
};
template <class... T>
tuple(T...) -> tuple<detail::unwrap_t<T>...>;

template <size_t I, class... T>
decltype(auto) get(tuple<T...>& tup) {
    return tup[index<I>()];
}
template <size_t I, class... T>
decltype(auto) get(tuple<T...> const& tup) {
    return tup[index<I>()];
}
template <size_t I, class... T>
decltype(auto) get(tuple<T...>&& tup) {
    return std::move(tup)[index<I>()];
}

template <class... T>
tuple<T&...> tie(T&... args) {
    return tuple<T&...>{args...};
}

namespace literals {
template <char... D>
constexpr size_t size_t_from_digits() {
    static_assert((('0' <= D && D <= '9') && ...), "Must be integral literal");
    size_t num = 0;
    return ((num = num * 10 + (D - '0')), ..., num);
}
template <char... D>
using index_t = index<size_t_from_digits<D...>()>;
template <char... D>
constexpr index_t<D...> operator""_idx() {
    return {};
}
template <char... D>
constexpr index_t<D...> operator""_st() {
    return {};
}
template <char... D>
constexpr index_t<D...> operator""_nd() {
    return {};
}
template <char... D>
constexpr index_t<D...> operator""_rd() {
    return {};
}
template <char... D>
constexpr index_t<D...> operator""_th() {
    return {};
}
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
} // namespace std
