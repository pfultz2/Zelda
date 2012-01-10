/* 
 * File:   xfunction.h
 * Author: pfultz
 *
 * Created on January 4, 2012, 12:36 PM
 */

#ifndef XFUNCTION_H
#define	XFUNCTION_H

#include "pp.h"
#include "forward.h"
#include "closure.h"

#define ZELDA_HAS_DECLTYPE

//#define ZELDA_HAS_DECLTYPE

//Utility
//#define ZELDA_DETAIL_XTYPE_TEMPLATE(n, name) template<BOOST_PP_ENUM_PARAMS(n, class name)>
//
//#define ZELDA_DETAIL_XTYPE_TEMPLATE_CLASS(n, name, type) type<BOOST_PP_ENUM_PARAMS(n, name)>
//
//
//
//#define ZELDA_DETAIL_XTYPE_VARS_EACH(r, data, i, x) BOOST_PP_CAT(data, i) x;
//#define ZELDA_DETAIL_XTYPE_VARS(name, ...)  PP_ARGS_FOREACH_I(ZELDA_DETAIL_XTYPE_VARS_EACH, name, __VA_ARGS__)
//
//#define ZELDA_DETAIL_XTYPE_ARGS_REF_EACH(r, i, data) & BOOST_PP_SEQ_ELEM(i, data)
//#define ZELDA_DETAIL_XTYPE_ARGS_REF(...) \
//BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_ARGS_REF_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__))
//
////ZELDA_DETAIL_XTYPE_DECLARE((template_type)(return_type)(function_name), params...)
//#define ZELDA_DETAIL_XTYPE_DECLARE(seq, ...)\
//BOOST_PP_SEQ_ELEM(1, seq) BOOST_PP_SEQ_ELEM(2, seq) (ZELDA_DETAIL_XTYPE_ARGS(BOOST_PP_SEQ_ELEM(0, seq), __VA_ARGS__))
//#define ZELDA_DETAIL_XTYPE_DECLARE_TEMPLATE(seq, ...) \
//ZELDA_DETAIL_XTYPE_TEMPLATE(PP_NARGS(__VA_ARGS__), BOOST_PP_SEQ_ELEM(0, seq)) ZELDA_DETAIL_XTYPE_DECLARE(seq, __VA_ARGS__)\
//
//
//#define ZELDA_DETAIL_XTYPE_CONSTUCT_EACH(r, i, data) PP_ARGS_ELEM(i, BOOST_PP_SEQ_ELEM(0, data)) (PP_ARGS_ELEM(i, BOOST_PP_SEQ_ELEM(1, data)))
//#define ZELDA_DETAIL_XTYPE_CONSTRUCT(seq) \
//BOOST_PP_ENUM(PP_NARGS(BOOST_PP_SEQ_ELEM(0, seq)), ZELDA_DETAIL_XTYPE_CONSTUCT_EACH, seq)


///////////////////////////////////////////////////////////////////////////////////////////
//#define ZELDA_DETAIL_XTYPE_EACH(r, i, data) ZELDA_TEMPLATE_TYPE ## i BOOST_PP_SEQ_ELEM(i, data)
//
//#define ZELDA_DETAIL_XTYPE_EACH_VAR(r, data, i, x) static ZELDA_TEMPLATE_TYPE ## i x;
//
////#define ZELDA_DETAIL_XTYPE_TEMPLATE(n) template<BOOST_PP_ENUM_PARAMS(n, class ZELDA_TEMPLATE_TYPE)>
//
//#define ZELDA_DETAIL_XTYPE_ARGS(...) BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__))
//
//#define ZELDA_DETAIL_XTYPE_ARGS_FORWARD_EACH(r, i, data) std::forward<ZELDA_TEMPLATE_TYPE ## i>(data ## i)
//
//#define ZELDA_DETAIL_XTYPE_CLOSURE_VARS_EACH(r, i, data) ZELDA_TEMPLATE_CLOSURE ## i BOOST_PP_SEQ_ELEM(i, data)
//#define ZELDA_DETAIL_XTYPE_CLOSURE_ARGS(...) BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_CLOSURE_VARS_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__))
//
//#define ZELDA_DETAIL_XTYPE_CLOSURE_VARS_CONSTUCTOR_EACH(r, i, data) BOOST_PP_SEQ_ELEM(i, data) (BOOST_PP_SEQ_ELEM(i, data))
//#define ZELDA_DETAIL_XTYPE_CLOSURE_VARS_DECLARE_EACH(r, data, i, x) ZELDA_TEMPLATE_CLOSURE ## i x;
//#define ZELDA_DETAIL_XTYPE_CLOSURE_VARS(name, ...) \
//PP_ARGS_FOREACH_I(ZELDA_DETAIL_XTYPE_CLOSURE_VARS_DECLARE_EACH, data, __VA_ARGS__)\
//name(BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_CLOSURE_VARS_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__))) :\
//BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_CLOSURE_VARS_CONSTUCTOR_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__))\
//{}

#define ZELDA_DETAIL_XTYPE_ARGS_EACH(r, i, data) \
BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, data), i) ZELDA_FORWARD_REF(BOOST_PP_SEQ_ELEM(BOOST_PP_INC(i), data))
#define ZELDA_DETAIL_XTYPE_ARGS(name, ...) \
BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_ARGS_EACH, BOOST_PP_EXPAND((name)PP_ARGS_TO_SEQ(__VA_ARGS__)))

#define ZELDA_DETAIL_XTYPE_VARS_EACH(r, data, i, x) BOOST_PP_CAT(data, i) x;
#define ZELDA_DETAIL_XTYPE_VARS(name, ...)  PP_ARGS_FOREACH_I(ZELDA_DETAIL_XTYPE_VARS_EACH, name, __VA_ARGS__)

#ifdef ZELDA_HAS_DECLTYPE
#define ZELDA_DETAIL_XTYPE_RETURN(n) auto
#define ZELDA_DETAIL_XTYPE_BODY(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }
#define ZELDA_DETAIL_XTYPE_DEDUCER(...)

#else
#define ZELDA_DETAIL_XTYPE_DEDUCER(...) \
template<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), Zelda_Template_Type)> \
struct deducer\
{\
ZELDA_DETAIL_XTYPE_VARS(static Zelda_Template_Type, __VA_ARGS__)\
struct deduce;\
};

#define ZELDA_DETAIL_XTYPE_RETURN(n) typename deducer<BOOST_PP_ENUM_PARAMS(n, Zelda_Template_Type)>::deduce::type
#define ZELDA_DETAIL_XTYPE_BODY(n, ...) \
{ return __VA_ARGS__; } \
template<BOOST_PP_ENUM_PARAMS(n, Zelda_Template_Type)> \
struct deducer<BOOST_PP_ENUM_PARAMS(n, Zelda_Template_Type)>::deduce \
{\
typedef typeof(__VA_ARGS__) type;\
};

#endif

#define ZELDA_DETAIL_XTYPE_HEADER_PARAMS_EACH(z, n, data) forward(Zelda_Template_Type ## n) x ## n
#define ZELDA_DETAIL_XTYPE_HEADER_PARAMS(n) BOOST_PP_ENUM(n, ZELDA_DETAIL_XTYPE_HEADER_PARAMS_EACH, data) 

#define ZELDA_DETAIL_XTYPE_HEADER_FORWARD_PARAMS_EACH(z, n, data) zelda::forward<Zelda_Template_Type ## n>(x ## n)
#define ZELDA_DETAIL_XTYPE_HEADER_FORWARD_PARAMS(n) BOOST_PP_ENUM(n, ZELDA_DETAIL_XTYPE_HEADER_FORWARD_PARAMS_EACH, data) 

#define ZELDA_DETAIL_XTYPE_HEADER(...) \
ZELDA_DETAIL_XTYPE_DEDUCER(__VA_ARGS__) \
void zelda_run(...); \
ZELDA_FORWARD \
(template(BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), forward Zelda_Template_Type)) \
(ZELDA_DETAIL_XTYPE_RETURN(PP_NARGS(__VA_ARGS__))) \
(operator()) \
(ZELDA_DETAIL_XTYPE_HEADER_PARAMS(PP_NARGS(__VA_ARGS__))) \
( \
return zelda_run< BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), Zelda_Template_Type)> \
(ZELDA_DETAIL_XTYPE_HEADER_FORWARD_PARAMS(PP_NARGS(__VA_ARGS__))); \
) \
) \
template< BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), class Zelda_Template_Type) > \
ZELDA_DETAIL_XTYPE_RETURN(n) zelda_run \
(ZELDA_DETAIL_XTYPE_ARGS(Zelda_Template_Type, __VA_ARGS__))

/*******************************************************************************
 *******************************************************************************/

#ifdef ZELDA_HAS_DECLTYPE

//********************************************
#define ZELDA_DETAIL_XTYPE_I(...) ZELDA_DETAIL_XTYPE_BODY(__VA_ARGS__)\
}

//********************************************
#define ZELDA_XTYPE(name, ...) \
struct name \
{\
ZELDA_DETAIL_XTYPE_HEADER(__VA_ARGS__)\
ZELDA_DETAIL_XTYPE_I


#define ZELDA_XTYPE_CLOSURE(name, ...) \
struct name \
{\
ZELDA_CLOSURE_HEADER(operator(), zelda_closure, __VA_ARGS__) \
ZELDA_DETAIL_XTYPE_CLOSURE_I

#define ZELDA_DETAIL_XTYPE_CLOSURE_I(...) \
ZELDA_DETAIL_XTYPE_HEADER(__VA_ARGS__) \
ZELDA_DETAIL_XTYPE_CLOSURE_II

#define ZELDA_DETAIL_XTYPE_CLOSURE_II(...) \
ZELDA_DETAIL_XTYPE_BODY(__VA_ARGS__) }; }

#define ZELDA_DETAIL_XTYPE_PIPE_NARGS(...) BOOST_PP_DEC(PP_NARGS(__VA_ARGS__))

//TODO: Handle pipes that only have one parameter
#define ZELDA_XTYPE_PIPE(name, ...) ZELDA_DETAIL_XTYPE_PIPE_II(name, __VA_ARGS__)

//TODO: Use forward instead of rvalue reference
#define ZELDA_DETAIL_XTYPE_PIPE_II(name, ...) \
struct name \
{\
ZELDA_CLOSURE_HEADER(operator(), zelda_closure, PP_ARGS_POP_FRONT(__VA_ARGS__)) \
template<class Zelda_Pipe>\
friend auto operator|(Zelda_Pipe && p, zelda_closure c) \
ZELDA_DETAIL_XTYPE_BODY(c(zelda::forward<Zelda_Pipe>(p))) \
ZELDA_DETAIL_XTYPE_HEADER(PP_ARGS_FRONT(__VA_ARGS__)) \
ZELDA_DETAIL_XTYPE_PIPE_III

#define ZELDA_DETAIL_XTYPE_PIPE_III(...) \
ZELDA_DETAIL_XTYPE_BODY(__VA_ARGS__) }; } 



//#include <utility>
//
////TODO: Add rvalue refs to function call
//
//#define ZELDA_DETAIL_XTYPE_ARGS_FORWARD_EACH(r, i, data) \
//std::forward<BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, data), i)>(BOOST_PP_SEQ_ELEM(BOOST_PP_INC(i), data))
//#define ZELDA_DETAIL_XTYPE_ARGS_FORWARD(name, ...)  BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_ARGS_FORWARD_EACH, BOOST_PP_EXPAND((name)PP_ARGS_TO_SEQ(__VA_ARGS__)))
//
//#define ZELDA_DETAIL_XTYPE_ARGS_FORWARD_CURRY_EACH(r, i, data) \
//std::forward<BOOST_PP_CAT(BOOST_PP_SEQ_ELEM(0, data), BOOST_PP_INC(i))>(BOOST_PP_SEQ_ELEM(BOOST_PP_INC(BOOST_PP_INC(i)), data))
//#define ZELDA_DETAIL_XTYPE_ARGS_FORWARD_CURRY(name, ...) BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_XTYPE_ARGS_FORWARD_EACH, BOOST_PP_EXPAND((name)PP_ARGS_TO_SEQ(__VA_ARGS__)))
//
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_BODY(...) -> decltype(__VA_ARGS__) { return __VA_ARGS__; }
//
//#define ZELDA_DETAIL_XTYPE_HEADER(...)\
//ZELDA_DETAIL_XTYPE_DECLARE_TEMPLATE((ZELDA_TEMPLATE_TYPE)(auto)(operator()), __VA_ARGS__)
//
////********************************************
////#define ZELDA_DETAIL_XTYPE_SELF_FORWARD_EACH(r, i, data) std::forward<ZELDA_TEMPLATE_TYPE ## i>(data ## i)
////#define ZELDA_DETAIL_XTYPE_SELF(name, ...) \
////name & zelda_get_me() { return *this; } \
////template<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), class ZELDA_TEMPLATE_TYPE)>\
////auto self(BOOST_PP_ENUM_BINARY_PARAMS(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_TYPE, && zelda_x_)) \
////ZELDA_DETAIL_XTYPE_BODY(zelda_get_me()(ZELDA_DETAIL_XTYPE_ARGS_FORWARD(ZELDA_TEMPLATE_TYPE, BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), zelda_x_))))
//
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_I(...) ZELDA_DETAIL_XTYPE_BODY(__VA_ARGS__)\
//}
//
////********************************************
//#define ZELDA_XTYPE(name, ...) \
//struct name \
//{\
//ZELDA_DETAIL_XTYPE_HEADER(__VA_ARGS__)\
//ZELDA_DETAIL_XTYPE_I
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(...) \
//ZELDA_DETAIL_XTYPE_TEMPLATE(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_CLOSURE)\
//struct zelda_closure
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_CLOSURE_HEADER(...) \
//ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(__VA_ARGS__); \
//ZELDA_DETAIL_XTYPE_DECLARE_TEMPLATE( \
//(ZELDA_TEMPLATE_CLOSURE) \
//(ZELDA_DETAIL_XTYPE_TEMPLATE_CLASS(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_CLOSURE, zelda_closure)) \
//(operator()), BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), && x))\
//{ return ZELDA_DETAIL_XTYPE_TEMPLATE_CLASS(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_CLOSURE, zelda_closure)(BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), x)); }\
//ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(__VA_ARGS__)\
//{\
//\
//ZELDA_DETAIL_XTYPE_VARS(ZELDA_TEMPLATE_CLOSURE, ZELDA_DETAIL_XTYPE_ARGS_REF(__VA_ARGS__))\
//ZELDA_DETAIL_XTYPE_DECLARE((ZELDA_TEMPLATE_CLOSURE)()(zelda_closure), BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), & x))\
//: ZELDA_DETAIL_XTYPE_CONSTRUCT((__VA_ARGS__)(BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), x))) {}\
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_CLOSURE_BODY(...) \
//ZELDA_DETAIL_XTYPE_BODY(__VA_ARGS__) \
//};
//
//
////********************************************
//#define ZELDA_XTYPE_CLOSURE(name, ...) \
//struct name \
//{\
//ZELDA_DETAIL_XTYPE_CLOSURE_HEADER(__VA_ARGS__) \
//ZELDA_DETAIL_XTYPE_CLOSURE_I
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_CLOSURE_I(...) \
//ZELDA_DETAIL_XTYPE_HEADER(__VA_ARGS__) \
//ZELDA_DETAIL_XTYPE_CLOSURE_II
//
////********************************************
//#define ZELDA_DETAIL_XTYPE_CLOSURE_II(...) \
//ZELDA_DETAIL_XTYPE_CLOSURE_BODY(__VA_ARGS__) \
//}
//
//#define ZELDA_DETAIL_XTYPE_PIPE_NARGS(...) BOOST_PP_DEC(PP_NARGS(__VA_ARGS__))
//
////TODO: Handle pipes that only have one parameter
//#define ZELDA_XTYPE_PIPE(name, ...) ZELDA_DETAIL_XTYPE_PIPE_II(name, __VA_ARGS__)
//
//#define ZELDA_DETAIL_XTYPE_PIPE_II(name, ...) \
//struct name \
//{\
//ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(PP_ARGS_POP_FRONT(__VA_ARGS__)) \
//template<class ZELDA_TEMPLATE_TYPE, BOOST_PP_ENUM_PARAMS(ZELDA_DETAIL_XTYPE_PIPE_NARGS(__VA_ARGS__), class ZELDA_TEMPLATE_TYPE)>\
//auto operator()(ZELDA_TEMPLATE_TYPE && p, BOOST_PP_ENUM_BINARY_PARAMS(ZELDA_DETAIL_XTYPE_PIPE_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_TYPE, && x))\
//ZELDA_DETAIL_XTYPE_BODY(\
//zelda_closure<BOOST_PP_ENUM_PARAMS(ZELDA_DETAIL_XTYPE_PIPE_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_TYPE)>\
//(BOOST_PP_ENUM_PARAMS(ZELDA_DETAIL_XTYPE_PIPE_NARGS(__VA_ARGS__), x))(p)) \
//ZELDA_DETAIL_XTYPE_CLOSURE_HEADER(PP_ARGS_POP_FRONT(__VA_ARGS__)) \
//template<class ZELDA_TEMPLATE_PIPE>\
//friend auto operator|(ZELDA_TEMPLATE_PIPE && p, zelda_closure& c)\
//ZELDA_DETAIL_XTYPE_BODY(c(std::forward<ZELDA_TEMPLATE_PIPE>(p)))\
//template<class ZELDA_TEMPLATE_PIPE>\
//friend auto operator|(ZELDA_TEMPLATE_PIPE && p, zelda_closure&& c)\
//ZELDA_DETAIL_XTYPE_BODY(c(std::forward<ZELDA_TEMPLATE_PIPE>(p)))\
//ZELDA_DETAIL_XTYPE_HEADER(PP_ARGS_FRONT(__VA_ARGS__)) \
//ZELDA_DETAIL_XTYPE_PIPE_III
//
//#define ZELDA_DETAIL_XTYPE_PIPE_III(...) \
//ZELDA_DETAIL_XTYPE_CLOSURE_BODY(__VA_ARGS__) \
//}

#else

#define ZELDA_DETAIL_XTYPE_FORWARD_Q_SEQ_TRANSFORMER(s, data, elem) (elem)(const elem)
#define ZELDA_DETAIL_XTYPE_FORWARD_Q_SEQ(seq) BOOST_PP_SEQ_TRANSFORM(ZELDA_DETAIL_XTYPE_FORWARD_Q_SEQ_TRANSFORMER, data, seq)

#define ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQ_I_EACH(z, i, name) (name ## i)
#define ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQ_I(n, name) BOOST_PP_REPEAT(n, ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQ_I_EACH, name) 

#define ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQS(n, name) ZELDA_DETAIL_XTYPE_FORWARD_Q_SEQ(ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQ_I(n, name))

#define ZELDA_DETAIL_XTYPE_FORWARD_ENUM_PARAM_EACH(z, i, data) BOOST_PP_SEQ_ELEM(i, data) & zelda_x_ ## i
#define ZELDA_DETAIL_XTYPE_FORWARD_ENUM_PARAM(seq) BOOST_PP_ENUM(BOOST_PP_SEQ_SIZE(seq), ZELDA_DETAIL_XTYPE_FORWARD_ENUM_PARAM_EACH, seq) 

#define ZELDA_DETAIL_XTYPE_FORWARD_ARGS(seq) BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), zelda_x_)

//seq = (const ZELDA_TEMPLATE_TYPE0)(ZELDA_TEMPLATE_TYPE1)(const ZELDA_TEMPLATE_TYPE0)
#define ZELDA_DETAIL_XTYPE_FORWARD_EACH(r, seq) \
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), class ZELDA_TEMPLATE_TYPE)> \
typename deducer<BOOST_PP_SEQ_ENUM(seq)>::deduce::type \
operator()(ZELDA_DETAIL_XTYPE_FORWARD_ENUM_PARAM(seq)) \
{ return zelda_run<BOOST_PP_SEQ_ENUM(seq)>(ZELDA_DETAIL_XTYPE_FORWARD_ARGS(seq)); }

#define ZELDA_DETAIL_XTYPE_FORWARD(n) \
BOOST_PP_SEQ_FOR_EACH_PRODUCT(ZELDA_DETAIL_XTYPE_FORWARD_EACH, ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQS(n, ZELDA_TEMPLATE_TYPE))

#define ZELDA_DETAIL_XTYPE_DECLARE_RUN(...) \
void zelda_run(...)

#define ZELDA_DETAIL_XTYPE_DEFINE_RUN(...) \
template<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), class ZELDA_TEMPLATE_TYPE)> \
typename deducer<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_TYPE)>::deduce::type \
zelda_run(ZELDA_DETAIL_XTYPE_ARGS(ZELDA_TEMPLATE_TYPE, ZELDA_DETAIL_XTYPE_ARGS_REF(__VA_ARGS__)))\




//********************************************
#define ZELDA_DETAIL_XTYPE_HEADER(...) \
ZELDA_DETAIL_XTYPE_TEMPLATE(PP_NARGS(__VA_ARGS__), ZELDA_DEDUCE_TEMPLATE_TYPE)\
struct deducer\
{\
ZELDA_DETAIL_XTYPE_VARS(static ZELDA_DEDUCE_TEMPLATE_TYPE, __VA_ARGS__)\
struct deduce;\
};\
ZELDA_DETAIL_XTYPE_DECLARE_RUN(__VA_ARGS__); \
ZELDA_DETAIL_XTYPE_FORWARD(PP_NARGS(__VA_ARGS__)) \
ZELDA_DETAIL_XTYPE_DEFINE_RUN(__VA_ARGS__)

//********************************************
#define ZELDA_DETAIL_XTYPE_BODY(n, ...) \
{ return __VA_ARGS__; } \
ZELDA_DETAIL_XTYPE_TEMPLATE(n, ZELDA_DEDUCE_TEMPLATE_TYPE)\
struct deducer<BOOST_PP_ENUM_PARAMS(n, ZELDA_DEDUCE_TEMPLATE_TYPE)>::deduce\
{\
typedef typeof(__VA_ARGS__) type;\
};\

//********************************************
//TODO: Check that number of args <= 8
#define ZELDA_XTYPE(name, ...) \
struct name \
{\
ZELDA_DETAIL_XTYPE_HEADER(__VA_ARGS__)\
BOOST_PP_CAT(ZELDA_DETAIL_XTYPE_I_, PP_NARGS(__VA_ARGS__))

//********************************************
#define ZELDA_DETAIL_XTYPE_I(n, ...) \
ZELDA_DETAIL_XTYPE_BODY(n, __VA_ARGS__)\
}

#define ZELDA_DETAIL_XTYPE_I_0(...) ZELDA_DETAIL_XTYPE_I(0, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_1(...) ZELDA_DETAIL_XTYPE_I(1, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_2(...) ZELDA_DETAIL_XTYPE_I(2, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_3(...) ZELDA_DETAIL_XTYPE_I(3, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_4(...) ZELDA_DETAIL_XTYPE_I(4, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_5(...) ZELDA_DETAIL_XTYPE_I(5, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_6(...) ZELDA_DETAIL_XTYPE_I(6, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_I_7(...) ZELDA_DETAIL_XTYPE_I(7, __VA_ARGS__)



#define ZELDA_DETAIL_XTYPE_CLOSURE_FORWARD_EACH(r, seq) \
template<BOOST_PP_ENUM_PARAMS(BOOST_PP_SEQ_SIZE(seq), class ZELDA_CLOSURE_TYPE)> \
zelda_closure<BOOST_PP_SEQ_ENUM(seq)> \
operator()(ZELDA_DETAIL_XTYPE_FORWARD_ENUM_PARAM(seq)) \
{ return zelda_closure<BOOST_PP_SEQ_ENUM(seq)>(ZELDA_DETAIL_XTYPE_FORWARD_ARGS(seq)); }

#define ZELDA_DETAIL_XTYPE_CLOSURE_FORWARD(n) \
BOOST_PP_SEQ_FOR_EACH_PRODUCT(ZELDA_DETAIL_XTYPE_CLOSURE_FORWARD_EACH, ZELDA_DETAIL_XTYPE_FORWARD_MAKE_SEQS(n, ZELDA_CLOSURE_TYPE))


//********************************************
#define ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(...) \
ZELDA_DETAIL_XTYPE_TEMPLATE(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_CLOSURE)\
struct zelda_closure

//********************************************
#define ZELDA_DETAIL_XTYPE_CLOSURE_HEADER(...) \
ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(__VA_ARGS__); \
ZELDA_DETAIL_XTYPE_CLOSURE_FORWARD(PP_NARGS(__VA_ARGS__)) \
ZELDA_DETAIL_XTYPE_DECLARE_CLOSURE(__VA_ARGS__)\
{\
\
ZELDA_DETAIL_XTYPE_VARS(ZELDA_TEMPLATE_CLOSURE, ZELDA_DETAIL_XTYPE_ARGS_REF(__VA_ARGS__))\
ZELDA_DETAIL_XTYPE_DECLARE((ZELDA_TEMPLATE_CLOSURE)()(zelda_closure), BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), & x))\
: ZELDA_DETAIL_XTYPE_CONSTRUCT((__VA_ARGS__)(BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), x))) {}\

//********************************************
#define ZELDA_DETAIL_XTYPE_CLOSURE_BODY(n, ...) \
ZELDA_DETAIL_XTYPE_BODY(n, __VA_ARGS__) \
};

//********************************************
#define ZELDA_XTYPE_CLOSURE(name, ...) \
struct name \
{\
ZELDA_DETAIL_XTYPE_CLOSURE_HEADER(__VA_ARGS__) \
ZELDA_DETAIL_XTYPE_CLOSURE_I

//********************************************
#define ZELDA_DETAIL_XTYPE_CLOSURE_I(...) \
ZELDA_DETAIL_XTYPE_HEADER(__VA_ARGS__) \
BOOST_PP_CAT(ZELDA_DETAIL_XTYPE_CLOSURE_II_, PP_NARGS(__VA_ARGS__))

//********************************************
#define ZELDA_DETAIL_XTYPE_CLOSURE_II(n, ...) \
ZELDA_DETAIL_XTYPE_CLOSURE_BODY(n, __VA_ARGS__) \
}

#define ZELDA_DETAIL_XTYPE_CLOSURE_II_0(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(0, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_1(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(1, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_2(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(2, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_3(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(3, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_4(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(4, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_5(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(5, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_6(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(6, __VA_ARGS__)
#define ZELDA_DETAIL_XTYPE_CLOSURE_II_7(...) ZELDA_DETAIL_XTYPE_CLOSURE_II(7, __VA_ARGS__)

#endif

#define ZELDA_XFUNCTION(...) ZELDA_XTYPE(PP_UNIQUE_NAME(zelda_private_xtype_), __VA_ARGS__)

#define ZELDA_XCLOSURE(...) ZELDA_XTYPE_CLOSURE(PP_UNIQUE_NAME(zelda_private_xclosure_), __VA_ARGS__)

#define ZELDA_XPIPE(...) ZELDA_XTYPE_PIPE(PP_UNIQUE_NAME(zelda_private_xpipe_), __VA_ARGS__)
    
//ZELDA_XCLOSURE(x)(y)(x + y) max;

//ZELDA_XCLOSURE(x)(y)(x > y ? x : y) max;

//ZELDA_DETAIL_XTYPE_FORWARD(2)
//#define MACRO(r, product) BOOST_PP_SEQ_TO_TUPLE(product)
//
//BOOST_PP_SEQ_FOR_EACH_PRODUCT(MACRO, ((T0)(const T0)) ((T1)(const T1)) ((T2)(const T2)))
//    
//( ZELDA_TEMPLATE_TYPE0 , const ZELDA_TEMPLATE_TYPE0 ) 
//    ( ZELDA_TEMPLATE_TYPE0 , const ZELDA_TEMPLATE_TYPE1 ) 
//    ( ZELDA_TEMPLATE_TYPE1 , const ZELDA_TEMPLATE_TYPE0 ) 
//    ( ZELDA_TEMPLATE_TYPE1 , const ZELDA_TEMPLATE_TYPE1 )

#endif	/* XFUNCTION_H */

