#include "range_view.h"
#include "count_iter.h"

namespace Dlugosz::d3 {

inline namespace minirange {


template <typename T>
auto iota (T count)
{
    using iter = count_iter<T>;
    using iota_range = range_view<iter>;
    return iota_range{iter{}, iter{count}};
}




}}



