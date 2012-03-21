#ifndef ZELDA_RESULT_OF_H
#define ZELDA_RESULT_OF_H

#include "introspection.h"
#include "requires.h"
#include "typeof.h"

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

#if 0
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
struct is_callable : result_details::has_type<typename result_of<F>::type> {};

#endif

template<class F>
struct is_callable {};

template<class F, class... Args>
struct is_callable<F(Args...)>
{
    typedef char yes; 
    typedef long no; 
    template<class T> 
    struct selector {}; 

    template<class U> 
    static yes check( selector<ZELDA_TYPEOF( zelda::declval<U>()(zelda::declval<Args>()...) )> * ); 

    template<class U> 
    static no check(...); 
    
    static const bool value = sizeof(check<F>(0)) == sizeof(yes); 

    typedef boost::mpl::bool_<value> type; 
};

template<class F, class Enable = void>
struct result_of {};
//TODO: Use XTYPEOF
template<class F, class... Args>
struct result_of<F(Args...), ZELDA_CLASS_REQUIRES(is_callable<F(Args...)>)>
: boost::mpl::identity<ZELDA_TYPEOF(zelda::declval<F>()(zelda::declval<Args>()...))> {};

// template<class F>
// struct is_callable<F, ZELDA_CLASS_REQUIRES(result_details::has_enable<typename result_details::function_object<F>::type>)>
// : result_details::function_object<F>::type::template enable<F>::type {};

// template<class F>
// struct is_callable<F, ZELDA_CLASS_REQUIRES(result_details::has_type<typename result_of<F>::type>)>
// : result_details::has_type< typename result_details::function_object<F>::type::template result<F> >::type {};

}

#endif