namespace Dlugosz::d3 {

inline namespace minirange {



struct count_iter {
    int value;
    int operator* () const { return value; }
    auto operator++ ()  { ++value;  return *this; }
};

bool operator== (const count_iter& left, const count_iter& right)
{
    return left.value == right.value;
}


bool operator!= (const count_iter& left, const count_iter& right)
{
    return !(left==right);
}





}}

template<>
struct std::iterator_traits<Dlugosz::d3::count_iter> {
    using difference_type = int;
    using value_type = int;
    using pointer = int;
    using reference = int&;
    using iterator_category = std::forward_iterator_tag;   // >>  for now
};

