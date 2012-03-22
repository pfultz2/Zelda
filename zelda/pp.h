/* 
 * File:   pp.h
 * Author: pfultz
 *
 * Created on October 27, 2011, 12:11 PM
 */

#ifndef ZELDA_PP_H
#define ZELDA_PP_H

#define BOOST_PP_VARIADICS 1
#include <boost/preprocessor.hpp>
#include <boost/preprocessor/variadic.hpp>
#include <boost/preprocessor/facilities/is_empty.hpp>
#include "tokens.h"

//Utility

 /**
 * PP_NARGS returns the number of args in __VA_ARGS__
 */
#define PP_NARGS(...) \
         PP_DETAIL_NARG((__VA_ARGS__,PP_DETAIL_RSEQ_N()))
#define PP_DETAIL_NARG(args) \
         PP_DETAIL_ARG_N args
#define PP_DETAIL_ARG_N( \
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
#define PP_DETAIL_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 

#ifdef _MSC_VER
#define DETAIL_PP_CAT(args) PP_CAT args
#define PP_CAT(x, y) DETAIL_PP_CAT((x, y))
#else
#define PP_CAT BOOST_PP_CAT
#endif

#define PP_NOP(...)
#define PP_OUT(...) __VA_ARGS__

//Detection

#define DETAIL_PP_CHECK_N(x, n, ...) n
#ifdef _MSC_VER
#define DETAIL_PP_CHECK_RES(x) x
#define DETAIL_PP_CHECK_I(args) DETAIL_PP_CHECK_RES(DETAIL_PP_CHECK_N args)
#define PP_CHECK(...) DETAIL_PP_CHECK_I((__VA_ARGS__, 0))
#else
#define PP_CHECK(...) DETAIL_PP_CHECK_N(__VA_ARGS__, 0)
#endif

#define PP_PROBE ~, 1

#define DETAIL_PP_IS_EMPTY_0(...) 0
#define PP_IS_EMPTY(...) \
BOOST_PP_IF(BOOST_PP_EQUAL(PP_NARGS(__VA_ARGS__), 1), BOOST_PP_IS_EMPTY, DETAIL_PP_IS_EMPTY_0)(__VA_ARGS__) 

#define PP_IS_UNARY BOOST_PP_IS_UNARY

//Check if is parenthesis
#define DETAIL_PP_IS_PAREN_PROBE(...) PP_PROBE
#define PP_IS_PAREN(...) PP_CHECK(DETAIL_PP_IS_PAREN_PROBE __VA_ARGS__)

//Safely concat two tokens even if the they are parens
#define DETAIL_PP_JOIN_00(x, y) BOOST_PP_CAT(x, y)
#define DETAIL_PP_JOIN_01(x, y) x y
#define DETAIL_PP_JOIN_10(x, y) x y
#define DETAIL_PP_JOIN_11(x, y) x y
#define PP_JOIN(x, y) \
BOOST_PP_CAT(DETAIL_PP_JOIN_, BOOST_PP_CAT(PP_IS_PAREN(x), PP_IS_PAREN(y)))(x, y)

//Condtional/logic

#define PP_AND BOOST_PP_AND
#define PP_OR BOOST_PP_OR
#define PP_NOT BOOST_PP_NOT
#define PP_BITAND BOOST_PP_BITAND
#define PP_BITOR BOOST_PP_BITOR
#define PP_COMPL BOOST_PP_COMPL

#define DETAIL_PP_IF_TRUE(t, f) t
#define DETAIL_PP_IF_FALSE(t, f) f
#define PP_IIF(cond) BOOST_PP_IIF(cond, DETAIL_PP_IF_TRUE, DETAIL_PP_IF_FALSE)


#define PP_EQUAL(a, b) \
 PP_IIF(PP_BITAND( \
   PP_IS_UNARY( PP_CAT(PP_TOKEN_, a)((unused)) ) ,\
   PP_IS_UNARY( PP_CAT(PP_TOKEN_, b)((unused)) ) )) \
   (  \
      DETAIL_PP_EQUAL_I, \
      0 PP_NOP \
   )(a, b) \


#define DETAIL_PP_EQUAL_I(a, b) PP_COMPL(PP_IS_UNARY( \
PP_TOKEN_ ## a(PP_TOKEN_ ## b)((unused)) )) \



#define PP_IF(cond) BOOST_PP_IF(cond, DETAIL_PP_IF_TRUE, DETAIL_PP_IF_FALSE)

#define DETAIL_PP_WHEN_TRUE(...) __VA_ARGS__
#define DETAIL_PP_WHEN_FALSE(...)
#define PP_WHEN(cond) BOOST_PP_IF(cond, DETAIL_PP_WHEN_TRUE, DETAIL_PP_WHEN_FALSE)

//Args
//Head
#define DETAIL_PP_HEAD(x, ...) x
#ifdef _MSC_VER
#define DETAIL_PP_HEAD_RES(x) x
#define DETAIL_PP_HEAD_I(args) DETAIL_PP_HEAD_RES(DETAIL_PP_HEAD args)
#define PP_HEAD(...) DETAIL_PP_HEAD(__VA_ARGS__, ~)
#else
#define PP_HEAD(...) DETAIL_PP_HEAD(__VA_ARGS__, ~)
#endif
//Tail
#ifdef _MSC_VER
#define DETAIL_PP_TAIL_II(x, ...) __VA_ARGS__
#define DETAIL_PP_TAIL_I(args) DETAIL_PP_TAIL_II args
#define DETAIL_PP_TAIL(...) DETAIL_PP_TAIL_I((__VA_ARGS__))
#else
#define DETAIL_PP_TAIL(x, ...) __VA_ARGS__
#endif
#define PP_TAIL(...) PP_IF(PP_EQUAL(PP_NARGS(__VA_ARGS__), 1)) \
   ( \
      PP_NOP, \
      DETAIL_PP_TAIL \
   )(__VA_ARGS__) 


//Tuple
#define PP_TUPLE_EAT(...)
#define PP_TUPLE_ELEM(i, tuple) BOOST_PP_TUPLE_ELEM(PP_TUPLE_SIZE(tuple), i, tuple)
#define DETAIL_PP_TUPLE_REM(...) __VA_ARGS__
#define PP_TUPLE_REM(tuple) DETAIL_PP_TUPLE_REM tuple
#define PP_TUPLE_SIZE(tuple) PP_NARGS tuple

//Args
#define DETAIL_PP_ARGS_TO_SEQ(size, tuple) BOOST_PP_TUPLE_TO_SEQ(size, tuple)
#define PP_ARGS_TO_SEQ(...) DETAIL_PP_ARGS_TO_SEQ(PP_NARGS(__VA_ARGS__), (__VA_ARGS__))
#define PP_ARGS_TO_TUPLE(...) (__VA_ARGS__)


//Closure

//#define DETAIL_PP_CLOSURE_INVOKE(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_STATE() BOOST_PP_AUTO_REC(DETAIL_PP_CLOSURE_INVOKE_P, 8)
#define PP_CLOSURE_INVOKE PP_CAT(DETAIL_PP_CLOSURE_INVOKE_, DETAIL_PP_CLOSURE_INVOKE_STATE())

#define DETAIL_PP_CLOSURE_INVOKE_P(n) BOOST_PP_CAT(DETAIL_PP_CLOSURE_INVOKE_CHECK_, DETAIL_PP_CLOSURE_INVOKE_## n(PP_NIL,))
#define DETAIL_PP_CLOSURE_INVOKE_1(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_2(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_3(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_4(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_5(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_6(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_7(macro, args) macro args
#define DETAIL_PP_CLOSURE_INVOKE_8(macro, args) macro args

#define DETAIL_PP_CLOSURE_INVOKE_CHECK_PP_NIL 1
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_1(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_2(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_3(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_4(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_5(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_6(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_7(macro, args) 0
#define DETAIL_PP_CLOSURE_INVOKE_CHECK_DETAIL_PP_CLOSURE_INVOKE_8(macro, args) 0

#define DETAIL_PP_CLOSURE_ARGS(r, data, elem) \
   (r, \
   PP_IF(PP_IS_EMPTY data) (PP_HEAD, PP_OUT) (elem, PP_TUPLE_REM(data)) \
   )
//TODO: Make recursive
#define DETAIL_PP_CLOSURE(r, data, elem) PP_CLOSURE_INVOKE(PP_TUPLE_ELEM(0, data), DETAIL_PP_CLOSURE_ARGS(r, PP_TUPLE_ELEM(1, data), elem) )

#define DETAIL_PP_CLOSURE_STATE(tuple) PP_TUPLE_REM(PP_TUPLE_ELEM(1, tuple))


//Sequences

#define PP_SEQ_CAT BOOST_PP_SEQ_CAT
#define PP_SEQ_ELEM BOOST_PP_SEQ_ELEM
#define PP_SEQ_ENUM BOOST_PP_SEQ_ENUM
#define PP_SEQ_FIRST_N BOOST_PP_SEQ_FIRST_N
#define PP_SEQ_HEAD BOOST_PP_SEQ_HEAD
#define PP_SEQ_INSERT BOOST_PP_SEQ_INSERT
#define PP_SEQ_POP_BACK BOOST_PP_SEQ_POP_BACK
#define PP_SEQ_POP_FRONT BOOST_PP_SEQ_POP_FRONT
#define PP_SEQ_PUSH_BACK BOOST_PP_SEQ_PUSH_BACK
#define PP_SEQ_PUSH_FRONT BOOST_PP_SEQ_PUSH_FRONT
#define PP_SEQ_REMOVE BOOST_PP_SEQ_REMOVE
#define PP_SEQ_REPLACE BOOST_PP_SEQ_REPLACE
#define PP_SEQ_REST_N BOOST_PP_SEQ_REST_N
#define PP_SEQ_REVERSE BOOST_PP_SEQ_REVERSE
#define PP_SEQ_SIZE BOOST_PP_SEQ_SIZE
#define PP_SEQ_SUBSEQ BOOST_PP_SEQ_SUBSEQ
#define PP_SEQ_TAIL BOOST_PP_SEQ_TAIL
#define PP_SEQ_TO_TUPLE BOOST_PP_SEQ_TO_TUPLE

#define PP_SEQ_FRONT PP_SEQ_HEAD
#define PP_SEQ_BACK(seq) PP_SEQ_ELEM(BOOST_PP_DEC(PP_SEQ_SIZE(seq)), seq)


//Algos
#define PP_SEQ_FILTER(m, ...) BOOST_PP_SEQ_FILTER(DETAIL_PP_CLOSURE, (m, (PP_TAIL(__VA_ARGS__))), PP_HEAD(__VA_ARGS__))
#define PP_SEQ_FOLD_LEFT(m, ...) DETAIL_PP_CLOSURE_STATE(BOOST_PP_SEQ_FOLD_LEFT(DETAIL_PP_CLOSURE, (m, (PP_TAIL(__VA_ARGS__))), PP_HEAD(__VA_ARGS__)))
#define PP_SEQ_FOLD_RIGHT(m, ...) DETAIL_PP_CLOSURE_STATE(BOOST_PP_SEQ_FOLD_LEFT(DETAIL_PP_CLOSURE, (m, (PP_TAIL(__VA_ARGS__))), PP_HEAD(__VA_ARGS__)))
#define PP_SEQ_FOR_EACH(m, ...) BOOST_PP_SEQ_FOR_EACH(DETAIL_PP_CLOSURE, (m, (PP_TAIL(__VA_ARGS__))), PP_HEAD(__VA_ARGS__))
#define PP_SEQ_TRANSFORM(m, ...) BOOST_PP_SEQ_TRANSFORM(DETAIL_PP_CLOSURE, (m, (PP_TAIL(__VA_ARGS__))), PP_HEAD(__VA_ARGS__))

#define DETAIL_PP_SEQ_ZIP_ELEM(i, j, tuple) PP_SEQ_ELEM(i, BOOST_PP_TUPLE_ELEM(2, j, data))
#define DETAIL_PP_SEQ_ZIP(z, n, data) ( (DETAIL_PP_SEQ_ZIP_ELEM(n, 0, data), DETAIL_PP_SEQ_ZIP_ELEM(n, 1, data)) )
#define PP_SEQ_ZIP(seq1, seq2) BOOST_PP_REPEAT(PP_SEQ_SIZE(seq1), DETAIL_PP_SEQ_ZIP, (seq1, seq2)) 


#define DETAIL_PP_SEQ_GEN_EACH(z, i, data) (PP_CAT(data, i))
#define PP_SEQ_GEN(n, x) BOOST_PP_REPEAT(n, DETAIL_PP_SEQ_GEN_EACH, x)

#define DETAIL_PP_SEQ_FOR_EACH_PRODUCT_EACH_I(macro, data, product) macro(data, product) 
#define DETAIL_PP_SEQ_FOR_EACH_PRODUCT_EACH(r, product) \
DETAIL_PP_SEQ_FOR_EACH_PRODUCT_EACH_I(BOOST_PP_SEQ_ELEM(0, product), BOOST_PP_SEQ_ELEM(1, product), BOOST_PP_SEQ_REST_N(2,product))

#define PP_SEQ_FOR_EACH_PRODUCT(macro, data, seqs) \
BOOST_PP_SEQ_FOR_EACH_PRODUCT(DETAIL_PP_SEQ_FOR_EACH_PRODUCT_EACH, ((macro))((data))seqs)

#define DETAIL_PP_TUPLE_SEQ_0(...)                                \
    ((__VA_ARGS__)) DETAIL_PP_TUPLE_SEQ_1
#define DETAIL_PP_TUPLE_SEQ_1(...)                                \
    ((__VA_ARGS__)) DETAIL_PP_TUPLE_SEQ_0
#define DETAIL_PP_TUPLE_SEQ_0_END
#define DETAIL_PP_TUPLE_SEQ_1_END
#define PP_TUPLE_SEQ(seq) PP_CAT(DETAIL_PP_TUPLE_SEQ_0 seq, _END)


//Tokens        
#ifndef PP_PREFIX_token
#define PP_PREFIX_token token,
#endif
#ifndef PP_TOKEN_token
#define PP_TOKEN_token(x) x
#endif    

#define PP_IS_TOKEN(x) PP_IS_UNARY( PP_JOIN(PP_TOKEN_, x)((unused)) ) 

#define DETAIL_PP_TOKENS_PEEK_I(token) PP_WHEN(PP_IS_TOKEN(token))(token)
#define DETAIL_PP_TOKENS_PEEK(x) DETAIL_PP_TOKENS_PEEK_I(PP_HEAD(PP_CAT(PP_PREFIX_, x)))
#define PP_TOKENS_PEEK(...) DETAIL_PP_TOKENS_PEEK(PP_HEAD(__VA_ARGS__))

#define DETAIL_PP_TOKENS_POP_I(x, ...) PP_IIF(PP_IS_TOKEN(PP_HEAD(__VA_ARGS__)))(PP_TAIL(__VA_ARGS__), x)
#define PP_TOKENS_POP(x) DETAIL_PP_TOKENS_POP_I(x, PP_JOIN(PP_PREFIX_, x))

#define DETAIL_PP_TOKEN_REPLACE_I(token, replacement, x, ...) PP_IIF(PP_EQUAL(PP_HEAD(__VA_ARGS__), token))(replacement PP_TAIL(__VA_ARGS__), x)
#define PP_TOKEN_REPLACE(token, replacement, x) DETAIL_PP_TOKEN_REPLACE_I(token, replacement, x, PP_JOIN(PP_PREFIX_, x))

#define DETAIL_PP_TOKEN_TRANSFORM_APPLY_FOUND(token, found, not_found, x, ...) found(token, PP_TAIL(__VA_ARGS__))
#define DETAIL_PP_TOKEN_TRANSFORM_APPLY_NOT_FOUND(token, found, not_found, x, ...) not_found(x)
#define DETAIL_PP_TOKEN_TRANSFORM_I(token, found, not_found, x, ...) \
PP_IIF(PP_EQUAL(PP_HEAD(__VA_ARGS__), token))\
(\
   DETAIL_PP_TOKEN_TRANSFORM_APPLY_FOUND, \
   DETAIL_PP_TOKEN_TRANSFORM_APPLY_NOT_FOUND \
)(token, found, not_found, x, __VA_ARGS__)
#define PP_TOKEN_TRANSFORM(token, found, not_found, x) DETAIL_PP_TOKEN_TRANSFORM_I(token, found, not_found, x, PP_JOIN(PP_PREFIX_, x))





/**
 * This macro used to compensate for a bug in compilers in visual c++, see
 * http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
 */
// #define PP_MSVC_CALL BOOST_PP_CAT(DETAIL_PP_MSVC_CALL_, BOOST_PP_AUTO_REC(DETAIL_PP_MSVC_CALL_P, 8))

// #define DETAIL_PP_MSVC_CALL_P(n) BOOST_PP_CAT(DETAIL_PP_MSVC_CALL_CHECK_, DETAIL_PP_MSVC_CALL_## n(PP_NIL,))

// #define DETAIL_PP_MSVC_CALL_1(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_2(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_3(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_4(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_5(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_6(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_7(macro, args) macro args
// #define DETAIL_PP_MSVC_CALL_8(macro, args) macro args

// #define DETAIL_PP_MSVC_CALL_CHECK_PP_NIL 1
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_1(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_2(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_3(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_4(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_5(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_6(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_7(macro, args) 0
// #define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_8(macro, args) 0



// #define PP_TOKEN PP_NIL, 1,

// #ifndef PP_TOKEN_token_token
// #define PP_TOKEN_token_token PP_TOKEN
// #endif

// #define PP_DETAIL_TOKEN_TAIL_I(x, n, ...) __VA_ARGS__
// #define PP_DETAIL_TOKEN_TAIL(...) PP_MSVC_CALL(PP_DETAIL_TOKEN_TAIL_I, (__VA_ARGS__))
// #define PP_DETAIL_TOKEN_N(x, n, ...) n
// #define PP_DETAIL_TOKEN_CHECK(...) PP_MSVC_CALL(PP_DETAIL_TOKEN_N, (__VA_ARGS__, 0))
// #define PP_DETAIL_TOKEN_HAS(x, probe) PP_DETAIL_TOKEN_CHECK(PP_JOIN(probe, x))
// #define PP_DETAIL_TOKEN_EAT(x, probe) PP_DETAIL_TOKEN_TAIL(PP_JOIN(probe, x))
// #define PP_DETAIL_TOKEN_HAS_PROBE(token) BOOST_PP_CAT(PP_TOKEN_, BOOST_PP_CAT(token, _))

// /**
//  */
// #define PP_TOKEN_HAS(x, token) PP_DETAIL_TOKEN_HAS(x, PP_DETAIL_TOKEN_HAS_PROBE(token))
// #define PP_TOKEN_EAT(x, token) PP_DETAIL_TOKEN_EAT(x, PP_DETAIL_TOKEN_HAS_PROBE(token))
// #define PP_TOKEN_REPLACE(x, token, replacement) \
// BOOST_PP_IIF(PP_TOKEN_HAS(x, token), replacement PP_TOKEN_EAT(x, token), x)






// #define PP_EXPAND(...) DETAIL_PP_EXPAND_I((__VA_ARGS__))
// #define DETAIL_PP_EXPAND_I(args) DETAIL_PP_EXPAND_II args
// #define DETAIL_PP_EXPAND_II(...) DETAIL_PP_EXPAND_III((__VA_ARGS__))
// #define DETAIL_PP_EXPAND_III(args) DETAIL_PP_EXPAND_IIII args
// #define DETAIL_PP_EXPAND_IIII(...) __VA_ARGS__


// #define DETAIL_PP_ARGS_TO_SEQ(size, tuple) BOOST_PP_TUPLE_TO_SEQ(size, tuple)
// #define PP_ARGS_TO_SEQ(...) DETAIL_PP_ARGS_TO_SEQ(PP_NARGS(__VA_ARGS__), (__VA_ARGS__))

// #define DETAIL_PP_SEQ_TO_ARGS_REM(...) __VA_ARGS__
// #define PP_SEQ_TO_ARGS(seq) PP_EXPAND(DETAIL_PP_SEQ_TO_ARGS_REM BOOST_PP_SEQ_TO_TUPLE(seq))

// #define PP_ARGS_BACK(...) PP_ARGS_ELEM(BOOST_PP_DEC(PP_NARGS(__VA_ARGS__)), __VA_ARGS__)
// #define PP_ARGS_CAT(...) BOOST_PP_SEQ_CAT(PP_ARGS_TO_SEQ(__VA_ARGS__)) 
// #define PP_ARGS_ELEM(i, ...) BOOST_PP_SEQ_ELEM(i, PP_ARGS_TO_SEQ(__VA_ARGS__))
// #define PP_ARGS_FILTER(pred, data, ...) BOOST_PP_SEQ_FILTER(pred, data, PP_ARGS_TO_SEQ(__VA_ARGS__)) 
// #define PP_ARGS_FOREACH(macro, data, ...) BOOST_PP_SEQ_FOR_EACH(macro, data, PP_ARGS_TO_SEQ(__VA_ARGS__)) 
// #define PP_ARGS_FOREACH_I(macro, data, ...) BOOST_PP_SEQ_FOR_EACH_I(macro, data, PP_ARGS_TO_SEQ(__VA_ARGS__))
// #define PP_ARGS_FRONT(...) PP_ARGS_ELEM(0, __VA_ARGS__)
// #define PP_ARGS_POP_BACK(...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_POP_BACK(PP_ARGS_TO_SEQ(__VA_ARGS__)))
// #define PP_ARGS_POP_FRONT(...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_POP_FRONT(PP_ARGS_TO_SEQ(__VA_ARGS__)))
// #define PP_ARGS_PUSH_BACK(elem, ...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_PUSH_BACK(PP_ARGS_TO_SEQ(__VA_ARGS__), elem))
// #define PP_ARGS_PUSH_FRONT(elem, ...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_PUSH_FRONT(PP_ARGS_TO_SEQ(__VA_ARGS__), elem))
// #define PP_ARGS_TAIL(...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_TAIL(PP_ARGS_TO_SEQ(__VA_ARGS__)))
// #define PP_ARGS_TRANSFORM(macro, data, ...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_TRANSFORM(macro, data, PP_ARGS_TO_SEQ(__VA_ARGS__)))

// #define DETAIL_PP_ARGS_STATEMENTS(r, prefix, x) prefix x;
// #define PP_ARGS_STATEMENTS(prefix, ...) DETAIL_PP_ARGS_STATEMENTS(macro, prefix, __VA_ARGS__)

// #define DETAIL_PP_ARGS_TOKEN_REPLACE_T_I(x, y) x
// #define DETAIL_PP_ARGS_TOKEN_REPLACE_R_I(x, y) y
// #define DETAIL_PP_ARGS_TOKEN_REPLACE_T(x) DETAIL_PP_ARGS_TOKEN_REPLACE_T_I x
// #define DETAIL_PP_ARGS_TOKEN_REPLACE_R(x) DETAIL_PP_ARGS_TOKEN_REPLACE_R_I x
// #define DETAIL_PP_ARGS_TOKEN_REPLACE_EACH(s, data, x) \
// PP_EXPAND(PP_TOKEN_REPLACE(x, DETAIL_PP_ARGS_TOKEN_REPLACE_T(data), DETAIL_PP_ARGS_TOKEN_REPLACE_R(data)))

// #define PP_ARGS_TOKEN_REPLACE(token, replacement, ...)\
// PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_TOKEN_REPLACE_EACH, (token, replacement), __VA_ARGS__)

// #define PP_SEQ_TOKEN_REPLACE(token, replacement, seq)\
// BOOST_PP_SEQ_TRANSFORM(DETAIL_PP_ARGS_TOKEN_REPLACE_EACH, (token, replacement), seq)

// #define DETAIL_PP_TOKEN_FILTER_P(s, data, x) PP_TOKEN_HAS(x, data)
// #define PP_ARGS_TOKEN_FILTER(token, ...) PP_ARGS_FILTER(DETAIL_PP_TOKEN_FILTER_P, token, __VA_ARGS__)
// #define PP_SEQ_TOKEN_FILTER(token, seq) BOOST_PP_SEQ_FILTER(DETAIL_PP_TOKEN_FILTER_P, token, seq)


// #define PP_OPTIONAL_FIRST(...) PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
// DETAIL_PP_OPTIONAL_FIRST_I, \
// DETAIL_PP_OPTIONAL_FIRST_II \
// ),(__VA_ARGS__))

// #define DETAIL_PP_OPTIONAL_FIRST_II(x, ...) x
// #define DETAIL_PP_OPTIONAL_FIRST_I(x) x

// #define PP_OPTIONAL_TAIL(...) PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
// PP_NOP, \
// DETAIL_PP_OPTIONAL_TAIL \
// ),(__VA_ARGS__))

// #define PP_OPTIONAL_WHEN(macro, ...) PP_EXPAND(PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
// PP_NOP, \
// macro DETAIL_PP_OPTIONAL_WHEN_DISPATCH_TAIL \
// ),(__VA_ARGS__)))

// #define PP_OPTIONAL_IF(exist, empty, ...) PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
// empty, \
// exist \
// ),(__VA_ARGS__))

// #define DETAIL_PP_OPTIONAL_WHEN_DISPATCH_TAIL(...) (PP_OPTIONAL_TAIL(__VA_ARGS__))

// #define DETAIL_PP_OPTIONAL_TAIL(x, ...) __VA_ARGS__

// #define DETAIL_PP_PAIR_TYPE_I(x) (x), 
// #define DETAIL_PP_PAIR_TYPE_CHECK_III(x) x
// #define DETAIL_PP_PAIR_TYPE_CHECK_II(x, ...) DETAIL_PP_PAIR_TYPE_CHECK_III x
// #define DETAIL_PP_PAIR_TYPE_CHECK_I(x) DETAIL_PP_PAIR_TYPE_CHECK_II x
// #define DETAIL_PP_PAIR_TYPE_CHECK(...) DETAIL_PP_PAIR_TYPE_CHECK_I((__VA_ARGS__))

// #define DETAIL_PP_PAIR_NAME_IGNORE(x)

// #define PP_PAIR_TYPE(x) DETAIL_PP_PAIR_TYPE_CHECK(DETAIL_PP_PAIR_TYPE_I x)
// #define PP_PAIR_NAME(x) DETAIL_PP_PAIR_NAME_IGNORE x

// #define DETAIL_PP_ARGS_PAIR_NAME_EACH(r, data, x) PP_PAIR_NAME(x)
// #define PP_ARGS_PAIR_NAME(...) PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_PAIR_NAME_EACH, data, __VA_ARGS__) 

// #define DETAIL_PP_ARGS_PAIR_TYPE_EACH(r, data, x) PP_PAIR_TYPE(x)
// #define PP_ARGS_PAIR_TYPE(...) PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_PAIR_TYPE_EACH, data, __VA_ARGS__) 

// #define DETAIL_PP_ARGS_PAIR_EACH(r, data, x) PP_PAIR_TYPE(x) PP_PAIR_NAME(x)
// #define PP_ARGS_PAIR(...) PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_PAIR_EACH, data, __VA_ARGS__)


// #define PP_SAFE_CONTEXT(x) x

// #define PP_SAFE_DEFINE(symbol, macro) BOOST_PP_IIF\
// (BOOST_PP_CAT(DETAIL_PP_NIL_, PP_SAFE_CONTEXT(0)),\
// macro,\
// symbol\
// )

// #define DETAIL_PP_NIL_0 0
// #define DETAIL_PP_NIL_PP_SAFE_CONTEXT(x) 1

// # define PP_INDIRECT(n) BOOST_PP_CAT(PP_INDIRECT_, n)
// # define PP_INDIRECT_0 (m) m(0 )
// # define PP_INDIRECT_1 (m) m(1 )
// # define PP_INDIRECT_2 (m) m(2 )
// # define PP_INDIRECT_3 (m) m(3 )
// # define PP_INDIRECT_4 (m) m(4 )
// # define PP_INDIRECT_5 (m) m(5 )
// # define PP_INDIRECT_6 (m) m(6 )
// # define PP_INDIRECT_7 (m) m(7 )
// # define PP_INDIRECT_8 (m) m(8 )
// # define PP_INDIRECT_9 (m) m(9 )
// # define PP_INDIRECT_10(m) m(10)
// # define PP_INDIRECT_11(m) m(11)
// # define PP_INDIRECT_12(m) m(12)
// # define PP_INDIRECT_13(m) m(13)
// # define PP_INDIRECT_14(m) m(14)
// # define PP_INDIRECT_15(m) m(15)

// # define PP_INDIRECT_ARGS(n) BOOST_PP_CAT(PP_INDIRECT_ARGS_, n)
// # define PP_INDIRECT_ARGS_0(m, ...) m(0, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_1(m, ...) m(1, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_2(m, ...) m(2, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_3(m, ...) m(3, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_4(m, ...) m(4, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_5(m, ...) m(5, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_6(m, ...) m(6, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_7(m, ...) m(7, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_8(m, ...) m(8, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_9(m, ...) m(9, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_10(m, ...) m(10, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_11(m, ...) m(11, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_12(m, ...) m(12, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_13(m, ...) m(13, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_14(m, ...) m(14, __VA_ARGS__)
// # define PP_INDIRECT_ARGS_15(m, ...) m(15, __VA_ARGS__)

// #define DETAIL_PP_LOCAL_NAME(name) BOOST_PP_CAT(name, _unique_local_)
// #define PP_UNIQUE_LOCAL_NAME(name) BOOST_PP_CAT(DETAIL_PP_LOCAL_NAME(name), __LINE__)
// #define PP_UNIQUE_NAME(name) PP_UNIQUE_LOCAL_NAME(BOOST_PP_CAT(name, __COUNTER__))


#endif	/* ZELDA_PP_H */

