#pragma once
#include <type_traits>
//#include <iterator>
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
    auto end() const { return End(r1); }

    using r_it = decltype(Begin(r1));
    class iterator {
        r_it it;  // regular iterator to collection
        size_t index = 0;
    public:
        explicit iterator (r_it it) : it{it} {}
        bool operator != (r_it e) const
        {
            return it != e;
        }
    };

    iterator begin() const { return iterator{Begin(r1)}; }
};


template <typename R>
bool operator != (const typename indexed_range<R>::iterator& e, typename indexed_range<R>::r_it b)
{
    return b != e;
}



template <typename R,
    typename = std::enable_if_t<is_range_v<R>> >
auto iota (R&& r)
{
    return indexed_range (std::forward<R>(r));
}

}}



