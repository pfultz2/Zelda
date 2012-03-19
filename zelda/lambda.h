/* 
 * File:   lambda.h
 * Author: pfultz
 *
 * Created on December 8, 2011, 8:28 PM
 */

#ifndef LAMBDA_H
#define	LAMBDA_H

#include "pp.h"
#include "./variadic.h"


//#define ZELDA_HAS_CLOSURE_BLOCKS


#ifdef ZELDA_HAS_LAMBDA
#define ZELDA_LAMBDA(...) [&](__VA_ARGS__) DETAIL_ZELDA_LAMBDA
#define DETAIL_ZELDA_LAMBDA(...) { return __VA_ARGS__; }
#endif

#ifdef ZELDA_HAS_CLOSURE_BLOCKS
#define ZELDA_LAMBDA(...) ^(__VA_ARGS__) DETAIL_ZELDA_LAMBDA
#define DETAIL_ZELDA_LAMBDA(...) { return __VA_ARGS__; }
#endif

#if 0
#define ZELDA_DETAIL_LAMBDA_RESULT_DECLARE(...) \
struct zelda_result_type \
{ \
PP_ARGS_STATEMENTS(static, __VA_ARGS__) \
struct result_type;\
}; \

#define ZELDA_DETAIL_LAMBDA_RESULT_DEDUCE(...) \
struct zelda_result_type::result_type \
{ \
typedef typeof(__VA_ARGS__) type; \
}; \


#define ZELDA_LAMBDA(...) \
({ \
ZELDA_DETAIL_LAMBDA_RESULT_DECLARE(__VA_ARGS__) \
typename zelda_result_type::result_type::type \
zelda_lambda(__VA_ARGS__) DETAIL_ZELDA_LAMBDA \

#define DETAIL_ZELDA_LAMBDA(...) \
{ \
return __VA_ARGS__; \
} \
ZELDA_DETAIL_LAMBDA_RESULT_DEDUCE(__VA_ARGS__) \
})

#endif

namespace zelda {

namespace lambda {

namespace details {
template<class F>
struct fun_adaptor
{    
};


#define ZELDA_LAMBDA_FUN_ADAPTOR(pack, unpack) \
template<class Result ZELDA_VARIADIC_COMMA(pack(class Arg))> \
struct fun_adaptor<Result (pack(Arg))> \
{ \
Result(*ptr)(pack(Arg)); \
fun_adaptor(Result(*x)(pack(Arg))) : ptr(x) \
{} \
typedef Result result_type; \
Result operator()(pack(Arg, x)) const \
{ \
return ptr(pack(x)); \
} \
}; \
template<class Result ZELDA_VARIADIC_COMMA(pack(class Arg))> \
fun_adaptor<Result (pack(Arg))> \
make_fun_adaptor(Result(*ptr)(pack(Arg))) \
{ \
return fun_adaptor<Result (pack(Arg))>(ptr); \
}

ZELDA_VARIADIC(ZELDA_LAMBDA_FUN_ADAPTOR)

}

template <class _Arg, class _Result>
class pointer_to_unary_function //: public unary_function<_Arg, _Result>
{
protected:
    _Result(*_M_ptr)(_Arg);
public:

    pointer_to_unary_function()
    {
    }

    explicit
    pointer_to_unary_function(_Result(*__x)(_Arg))
    : _M_ptr(__x)
    {
    }

    _Result
    operator()(_Arg __x) const
    {
        return _M_ptr(__x);
    }
};

template <class _Arg, class _Result>
inline pointer_to_unary_function<_Arg, _Result>
ptr_fun(_Result(*__x)(_Arg))
{
    return pointer_to_unary_function<_Arg, _Result > (__x);
}

}

}

#endif	/* LAMBDA_H */

