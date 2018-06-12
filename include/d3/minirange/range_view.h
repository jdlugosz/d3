
#pragma once
#include "d3/is_range.h"

namespace Dlugosz::d3 {

inline namespace minirange {

template <typename B_iter, typename E_iter=B_iter>
struct range_view {
    B_iter first;
    E_iter second;
    using difference_type = typename std::iterator_traits<B_iter>::difference_type;
    auto begin() const { return first; }
    auto end() const { return second; }

    range_view() =delete;
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
    range_view& operator+= (difference_type n) noexcept {  first+=n;  return *this;  }
    range_view& operator-- () noexcept { --first;  return *this; }
    range_view operator-- (int)  { auto temp= *this;  --first;  return temp;  }
    range_view& operator-= (difference_type n) {  first-=n;  return *this;  }
};


// Visual Studio 15.8.0p1 does not support user-defined deduction guides yet.

template <typename R>
auto make_range_view (const R& range)
{
    return range_view {Begin(range), End(range)};
}


template <typename Tb, typename Te>
bool operator== (const range_view<Tb,Te>& left, const range_view<Tb,Te>& right) noexcept
{
    // comparing ranges compares both ends
    return left.first == right.first && left.second == right.second;
}


template <typename Tb, typename Te>
bool operator== (const range_view<Tb,Te>& left, const Tb& right) noexcept
{
    return left.first == right;
}



template <typename Tb, typename Te>
bool operator== (const Tb& left, const range_view<Tb,Te>& right) noexcept
{
    return left == right.first;
}



////////
namespace rv_detail {

template <typename T1, typename T2>
struct comp_args
{
    static constexpr bool value = std::is_same_v<T1,T2>;
};


template <typename Tb, typename Te>
struct comp_args<range_view<Tb,Te>, Tb>
{
    static constexpr bool value = true;
};


template <typename T1, typename T2>
inline constexpr bool comp_args_v = comp_args<T1,T2>::value || comp_args<T2,T1>::value;

}
////////


template <typename T1, typename T2,
    typename = std::enable_if_t<rv_detail::comp_args_v<T1,T2> > >
bool operator!= (const T1& left, const T2& right) noexcept
{
    return !(left==right);
}




template <typename Tb, typename Te>
range_view<Tb,Te> operator+ (range_view<Tb,Te> left, typename range_view<Tb,Te>::difference_type right) noexcept
{
    left += right;
    return left;
}


template <typename Tb, typename Te>
range_view<Tb,Te> operator+ (typename range_view<Tb,Te>::difference_type left, range_view<Tb,Te> right) noexcept
{
    right += left;
    return right;
}


template <typename Tb, typename Te>
range_view<Tb,Te> operator- (range_view<Tb,Te> left, typename range_view<Tb,Te>::difference_type right) noexcept
{
    left -= right;
    return left;
}



// >>>>>> NO
template <typename Tb, typename Te>
typename range_view<Tb,Te>::difference_type
operator- (range_view<Tb,Te> left, range_view<Tb,Te> right) noexcept
{
    return right.first - left.first;
}


// ===== relational

template <typename Tb, typename Te>
bool operator< (range_view<Tb,Te> left, Tb right) noexcept
{
    return left.first < right;
}


template <typename Tb, typename Te>
bool operator< (Tb left, range_view<Tb,Te> right) noexcept
{
    return left < right.first;
}


template <typename Tb, typename Te>
bool operator> (range_view<Tb,Te> left, Tb right) noexcept
{
    return left.first > right;
}


template <typename Tb, typename Te>
bool operator> (Tb left, range_view<Tb,Te> right) noexcept
{
    return left > right.first;
}


template <typename Tb, typename Te>
bool operator<= (range_view<Tb,Te> left, Tb right) noexcept
{
    return left.first <= right;
}


template <typename Tb, typename Te>
bool operator<= (Tb left, range_view<Tb,Te> right) noexcept
{
    return left <= right.first;
}

template <typename Tb, typename Te>
bool operator>= (range_view<Tb,Te> left, Tb right) noexcept
{
    return left.first >= right;
}


template <typename Tb, typename Te>
bool operator>= (Tb left, range_view<Tb,Te> right) noexcept
{
    return left >= right.first;
}



}}
// back to global namespace here.


// Iterator Traits of a range_view gives me that of its Begin/first member.
template<typename B_iter, typename E_iter>
struct std::iterator_traits<Dlugosz::d3::range_view<B_iter,E_iter> >
    : public std::iterator_traits<B_iter>
{ };

