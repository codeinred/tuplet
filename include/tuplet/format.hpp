#ifndef TUPLET_FMTLIB_HPP_IMPLEMENTATION
#define TUPLET_FMTLIB_HPP_IMPLEMENTATION

#include <fmt/format.h>
#include <tuplet/tuple.hpp>

template <class... T>
struct fmt::formatter<tuplet::tuple<T...>> {
    char open_char = '{';
    char separator = ',';
    char close_char = '}';
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        std::string_view view(ctx.begin(), ctx.end() - ctx.begin());
        if (view.size() == 0) {
            return ctx.begin();
        }
        if (view.size() == 3) {
            open_char = view[0];
            close_char = view[1];
        }
        if (view.size() == 4) {
            open_char = view[0];
            separator = view[1];
            close_char = view[2];
        }
        return ctx.begin() + view.size() - 1;
    }
    template <typename FormatContext>
    constexpr auto format(const tuplet::tuple<T...>& p, FormatContext& ctx)
        -> decltype(ctx.out()) {
        if constexpr (sizeof...(T) >= 1) {
            auto print_elems = [&](auto const& first, auto const&... rest) {
                auto out = format_to(ctx.out(), "{}{}", open_char, first);
                ((out = format_to(out, "{} {}", separator, rest)), ...);
                return format_to(out, "{}", close_char);
            };
            return apply(print_elems, p);
        } else {
            return format_to(ctx.out(), "{}{}", open_char, close_char);
        }
    }
};
#endif
