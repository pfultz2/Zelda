/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    overload.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_OVERLOAD_H
#define ZELDA_GUARD_FUNCTION_OVERLOAD_H

#include <zelda/function/adaptor.h>
#include <zelda/function/defer.h>

#ifndef ZELDA_OVERLOAD_LIMIT
#define ZELDA_OVERLOAD_LIMIT 16
#endif

namespace zelda {

namespace detail {
#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class...Fs> struct overload_adaptor_base;
 
template<class F1, class...Fs>
struct overload_adaptor_base<F1, Fs...> : F1, overload_adaptor_base<Fs...>
{
    typedef overload_adaptor_base<Fs...> base;

    overload_adaptor_base()
    {}

    template<class T, class... Ts>
    overload_adaptor_base(T head, Ts... tail)
    : F1(head), base(tail...)
    {}

    using F1::operator();
    using base::operator();
};


#else
template<class F, ZELDA_PP_PARAMS_Z(1, ZELDA_OVERLOAD_LIMIT, class F, = void BOOST_PP_INTERCEPT)>
struct overload_adaptor_base;

#define ZELDA_OVERLOAD_ADAPTOR_BASE(z, n, data) \
template<class F, ZELDA_PP_PARAMS_Z(1, n, class F)>
struct overload_adaptor_base<F, ZELDA_PP_PARAMS_Z(1, n, F)> : F, overload_adaptor_base<ZELDA_PP_PARAMS_Z(1, n, F)> \
{ \
    typedef overload_adaptor_base<ZELDA_PP_PARAMS_Z(1, n, F)> base; \
    overload_adaptor_base() {} \
    \
    template<class T, ZELDA_PP_PARAMS_Z(1, n, class T)> \
    overload_adaptor_base(T head, ZELDA_PP_PARAMS_Z(1, n, T, tail)) \
    : F1(head), base(ZELDA_PP_PARAMS_Z(1, n, tail)) \
    {} \
    \
    using F1::operator(); \
    using base::operator(); \
};
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_OVERLOAD_LIMIT, ZELDA_OVERLOAD_ADAPTOR_BASE, ~)

#endif

template<class F>
struct overload_adaptor_base<F> : F
{
    typedef F base;
    using F::operator();

    overload_adaptor_base()
    {}

    template<class T>
    overload_adaptor_base(T f) : F(f)
    {}
};
}

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class...Fs> 
struct overload_adaptor : defer_adaptor<detail::overload_adaptor_base<Fs...> >
{
    typedef detail::overload_adaptor_base<Fs...> core;
    typedef defer_adaptor<core> base;
    overload_adaptor()
    {}

    template<class... T>
    overload_adaptor(T... x) : base(core(x...))
    {}
};

template<class...Fs>
typename overload_adaptor<Fs...>::type overload(Fs...x)
{ 
    return overload_adaptor<Fs...>(x...); 
}
#else

template<ZELDA_PP_PARAMS_Z(1, ZELDA_OVERLOAD_LIMIT, class F, = void BOOST_PP_INTERCEPT)> 
struct overload_adaptor : defer_adaptor<detail::overload_adaptor_base<ZELDA_PP_PARAMS_Z(1, ZELDA_OVERLOAD_LIMIT, F)> >
{
    typedef detail::overload_adaptor_base<ZELDA_PP_PARAMS_Z(1, ZELDA_OVERLOAD_LIMIT, F)> core;
    typedef defer_adaptor<core> base;
    overload_adaptor()
    {}
#define ZELDA_OVERLOAD_ADAPTOR_CONSTRUCTOR(z, n, data)
    template<ZELDA_PP_PARAMS_Z(1, n, class T)> \
    overload_adaptor(ZELDA_PP_PARAMS_Z(1, n, class T, x)) : base(core(ZELDA_PP_PARAMS_Z(1, n, x))) \
    {}
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_OVERLOAD_LIMIT, ZELDA_OVERLOAD_ADAPTOR_CONSTRUCTOR, ~)
};


#define ZELDA_OVERLOAD_FUNCTION(z, n, data) \
template<ZELDA_PP_PARAMS_Z(1, n, class F)> \
typename overload_adaptor<ZELDA_PP_PARAMS_Z(1, n, F)>::type overload(ZELDA_PP_PARAMS_Z(1, n, F, x)) \
{ \
    return overload_adaptor<ZELDA_PP_PARAMS_Z(1, n, F)>(ZELDA_PP_PARAMS_Z(1, n, x)); \
}
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_OVERLOAD_LIMIT, ZELDA_OVERLOAD_FUNCTION, ~)
#endif



}

#endif
