#pragma once

#include <iterator>
#include <utility>


#if defined X
#    error This header uses "X macros".  \
     Either this #include is nested within another usage, or \
     an earlier header neglected to #undef it when done.
#endif

#define X_LIST \
	X(begin,Begin) \
	X(end,End) \
	X(cbegin,CBegin) \
	X(cend,CEnd) \
	X(rbegin,RBegin) \
	X(rend,REnd) \
	X(crbegin,CRBegin) \
	X(crend,CREnd) \
    X(advance,Advance) \
    X(distance,Distance) \
    X(next,Next) \
    X(prev,Prev) \
    X(size,Size) \
    X(empty,Empty) \
    X(data,Data) \
    X(swap,Swap) \


namespace Dlugosz::d3 {

namespace twostep_wrapper {
// The std:: declarations need to be in scope for the two-step functions to work,
// but we don't want them to appear as declared names in namespace twostep.  That way,
// `using namespace twostep;` will bring in Begin but not bring in begin from std.


#define X(xname,XName) using std::xname;
X_LIST
#undef X


namespace twostep_inner {

#define X(xname,XName) \
inline constexpr auto XName = [](auto&& r) \
    noexcept(noexcept(xname(std::forward<decltype(r)>(r)))) /* ask if the code is noexcept */ \
    -> decltype(      xname(std::forward<decltype(r)>(r)))  /* using return type to do SFINAE */ \
        {      return xname(std::forward<decltype(r)>(r));  /* the real body to execute */ };


X_LIST
#undef X


} // namespace twostep_inner

} // scope wrapper

// back in namespace d3
namespace twostep = twostep_wrapper::twostep_inner;
using namespace twostep;

}

#undef X_LIST
