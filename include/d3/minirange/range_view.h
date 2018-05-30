
#include "d3/is_range.h"

namespace Dlugosz::d3 {

inline namespace minirange {

template <typename B_iter, typename E_iter=B_iter>
struct range_view {
    B_iter first;
    E_iter second;
    auto begin() const { return first; }
    auto end() const { return second; }

    range_view() =delete;
//    range_view (const range_view&) =default;
    range_view (B_iter first, E_iter second) : first{first}, second{second} {}
    template <typename R /*, typename = is_range_t<R> */  >
    range_view (const R& range) : first{Begin(range), second{End(range)}}  {}

    [[nodiscard]] bool empty() const noexcept { return first == second; }
    bool operator!() const noexcept { return empty(); }
    explicit operator bool() const noexcept { return !empty(); }

    // act like the 'first' when used as an iterator
    auto operator*() const { return *first; }
    range_view& operator++()  { ++first; return *this; }
    range_view operator++(int) { auto temp= *this;  ++first;  return temp; }
};


// Visual Studio 15.8.0p1 does not support user-defined deduction guides yet.

template <typename R>
auto make_range_view (const R& range)
{
    return range_view {Begin(range), End(range)};
}

}}
