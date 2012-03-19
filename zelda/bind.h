/* 
 * File:   bind.h
 * Author: paul
 *
 * Created on March 3, 2012, 3:07 AM
 */

#ifndef BIND_H
#define	BIND_H

#include "variadic.h"

namespace zelda {
    
template<class F, ZELDA_VARIADIC_PACK(T)>
struct bind_front_function
{
    F f;
    ZELDA_VARIADIC_UNPACK_TUPLE(T) args;

    template<class Sig>
    struct result
    {
    };

    template<class F, class... Args>
    struct result<F(Args...)>
    {
        typedef typename boost::fusion::result_of::join<ZELDA_VARIADIC_UNPACK_TUPLE(T), boost::fusion::vector<Args...> >::type seq_t;
        typedef typename boost::fusion::result_of::invoke_function_object<Function, seq_t>::type type;
    };
};


}

#endif	/* BIND_H */

