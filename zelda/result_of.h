#ifndef ZELDA_RESULT_OF_H
#define ZELDA_RESULT_OF_H

#include "introspection.h"
#include "requires.h"

namespace zelda{


namespace result_details {
template<class F>
struct function_object {};

template<class F, class... Args>
struct function_object<F(Args...)> 
{
    typedef F type;
};
ZELDA_HAS_TEMPLATE(result);
ZELDA_HAS_TEMPLATE(enable);
ZELDA_HAS_TYPE(result_type);
ZELDA_HAS_TYPE(type);
}


template<class F, class Enable = void>
struct result_of {};

template<class F>
struct result_of<F, ZELDA_CLASS_REQUIRES(result_details::has_result<typename result_details::function_object<F>::type >)>
: result_details::function_object<F>::type::template result<F> {};

template<class F>
struct result_of<F, ZELDA_CLASS_REQUIRES(result_details::has_result_type<typename result_details::function_object<F>::type >)>
{
    typedef typename result_details::function_object<F>::type::result_type type;
};

template<class F, class Enable = void>
struct is_callable {};

template<class F>
struct is_callable<F, ZELDA_CLASS_REQUIRES(result_details::has_enable<typename result_details::function_object<F>::type>)>
: result_details::function_object<F>::type::template enable<F>::type {};

// template<class F>
// struct is_callable<F, ZELDA_CLASS_REQUIRES(result_details::has_result<typename result_details::function_object<F>::type>)>
// : result_details::has_type< typename result_details::function_object<F>::type::template result<F> >::type {};

template<class F>
struct test;

template<class F, class T>
struct test<F(T x)>
{
    typedef typeof(x+x) type;
};




}

#endif