#pragma once
#include <type_traits>
#include "range_view.h"
#include "count_iter.h"
#include "is_range.h"

namespace Dlugosz::d3 {

inline namespace minirange {

// ===== plain counter only

template <typename T,
    typename = std::enable_if_t<std::is_integral_v<T>> >
auto iota (T count)
{
    using iter = count_iter<T>;
    using iota_range = range_view<iter>;
    return iota_range{iter{}, iter{count}};
}


// ===== indexed range


template <typename R>
class indexed_range {
    R& r1;
public:
    indexed_range (R&& param) : r1{param}  { }
    indexed_range (R& param) : r1{param}  { }

};


#if 0
template <typename R,
    typename = std::enable_if_t<is_range_v<R>> >
auto make_indexed_range (R&& r)   // temporary name, before overloading
{
    return indexed_range (std::forward<R>(r));
}

#endif
}}



