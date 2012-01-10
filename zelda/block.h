/* 
 * File:   block.h
 * Author: pfultz
 *
 * Created on November 23, 2011, 10:27 AM
 */

#ifndef LAMBDA_BLOCK_H
#define	LAMBDA_BLOCK_H

#include "pp.h"
#include <boost/mpl/at.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>

namespace zelda {

template<class T>
struct identity
{
    typedef T type;
};

namespace details {
template<class Function, int N>
struct lambda_holder
{
    Function f;
    lambda_holder(Function f) : f(f) {};
    static const int value = N;
};

template<class N>
struct params_n
{
    static const int value = N;
    
    template<class Function>
    lambda_holder<Function, N> operator&(Function f)
    {
        return lambda_holder<Function, N>(f);
    }
};
}


template<class Derived, class L>
struct lambda_block
{
    template<int N, int Size = 0>
    struct lambda_param
    {
        typedef typename boost::function_types::parameter_types<L>::type sequence;
        typedef typename boost::mpl::at_c<sequence, N>::type type;
    };
    
    template<class Lambda>
    typename boost::function_types::result_type<L>::type
    operator^(Lambda y)
    {
        static_cast<Derived&>(*this)(y.f);
    }
};

template<class Derived, class Result>
struct multi_lambda_block
{   
    template<class Lambda>
    Result
    operator^(Lambda y)
    {
        static_cast<Derived&>(*this)(y.f, boost::mpl::int_<Lambda::value>());
    }
};

#define DETAIL_LAMBDA_PARAMETERS_EACH(r, data, i, elem) typename identity<decltype(BOOST_PP_SEQ_ELEM(0, data))>::type::lambda_param<i, BOOST_PP_SEQ_ELEM(1, data)>::type elem
#define DETAIL_LAMBDA_PARAMETERS(...) PP_ARGS_FOREACH_I(DETAIL_LAMBDA_PARAMETERS_EACH, \
(PP_OPTIONAL_FIRST(__VA_ARGS__))(PP_NARGS(__VA_ARGS__)), \
PP_OPTIONAL_TAIL(__VA_ARGS__))
#define DETAIL_LAMBDA_PARAMETERS_EMPTY(...) 
#define ZELDA_LAMBDA_CALL(...) PP_OPTIONAL_FIRST(__VA_ARGS__) ^\
zelda::details::params_n<PP_NARGS(__VA_ARGS__)> &\
[=] ( PP_OPTIONAL_IF(DETAIL_LAMBDA_PARAMETERS, DETAIL_LAMBDA_PARAMETERS_EMPTY, __VA_ARGS__) )

//TODO: Check if it is a lambda block function

}

#endif	/* LAMBDA_H */

