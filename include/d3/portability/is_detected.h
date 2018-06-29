#pragma once

#include <type_traits>
#include "PORT_CHOICE.hh"

// first, figure out which way to do it.  It can be pre-specified as a #define or -D at compile time

#if !defined DLU_PORT_DETECT
// if specified by user directly, keep it.

#if defined DLU_PORT_ALL
// a wildcard that applies to all PORTs that are not otherwise explicitly set

#    define DLU_PORT_DETECT DLU_PORT_ALL

#elif defined _MSC_VER
// The Microsoft compiler

#if _MSC_FULL_VER > 191526608  // the last one I tried

//  >>> NOTE: next time compiler updates, also remove the work-around in twostep.h !!!!             <<<<

# error Check to see if detection idiom is avalable yet
#else
#define DLU_PORT_DETECT DLU_PORT_CHOICE_BOOST
#endif


// #elif other compiler brands go here *****
          //  if you get here, please update and send a pull request.
#endif


#endif


// ===========
// Use the chosen option


#if DLU_PORT_DETECT == DLU_PORT_CHOICE_STD

#    error Need to update file.
		// this does not exist in the standard yet.  When a compiler is observed to have it,
		// put in the proper lines to match the actuality.

		   // == presumed ==
		   // <type_traits> already included
    namespace Dlugosz::d3 {
    inline namespace detection_idiom {
		using std::is_detected;
		using std::is_detected_v;
		using std::detected_t;
		using std::detected_or;
		using std::detected_or_t;
		using std::is_detected_exact;
		using std::is_detected_exact_v;
		using std::is_detected_convertable;
		using std::is_detected_convertable_v;

#elif DLU_PORT_DETECT == DLU_PORT_CHOICE_EXPERIMENTAL

#   include <experimental/type_traits>
    namespace Dlugosz::d3 {
    inline namespace detection_idiom {
		using std::experimental::is_detected;
		using std::experimental::is_detected_v;
		using std::experimental::detected_t;
		using std::experimental::detected_or;
		using std::experimental::detected_or_t;
		using std::experimental::is_detected_exact;
		using std::experimental::is_detected_exact_v;
		using std::experimental::is_detected_convertable;
		using std::experimental::is_detected_convertable_v;

#elif DLU_PORT_DETECT == DLU_PORT_CHOICE_BOOST

#   include <boost/type_traits/is_detected.hpp>
    namespace Dlugosz::d3 {
    inline namespace detection_idiom {
		using boost::is_detected;
		using boost::is_detected_v;

		template <template<class...> class Op, class... Args>
		using detected_t = typename is_detected<Op, Args...>::type;

		// Boost (as of 1.67.0) does not include all the forms.  Copied these definitions
		// from n4562 §3.3.4, using Boost’s underlying helper and qualifying std:: type traits used.

		template <class Default, template<class...> class Op, class... Args>
		using detected_or = boost::detail::detector<Default, void, Op, Args...>;

		template <class Default, template<class...> class Op, class... Args>
		using detected_or_t = typename detected_or<Default, Op, Args...>::type;

		template <class Expected, template<class...> class Op, class... Args>
		using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

		template <class Expected, template<class...> class Op, class... Args>
		constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

		template <class To, template<class...> class Op, class... Args>
		using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

		template <class To, template<class...> class Op, class... Args>
		constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;


#else

#    error Detection idiom configuration has bad value

#endif

} }
