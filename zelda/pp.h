/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    pp.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_PP_H
#define ZELDA_GUARD_PP_H

//#define BOOST_PP_VARIADICS 1
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include <boost/preprocessor/facilities/overload.hpp>
#include "tokens.h"

//Utility

// Various utilities
#define ZELDA_PP_EAT(...)
#define ZELDA_PP_REM(...) __VA_ARGS__
#define ZELDA_PP_EXPAND(...) __VA_ARGS__
#define ZELDA_PP_EMPTY(...)
#define ZELDA_PP_TRUE(...) 1
#define ZELDA_PP_FALSE(...) 0

 /**
 * ZELDA_PP_NARGS returns the number of args in __VA_ARGS__
 */
#define ZELDA_PP_NARGS(...) \
         ZELDA_PP_DETAIL_NARG((__VA_ARGS__,ZELDA_PP_DETAIL_RSEQ_N()))
#define ZELDA_PP_DETAIL_NARG(args) \
         ZELDA_PP_DETAIL_ARG_N args
#define ZELDA_PP_DETAIL_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define ZELDA_PP_DETAIL_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 


//
// ZELDA_PP_IS_PAREN is used to detect if the first token is a parenthesis.
// It expands to 1 if it is, otherwise it expands to 0.
//
#define ZELDA_PP_IS_PAREN(x) ZELDA_PP_IS_PAREN_CHECK(ZELDA_PP_IS_PAREN_PROBE x)
#define ZELDA_PP_IS_PAREN_CHECK(...) ZELDA_PP_IS_PAREN_CHECK_N(__VA_ARGS__,0)
#define ZELDA_PP_IS_PAREN_PROBE(...) ~, 1,
#ifndef _MSC_VER
#define ZELDA_PP_IS_PAREN_CHECK_N(x, n, ...) n
#else
// MSVC workarounds
#define ZELDA_PP_IS_PAREN_CHECK_RES(x) x
#define ZELDA_PP_IS_PAREN_CHECK_II(x, n, ...) n
#define ZELDA_PP_IS_PAREN_CHECK_I(x) ZELDA_PP_IS_PAREN_CHECK_RES(ZELDA_PP_IS_PAREN_CHECK_II x)
#define ZELDA_PP_IS_PAREN_CHECK_N(...) ZELDA_PP_IS_PAREN_CHECK_I((__VA_ARGS__))
#endif

//
// ZELDA_PP_IS_EMPTY will expands to 1 if the parameter is empty, otherwise
// it expands to 0. This will work even if the parameter given is a set
// of parenthesis.
//
#define ZELDA_PP_IS_EMPTY(x) BOOST_PP_CAT(ZELDA_PP_DETAIL_IS_EMPTY_, ZELDA_PP_IS_PAREN(x))(x)
#define ZELDA_PP_DETAIL_IS_EMPTY_0(x) BOOST_PP_IS_EMPTY(x)
#define ZELDA_PP_DETAIL_IS_EMPTY_1(x) 0 

//
// ZELDA_PP_HEAD retrieves the first element of a sequence.
// Example:
//
//     ZELDA_PP_HEAD((1)(2)(3)) // Expands to (1)
//
#define ZELDA_PP_HEAD(x) ZELDA_PP_PICK_HEAD(ZELDA_PP_MARK x)
#define ZELDA_PP_MARK(...) (__VA_ARGS__), 
#define ZELDA_PP_PICK_HEAD(...) ZELDA_PP_PICK_HEAD_I(__VA_ARGS__,)
#ifndef _MSC_VER
#define ZELDA_PP_PICK_HEAD_I(x, ...) x
#else
// MSVC workarounds
#define ZELDA_PP_PICK_HEAD_III(x, ...) x
#define ZELDA_PP_PICK_HEAD_II(x) ZELDA_PP_PICK_HEAD_III x
#define ZELDA_PP_PICK_HEAD_I(...) ZELDA_PP_PICK_HEAD_II((__VA_ARGS__))
#endif


//
// ZELDA_PP_TAIL retrieves the tail of a sequence.
// Example:
//
//     ZELDA_PP_TAIL((1)(2)(3)) // Expands to (2)(3)
//
#define ZELDA_PP_TAIL(x) ZELDA_PP_EAT x



//
// ZELDA_PP_STRING_TOKEN
//
#define ZELDA_PP_STRING_TOKEN(x) BOOST_PP_IIF(ZELDA_PP_IS_PAREN(string), ZELDA_PP_STRING_TOKEN_PAREN, ZELDA_PP_STRING_TOKEN_KEYWORD)(x)
#define ZELDA_PP_STRING_TOKEN_KEYWORD(x) ZELDA_PP_STRING_TOKEN_KEYWORD_CHECK(BOOST_PP_CAT(ZELDA_PP_STRING_, x), x)
#define ZELDA_PP_STRING_TOKEN_KEYWORD_CHECK(tokened, raw) BOOST_PP_IIF(ZELDA_PP_IS_PAREN(tokened), tokened, (raw))
#define ZELDA_PP_STRING_TOKEN_PAREN(x) (ZELDA_PP_HEAD(x)) ZELDA_PP_TAIL(x) 

//
// ZELDA_PP_STRING_TO_SEQ converts a string into a preprocessor sequence
// 
#define ZELDA_PP_STRING_TO_SEQ(x) ZELDA_PP_STRING_TO_SEQ_WHILE_M \
( \
    BOOST_PP_WHILE(ZELDA_PP_STRING_TO_SEQ_WHILE_P, ZELDA_PP_STRING_TO_SEQ_WHILE_O, (,x)) \
)

#define ZELDA_PP_STRING_TO_SEQ_WHILE_P(r, state) ZELDA_PP_STRING_TO_SEQ_P state
#define ZELDA_PP_STRING_TO_SEQ_WHILE_O(r, state) ZELDA_PP_STRING_TO_SEQ_O state
#define ZELDA_PP_STRING_TO_SEQ_WHILE_M(state) ZELDA_PP_STRING_TO_SEQ_M state

#define ZELDA_PP_STRING_TO_SEQ_P(prev, tail) BOOST_PP_NOT(ZELDA_PP_IS_EMPTY(tail))
#define ZELDA_PP_STRING_TO_SEQ_O(prev, tail)  \
   ZELDA_PP_STRING_TO_SEQ_REPLACE(prev, ZELDA_PP_STRING_TOKEN(tail))

#define ZELDA_PP_STRING_TO_SEQ_REPLACE(prev, tail) \
    (prev ZELDA_PP_HEAD(tail), ZELDA_PP_TAIL(tail))

#define ZELDA_PP_STRING_TO_SEQ_M(prev, tail) prev

//
// ZELDA_PP_SEQ_TO_STRING outputs the sequence
//
#define ZELDA_PP_SEQ_TO_STRING(seq) BOOST_PP_SEQ_FOR_EACH(ZELDA_PP_SEQ_TO_STRING_EACH, ~, seq) 
#define ZELDA_PP_SEQ_TO_STRING_EACH(r, data, x) x 

//
// ZELDA_PP_EQUAL
//
#define ZELDA_PP_IS_COMPARABLE(x) BOOST_PP_IIF(ZELDA_PP_IS_PAREN(x), ZELDA_PP_FALSE, ZELDA_PP_PRIMITIVE_IS_COMPARABLE)(x)

#define ZELDA_PP_PRIMITIVE_IS_COMPARABLE(x) ZELDA_PP_IS_PAREN( ZELDA_PP_COMPARE_ ## x (()) )

#define ZELDA_PP_PRIMITIVE_COMPARE(x, y) ZELDA_PP_IS_PAREN \
( \
ZELDA_PP_COMPARE_ ## x ( ZELDA_PP_COMPARE_ ## y ZELDA_PP_EMPTY()(()) ) \
)

#define ZELDA_PP_NOT_EQUAL(x, y) \
BOOST_PP_IIF(BOOST_PP_BITAND( ZELDA_PP_PRIMITIVE_IS_COMPARABLE(x), ZELDA_PP_PRIMITIVE_IS_COMPARABLE(y) )), \
   ZELDA_PP_PRIMITIVE_COMPARE, \
   ZELDA_PP_TRUE \
)(x, y)

#define ZELDA_PP_EQUAL(x, y) BOOST_PP_COMPL(ZELDA_PP_NOT_EQUAL(x, y))


//
// SEQ
//
// ZELDA_PP_SEQ_FOR_EACH_PRODUCT for each product with data
#define ZELDA_PP_SEQ_FOR_EACH_PRODUCT(macro, data, seqs) \
BOOST_ZELDA_PP_SEQ_FOR_EACH_PRODUCT(DETAIL_ZELDA_PP_SEQ_FOR_EACH_PRODUCT_EACH, ((macro))((data))seqs)

#define DETAIL_ZELDA_PP_SEQ_FOR_EACH_PRODUCT_EACH_I(r, macro, data, product) macro(r, data, product) 
#define DETAIL_ZELDA_PP_SEQ_FOR_EACH_PRODUCT_EACH(r, product) \
DETAIL_ZELDA_PP_SEQ_FOR_EACH_PRODUCT_EACH_I(r, BOOST_PP_SEQ_ELEM(0, product), BOOST_PP_SEQ_ELEM(1, product), BOOST_PP_SEQ_REST_N(2,product))

//
// ZELDA_PP_MSVC_INVOKE invokes a vardiac macro on msvc
//
#define ZELDA_PP_MSVC_INVOKE BOOST_PP_CAT(ZELDA_PP_MSVC_INVOKE_, BOOST_PP_AUTO_REC(ZELDA_PP_DETAIL_MSVC_CALL_P, 8))

#define ZELDA_PP_DETAIL_MSVC_CALL_P(n) ZELDA_PP_IS_PAREN( ZELDA_PP_MSVC_INVOKE_ ## n((),) )

#define ZELDA_PP_MSVC_INVOKE_1(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_2(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_3(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_4(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_5(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_6(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_7(macro, args) macro args
#define ZELDA_PP_MSVC_INVOKE_8(macro, args) macro args

//
// ZELDA_PP_ARGS_TO_SEQ converts args to a seq
//
#define ZELDA_PP_ARGS_TO_SEQ(...) ZELDA_PP_DETAIL_ARGS_TO_SEQ(ZELDA_PP_NARGS(__VA_ARGS__), (__VA_ARGS__))
#define ZELDA_PP_DETAIL_ARGS_TO_SEQ(size, tuple) BOOST_PP_TUPLE_TO_SEQ(size, tuple)



#define ZELDA_PP_TYPEOF(x) ZELDA_PP_DETAIL_TYPEOF(ZELDA_PP_DETAIL_TYPEOF_PROBE x,)
#ifndef _MSC_VER
#define ZELDA_PP_DETAIL_TYPEOF(...) ZELDA_PP_DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#else
// MSVC Workarounds
#define ZELDA_PP_DETAIL_TYPEOF(...) ZELDA_PP_MSVC_INVOKE(ZELDA_PP_DETAIL_TYPEOF_HEAD, (__VA_ARGS__))
#endif
#define ZELDA_PP_DETAIL_TYPEOF_HEAD(x, ...) ZELDA_PP_REM x
#define ZELDA_PP_DETAIL_TYPEOF_PROBE(...) (__VA_ARGS__),

#define ZELDA_PP_STRIP(x) ZELDA_PP_EAT x

#define ZELDA_PP_PAIR(x) ZELDA_PP_REM x

#define ZELDA_PP_VARN_CAT(n, tuple) ZELDA_DETAIL_PP_VARN_INVOKE((n, ZELDA_PP_REM tuple, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT, BOOST_PP_INTERCEPT))
#ifndef _MSC_VER
#define ZELDA_DETAIL_PP_VARN_INVOKE(data)  ZELDA_DETAIL_PP_VARN_CAT data
#else
// MSVC Workarounds
#define ZELDA_DETAIL_PP_VARN_INVOKE(data)  ZELDA_PP_MSVC_INVOKE(ZELDA_DETAIL_PP_VARN_CAT, data)
#endif
#define ZELDA_DETAIL_PP_VARN_CAT(n, a, b, c, d, e, f, g, h, ...) a ## n b ## n c ## n d ## n e ## n f ## n g ## n h ## n


#define ZELDA_PP_CONSTRUCT(n, ...) BOOST_PP_ENUM(n, ZELDA_DETAIL_PP_CONSTRUCT_EACH, (__VA_ARGS__)) 
#define ZELDA_DETAIL_PP_CONSTRUCT_EACH(z, i, data) ZELDA_DETAIL_PP_CONSTRUCT_INVOKE((i, ZELDA_PP_REM data))
#define ZELDA_DETAIL_PP_CONSTRUCT_INVOKE(data) ZELDA_DETAIL_PP_CONSTRUCT_OP data
#define ZELDA_DETAIL_PP_CONSTRUCT_OP(n, ...) BOOST_PP_CAT(ZELDA_DETAIL_PP_CONSTRUCT_CAT_, ZELDA_PP_NARGS(__VA_ARGS__))(n, __VA_ARGS__)
#define ZELDA_DETAIL_PP_CONSTRUCT_CAT_1(n, x) x ## n (x ## n)
#define ZELDA_DETAIL_PP_CONSTRUCT_CAT_2(n, x, y) x ## n (y ## n)

#define ZELDA_PP_FIXED_PARAMS(n, max, var, fixed) \
BOOST_PP_ENUM_PARAMS(n,var)                                 \
BOOST_PP_COMMA_IF(n)                                      \
BOOST_PP_ENUM_PARAMS(BOOST_PP_SUB(max,n), fixed BOOST_PP_INTERCEPT)

#define ZELDA_PP_PARAMS(n, ...) BOOST_PP_ENUM(n, ZELDA_DETAIL_PP_PARAMS_EACH, (__VA_ARGS__))
#define ZELDA_PP_PARAMS_Z(z, n, ...) BOOST_PP_ENUM_ ## z(n, ZELDA_DETAIL_PP_PARAMS_EACH, (__VA_ARGS__)) 
#define ZELDA_DETAIL_PP_PARAMS_EACH(z, n, data) ZELDA_PP_VARN_CAT(n, data)

#define ZELDA_PP_GEN(n, ...) BOOST_PP_REPEAT(n, ZELDA_DETAIL_PP_GEN_EACH, (__VA_ARGS__)) 
#define ZELDA_PP_GEN_Z(z, n, ...) BOOST_PP_REPEAT_ ## z(n, ZELDA_DETAIL_PP_GEN_EACH, (__VA_ARGS__)) 
#define ZELDA_DETAIL_PP_GEN_EACH(z, n, data) ZELDA_PP_VARN_CAT(n, data)

#define ZELDA_PP_SEQ_GEN(z, n, ...) BOOST_PP_REPEAT_ ## z(n, ZELDA_DETAIL_PP_GEN_SEQ_EACH, (__VA_ARGS__)) 
#define ZELDA_DETAIL_PP_GEN_SEQ_EACH(z, n, data) (ZELDA_PP_VARN_CAT(n, data))

#define ZELDA_PP_SEQ_PREFIX(prefix, seq) BOOST_PP_SEQ_TRANSFORM(op, prefix, seq)  
#define ZELDA_DETAIL_PP_SEQ_PREFIX_OP(d, prefix, x) prefix x

#define ZELDA_PP_SEQ_POSTFIX(postfix, seq) BOOST_PP_SEQ_TRANSFORM(op, postfix, seq)  
#define ZELDA_DETAIL_PP_SEQ_POSTFIX_OP(d, postfix, x) postfix x

#define ZELDA_PP_SEQ_POST_ENUM(seq, ...) BOOST_PP_SEQ_FOR_EACH_I(ZELDA_DETAIL_PP_SEQ_POST_ENUM_EACH, (__VA_ARGS__), seq) 
#define ZELDA_DETAIL_PP_SEQ_POST_ENUM_EACH(r, data, i, elem) (ZELDA_PP_VARN_CAT(i, data) elem)

#define ZELDA_PP_SEQ_MERGE(seq1, seq2) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(seq1), ZELDA_DETAIL_PP_SEQ_MERGE_EACH, (seq1, seq2))
#define ZELDA_DETAIL_PP_SEQ_MERGE_EACH(z, n, data) ZELDA_DETAIL_PP_SEQ_MERGE_INVOKE((n, ZELDA_PP_REM data))
#define ZELDA_DETAIL_PP_SEQ_MERGE_INVOKE(data) ZELDA_DETAIL_PP_SEQ_MERGE_OP data
#define ZELDA_DETAIL_PP_SEQ_MERGE_OP(n, seq1, seq2) BOOST_PP_SEQ_ELEM(n, seq1) BOOST_PP_SEQ_ELEM(n, seq2)

#define ZELDA_PP_SEQ_MERGE_CAT(seq1, seq2) BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(seq1), ZELDA_DETAIL_PP_SEQ_MERGE_CAT_EACH, (seq1, seq2))
#define ZELDA_DETAIL_PP_SEQ_MERGE_CAT_EACH(z, n, data) ZELDA_DETAIL_PP_SEQ_MERGE_CAT_INVOKE((n, ZELDA_PP_REM data))
#define ZELDA_DETAIL_PP_SEQ_MERGE_CAT_INVOKE(data) ZELDA_DETAIL_PP_SEQ_MERGE_CAT_OP data
#define ZELDA_DETAIL_PP_SEQ_MERGE_CAT_OP(n, seq1, seq2) BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(n, seq1), BOOST_PP_SEQ_ELEM(n, seq2))

#define ZELDA_PP_SEQ_HEAD(seq) ZELDA_DETAIL_PP_SEQ_HEAD(ZELDA_PP_REM ZELDA_PP_HEAD(head))
#define ZELDA_DETAIL_PP_SEQ_HEAD(...) __VA_ARGS__

#define ZELDA_PP_SEQ_TAIL(seq) ZELDA_PP_EAT seq

//
// ZELDA_PP_SEQ_TRANSFORM_I
//
#define ZELDA_PP_SEQ_TRANSFORM_I(op, data, seq) \
ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_M(BOOST_PP_WHILE(ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_P, ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_OP, (op, data, 0, , seq)))

#define ZELDA_PP_SEQ_TRANSFORM_I_D(d, op, data, seq) \
ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_M(BOOST_PP_WHILE_ ## d(ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_P, ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_OP, (op, data, 0, , seq)))

#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_OP(d, state) ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_INVOKE(ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_OP, (d, ZELDA_PP_REM state))
#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_P(d, state) ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_INVOKE(ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_P, (d, ZELDA_PP_REM state))
#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_M(state) ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_M state
#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_WHILE_INVOKE(macro, args) macro args

#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_OP(d, op, data, i, result, seq) (op, data, BOOST_PP_INC(i), result(op(d, data, i, BOOST_PP_SEQ_HEAD(seq))), BOOST_PP_SEQ_TAIL(seq))
#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_P(d, op, data, i, result, seq) ZELDA_PP_IS_PAREN(seq)
#define ZELDA_DETAIL_PP_SEQ_TRANSFORM_I_M(op, data, i, result, seq) result


#endif	/* ZELDA_GUARD_PP_H */

