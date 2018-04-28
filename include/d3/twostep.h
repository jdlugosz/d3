#pragma once

#include <iterator>

namespace Dlugosz::d3 {

inline namespace twostep_wrapper {
// The std:: declarations need to be in scope for the two-step functions to work,
// but we don't want them to appear as declared names in namespace twostep.  That way,
// `using namespace twostep;` will bring in Begin but not bring in begin from std.

using std::begin;
using std::end;

inline namespace twostep_inner {

inline auto Begin = [](auto&& r) ->
    decltype(begin(std::forward<decltype(r)>(r))) // using return type to do SFINAE
{
	return begin(std::forward<decltype(r)>(r));
};

inline auto End = [](auto&& r) ->
decltype(end(std::forward<decltype(r)>(r)))
{
	return end(std::forward<decltype(r)>(r));
};

} // namespace twostep_inner

} // scope wrapper

// back in namespace d3
namespace twostep = twostep_wrapper::twostep_inner;
using namespace twostep;

}
