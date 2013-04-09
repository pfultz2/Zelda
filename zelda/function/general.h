/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    general.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_GENERAL_H
#define ZELDA_GUARD_FUNCTION_GENERAL_H

#include <zelda/function/lazy.h>
#include <zelda/function/perfect.h>
#include <zelda/function/variadic.h>
#include <zelda/function/conditional.h>
#include <zelda/function/invoke.h>


#include <boost/phoenix/function/function.hpp>
#include <boost/phoenix/core/is_actor.hpp>
#include <zelda/mpl/any_of.h>
#include <boost/mpl/find_if.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/quote.hpp>
#include <boost/mpl/begin_end.hpp>

namespace zelda { 

namespace detail {

template<class F>
struct general_adaptor_base : function_adaptor_base<F>
{
    typedef void zelda_is_callable_by_result_tag;
    general_adaptor_base() {};

    template<class X>
    general_adaptor_base(X x) : function_adaptor_base<F>(x)
    {};

    template<class X, class Enable = void>
    struct result;

    template<class X, class T>
    struct result<X(T), ZELDA_CLASS_REQUIRES(exclude zelda::mpl::any_of<typename boost::decay<T>::type, boost::phoenix::is_actor<boost::mpl::_1> >)>
    : invoke_result<F, const typename boost::decay<T>::type&> 
    {}; 

    template<class T>
    typename result<F(const T&)>::type operator()(const T & x) const
    {
        return invoke(this->get_function(), x);
    }
};
}

template<class F>
struct general_adaptor 
: conditional_adaptor<variadic_adaptor<detail::general_adaptor_base<F> >, lazy_adaptor<F> >
{
    general_adaptor() {}

    template<class X>
    general_adaptor(X x) : conditional_adaptor<detail::general_adaptor_base<F>, lazy_adaptor<F> >(x, x)
    {}
};

template<class F>
general_adaptor<F> general(F f)
{
    return general_adaptor<F>(f);
}

}

ZELDA_NULLARY_TR1_RESULT_OF_N(1, zelda::general_adaptor)


#ifdef ZELDA_TEST
#include <zelda/test.h>
#include <boost/phoenix/core/argument.hpp>

zelda::static_<zelda::general_adaptor<binary_class> > binary_general = {};

ZELDA_TEST_CASE(general_test)
{
    boost::phoenix::expression::argument<1>::type const arg1 = {};
    boost::phoenix::expression::argument<2>::type const arg2 = {};
    ZELDA_TEST_EQUAL(3, binary_general(1, 2));
    ZELDA_TEST_EQUAL(3, binary_general(arg1, arg2)(1, 2));
    ZELDA_TEST_EQUAL(3, binary_general(arg1, 2)(1));
}
#endif

#endif
