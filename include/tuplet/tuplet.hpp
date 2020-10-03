#ifndef TUPLET_TUPLET_HPP_IMPLEMENTATION
#define TUPLET_TUPLET_HPP_IMPLEMENTATION

#include <cstddef>
#include <type_traits>
#include <utility>

// tuplet type traits & concepts
namespace tuplet {
template <class T>
using identity_t = T;

template <size_t I>
using tag = std::integral_constant<size_t, I>;

template <size_t I>
constexpr tag<I> tag_v {};

template <size_t N>
using tag_range = std::make_index_sequence<N>;
} // namespace tuplet

// tuplet concepts
namespace tuplet {
// clang-format off
template <class T, class U>
concept same_as = std::is_same_v<T, U>&& std::is_same_v<U, T>;

template <class T, class U>
concept other_than = !std::is_same_v<std::decay_t<T>, U>;

template <class Wrapper>
concept wrapper = requires(Wrapper w) {
    typename Wrapper::type;
    { w.get() }
    ->same_as<typename Wrapper::type&>;
    (typename Wrapper::type&)(w);
};

template <class Tuple>
concept base_list_tuple = requires() {
    typename std::decay_t<Tuple>::base_list;
};

template <class T>
concept empty_type = std::is_empty_v<T>;

template <class T>
concept indexable = empty_type<T> || requires(T t) {
    t[tag<0>()];
};

template <class T>
concept tuple_type = indexable<T> || requires(T t) {
    std::get<0>(t);
};

template <class U, class T>
concept assignable_to = requires(U u, T t) {
    t = u;
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
};

template <class T>
struct deduce_elem {
    using type = T;
};
template <wrapper T>
struct deduce_elem<T> {
    using type = typename T::type&;
};
template <class T>
using deduce_elem_t = typename deduce_elem<T>::type;
} // namespace tuplet

// tuplet::detail::get_tuple_base implementation
// tuplet::detail::convert_impl

namespace tuplet::detail {
template <class A, class... T>
struct get_tuple_base;

template <size_t... I, class... T>
struct get_tuple_base<std::index_sequence<I...>, T...> {
    using type = type_map<tuple_elem<I, T>...>;
};

template <class Out, class In, class... Bases>
constexpr Out convert_impl(In&& in, type_list<Bases...>) {
    return Out {std::forward<In>(in).identity_t<Bases>::value...};
}
template <class F, class Tup, class... Bases>
constexpr decltype(auto) apply_impl(F&& f, Tup&& t, type_list<Bases...>) {
    return std::forward<F>(f)(std::forward<Tup>(t).identity_t<Bases>::value...);
}
template <class T, class... U, class... B>
void assign_impl(T&& tup, type_list<B...>, U&&... u) {
    (void(tup.identity_t<B>::value = std::forward<U>(u)), ...);
}
} // namespace tuplet::detail

namespace tuplet {
template <class... T>
using tuple_base_t =
    typename detail::get_tuple_base<tag_range<sizeof...(T)>, T...>::type;

template <class... T>
struct tuple : tuple_base_t<T...> {
    constexpr static size_t N = sizeof...(T);
    using super = tuple_base_t<T...>;
    using super::operator[];
    using base_list = typename super::base_list;
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
        detail::assign_impl(*this, base_list(), std::forward<U>(values)...);
        return *this;
    }

   private:
    template <class Tuple, class... B1, class... B2>
    void eq_impl(Tuple&& t, type_list<B1...>, type_list<B2...>) {
        (void(B1::value = t.identity_t<B2>::value), ...);
    }
    template <class U, size_t... I>
    void eq_impl(U&& u, std::index_sequence<I...>) {
        (void(tuple_elem<I, T>::value = get<I>(std::forward<U>(u))), ...);
    }
};
template <class... T>
tuple(T...) -> tuple<deduce_elem_t<T>...>;
} // namespace tuplet

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
};
template <class A, class B>
pair(A, B) -> pair<deduce_elem_t<A>, deduce_elem_t<B>>;
} // namespace tuplet

namespace tuplet {
// Converts from one tuple type to any other tuple or U
template <class Tuple>
struct convert {
    using base_list = typename std::decay_t<Tuple>::base_list;
    Tuple tuple;
    template <class U>
    constexpr operator U() const {
        return detail::convert_impl<U>((tuple), base_list());
    }
    template <class U>
    constexpr operator U() {
        return detail::convert_impl<U>((std::move(*this).tuple), base_list());
    }
};
template <class Tuple>
convert(Tuple&) -> convert<Tuple&>;
template <class Tuple>
convert(Tuple &&) -> convert<Tuple&>;
} // namespace tuplet

namespace tuplet {
template <size_t I, indexable Tup>
decltype(auto) get(Tup&& tup) {
    return std::forward<Tup>(tup)[tag<I>()];
}

template <class... T>
tuple<T&...> tie(T&... t) {
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

namespace tuplet::literals {
// clang-format off
template <char... D>
constexpr size_t size_t_from_digits() {
    static_assert((('0' <= D && D <= '9') && ...), "Must be integral literal");
    size_t num = 0;
    return ((num = num * 10 + (D - '0')), ..., num);
}
template <char... D> using index_t = tag<size_t_from_digits<D...>()>;
template <char... D> constexpr index_t<D...> operator""_tag() { return {}; }
template <char... D> constexpr index_t<D...> operator""_st() { return {}; }
template <char... D> constexpr index_t<D...> operator""_nd() { return {}; }
template <char... D> constexpr index_t<D...> operator""_rd() { return {}; }
template <char... D> constexpr index_t<D...> operator""_th() { return {}; }
// clang-format on
} // namespace tuplet::literals

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
