namespace Dlugosz::d3 {

inline namespace minirange {



template <typename T = int>
struct count_iter {
    T value;
    using difference_type = std::make_signed_t<T>;
    count_iter (T value) : value{value} { }
    T operator* () const { return value; }
    T& operator* () { return value; }
    // no operator-> because T has no members!
    count_iter& operator++ ()  { ++value;  return *this; }
    count_iter operator++ (int)  { auto temp= *this;  ++value;  return temp;  }
    count_iter& operator+= (difference_type n) {  value+=n;  return *this;  }
    count_iter& operator-- ()  { --value;  return *this; }
    count_iter operator-- (int)  { auto temp= *this;  --value;  return temp;  }
    count_iter& operator-= (difference_type n) {  value-=n;  return *this;  }
};

template <typename T>
bool operator== (const count_iter<T>& left, const count_iter<T>& right)
{
    return left.value == right.value;
}


template <typename T>
bool operator!= (const count_iter<T>& left, const count_iter<T>& right)
{
    return !(left==right);
}


template <typename T>
auto operator+ (count_iter<T> left, typename count_iter<T>::difference_type right) -> count_iter<T>
{
    left += right;
    return left;
}


template <typename T>
auto operator+ (typename count_iter<T>::difference_type left, count_iter<T> right) -> count_iter<T>
{
    right += left;
    return right;
}


template <typename T>
auto operator- (count_iter<T> left, typename count_iter<T>::difference_type right) -> count_iter<T>
{
    left -= right;
    return left;
}


template <typename T>
auto operator- (count_iter<T> left, count_iter<T> right) -> typename count_iter<T>::difference_type
{
    return right.value - left.value;
}

template <typename T>
bool operator< (count_iter<T> left, count_iter<T> right)
{
    return left.value < right.value;
}


template <typename T>
bool operator> (count_iter<T> left, count_iter<T> right)
{
    return left.value > right.value;
}


template <typename T>
bool operator<= (count_iter<T> left, count_iter<T> right)
{
    return left.value <= right.value;
}


template <typename T>
bool operator>= (count_iter<T> left, count_iter<T> right)
{
    return left.value >= right.value;
}



}}


template <typename T>
struct std::iterator_traits<Dlugosz::d3::count_iter<T>> {
    using difference_type = typename Dlugosz::d3::count_iter<T>::difference_type;
    using value_type = T;
    using pointer = T;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;
};

