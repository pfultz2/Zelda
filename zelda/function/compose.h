/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    compose.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_COMPOSE_H
#define ZELDA_GUARD_FUNCTION_COMPOSE_H

#include <zelda/function/variadic.h>
#include <zelda/function/fuse.h>
#include <boost/fusion/algorithm/iteration/fold.hpp>
#include <zelda/function/detail/nullary_tr1_result_of.h>

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
#include <tuple>
#include <boost/fusion/adapted/std_tuple.hpp>
#else
#include <boost/fusion/container/vector.hpp>
#include <boost/fusion/container/generation/make_vector.hpp>
#endif

#ifndef ZELDA_COMPOSE_LIMIT
#define ZELDA_COMPOSE_LIMIT 10
#endif

namespace zelda { namespace detail {

struct compose_fold
{
    template<class>
    struct result;

    template<class X, class State, class F>
    struct result<X(State, F)>
    : zelda::result_of<typename boost::decay<F>::type(State)>
    {};

    template<class State, class F>
    typename result<compose_fold(State, F)>::type 
    operator()(State s, F f) const
    {
        return f(s);
    }
};
template<class Sequence>
struct compose_base
{

    typedef typename boost::decay<typename boost::fusion::result_of::at_c<Sequence, 0>::type>::type first;
    Sequence seq;

    compose_base() {}

    compose_base(Sequence seq) : seq(seq)
    {}

    template<class, class Enable = void>
    struct result;

    template<class X, class T>
    struct result<X(T), ZELDA_CLASS_REQUIRES(is_callable<first(T)>)>
    : boost::fusion::result_of::fold<Sequence, T, compose_fold>
    {};

    template<class T>
    typename result<compose_base(T)>::type 
    operator()(const T& x) const
    {
        return boost::fusion::fold(seq, x, compose_fold());
    }
};

}

#ifndef ZELDA_NO_VARIADIC_TEMPLATES
template<class F, class... Fs>
struct compose_adaptor
: zelda::variadic_adaptor<detail::compose_base< std::tuple<zelda::fuse_adaptor<F>, Fs...> > >
{
    typedef zelda::variadic_adaptor<detail::compose_base<std::tuple<zelda::fuse_adaptor<F>, Fs...> > > base;
    compose_adaptor() {}

    compose_adaptor(F f, Fs... fs) : base(std::make_tuple(zelda::fuse_adaptor<F>(f), fs...))
    {}
};

template<class... Fs>
compose_adaptor<Fs...> compose(Fs... fs)
{
    return compose_adaptor<Fs...>(fs...);
}
#else
template<class T = void, ZELDA_PP_PARAMS_Z(1, ZELDA_COMPOSE_LIMIT, class T, = void BOOST_PP_INTERCEPT)>
struct compose_adaptor;
#define ZELDA_COMPOSE_ADAPTOR(z, n, data) \
template<class F, ZELDA_PP_PARAMS_Z(z, n, class Fs)> \
struct compose_adaptor<F, ZELDA_PP_PARAMS_Z(z, n, Fs)> \
: zelda::variadic_adaptor<detail::compose_base< boost::fusion::vector<zelda::fuse_adaptor<F>, ZELDA_PP_PARAMS_Z(z, n, Fs)> > > \
{ \
    typedef zelda::variadic_adaptor<detail::compose_base<boost::fusion::vector<zelda::fuse_adaptor<F>, ZELDA_PP_PARAMS_Z(z, n, Fs)> > > base; \
    compose_adaptor() {} \
 \
    compose_adaptor(F f, ZELDA_PP_PARAMS_Z(z, n, Fs, fs)) : base(boost::fusion::make_vector(zelda::fuse_adaptor<F>(f), ZELDA_PP_PARAMS_Z(z, n, fs))) \
    {} \
};
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_COMPOSE_LIMIT, ZELDA_COMPOSE_ADAPTOR, ~)

#define ZELDA_COMPOSE_FUN(z, n, data) \
template<ZELDA_PP_PARAMS_Z(z, n, class Fs)> \
compose_adaptor<ZELDA_PP_PARAMS_Z(z, n, Fs)> compose(ZELDA_PP_PARAMS_Z(z, n, Fs, fs)) \
{ \
    return compose_adaptor<ZELDA_PP_PARAMS_Z(z, n, Fs)>(ZELDA_PP_PARAMS_Z(z, n, fs)); \
}
BOOST_PP_REPEAT_FROM_TO_1(1, ZELDA_COMPOSE_LIMIT, ZELDA_COMPOSE_FUN, ~)
#endif

}

ZELDA_NULLARY_TR1_RESULT_OF_N(ZELDA_COMPOSE_LIMIT, zelda::compose_adaptor)


#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <zelda/function/static.h>
#include <zelda/function/reveal.h>
#include <zelda/function/placeholders.h>
#include <zelda/function/lazy.h>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/statement/if.hpp>

ZELDA_TEST_CASE(compose_test)
{
    using zelda::ph::_1;
    // int r = (_1 + 1)()(3);
    int r = zelda::compose(_1 + 1, _1 - 1, _1 + 1)(3);
    ZELDA_TEST_EQUAL(r, 4);
}

#endif

#endif
