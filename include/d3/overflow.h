#pragma once

#include "twostep.h"

namespace Dlugosz::d3 {

enum as_signed { };

namespace detail {

    template <typename R>
    struct impl_result {
        template <typename T>
        static R call (T x)
        {
        return static_cast<R>(x);
        }
    };


    template<>
    struct impl_result<as_signed> {
        template <typename T>
        static auto call (T x)
        {
        using R = std::make_signed_t<T>;
        return impl_result<T>::call(x);
        }
    };

}  // end detail


template <typename R, typename T>
auto certainly (T x)
{
    return detail::impl_result<R>::call(x);
}




template <typename Container>
inline constexpr auto sSize (Container&& r)
    noexcept (noexcept(twostep::Size(std::forward<decltype(r)>(r))))
    -> std::make_signed_t<decltype(twostep::Size(std::forward<decltype(r)>(r)))>
        {
        return certainly<as_signed>( twostep::Size(std::forward<decltype(r)>(r)) );
        }


}
