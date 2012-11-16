/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    builder.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_BUILDER_H
#define ZELDA_GUARD_FUNCTION_BUILDER_H

#include <zelda/typeof.h>
#include <zelda/forward.h>
#include <zelda/function/poly.h>
#include <zelda/function/pipable.h>

#ifndef ZELDA_PP_STRING_if
#define ZELDA_PP_STRING_if (if)
#endif

#ifndef ZELDA_PP_STRING_else
#define ZELDA_PP_STRING_else (else)
#endif

#ifndef ZELDA_PP_COMPARE_else
#define ZELDA_PP_COMPARE_else(x) x
#endif

#define ZELDA_FUNCTION_PARAM(x) BOOST_PP_CAT(ZELDA_FUNCTION_PARAM_, x)
#define ZELDA_FUNCTION_PARAM_const (const &)
#define ZELDA_FUNCTION_PARAM_mutable (&)
#define ZELDA_FUNCTION_PARAM_auto (ZELDA_FORWARD_REF())

#define ZELDA_FUNCTION_PARAM_EAT_PAREN(x, raw) BOOST_PP_IIF(ZELDA_PP_IS_PAREN(x), ZELDA_PP_EAT x, raw)
#define ZELDA_FUNCTION_PARAM_REMOVE_PAREN(x, raw) BOOST_PP_IIF(ZELDA_PP_IS_PAREN(x), ZELDA_PP_EAT x, raw)

#define ZELDA_FUNCTION_EAT_PARAM(x) ZELDA_FUNCTION_PARAM_EAT_PAREN(ZELDA_FUNCTION_PARAM(x), x)
#define ZELDA_FUNCTION_REMOVE_PARAM(x) ZELDA_FUNCTION_PARAM_REMOVE_PAREN(ZELDA_FUNCTION_PARAM(x), x)

#define ZELDA_FUNCTION_PARAMS_EAT(seq) BOOST_PP_CAT(ZELDA_FUNCTION_PARAMS_EAT_1 seq, _END)
#define ZELDA_FUNCTION_PARAMS_EAT_1(x) (ZELDA_FUNCTION_EAT_PARAM(x)) ZELDA_FUNCTION_PARAMS_EAT_2
#define ZELDA_FUNCTION_PARAMS_EAT_2(x) (ZELDA_FUNCTION_EAT_PARAM(x)) ZELDA_FUNCTION_PARAMS_EAT_1
#define ZELDA_FUNCTION_PARAMS_EAT_1_END
#define ZELDA_FUNCTION_PARAMS_EAT_2_END


#define ZELDA_FUNCTION_PARAMS_REMOVE(seq) BOOST_PP_CAT(ZELDA_FUNCTION_PARAMS_EAT_1 seq, _END)
#define ZELDA_FUNCTION_PARAMS_REMOVE_1(x) (ZELDA_FUNCTION_REMOVE_PARAM(x)) ZELDA_FUNCTION_PARAMS_REMOVE_2
#define ZELDA_FUNCTION_PARAMS_REMOVE_2(x) (ZELDA_FUNCTION_REMOVE_PARAM(x)) ZELDA_FUNCTION_PARAMS_REMOVE_1
#define ZELDA_FUNCTION_PARAMS_REMOVE_1_END
#define ZELDA_FUNCTION_PARAMS_REMOVE_2_END


#define ZELDA_FUNCTION_PARAMS_PROCESS(seq) BOOST_PP_CAT(ZELDA_FUNCTION_PARAMS_PROCESS_1 seq, _END)
#define ZELDA_FUNCTION_PARAMS_PROCESS_1(x) (BOOST_PP_CAT(Zelda_T, ZELDA_FUNCTION_EAT_PARAM(x) ZELDA_FUNCTION_REMOVE_PARAM(x))) ZELDA_FUNCTION_PARAMS_PROCESS_2
#define ZELDA_FUNCTION_PARAMS_PROCESS_2(x) (BOOST_PP_CAT(Zelda_T, ZELDA_FUNCTION_EAT_PARAM(x) ZELDA_FUNCTION_REMOVE_PARAM(x))) ZELDA_FUNCTION_PARAMS_PROCESS_1
#define ZELDA_FUNCTION_PARAMS_PROCESS_1_END
#define ZELDA_FUNCTION_PARAMS_PROCESS_2_END

#define ZELDA_FUNCTION_PARAMS_STATIC_GEN(seq) BOOST_PP_CAT(ZELDA_FUNCTION_PARAMS_STATIC_GEN_1 seq, _END)
#define ZELDA_FUNCTION_PARAMS_STATIC_GEN_1(x) static x; ZELDA_FUNCTION_PARAMS_STATIC_GEN_2
#define ZELDA_FUNCTION_PARAMS_STATIC_GEN_2(x) static x; ZELDA_FUNCTION_PARAMS_STATIC_GEN_1
#define ZELDA_FUNCTION_PARAMS_STATIC_GEN_1_END
#define ZELDA_FUNCTION_PARAMS_STATIC_GEN_2_END


#define ZELDA_FUNCTION_PREFIX_CLASS(seq) BOOST_PP_CAT(ZELDA_FUNCTION_PREFIX_CLASS_1 seq, _END)
#define ZELDA_FUNCTION_PREFIX_CLASS_1(x) (class x)  ZELDA_FUNCTION_PREFIX_CLASS_2
#define ZELDA_FUNCTION_PREFIX_CLASS_2(x) (class x)  ZELDA_FUNCTION_PREFIX_CLASS_1
#define ZELDA_FUNCTION_PREFIX_CLASS_1_END
#define ZELDA_FUNCTION_PREFIX_CLASS_2_END

#define ZELDA_FUNCTION_PREFIX_ZELDA_T(seq) BOOST_PP_CAT(ZELDA_FUNCTION_PREFIX_ZELDA_T_1 seq, _END)
#define ZELDA_FUNCTION_PREFIX_ZELDA_T_1(x) (Zelda_T ## x)  ZELDA_FUNCTION_PREFIX_ZELDA_T_2
#define ZELDA_FUNCTION_PREFIX_ZELDA_T_2(x) (Zelda_T ## x)  ZELDA_FUNCTION_PREFIX_ZELDA_T_1
#define ZELDA_FUNCTION_PREFIX_ZELDA_T_1_END
#define ZELDA_FUNCTION_PREFIX_ZELDA_T_2_END

// ZELDA_DETAIL_FUNCTION_CLASS_K
// params = x, y, z, ...
// function_params = Zelda_Tx x, Zelda_Ty y, Zelda_Tz z, ...
// template_params = Zelda_Tx, Zelda_Ty, Zelda_Tz, ...
//
#define ZELDA_DETAIL_FUNCTION_CLASS_K(name, n, params, function_params, template_params, reqs, body) \
struct name \
{ \
    template<class Zelda_X> \
    struct enable; \
    template<class Zelda_X, BOOST_PP_SEQ_ENUM(ZELDA_FUNCTION_PREFIX_CLASS(params))> \
    struct enable<Zelda_X(BOOST_PP_SEQ_ENUM(params))> \
    { \
        typedef typename ZELDA_DETAIL_REQUIRES_CLAUSE(ZELDA_PP_REM reqs)::type type;\
    }; \
    \
    template<class Zelda_X, class Zelda_Enable = void> \
    struct result; \
    \
    template<class Zelda_X, BOOST_PP_SEQ_ENUM(ZELDA_FUNCTION_PREFIX_CLASS(template_params))> \
    struct result<Zelda_X(BOOST_PP_SEQ_ENUM(template_params)), typename enable<Zelda_X(BOOST_PP_SEQ_ENUM(template_params))>::type> \
    { \
        ZELDA_FUNCTION_PARAMS_STATIC_GEN(function_params) \
        typedef ZELDA_XTYPEOF_TPL(body) type; \
        \
    }; \
    \
    template<BOOST_PP_SEQ_ENUM(ZELDA_FUNCTION_PREFIX_CLASS(template_params))> \
    typename result<void(BOOST_PP_SEQ_ENUM(function_params))>::type \
    operator()(BOOST_PP_SEQ_ENUM(function_params)) \
    { return body; } \
};

// TODO: Add support for bodies without double parenthesis, so you can write:
// ZELDA_FUNCTIONS_CLASS((foo)(x, y) x + y)
#define ZELDA_DETAIL_FUNCTION_CLASS(name, n, params, body) \
BOOST_PP_CAT(ZELDA_DETAIL_FUNCTION_CLASS_, BOOST_PP_SEQ_SIZE(body))\
(name, n, ZELDA_FUNCTION_PARAMS_EAT(params), ZELDA_FUNCTION_PARAMS_PROCESS(params), ZELDA_FUNCTION_PREFIX_ZELDA_T(params), body)
#define ZELDA_DETAIL_FUNCTION_CLASS_1(name, n, params, function_params, template_params, body) \
ZELDA_DETAIL_FUNCTION_CLASS_K(name, n, params, function_params, template_params, (boost::mpl::bool_<true>), BOOST_PP_SEQ_HEAD(body))
#define ZELDA_DETAIL_FUNCTION_CLASS_3(name, n, params, function_params, template_params, body) \
ZELDA_DETAIL_FUNCTION_CLASS_K(name, n, params, function_params, template_params, BOOST_PP_SEQ_ELEM(1, body), BOOST_PP_SEQ_ELEM(2, body))

#define ZELDA_DETAIL_FUNCTION_CLASS_EACH(r, data, i, x) ZELDA_DETAIL_FUNCTION_CLASS_INVOKE((i, ZELDA_PP_REM data, x))
// TODO: Msvc invoke
#define ZELDA_DETAIL_FUNCTION_CLASS_INVOKE(x) ZELDA_DETAIL_FUNCTION_CLASS_OP x
#define ZELDA_DETAIL_FUNCTION_CLASS_OP(i, name, n, params, body) \
ZELDA_DETAIL_FUNCTION_CLASS(ZELDA_DETAIL_FUNCTION_CLASS_NAME(i, name), n, params, body)

#define ZELDA_DETAIL_FUNCTION_CLASS_NAME(i, name) BOOST_PP_CAT(zelda_private_function_class_, BOOST_PP_CAT(i, name))
#define ZELDA_DETAIL_FUNCTION_CLASS_ALL_NAME(n, name) BOOST_PP_ENUM(n, ZELDA_DETAIL_FUNCTION_CLASS_ALL_NAME_EACH, name)
#define ZELDA_DETAIL_FUNCTION_CLASS_ALL_NAME_EACH(z, i, name) ZELDA_DETAIL_FUNCTION_CLASS_NAME(i, name)

#define ZELDA_DETAIL_FUNCTION_CLASS_ALL(name, params, bodies) \
BOOST_PP_SEQ_FOR_EACH_I(ZELDA_DETAIL_FUNCTION_CLASS_EACH, \
    (ZELDA_PP_REM name, ZELDA_PP_NARGS params, ZELDA_PP_ARGS_TO_SEQ params), \
    bodies) \
    typedef zelda::poly_adaptor<ZELDA_DETAIL_FUNCTION_CLASS_ALL_NAME(BOOST_PP_SEQ_SIZE(bodies), ZELDA_PP_REM name) > ZELDA_PP_REM name; 

#define ZELDA_DETAIL_FUNCTION_CLASS_P(s, data, x) ZELDA_PP_EQUAL(x, else)

#define ZELDA_DETAIL_FUNCTION_CLASS_TRANSFORM(seq) \
ZELDA_DETAIL_FUNCTION_CLASS_ALL(BOOST_PP_SEQ_ELEM(0, seq), BOOST_PP_SEQ_ELEM(1, seq), ZELDA_PP_SEQ_SPLIT(BOOST_PP_SEQ_REST_N(2, seq), ZELDA_DETAIL_FUNCTION_CLASS_P, data))


#define ZELDA_DETAIL_FUNCTION_CLASS_X(...) __VA_ARGS__

// class
#define ZELDA_FUNCTION_CLASS(x) ZELDA_DETAIL_FUNCTION_CLASS_TRANSFORM(ZELDA_PP_STRING_TO_SEQ(x))

// function object
#define ZELDA_DETAIL_FUNCTION_OBJECT(name, x) ZELDA_FUNCTION_CLASS((BOOST_PP_CAT(zelda_class_, name))x) zelda::static_<BOOST_PP_CAT(zelda_class_, name)> name = {};
#define ZELDA_FUNCTION_OBJECT(x) ZELDA_DETAIL_FUNCTION_OBJECT(BOOST_PP_SEQ_HEAD(x), BOOST_PP_SEQ_TAIL(x))

// pipe
#define ZELDA_DETAIL_FUNCTION_PIPE_OBJECT(name, x) ZELDA_FUNCTION_CLASS((BOOST_PP_CAT(zelda_class_, name))x) zelda::static_<zelda::pipable_adaptor<BOOST_PP_CAT(zelda_class_, name) > > name = {};
#define ZELDA_FUNCTION_PIPE_OBJECT(x) ZELDA_DETAIL_FUNCTION_PIPE_OBJECT(BOOST_PP_SEQ_HEAD(x), BOOST_PP_SEQ_TAIL(x))

#define ZELDA_FUNCTION_KEYWORD_class (ZELDA_FUNCTION_CLASS)

#define ZELDA_FUNCTION_KEYWORD_pipe (ZELDA_FUNCTION_PIPE_OBJECT)

// Register with ZLANG
#define ZLANG_zelda_function (ZELDA_FUNCTION)

#define ZELDA_FUNCTION_INVOKE(m, args) m args
#define ZELDA_FUNCTION_II(x) ZELDA_FUNCTION_INVOKE(BOOST_PP_SEQ_HEAD(x), (BOOST_PP_SEQ_TAIL(x)))
#define ZELDA_FUNCTION_I(x) ZELDA_FUNCTION_II(BOOST_PP_CAT(ZELDA_FUNCTION_KEYWORD_, x))
#define ZELDA_FUNCTION(x) BOOST_PP_IIF(BOOST_PP_IS_PAREN(x), ZELDA_FUNCTION_OBJECT, ZELDA_FUNCTION_I)(x)



#if 0
namespace zelda {

namespace zelda_function_test {

// class
ZELDA_FUNCTION_CLASS((foo_class)(x, y) if(boost::is_integral<x>, boost::is_integral<y>)(x + y) else (1))
// object
ZELDA_FUNCTION_OBJECT((foo)(x, y) if(boost::is_integral<x>, boost::is_integral<y>)(x + y) else (1))
// pipe
ZELDA_FUNCTION_PIPE_OBJECT((foo_pipe)(x, y) if(boost::is_integral<x>, boost::is_integral<y>)(x + y) else (1))

}

}
#endif

#endif
