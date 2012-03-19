/* 
 * File:   forward.h
 * Author: pfultz
 *
 * Created on November 19, 2011, 7:01 PM
 */

#ifndef FORWARD_H
#define	FORWARD_H

#include <boost/type_traits/remove_reference.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/utility/result_of.hpp>
#include <utility>
#include "pp.h"

namespace zelda {



#ifndef ZELDA_NO_RVALUE_REFS
#define ZELDA_FORWARD_REF(x) x &&
template< class T >
T&& forward( typename boost::remove_reference<T>::type& t )
{
    return static_cast<T&&>(t);
}

template< class T >
T&& forward( typename boost::remove_reference<T>::type&& t )
{
    return static_cast<T&&>(t);
}
#else
#define ZELDA_FORWARD_REF(x) x &
template< class T >
T& forward( typename boost::remove_reference<T>::type& t )
{
    return static_cast<T&>(t);
}
#endif
}


#define DETAIL_ZELDA_FORWARD_MAKE_SEQ_EACH(s, data, x) (x)(const x) 
#define DETAIL_ZELDA_FORWARD_MAKE_SEQ(seq) \
BOOST_PP_SEQ_TRANSFORM(DETAIL_ZELDA_FORWARD_MAKE_SEQ_EACH, data, seq)

#define DETAIL_ZELDA_FORWARD_MAKE_CLASS_SEQ_EACH(s, data, x) class x
#define DETAIL_ZELDA_FORWARD_MAKE_CLASS_SEQ(seq) \
BOOST_PP_SEQ_TRANSFORM(DETAIL_ZELDA_FORWARD_MAKE_CLASS_SEQ_EACH, data, seq)

//forward_pack, template_pack, data
#define DETAIL_ZELDA_FORWARD_S_EACH_I(macro, seq, data, product) \
macro(product, seq, data)

#define DETAIL_ZELDA_FORWARD_S_EACH(data, product) \
DETAIL_ZELDA_FORWARD_S_EACH_I(BOOST_PP_SEQ_ELEM(0, data), BOOST_PP_SEQ_ELEM(1, data), BOOST_PP_SEQ_ELEM(2, data), product)

#define DETAIL_ZELDA_FORWARD_S(macro, seq, data) \
PP_SEQ_FOR_EACH_PRODUCT(DETAIL_ZELDA_FORWARD_S_EACH, (macro)(seq)(data), DETAIL_ZELDA_FORWARD_MAKE_SEQ(seq))
 

#define DETAIL_ZELDA_FORWARD_UNPACK_1 BOOST_PP_SEQ_ENUM

#define DETAIL_ZELDA_FORWARD_UNPACK_ARGS_EACH(z, n, data)\
ZELDA_FORWARD_REF(BOOST_PP_SEQ_ELEM(BOOST_PP_INC(n), data)) PP_CAT(BOOST_PP_SEQ_HEAD(data), n)
#define DETAIL_ZELDA_FORWARD_UNPACK_2(pack, arg)\
BOOST_PP_ENUM(BOOST_PP_SEQ_SIZE(pack), DETAIL_ZELDA_FORWARD_UNPACK_ARGS_EACH, (arg) pack))

#define ZELDA_FORWARD_UNPACK(...) PP_CAT(DETAIL_ZELDA_FORWARD_UNPACK_, PP_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define ZELDA_FORWARD_UNPACK_TEMPLATE(pack) \
ZELDA_FORWARD_UNPACK(DETAIL_ZELDA_FORWARD_MAKE_CLASS_SEQ(pack))

#define DETAIL_ZELDA_FORWARD_UNPACK_FWD_EACH(z, n, data) \
zelda::forward<BOOST_PP_SEQ_ELEM(BOOST_PP_INC(n), data)>(PP_CAT(BOOST_PP_SEQ_HEAD(data), n))
#define ZELDA_FORWARD_UNPACK_FWD(pack, arg) \
BOOST_PP_ENUM(BOOST_PP_SEQ_SIZE(pack), DETAIL_ZELDA_FORWARD_UNPACK_FWD_EACH, (arg) pack))


#define ZELDA_FORWARD_A(macro, data, ...) \
DETAIL_ZELDA_FORWARD_S(macro, PP_ARGS_TO_SEQ(__VA_ARGS__), data)

#define ZELDA_FORWARD_N(macro, data, n, name) \
DETAIL_ZELDA_FORWARD_S(macro, PP_SEQ_GEN(n, name) , data)




#define ZELDA_DETAIL_FORWARD_ADAPT_OP(fwd_pack, tmp_pack, data) \
template<ZELDA_FORWARD_UNPACK_TEMPLATE(tmp_pack)> \
typename Function::template result<Function(ZELDA_FORWARD_UNPACK(fwd_pack))>::type \
operator()(ZELDA_FORWARD_UNPACK(fwd_pack, x)) const \
{ \
    return Function()(ZELDA_FORWARD_UNPACK_FWD(fwd_pack, x)); \
} \

#define ZELDA_DETAIL_FORWARD_ADAPT(z, n, data) \
template<class Function, class Enable> \
struct forward_adaptor<Function, n, Enable> \
{ \
ZELDA_FORWARD_N(ZELDA_DETAIL_FORWARD_ADAPT_OP, data, n, T) \
}; \


namespace zelda {

template<class Function, int N, class Enable>
struct forward_adaptor
{
};

BOOST_PP_REPEAT_FROM_TO(1, 8, ZELDA_DETAIL_FORWARD_ADAPT, data) 
        


}


//#define ZELDA_TEST_FORWARD(fwd_pack, tmp_pack, data) \
//template<ZELDA_FORWARD_UNPACK_TEMPLATE(tmp_pack)> \
//tuple<ZELDA_FORWARD_UNPACK(fwd_pack)> \
//foo(ZELDA_FORWARD_UNPACK(fwd_pack, x)) \
//{ \
//  return tuple<ZELDA_FORWARD_UNPACK(fwd_pack)> \
//  (ZELDA_FORWARD_UNPACK_FWD(fwd_pack, x)); \
//}

//DETAIL_ZELDA_FORWARD_MAKE_SEQ((T1)(T2)(T3))

//ZELDA_FORWARD_N(ZELDA_TEST_FORWARD, data, 2, T)
        
        



////////////////////////////////////////////////////////////////////////////////

//#ifdef ZELDA_HAS_RVALUE_REFS
//#define ZELDA_DETAIL_FORWARD_TYPENAME(...) __VA_ARGS__
//#else
//#define ZELDA_DETAIL_FORWARD_TYPENAME(x) BOOST_PP_CAT(Zelda_Forward_, x)
//#endif
//
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_N(seq) BOOST_PP_SEQ_SIZE(PP_SEQ_TOKEN_FILTER(forward, seq))
// 
//#define ZELDA_DETAIL_FORWARD_TEMPLATE(x) x,
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_SPLIT_I(x, y) x
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_T(s, data, x) \
//ZELDA_DETAIL_FORWARD_TEMPLATE_SPLIT_I(PP_TOKEN_REPLACE(x, forward, ZELDA_DETAIL_FORWARD_TEMPLATE))
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_CLASS_T(s, data, x) class PP_TOKEN_EAT(x, const)
//
//#define ZELDA_DETAIL_FORWARD_TEMPLATE(seq) \
//BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_TEMPLATE_T, data, PP_SEQ_TOKEN_FILTER(forward, seq))
//
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_CLASS(seq) \
//BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_TEMPLATE_CLASS_T, data, ZELDA_DETAIL_FORWARD_TEMPLATE(seq))
//
//#define ZELDA_DETAIL_FORWARD_MAKE_REF(seq) \
//PP_SEQ_TOKEN_REPLACE(forward, ZELDA_FORWARD_REF, seq)
//
//
//#define ZELDA_DETAIL_FORWARD_CONST(x) forward(const x)
//#define ZELDA_DETAIL_FORWARD_MAKE_SEQ_T(s, data, x) \
//BOOST_PP_IIF(PP_TOKEN_HAS(x, forward), (x)(PP_TOKEN_REPLACE(x, forward, ZELDA_DETAIL_FORWARD_CONST)), (x))
//#define ZELDA_DETAIL_FORWARD_MAKE_SEQ(seq) \
//BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_MAKE_SEQ_T, data, seq)
//
//
////seq => (return)(name)(args)...
//#define ZELDA_DETAIL_FORWARD_ARGS(seq) BOOST_PP_SEQ_REST_N(2, seq)
//#define ZELDA_DETAIL_FORWARD_RETURN(seq) PP_TOKEN_REPLACE(BOOST_PP_SEQ_ELEM(0, seq), friend, static)
//#define ZELDA_DETAIL_FORWARD_NAME(seq) BOOST_PP_SEQ_ELEM(1, seq)
//
//#define ZELDA_DETAIL_FORWARD_NAME_F_I(x) \
//BOOST_PP_IIF(PP_TOKEN_HAS(x, operator), PP_UNIQUE_NAME(zzelda_forward_operator_), BOOST_PP_CAT(zzelda_forward_, x))
//#define ZELDA_DETAIL_FORWARD_NAME_F(seq) ZELDA_DETAIL_FORWARD_NAME_F_I(BOOST_PP_SEQ_ELEM(1, seq))
//
//#define ZELDA_DETAIL_FORWARD_RETURN_KEYWORD_I(x) \
//BOOST_PP_IF(PP_TOKEN_HAS(x, friend), friend,) \
//BOOST_PP_IF(PP_TOKEN_HAS(x, static), static,)
//#define ZELDA_DETAIL_FORWARD_RETURN_KEYWORD(seq) ZELDA_DETAIL_FORWARD_RETURN_KEYWORD_I(BOOST_PP_SEQ_ELEM(0, seq))
//
//#define ZELDA_DETAIL_FORWARD_EACH_TEMPLATE(seq) \
//template<BOOST_PP_ENUM(ZELDA_DETAIL_FORWARD_TEMPLATE_N(seq), class Zelda_Forward_T)>
//
//#define ZELDA_DETAIL_FORWARD_EACH_RETURN(seq) \
//ZELDA_DETAIL_FORWARD_RETURN_KEYWORD(seq) \
//typeof_tpl\
//(\
//ZELDA_DETAIL_FORWARD_NAME_F(seq)<BOOST_PP_ENUM(ZELDA_DETAIL_FORWARD_TEMPLATE_N(seq), Zelda_Forward_T)>\
//()\
//)
//
//
//#define ZELDA_DETAIL_FORWARD_EACH(r, seq)  \
//ZELDA_DETAIL_FORWARD_EACH_TEMPLATE(ZELDA_DETAIL_FORWARD_ARGS_SEQ(seq)) \
//
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//
//
//
////Check for keyword forward
//#define ZELDA_DETAIL_FORWARD_N(x, n, ...) n
//#define ZELDA_DETAIL_FORWARD_CHECK(...) ZELDA_DETAIL_FORWARD_N(__VA_ARGS__, 0)
//#define ZELDA_DETAIL_FORWARD_PROBE_forward PP_NIL, 1,
//#define ZELDA_DETAIL_FORWARD_HAS_FORWARD(x) ZELDA_DETAIL_FORWARD_CHECK(PP_JOIN(ZELDA_DETAIL_FORWARD_PROBE_, x))
//
////Remove keyword forward and replace it with a reference
////TODO: Make it a ref when ZELDA_FORWARD_REF is used also
//#define ZELDA_DETAIL_FORWARD_MAKE_REF_forward(...) ZELDA_DETAIL_FORWARD_TYPENAME(ZELDA_FORWARD_REF(__VA_ARGS__))
//#define ZELDA_DETAIL_FORWARD_MAKE_REF_0(x) x
//#define ZELDA_DETAIL_FORWARD_MAKE_REF_1(x) BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_MAKE_REF_, x)
//#define ZELDA_DETAIL_FORWARD_MAKE_REF(x) BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_MAKE_REF_, ZELDA_DETAIL_FORWARD_HAS_FORWARD(x))(x)
//
////Remove forward keyword
//#define ZELDA_DETAIL_FORWARD_REMOVE_forward 
//#define ZELDA_DETAIL_FORWARD_REMOVE_0(x) x
//#define ZELDA_DETAIL_FORWARD_REMOVE_1(x) BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_REMOVE_, x)
//#define ZELDA_DETAIL_FORWARD_REMOVE(x) BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_REMOVE_, ZELDA_DETAIL_FORWARD_HAS_FORWARD(x))(x)
//
////Replaces forward with class
//#define ZELDA_DETAIL_FORWARD_CLASS(x) BOOST_PP_IF(\
//ZELDA_DETAIL_FORWARD_HAS_FORWARD(x), \
//class ZELDA_DETAIL_FORWARD_TYPENAME(ZELDA_DETAIL_FORWARD_REMOVE(x)), \
//x) 
//
////Return forward type
//#define ZELDA_DETAIL_FORWARD_TYPE_forward(x) x,
//#define ZELDA_DETAIL_FORWARD_TYPE_II(x, ...) x 
//#define ZELDA_DETAIL_FORWARD_TYPE_I(x) ZELDA_DETAIL_FORWARD_TYPE_II x
//#define ZELDA_DETAIL_FORWARD_TYPE(x) ZELDA_DETAIL_FORWARD_TYPE_I((BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_TYPE_, x)))
//
////Return a sequence of only the elements that start with the keyword forward
//#define ZELDA_DETAIL_FORWARD_SEQ_HAS_FORWARD_FILTER_P(s, data, x) ZELDA_DETAIL_FORWARD_HAS_FORWARD(x)
//#define ZELDA_DETAIL_FORWARD_SEQ_HAS_FORWARD(seq) BOOST_PP_SEQ_FILTER(ZELDA_DETAIL_FORWARD_SEQ_HAS_FORWARD_FILTER_P, data, seq)
//
////Return a sequence of the types declared by forward
//#define ZELDA_DETAIL_FORWARD_SEQ_FORWARD_TYPES_TRANSFORM(s, data, x) ZELDA_DETAIL_FORWARD_TYPE(x)
//#define ZELDA_DETAIL_FORWARD_SEQ_FORWARD_TYPES(seq) \
//BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_SEQ_FORWARD_TYPES_TRANSFORM, data, ZELDA_DETAIL_FORWARD_SEQ_HAS_FORWARD(seq))
//
////Forward sequence
////0 - not a forward parameter
////1 - forward but not const
////2 - forward and const
////This macro makes the forward seqs from args
//#define ZELDA_DETAIL_FORWARD_MAKE_FORWARD_SEQS_TRANSFORM(s, data, x) BOOST_PP_IF(ZELDA_DETAIL_FORWARD_HAS_FORWARD(x), (1)(2), (0))
//#define ZELDA_DETAIL_FORWARD_MAKE_FORWARD_SEQS(...) BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_MAKE_FORWARD_SEQS_TRANSFORM, data, PP_ARGS_TO_SEQ(__VA_ARGS__)) 
//
////Create default params for template parameter list
////The default template param is used to emulate reference collapsing
////So type T can either be const T or T
//#define ZELDA_DETAIL_FORWARD_SEQ_FORWARD_DEFAULTS_EACH(r, seq, i, x) \
//BOOST_PP_EXPR_IF(BOOST_PP_NOT_EQUAL(BOOST_PP_SEQ_ELEM(i, seq), 0) , \
//(class ZELDA_DETAIL_FORWARD_TYPE(x) = BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_ELEM(i, seq), 2), const) \
//ZELDA_DETAIL_FORWARD_TYPENAME(ZELDA_DETAIL_FORWARD_TYPE(x)) ))
//#define ZELDA_DETAIL_FORWARD_SEQ_FORWARD_DEFAULTS(args_seq, forward_seq) \
//BOOST_PP_SEQ_FOR_EACH_I(ZELDA_DETAIL_FORWARD_SEQ_FORWARD_DEFAULTS_EACH, forward_seq, args_seq) 
//
////Return a template parameter list as a sequence
//#define ZELDA_DETAIL_FORWARD_MAKE_TMPL_SEQ_TRANSFORM(s, data, x) ZELDA_DETAIL_FORWARD_CLASS(x)
//#define ZELDA_DETAIL_FORWARD_MAKE_TMPL_SEQ(template_seq, args_seq, forward_seq) \
//BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_MAKE_TMPL_SEQ_TRANSFORM, data, template_seq) \
//ZELDA_DETAIL_FORWARD_SEQ_FORWARD_DEFAULTS(args_seq, forward_seq)
//
////Return the argument list as a sequence
//#define ZELDA_DETAIL_FORWARD_MAKE_ARGS_SEQ_EACH(r, seq, i, x) \
//(BOOST_PP_EXPR_IF(BOOST_PP_EQUAL(BOOST_PP_SEQ_ELEM(i, seq), 2), const) ZELDA_DETAIL_FORWARD_MAKE_REF(x))
//#define ZELDA_DETAIL_FORWARD_MAKE_ARGS_SEQ(args_seq, forward_seq) \
//BOOST_PP_SEQ_FOR_EACH_I(ZELDA_DETAIL_FORWARD_MAKE_ARGS_SEQ_EACH, forward_seq, args_seq) 
//
////Makes it a sequence of sequence
//#define ZELDA_DETAIL_FORWARD_DOUBLE_SEQ_TRANSFORM(s, data, x) (x)
//#define ZELDA_DETAIL_FORWARD_DOUBLE_SEQ(seq) BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_DOUBLE_SEQ_TRANSFORM, data, seq) 
//
//
////Remove parens
//#define ZELDA_DETAIL_FORWARD_REMOVE_PAREN_I(...) __VA_ARGS__
//#define ZELDA_DETAIL_FORWARD_REMOVE_PAREN(...) ZELDA_DETAIL_FORWARD_REMOVE_PAREN_I __VA_ARGS__
//#define ZELDA_DETAIL_FORWARD_ELEM(i, seq) ZELDA_DETAIL_FORWARD_REMOVE_PAREN(BOOST_PP_SEQ_ELEM(i, seq))
//
////Forward body, if it starts with returns, then it uses the trailing return type
//#define ZELDA_DETAIL_FORWARD_BODY_PROBE_returns(...) PP_NIL, 1,
//#define ZELDA_DETAIL_FORWARD_BODY_HAS_RETURNS(x) \
//ZELDA_DETAIL_FORWARD_CHECK(PP_JOIN(ZELDA_DETAIL_FORWARD_BODY_PROBE_, x))
//#define ZELDA_DETAIL_FORWARD_BODY_returns(...) -> decltype(__VA_ARGS__) \
//{ return __VA_ARGS__; } 
//#define ZELDA_DETAIL_FORWARD_BODY(...)  ZELDA_DETAIL_FORWARD_REMOVE_PAREN(\
//BOOST_PP_IF(ZELDA_DETAIL_FORWARD_BODY_HAS_RETURNS(__VA_ARGS__), \
//(BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_BODY_, __VA_ARGS__)), \
//({ __VA_ARGS__ }) ))
//
////seq => ((template))((return))((name))((args))((body))(forward_seq)...
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_SEQ(seq) PP_ARGS_TO_SEQ(ZELDA_DETAIL_FORWARD_ELEM(0, seq))
//#define ZELDA_DETAIL_FORWARD_RETURN_SEQ(seq) ZELDA_DETAIL_FORWARD_ELEM(1, seq)
//#define ZELDA_DETAIL_FORWARD_NAME_SEQ(seq) ZELDA_DETAIL_FORWARD_ELEM(2, seq)
//#define ZELDA_DETAIL_FORWARD_ARGS_SEQ(seq) PP_ARGS_TO_SEQ(ZELDA_DETAIL_FORWARD_ELEM(3, seq))
//#define ZELDA_DETAIL_FORWARD_BODY_SEQ(seq) ZELDA_DETAIL_FORWARD_ELEM(4, seq)
//#define ZELDA_DETAIL_FORWARD_FORWARD_SEQ(seq) BOOST_PP_SEQ_REST_N(5, seq)
//
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_LIST(seq) \
//BOOST_PP_SEQ_ENUM(ZELDA_DETAIL_FORWARD_MAKE_TMPL_SEQ( \
//ZELDA_DETAIL_FORWARD_TEMPLATE_SEQ(seq), ZELDA_DETAIL_FORWARD_ARGS_SEQ(seq), ZELDA_DETAIL_FORWARD_FORWARD_SEQ(seq)))
//#define ZELDA_DETAIL_FORWARD_ARGS_LIST(seq) BOOST_PP_SEQ_ENUM(ZELDA_DETAIL_FORWARD_MAKE_ARGS_SEQ( \
//ZELDA_DETAIL_FORWARD_ARGS_SEQ(seq), ZELDA_DETAIL_FORWARD_FORWARD_SEQ(seq)))
//
//#define ZELDA_DETAIL_FORWARD_EACH(r, seq) \
//template<ZELDA_DETAIL_FORWARD_TEMPLATE_LIST(seq)> \
//ZELDA_DETAIL_FORWARD_RETURN_SEQ(seq) ZELDA_DETAIL_FORWARD_NAME_SEQ(seq) \
//(ZELDA_DETAIL_FORWARD_ARGS_LIST(seq)) \
//ZELDA_DETAIL_FORWARD_BODY(ZELDA_DETAIL_FORWARD_BODY_SEQ(seq))
//
// 
//
//#ifdef ZELDA_HAS_RVALUE_REFS
//
//#define ZELDA_DETAIL_FORWARD(...) __VA_ARGS__
//
//#define ZELDA_DETAIL_FORWARD_BODY_I(...) { __VA_ARGS__ }
//#define ZELDA_DETAIL_FORWARD_ARGS_EACH(s, data, x) ZELDA_DETAIL_FORWARD_MAKE_REF(x)
//#define ZELDA_DETAIL_FORWARD_ARGS_I(...) (PP_ARGS_TRANSFORM(ZELDA_DETAIL_FORWARD_ARGS_EACH, data, __VA_ARGS__)) ZELDA_DETAIL_FORWARD_BODY_I
//#define ZELDA_DETAIL_FORWARD_NAME_I(...) __VA_ARGS__ ZELDA_DETAIL_FORWARD_ARGS_I
//#define ZELDA_DETAIL_FORWARD_RETURN_I(...) __VA_ARGS__ ZELDA_DETAIL_FORWARD_NAME_I
//#define ZELDA_DETAIL_FORWARD_TEMPLATE_EACH(s, data, x) ZELDA_DETAIL_FORWARD_CLASS(x)
//#define ZELDA_DETAIL_FORWARD_template(...) template<PP_ARGS_TRANSFORM(ZELDA_DETAIL_FORWARD_TEMPLATE_EACH, data, __VA_ARGS__)> ZELDA_DETAIL_FORWARD_RETURN_I
//
//#else
//
//#define ZELDA_DETAIL_FORWARD(seq) \
//BOOST_PP_SEQ_FOR_EACH_PRODUCT(ZELDA_DETAIL_FORWARD_EACH, \
//seq \
//ZELDA_DETAIL_FORWARD_MAKE_FORWARD_SEQS(ZELDA_DETAIL_FORWARD_REMOVE_PAREN(ZELDA_DETAIL_FORWARD_ELEM(3, seq))) )
//
//#define ZELDA_DETAIL_FORWARD_BODY_I(...) (((PP_EXPAND(__VA_ARGS__))))
//#define ZELDA_DETAIL_FORWARD_ARGS_I(...) (((PP_EXPAND(__VA_ARGS__)))) ZELDA_DETAIL_FORWARD_BODY_I
//#define ZELDA_DETAIL_FORWARD_NAME_I(...) (((PP_EXPAND(__VA_ARGS__)))) ZELDA_DETAIL_FORWARD_ARGS_I
//#define ZELDA_DETAIL_FORWARD_RETURN_I(...) (((PP_EXPAND(__VA_ARGS__)))) ZELDA_DETAIL_FORWARD_NAME_I
//#define ZELDA_DETAIL_FORWARD_template(...) (((PP_EXPAND(__VA_ARGS__)))) ZELDA_DETAIL_FORWARD_RETURN_I
//
//#endif
//
//#define ZELDA_FORWARD(x) ZELDA_DETAIL_FORWARD(BOOST_PP_CAT(ZELDA_DETAIL_FORWARD_, x))




//ZELDA_DETAIL_FORWARD_HAS_FORWARD(forward T)
//ZELDA_DETAIL_FORWARD_CLASS(forward T)

//BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_FORWARD_MAKE_TMPL_SEQ_TRANSFORM, data, (forward T)(class T))

//ZELDA_DETAIL_FORWARD_TYPE(forward(T) y)

//ZELDA_FORWARD
//(
//template(forward T, class U)
//(pair<T, U>)(foo)(U x, forward(T) y)
//(
//return bar(forward<T>(y));
//)
//)

//template< class ZELDA_FORWARD_T, class U, class T = const ZELDA_FORWARD_T > 
//T & foo(U x, ZELDA_FORWARD_T & y)
//{return bar(forward < T > (y));}
//template< class ZELDA_FORWARD_T, class U, class T = ZELDA_FORWARD_T > 
//T & foo(U x, const ZELDA_FORWARD_T & y)
//{return bar(forward < T > (y));}

#endif	/* FORWARD_H */

