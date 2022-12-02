#pragma once
#include <string_view>

namespace refl {
    template <class T>
    constexpr std::string_view get_name_of_type() {
#ifdef __GNUC__
        // GCC: constexpr name_of_type get_name_of_type() [with T = int]
        // Clang
        constexpr std::string_view name = __PRETTY_FUNCTION__;
        size_t start = name.find("T = ") + 4;
#if defined(__clang__) || defined(__INTEL_COMPILER)
        size_t end = name.rfind("]");
#else
        size_t end = name.rfind("; std::string_view = ");
#endif
        return name.substr(start, end - start);
#elif _MSC_VER
        constexpr std::string_view name = __FUNCSIG__;
        std::string_view start_token = "get_name_of_type<";
        size_t start = name.find(start_token) + start_token.size();
        size_t end = name.rfind(">(void)");
        return name.substr(start, end - start);
#else
        return "<some type>";
#endif
    };
    template <class T>
    constexpr std::string_view name_of_type = get_name_of_type<T>();
} // namespace refl
