#pragma once
#include <type_traits>
#include <iterator>

namespace Dlugosz::d3 {

inline namespace minirange {



template <typename T = int>
struct count_iter {
    static_assert (std::is_integral_v<T>, "count_iter is meant for integer-like types");
    T value;
    using difference_type = std::make_signed_t<T>;
    using value_type = T;
    using pointer = T;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;
    count_iter() noexcept : value{T()} { }
    explicit count_iter (T value) noexcept : value{value} { }
    T operator* () const noexcept { return value; }
    T& operator* () noexcept { return value; }
    // no operator-> because T has no members!
    count_iter& operator++ () noexcept { ++value;  return *this; }
    count_iter operator++ (int) noexcept { auto temp= *this;  ++value;  return temp;  }
    count_iter& operator+= (difference_type n) noexcept {  value+=n;  return *this;  }
    count_iter& operator-- () noexcept { --value;  return *this; }
    count_iter operator-- (int) noexcept { auto temp= *this;  --value;  return temp;  }
    count_iter& operator-= (difference_type n) noexcept {  value-=n;  return *this;  }
};

template <typename T>
bool operator== (const count_iter<T>& left, const count_iter<T>& right) noexcept
{
    return left.value == right.value;
}


template <typename T>
bool operator!= (const count_iter<T>& left, const count_iter<T>& right) noexcept
{
    return !(left==right);
}


template <typename T>
count_iter<T> operator+ (count_iter<T> left, typename count_iter<T>::difference_type right) noexcept
{
    left += right;
    return left;
}


template <typename T>
count_iter<T> operator+ (typename count_iter<T>::difference_type left, count_iter<T> right) noexcept
{
    right += left;
    return right;
}


template <typename T>
count_iter<T> operator- (count_iter<T> left, typename count_iter<T>::difference_type right) noexcept
{
    left -= right;
    return left;
}


template <typename T>
typename count_iter<T>::difference_type
operator- (count_iter<T> left, count_iter<T> right) noexcept
{
    return right.value - left.value;
}

template <typename T>
bool operator< (count_iter<T> left, count_iter<T> right) noexcept
{
    return left.value < right.value;
}


template <typename T>
bool operator> (count_iter<T> left, count_iter<T> right) noexcept
{
    return left.value > right.value;
}


template <typename T>
bool operator<= (count_iter<T> left, count_iter<T> right) noexcept
{
    return left.value <= right.value;
}


template <typename T>
bool operator>= (count_iter<T> left, count_iter<T> right) noexcept
{
    return left.value >= right.value;
}



}}


