#pragma once

#include "portability/is_detected.h"
#include "twostep.h"

namespace Dlugosz::d3 {

namespace range_detect_detail {

	template<typename R>
	using has_begin_t = decltype(Begin (std::declval<R>()));

	template<typename R>
	using has_end_t = decltype(End (std::declval<R>()));


	template<typename R>
	using is_range_t = typename std::integral_constant<bool, is_detected_v<has_begin_t, R> && is_detected_v<has_end_t, R>>::type;

	template<typename R>
	inline constexpr bool is_range_v = is_range_t<R>::value;
}

using range_detect_detail::is_range_t;
using range_detect_detail::is_range_v;


}
