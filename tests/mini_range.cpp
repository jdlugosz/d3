#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <catch.hpp>  // https://github.com/catchorg/Catch2
#include "d3/minirange.h"
#include "d3/overflow.h"

using std::string;
using std::string_view;
using std::cout;


/////////////////////////


namespace Dlugosz::d3 {


void f() {

    count_iter B {1};
    count_iter E {42};

    for (auto it=B;  it != E;  ++it)
        cout << ' ' << *it;
    cout << '\n';
}



void g() {

    range_view<count_iter<>> counter {count_iter{1},count_iter{42}};

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


template<typename R1, typename R2>
using can_eq = decltype( std::declval<R1>() == std::declval<R2>() );

template<typename R1, typename R2>
using can_neq = decltype( std::declval<R1>() != std::declval<R2>() );

template<typename R1, typename R2>
using can_relate = decltype( (std::declval<R1>() < std::declval<R2>()) || (std::declval<R1>() > std::declval<R2>())
    || (std::declval<R1>() <= std::declval<R2>())  || (std::declval<R1>() >= std::declval<R2>()) );

template<typename R1, typename R2>
using can_plus = decltype( std::declval<R1>() + std::declval<R2>() );


TEST_CASE ("iota and range_view") {
    using D3::count_iter;
    using D3::iota;
    using D3::make_range_view;
    using D3::range_view;
    std::stringstream os;

    constexpr char buffer[] = "This is a test.";

    SECTION ("example usage") {
        for (auto i : iota(10))  os << i << ' ';
        CHECK (os.str() == "0 1 2 3 4 5 6 7 8 9 ");
        for (auto i : 3+iota(9))  os << i << ' ';
    }

    SECTION ("equality") {
        auto r1 = iota (10);
        auto r2 { r1 };
        auto r3 = iota (10);
        auto r4 = iota (42);
        auto r5 = 7 + iota(42);

        // equality of range views
        REQUIRE (r1 == r2);
        REQUIRE (r1 == r3);
        REQUIRE (r3 == r2);
        REQUIRE_FALSE (r1 == r4);  // same begin, different end
        REQUIRE_FALSE (r4 == r5);  // different begin, same end

        // view against iterator
        count_iter b1 {0};
        count_iter b2 {7};
        count_iter b3 {42};
        REQUIRE (b1 == r1);
        REQUIRE (r1 == b1);  // different arg types so check both ways
        REQUIRE (b1 == r4);
        REQUIRE_FALSE (b1 == r5);
        REQUIRE_FALSE (r5 == b1);


        constexpr int A10[] = { 0,1,2,3,4,5,6,7,8,9 };
        // nothing to do with my types
        // bool z1 { A10 == r1};  // compile-time error
        constexpr bool zz1 = D3::is_detected_v<can_eq, decltype(A10),decltype(r1)>;
        REQUIRE_FALSE (zz1);


        // since != just calls ==, I mainly just need to check that the template picks up only
        // the forms it is supposed to.
        REQUIRE (r1 != r4);
        REQUIRE (b1 != r5);
        REQUIRE (r5 != b1);

        // bool z2 { A10 != r1};  // compile-time error
        constexpr bool zz2 = D3::is_detected_v<can_neq, decltype(A10),decltype(r1)>;
        REQUIRE_FALSE (zz2);
    }


    SECTION ("relational") {
        // there is no ordering between range views.  Relation operations are defined for using
        // the range view as a smart iterator.  To prevent misunderstanding, one side must explicitly
        // be an iterator.
        auto buf_it1 = Begin(buffer) + 5;
        range_view buf_view { buf_it1, End(buffer)};
        REQUIRE (buf_it1 == buf_view);
        CHECK (buf_it1 <= buf_view);
        CHECK (buf_view <= buf_it1);
        CHECK (buf_it1 >= buf_view);
        CHECK (buf_view >= buf_it1);
        buf_it1 -= 4;
        CHECK (buf_it1 < buf_view);
        CHECK (buf_it1 <= buf_view);
        CHECK_FALSE (buf_it1 > buf_view);
        CHECK_FALSE (buf_it1 >= buf_view);
        CHECK_FALSE (buf_view < buf_it1);
        CHECK_FALSE (buf_view <= buf_it1);
        CHECK (buf_view > buf_it1);
        CHECK (buf_view >= buf_it1);
        buf_it1 += 8;
        CHECK_FALSE (buf_it1 < buf_view);
        CHECK (buf_view < buf_it1);
        CHECK_FALSE (buf_it1 <= buf_view);
        CHECK (buf_view <= buf_it1);
        CHECK (buf_it1 > buf_view);
        CHECK_FALSE (buf_view > buf_it1);
        CHECK (buf_it1 >= buf_view);
        CHECK_FALSE (buf_view >= buf_it1);
        static_assert (!(D3::is_detected_v<can_relate, decltype(buf_view),decltype(buf_view)>), "view < view");
        int A2[2];
        static_assert (!(D3::is_detected_v<can_relate, decltype(Begin(A2)), decltype(buf_view)>), "wrong types");
        static_assert (!(D3::is_detected_v<can_relate, decltype(buf_view), decltype(Begin(A2))>), "wrong types");
    }


    SECTION ("add") {
        auto buf_it1 = Begin(buffer) + 5;
        auto buf_view = make_range_view(buffer);
        // range_view + distance, both order of arguments
        CHECK (buf_it1 == buf_view+5);
        CHECK (buf_it1 == 5+buf_view);
        auto bv2 = buf_view;
        static_assert (std::is_same_v<decltype(bv2),decltype(buf_view)>);
        // member +=
        bv2 += 5;
        CHECK (bv2 == buf_it1);
        // ++, both ways
        auto result= ++bv2;
        ++buf_it1;
        CHECK (result == bv2);  // returns new value after incrementing
        CHECK (result == buf_it1);
        result= bv2++;
        CHECK_FALSE (result == bv2);  // should return old value before incrementing
        CHECK (result == buf_it1);
        buf_it1++;
        CHECK (bv2 == buf_it1);
        // adding two ranges doesn’t make sence
        //  auto result= buf_view + bv2
        static_assert (!(D3::is_detected_v<can_plus, decltype(buf_view),decltype(bv2)>), "view + view");
}


    SECTION ("subtract") {
        auto buf_it1 = Begin(buffer) + 10;
        auto buf_view = make_range_view(buffer);
        buf_view += 10;
        REQUIRE (buf_view == buf_it1);
        // operator-- both ways
        auto result= --buf_view;
        CHECK (result == buf_view);  // returns the new value after decrementing
        --buf_it1;
        CHECK (result == buf_it1);
        result= buf_view--;
        CHECK_FALSE (result == buf_view);  // returns original value
        CHECK (result == buf_it1);
        buf_it1--;
        CHECK (buf_view == buf_it1);
        // -=
        buf_view -= 4;
        auto it2 = buf_it1 - 4;
        CHECK (buf_view == it2);
        // non-member - three forms
        CHECK (buf_view - 3 == it2 - 3);  // range_view - difference_type
        // range_view - iterator -> difference
        CHECK (buf_view - buf_it1 == -4);
        CHECK (buf_view - it2 == 0);
        // iterator - range_view -> difference
        CHECK (buf_it1 - buf_view == 4);
        CHECK (it2 - buf_view == 0);
    }

}


// easy way to create an rvalue from test values
template <typename T>
T identity (const T& param)
{
    return param;
}

TEST_CASE ("indexed range") {
    constexpr char buffer[] = "This is a test.";
    std::vector<float> float_vec (5);
    using D3::indexed_range;
    using D3::iota;

    SECTION ("constructor overloads") {
        indexed_range<std::vector<float>> rr1 (float_vec);
        indexed_range<std::vector<float>> rr2 {identity(float_vec)};
        indexed_range rr3 {float_vec};  // class argument deduction
    }

    SECTION ("iota") {
        auto rr1 = iota (float_vec);
        auto rr2 = iota (buffer);
    }

    SECTION ("semantics") {
        auto rr1 = iota (float_vec);
        // n4569 §9.5.4 states that the `for` loop will first look for unqualified member names `begin` and `end`.
        auto it1 = rr1.begin();
        auto end1 = rr1.end();

    }
}