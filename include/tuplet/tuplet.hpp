#include <type_traits>
#include <cstddef>
#include <utility>

namespace std {
template<class T>
class reference_wrapper;
}

namespace tuplet {
template<class... T>
struct tuple;

template<size_t I>
using index = std::integral_constant<size_t, I>;

namespace detail {
template<size_t I, class T>
struct tuple_elem {
    static T decl_elem(index<I>);

    [[no_unique_address]] T elem;

    decltype(auto) operator[](index<I>) & {
        return (elem);
    }
    decltype(auto) operator[](index<I>) const& {
        return (elem);
    }
    decltype(auto) operator[](index<I>) && {
        return (std::move(*this).elem);
    }
};

template<size_t I, class... T>
struct partial_tuple;

template<size_t I, class T>
struct partial_tuple<I, T> : tuple_elem<I, T> {
    using tuple_elem<I, T>::decl_elem;
    using tuple_elem<I, T>::operator[];
};

template<size_t I, class T, class... Rest>
struct partial_tuple<I, T, Rest...> : tuple_elem<I, T>, partial_tuple<I + 1, Rest...> {
    using tuple_elem<I, T>::decl_elem;
    using tuple_elem<I, T>::operator[];
    using partial_tuple<I + 1, Rest...>::decl_elem;
    using partial_tuple<I + 1, Rest...>::operator[];
};

template<class T>
struct unwrap_type {
    using type = T;
};
template<class T>
struct unwrap_type<std::reference_wrapper<T>> {
    using type = T&;
};
template<class T>
using unwrap_t = typename unwrap_type<T>::type;
}

template<class... T>
struct tuple : detail::partial_tuple<0, T...> {
    using detail::partial_tuple<0, T...>::operator[];
    using detail::partial_tuple<0, T...>::decl_elem;
};
template<class... T>
tuple(T...) -> tuple<detail::unwrap_t<T>...>;

template<size_t I, class... T>
decltype(auto) get(tuple<T...>& tup) {
    return tup[index<I>()];
}
template<size_t I, class... T>
decltype(auto) get(tuple<T...> const& tup) {
    return tup[index<I>()];
}
template<size_t I, class... T>
decltype(auto) get(tuple<T...>&& tup) {
    return std::move(tup)[index<I>()];
}
}

namespace std {
template<class... T>
struct tuple_size<tuplet::tuple<T...>>
  : std::integral_constant<size_t, sizeof...(T)> {};

template<size_t I, class... T>
struct tuple_element<I, tuplet::tuple<T...>> {
    using type = decltype(tuplet::tuple<T...>::decl_elem(tuplet::index<I>()));
};
}

