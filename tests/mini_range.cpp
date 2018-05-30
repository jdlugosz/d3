#include <iostream>
#include <numeric>
#include <algorithm>
#include <catch.hpp>  // https://github.com/catchorg/Catch2
#include "d3/minirange.h"
#include "d3/overflow.h"

using std::string;
using std::string_view;
using std::cout;


/////////////////////////

namespace Dlugosz::d3 {

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

}  // namespace


template<>
struct std::iterator_traits<Dlugosz::d3::count_iter> {
    using difference_type = int;
    using value_type = int;
    using pointer = int;
    using reference = int&;
    using iterator_category = std::forward_iterator_tag;   // >>  for now
};

namespace Dlugosz::d3 {


void f() {

    count_iter B {1};
    count_iter E {42};

    for (auto it=B;  it != E;  ++it)
        cout << ' ' << *it;
    cout << '\n';
}



void g() {

    range_view<count_iter> counter {{1},{42}};

    for (auto i : counter)
        cout << ' ' << i;
    cout << '\n';

    int total = std::accumulate (counter.begin(), counter.end(), 0);
    cout << "total accumulated is: " << total << '\n';

    auto found = std::find (counter.begin(), counter.end(), 18);
    cout << "found 18? " << *found << '\n';

    bool found2 = std::binary_search (counter.begin(), counter.end(), 18);
    cout << "binary search? " << found2 << '\n';
}

}




/////////////////////////

namespace D3 = Dlugosz::d3;
using namespace D3::twostep;
using D3::sSize;


TEST_CASE ("scratch work") {

    D3::f();
    D3::g();
}

TEST_CASE ("range_view") {

    std::vector<int> v1 { 1, 2, 6, 8, 20 };
//    D3::range_view rv1 { v1 };   // needs user-defined deduction guide in order to do that.
    auto rv1 = D3::make_range_view(v1);

    REQUIRE (  std::equal (Begin(rv1),End(rv1), Begin(v1))  );
    if (rv1) cout << *rv1++;
    for (auto i : rv1) {
        cout << ", " << i;
    }
    cout << '\n';

    bool found2 = std::binary_search (rv1.begin(), rv1.end(), 6);
    cout << "binary search? " << found2 << '\n';

    static_assert (std::is_same_v<std::iterator_traits<decltype (Begin(v1))>::iterator_category,  std::iterator_traits<decltype (rv1)>::iterator_category>);
}
