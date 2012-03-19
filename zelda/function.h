/* 
 * File:   function.h
 * Author: paul
 *
 * Created on February 29, 2012, 12:17 AM
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

#include "requires.h"

namespace zelda {

struct base_feature
{
    struct empty_base
    {
    };
    template<class Base= empty_base>
    struct base_base
    {
    };
    template<class T, class Base = empty_base>
    struct base 
    {
        typedef base_base<Base> type;
    };
    template<class Sig>
    struct result
    {
    };
    template<class T>
    struct enable : boost::mpl::false_
    {
    };

};

template<class Function, class Feature = base_feature>
struct function_adaptor : Feature::base<function_adaptor<Function, Feature> >::type
{
    Function f;

    template<class... T>
    function_adaptor(T &&... x) : f(forward<T>(x)...)
    {
    }

    template<class Sig>
    struct result
    {
    };

    template<class... T>
    ZELDA_FUNCTION_REQUIRES(ZELDA_EXCLUDE(Feature::enable<Function(T&&...)>))
    (typename result_of<Function(T&&...)>::type)
    operator()(T && ... x)
    {
        f(forward<T>(x)...);
    }


    template<class... T>
    ZELDA_FUNCTION_REQUIRES(Feature::enable<Function(T&&...)>)
    (typename Feature::result<Function(T&&...)>::type)
    operator()(T && ... x)
    {
        Feature()(f, forward<T>(x)...);
    }
};


}

#endif	/* FUNCTION_H */

