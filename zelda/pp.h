/* 
 * File:   pp.h
 * Author: pfultz
 *
 * Created on October 27, 2011, 12:11 PM
 */

#ifndef ZELDA_PP_H
#define	ZELDA_PP_H

#include <boost/preprocessor.hpp>

/**
 * This macro used to compensate for a bug in compilers in visual c++, see
 * http://connect.microsoft.com/VisualStudio/feedback/details/380090/variadic-macro-replacement
 */
#define PP_MSVC_CALL BOOST_PP_CAT(DETAIL_PP_MSVC_CALL_, BOOST_PP_AUTO_REC(DETAIL_PP_MSVC_CALL_P, 8))

#define DETAIL_PP_MSVC_CALL_P(n) BOOST_PP_CAT(DETAIL_PP_MSVC_CALL_CHECK_, DETAIL_PP_MSVC_CALL_## n(PP_NIL,))

#define DETAIL_PP_MSVC_CALL_1(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_2(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_3(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_4(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_5(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_6(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_7(macro, args) macro args
#define DETAIL_PP_MSVC_CALL_8(macro, args) macro args

#define DETAIL_PP_MSVC_CALL_CHECK_PP_NIL 1
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_1(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_2(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_3(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_4(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_5(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_6(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_7(macro, args) 0
#define DETAIL_PP_MSVC_CALL_CHECK_DETAIL_PP_MSVC_CALL_8(macro, args) 0


//Check if is parenthesis
#define DETAIL_PP_IS_PAREN_N(x, n, ...) n
#define DETAIL_PP_IS_PAREN_CHECK(...) PP_MSVC_CALL(DETAIL_PP_IS_PAREN_N, (__VA_ARGS__, 0))
#define DETAIL_PP_IS_PAREN_PROBE(...) PP_NIL, 1
#define PP_IS_PAREN(...) DETAIL_PP_IS_PAREN_CHECK(DETAIL_PP_IS_PAREN_PROBE __VA_ARGS__)

//Safely concat two token even if the they are parens
#define DETAIL_PP_JOIN_00(x, y) BOOST_PP_CAT(x, y)
#define DETAIL_PP_JOIN_00(x, y) x y
#define DETAIL_PP_JOIN_10(x, y) x y
#define DETAIL_PP_JOIN_11(x, y) x y
#define PP_JOIN(x, y) \
BOOST_PP_CAT(DETAIL_PP_JOIN_, BOOST_PP_CAT(PP_IS_PAREN(x), PP_IS_PAREN(y)))(x, y)


#define PP_TOKEN PP_NIL, 1,

#ifndef PP_TOKEN_token_token
#define PP_TOKEN_token_token PP_TOKEN
#endif

#define PP_DETAIL_TOKEN_TAIL_I(x, n, ...) __VA_ARGS__
#define PP_DETAIL_TOKEN_TAIL(...) PP_MSVC_CALL(PP_DETAIL_TOKEN_TAIL_I, (__VA_ARGS__))
#define PP_DETAIL_TOKEN_N(x, n, ...) n
#define PP_DETAIL_TOKEN_CHECK(...) PP_MSVC_CALL(PP_DETAIL_TOKEN_N, (__VA_ARGS__, 0))
#define PP_DETAIL_TOKEN_HAS(x, probe) PP_DETAIL_TOKEN_CHECK(PP_JOIN(probe, x))
#define PP_DETAIL_TOKEN_EAT(x, probe) PP_DETAIL_TOKEN_TAIL(PP_JOIN(probe, x))
#define PP_DETAIL_TOKEN_HAS_PROBE(token) BOOST_PP_CAT(PP_TOKEN_, BOOST_PP_CAT(token, _))

/**
 */
#define PP_TOKEN_HAS(x, token) PP_DETAIL_TOKEN_HAS(x, PP_DETAIL_TOKEN_HAS_PROBE(token))
#define PP_TOKEN_EAT(x, token) PP_DETAIL_TOKEN_EAT(x, PP_DETAIL_TOKEN_HAS_PROBE(token))
#define PP_TOKEN_REPLACE(x, token, replacement) \
BOOST_PP_IIF(PP_TOKEN_HAS(x, token), replacement PP_TOKEN_EAT(x, token), x)


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


#define PP_EXPAND(...) DETAIL_PP_EXPAND_I((__VA_ARGS__))
#define DETAIL_PP_EXPAND_I(args) DETAIL_PP_EXPAND_II args
#define DETAIL_PP_EXPAND_II(...) DETAIL_PP_EXPAND_III((__VA_ARGS__))
#define DETAIL_PP_EXPAND_III(args) DETAIL_PP_EXPAND_IIII args
#define DETAIL_PP_EXPAND_IIII(...) __VA_ARGS__


#define PP_OUT(...) __VA_ARGS__


#define PP_NOP(...)
#define DETAIL_PP_ARGS_TO_SEQ(size, tuple) BOOST_PP_TUPLE_TO_SEQ(size, tuple)
#define PP_ARGS_TO_SEQ(...) DETAIL_PP_ARGS_TO_SEQ(PP_NARGS(__VA_ARGS__), (__VA_ARGS__))

#define DETAIL_PP_SEQ_TO_ARGS_REM(...) __VA_ARGS__
#define PP_SEQ_TO_ARGS(seq) PP_EXPAND(DETAIL_PP_SEQ_TO_ARGS_REM BOOST_PP_SEQ_TO_TUPLE(seq))

#define PP_ARGS_BACK(...) PP_ARGS_ELEM(BOOST_PP_DEC(PP_NARGS(__VA_ARGS__)), __VA_ARGS__)
#define PP_ARGS_CAT(...) BOOST_PP_SEQ_CAT(PP_ARGS_TO_SEQ(__VA_ARGS__)) 
#define PP_ARGS_ELEM(i, ...) BOOST_PP_SEQ_ELEM(i, PP_ARGS_TO_SEQ(__VA_ARGS__)) 
#define PP_ARGS_FOREACH(macro, data, ...) BOOST_PP_SEQ_FOR_EACH(macro, data, PP_ARGS_TO_SEQ(__VA_ARGS__)) 
#define PP_ARGS_FOREACH_I(macro, data, ...) BOOST_PP_SEQ_FOR_EACH_I(macro, data, PP_ARGS_TO_SEQ(__VA_ARGS__))
#define PP_ARGS_FRONT(...) PP_ARGS_ELEM(0, __VA_ARGS__)
#define PP_ARGS_POP_BACK(...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_POP_BACK(PP_ARGS_TO_SEQ(__VA_ARGS__)))
#define PP_ARGS_POP_FRONT(...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_POP_FRONT(PP_ARGS_TO_SEQ(__VA_ARGS__)))
#define PP_ARGS_PUSH_BACK(elem, ...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_PUSH_BACK(PP_ARGS_TO_SEQ(__VA_ARGS__), elem))
#define PP_ARGS_PUSH_FRONT(elem, ...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_PUSH_FRONT(PP_ARGS_TO_SEQ(__VA_ARGS__), elem))
#define PP_ARGS_TRANSFORM(macro, data, ...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_TRANSFORM(macro, data, PP_ARGS_TO_SEQ(__VA_ARGS__)))


#define PP_ARGS_TAIL(...) PP_SEQ_TO_ARGS(BOOST_PP_SEQ_TAIL(PP_ARGS_TO_SEQ(__VA_ARGS__)))



#define PP_OPTIONAL_FIRST(...) PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
DETAIL_PP_OPTIONAL_FIRST_I, \
DETAIL_PP_OPTIONAL_FIRST_II \
),(__VA_ARGS__))

#define DETAIL_PP_OPTIONAL_FIRST_II(x, ...) x
#define DETAIL_PP_OPTIONAL_FIRST_I(x) x

#define PP_OPTIONAL_TAIL(...) PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
PP_NOP, \
DETAIL_PP_OPTIONAL_TAIL \
),(__VA_ARGS__))

#define PP_OPTIONAL_WHEN(macro, ...) PP_EXPAND(PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
PP_NOP, \
macro DETAIL_PP_OPTIONAL_WHEN_DISPATCH_TAIL \
),(__VA_ARGS__)))

#define PP_OPTIONAL_IF(exist, empty, ...) PP_MSVC_CALL(BOOST_PP_IF( BOOST_PP_EQUAL(1, PP_NARGS(__VA_ARGS__)),  \
empty, \
exist \
),(__VA_ARGS__))

#define DETAIL_PP_OPTIONAL_WHEN_DISPATCH_TAIL(...) (PP_OPTIONAL_TAIL(__VA_ARGS__))

#define DETAIL_PP_OPTIONAL_TAIL(x, ...) __VA_ARGS__

#define DETAIL_PP_PAIR_TYPE_I(x) (x), 
#define DETAIL_PP_PAIR_TYPE_CHECK_III(x) x
#define DETAIL_PP_PAIR_TYPE_CHECK_II(x, ...) DETAIL_PP_PAIR_TYPE_CHECK_III x
#define DETAIL_PP_PAIR_TYPE_CHECK_I(x) DETAIL_PP_PAIR_TYPE_CHECK_II x
#define DETAIL_PP_PAIR_TYPE_CHECK(...) DETAIL_PP_PAIR_TYPE_CHECK_I((__VA_ARGS__))

#define DETAIL_PP_PAIR_NAME_IGNORE(x)

#define PP_PAIR_TYPE(x) DETAIL_PP_PAIR_TYPE_CHECK(DETAIL_PP_PAIR_TYPE_I x)
#define PP_PAIR_NAME(x) DETAIL_PP_PAIR_NAME_IGNORE x

#define DETAIL_PP_ARGS_PAIR_NAME_EACH(r, data, x) PP_PAIR_NAME(x)
#define PP_ARGS_PAIR_NAME(...) PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_PAIR_NAME_EACH, data, __VA_ARGS__) 

#define DETAIL_PP_ARGS_PAIR_TYPE_EACH(r, data, x) PP_PAIR_TYPE(x)
#define PP_ARGS_PAIR_TYPE(...) PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_PAIR_TYPE_EACH, data, __VA_ARGS__) 

#define DETAIL_PP_ARGS_PAIR_EACH(r, data, x) PP_PAIR_TYPE(x) PP_PAIR_NAME(x)
#define PP_ARGS_PAIR(...) PP_ARGS_TRANSFORM(DETAIL_PP_ARGS_PAIR_EACH, data, __VA_ARGS__)


#define PP_SAFE_CONTEXT(x) x

#define PP_SAFE_DEFINE(symbol, macro) BOOST_PP_IIF\
(BOOST_PP_CAT(DETAIL_PP_NIL_, PP_SAFE_CONTEXT(0)),\
macro,\
symbol\
)

#define DETAIL_PP_NIL_0 0
#define DETAIL_PP_NIL_PP_SAFE_CONTEXT(x) 1

# define PP_INDIRECT(n) BOOST_PP_CAT(PP_INDIRECT_, n)
# define PP_INDIRECT_0(m) m(0)
# define PP_INDIRECT_1(m) m(1)
# define PP_INDIRECT_2(m) m(2)
# define PP_INDIRECT_3(m) m(3)
# define PP_INDIRECT_4(m) m(4)
# define PP_INDIRECT_5(m) m(5)
# define PP_INDIRECT_6(m) m(6)
# define PP_INDIRECT_7(m) m(7)
# define PP_INDIRECT_8(m) m(8)
# define PP_INDIRECT_9(m) m(9)
# define PP_INDIRECT_10(m) m(10)
# define PP_INDIRECT_11(m) m(11)
# define PP_INDIRECT_12(m) m(12)
# define PP_INDIRECT_13(m) m(13)
# define PP_INDIRECT_14(m) m(14)
# define PP_INDIRECT_15(m) m(15)

# define PP_INDIRECT_ARGS(n) BOOST_PP_CAT(PP_INDIRECT_ARGS_, n)
# define PP_INDIRECT_ARGS_0(m, ...) m(0, __VA_ARGS__)
# define PP_INDIRECT_ARGS_1(m, ...) m(1, __VA_ARGS__)
# define PP_INDIRECT_ARGS_2(m, ...) m(2, __VA_ARGS__)
# define PP_INDIRECT_ARGS_3(m, ...) m(3, __VA_ARGS__)
# define PP_INDIRECT_ARGS_4(m, ...) m(4, __VA_ARGS__)
# define PP_INDIRECT_ARGS_5(m, ...) m(5, __VA_ARGS__)
# define PP_INDIRECT_ARGS_6(m, ...) m(6, __VA_ARGS__)
# define PP_INDIRECT_ARGS_7(m, ...) m(7, __VA_ARGS__)
# define PP_INDIRECT_ARGS_8(m, ...) m(8, __VA_ARGS__)
# define PP_INDIRECT_ARGS_9(m, ...) m(9, __VA_ARGS__)
# define PP_INDIRECT_ARGS_10(m, ...) m(10, __VA_ARGS__)
# define PP_INDIRECT_ARGS_11(m, ...) m(11, __VA_ARGS__)
# define PP_INDIRECT_ARGS_12(m, ...) m(12, __VA_ARGS__)
# define PP_INDIRECT_ARGS_13(m, ...) m(13, __VA_ARGS__)
# define PP_INDIRECT_ARGS_14(m, ...) m(14, __VA_ARGS__)
# define PP_INDIRECT_ARGS_15(m, ...) m(15, __VA_ARGS__)

#define DETAIL_PP_LOCAL_NAME(name) BOOST_PP_CAT(name, _unique_local_)
#define PP_UNIQUE_LOCAL_NAME(name) BOOST_PP_CAT(DETAIL_PP_LOCAL_NAME(name), __LINE__)
#define PP_UNIQUE_NAME(name) PP_UNIQUE_LOCAL_NAME(BOOST_PP_CAT(name, __COUNTER__))


#endif	/* ZELDA_PP_H */

