#pragma once
#include <ostream>
#include <tuplet/format.hpp>
#include <tuplet/tuple.hpp>

namespace Catch {
    template <class... T>
    std::ostream& operator<<(
        std::ostream& cout,
        tuplet::tuple<T...> const& tup) {
        return cout << fmt::format("{:[]}", tup);
    }
} // namespace Catch
