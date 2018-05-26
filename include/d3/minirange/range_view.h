
#include "d3/is_range.h"

namespace Dlugosz::d3 {

inline namespace minirange {

template <typename B_iter, typename E_iter=B_iter>
struct range_view {
    B_iter first;
    E_iter second;
    auto begin() const { return first; }
    auto end() const { return second; }
    auto size() const { return second-first; }  // needs to be improved
    range_view() = delete;
    range_view (B_iter first, E_iter second) : first{first}, second{second} {}
    template <typename R /*, typename = is_range_t<R> */  >
    range_view (const R& range) : first{Begin(range), second{End(range)}}  {}

};


// Visual Studio 15.8.0p1 does not support user-defined deduction guides yet.

template <typename R>
auto make_range_view (const R& range)
{
    return range_view {Begin(range), End(range)};
}

}}
