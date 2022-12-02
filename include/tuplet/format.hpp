#ifndef TUPLET_FMTLIB_HPP_IMPLEMENTATION
#define TUPLET_FMTLIB_HPP_IMPLEMENTATION

#include <fmt/format.h>
#include <tuplet/tuple.hpp>

template <class... T>
struct fmt::formatter<tuplet::tuple<T...>> {
    char open_char = '(';
    char separator = ',';
    char close_char = ')';
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
        constexpr auto npos = std::string_view::npos;
        std::string_view view(ctx.begin(), ctx.end() - ctx.begin());
        if (view.size() == 0) {
            return ctx.begin();
        } else if (view.size() == 3) {
            open_char = view[0];
            close_char = view[1];
        } else if (view.size() == 4) {
            open_char = view[0];
            separator = view[1];
            close_char = view[2];
        } else {
            throw fmt::format_error(fmt::format(
                "Format specification {} is currently unsupported",
                view));
        }
        if (std::string_view("<{[(").find(open_char) == npos) {
            throw fmt::format_error(
                fmt::format("Enable to interpret open char {}", open_char));
        }
        if (std::string_view(">}])").find(close_char) == npos) {
            throw fmt::format_error(
                fmt::format("Unable to interpret close char {}", close_char));
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
            return tuplet::apply(print_elems, p);
        } else {
            return format_to(ctx.out(), "{}{}", open_char, close_char);
        }
    }
};
#endif
