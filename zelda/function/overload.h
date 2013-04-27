/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    overload.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_OVERLOAD_H
#define ZELDA_GUARD_FUNCTION_OVERLOAD_H

// @begin
// overload
// ========
// 
// Description
// -----------
// 
// The `overload` function adaptor combines several functions together and
// resolves which one should be called by using C++ overload resolution. This is
// different than than the `conditional` adaptor which resolves them based on
// order.
// 
// Synopsis
// --------
// 
//     template<class F1, class F2, ...>
//     overload_adaptor<F1, F2, ...> overload(F1 f1, F2 f2, ...);
// 
// Example
// -------
// 
//     struct int_class
//     {
//         int operator()(int) const
//         {
//             return 1;
//         }
//     };
// 
//     struct foo
//     {};
// 
//     struct foo_class
//     {
//         foo operator()(foo) const
//         {
//             return foo();
//         }
//     };
// 
//     typedef overload_adaptor<int_class, foo_class> fun;
// 
//     static_assert(boost::is_same<int, ZELDA_TYPEOF(fun()(1))>::value, "Failed overload");
//     static_assert(boost::is_same<foo, ZELDA_TYPEOF(fun()(foo()))>::value, "Failed overload");
// 
// @end

#include <zelda/function/adaptor.h>
#include <zelda/function/defer.h>
#include <zelda/function/detail/nullary_tr1_result_of.h>

#ifndef ZELDA_OVERLOAD_LIMIT
#define ZELDA_OVERLOAD_LIMIT 16
#endif

namespace zelda {

namespace detail {
#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class...Fs> struct overload_adaptor_base;
 
template<class F, class...Fs>
struct overload_adaptor_base<F, Fs...> : F, overload_adaptor_base<Fs...>
{
    typedef overload_adaptor_base<Fs...> base;

    overload_adaptor_base()
    {}

    template<class T, class... Ts>
    overload_adaptor_base(T head, Ts... tail)
    : F(head), base(tail...)
    {}

    using F::operator();
    using base::operator();
};


#else
template<class F, ZELDA_PP_PARAMS_Z(1, ZELDA_OVERLOAD_LIMIT, class F, = void BOOST_PP_INTERCEPT)>
struct overload_adaptor_base;

#define ZELDA_OVERLOAD_ADAPTOR_BASE(z, n, data) \
template<class F, ZELDA_PP_PARAMS_Z(z, n, class F)> \
struct overload_adaptor_base<F, ZELDA_PP_PARAMS_Z(z, n, F)> : F, overload_adaptor_base<ZELDA_PP_PARAMS_Z(z, n, F)> \
{ \
    typedef overload_adaptor_base<ZELDA_PP_PARAMS_Z(z, n, F)> base; \
    overload_adaptor_base() {} \
    \
    template<class T, ZELDA_PP_PARAMS_Z(z, n, class T)> \
    overload_adaptor_base(T head, ZELDA_PP_PARAMS_Z(z, n, T, tail)) \
    : F(head), base(ZELDA_PP_PARAMS_Z(z, n, tail)) \
    {} \
    \
    using F::operator(); \
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
#define ZELDA_OVERLOAD_ADAPTOR_CONSTRUCTOR(z, n, data) \
    template<ZELDA_PP_PARAMS_Z(z, n, class T)> \
    overload_adaptor(ZELDA_PP_PARAMS_Z(z, n, T, x)) : base(core(ZELDA_PP_PARAMS_Z(z, n, x))) \
    {}
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_OVERLOAD_LIMIT, ZELDA_OVERLOAD_ADAPTOR_CONSTRUCTOR, ~)
};


#define ZELDA_OVERLOAD_FUNCTION(z, n, data) \
template<ZELDA_PP_PARAMS_Z(z, n, class F)> \
typename overload_adaptor<ZELDA_PP_PARAMS_Z(z, n, F)>::type overload(ZELDA_PP_PARAMS_Z(z, n, F, x)) \
{ \
    return overload_adaptor<ZELDA_PP_PARAMS_Z(z, n, F)>(ZELDA_PP_PARAMS_Z(z, n, x)); \
}
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_OVERLOAD_LIMIT, ZELDA_OVERLOAD_FUNCTION, ~)
#endif



}

ZELDA_NULLARY_TR1_RESULT_OF_N(ZELDA_OVERLOAD_LIMIT, zelda::overload_adaptor)



#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/reveal.h>
#include <zelda/typeof.h>

namespace zelda { namespace overload_test {
struct int_class
{
    int operator()(int) const
    {
        return 1;
    }
};

struct foo
{};

struct foo_class
{
    foo operator()(foo) const
    {
        return foo();
    }
};

zelda::static_<zelda::overload_adaptor<int_class, foo_class> > fun = {};

typedef ZELDA_TYPEOF(reveal(fun)(1)) fun_type;

void test()
{
reveal(fun)(1);
}

static_assert(boost::is_same<int, ZELDA_TYPEOF(fun(1))>::value, "Failed overload");
static_assert(boost::is_same<foo, ZELDA_TYPEOF(fun(foo()))>::value, "Failed overload");
}}

#endif

#endif
