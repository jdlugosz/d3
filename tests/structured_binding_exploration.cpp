#include <iostream>
#include <complex>
#include <utility>
#include <catch.hpp>  // https://github.com/catchorg/Catch2
#include "d3/overflow.h"
#include "d3/minirange.h"
#include "d3/debug/noisy.h"

using std::string;
using std::string_view;
using std::cout;
using std::complex;

using namespace std::literals::complex_literals;

namespace D3 = Dlugosz::d3;
using namespace D3::twostep;
using D3::sSize;



template <typename T>
using complex_array_ref_t = T(&)[2];

template<typename T>
complex_array_ref_t<T> cartesian_1 (complex<T>& z)
{
    return reinterpret_cast<T(&)[2]>(z);
}



#if false
// does not work under MSVC.  Fine on Clang and gcc.
template<typename T>
decltype(auto) cartesian_2 (complex<T>& z)
{
    return reinterpret_cast<T(&)[2]>(z);
}
#endif




template<typename T>
auto cartesian_3 (const complex<T>& z)
{
    return std::make_pair (z.real(), z.imag());
}


template <typename T>
struct pole_pair
{
    T magnitude;
    T phase_angle;
    pole_pair (const std::complex<T>& z) : magnitude{abs(z)}, phase_angle{arg(z)} {}
};


extern const char test_pole_pair_label[] = "## pole_pair";

template <typename T>
struct test_pole_pair : D3::debug::noisy<test_pole_pair_label>
{
    T magnitude;
    T phase_angle;
    test_pole_pair (const std::complex<T>& z) : magnitude{abs(z)}, phase_angle{arg(z)} {}
};




TEST_CASE ("structured binding exploration", "[exploration]") {
    using ℂ = std::complex<double>;
    ℂ z1 = sin(1.0+2.1i);
    cout << z1 << '\n';

    SECTION ("explore1") {
        // this does not work with std::complex
        //  auto [a,b] = z1;

        auto [a,b] = reinterpret_cast<double(&)[2]>(z1);
        cout << a << ", " << b << '\n';

        //    auto [c,d] = cartesian2(z1);   // this does not work on MSVC 15.8.0p2, but OK on Clang and gcc
        auto [c,d] = cartesian_1(z1);   // this works fine, adding a name for the return type.
        cout << c << ", " << d << '\n';

        auto [e,f] = cartesian_3(z1);  // return a pair of results
        cout << e << ", " << f << '\n';

        auto [r,θ] = pole_pair(z1);
        cout << r << ", " << θ << " (" << θ*180/3.14159 << " degrees)\n";
    }

    SECTION ("no-extra-copies") {
        cout << "calling test_pole_pair\n";
        { // extra scope
        auto [r,θ] = test_pole_pair(z1);
        cout << r << ", " << θ << " (" << θ*180/3.14159 << " degrees)\n";
        }
        cout << "done with test_pole_pair -- only one object made and destroyed?\n";
        // even building with optimizations disabled, there should be 1 create and destroy, no copies.
    }
}


// ===== illustrate non-member get<> for using the class directly.

extern const char test_fred[] = "## fred_t";

class fred_t : D3::debug::noisy<test_fred> {
    string name;
    int age;
    double raw_score;
public:
    fred_t (string_view s, int age, double score) : name{s}, age{age}, raw_score{score} {};
    string get_name() const { return name; }
    int get_days() const { return 365*age; }
    double get_score() const
        {
        int a = std::min(age,50);
        return raw_score * 50/a;
        }
    template <size_t I>
    auto get() const {
        if constexpr (I==0) return get_name();
        else if constexpr (I==1)  return get_days();
        else if constexpr (I==2)  return get_score();
        else static_assert (I>=0 && I<3);
    }
};


namespace std {

template<>
class tuple_size<fred_t> : public integral_constant<size_t, 3> {};

template<size_t I>
class std::tuple_element<I, fred_t> {
public:
    using type = decltype (declval<fred_t>().get<I>());
};

};


TEST_CASE ("structured binding custom", "[exploration]") {

    SECTION ("by value") {
        cout << "Fred 1  ";
        fred_t fred { "Fred", 26, 80.7 };
        auto [v0,v1,v2] = fred;
        cout << v0 << ", " << v1 << ", " << v2 << '\n';
        REQUIRE (v0 == fred.get<0>());
        REQUIRE (v1 == fred.get<1>());
        REQUIRE (v2 == fred.get<2>());
        REQUIRE (v0 == fred.get_name());
        REQUIRE (v1 == fred.get_days());
        REQUIRE (v2 == fred.get_score());
    }
    SECTION ("no extra copies") {
        cout << "Fred 2  ";
        fred_t fred { "Freddy", 24, 72.3 };
        const auto& [v0,v1,v2] = fred;
        cout << v0 << ", " << v1 << ", " << v2 << '\n';
    }
};

