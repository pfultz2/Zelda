#ifndef ZELDA_RESULT_OF_H
#define ZELDA_RESULT_OF_H

#include "introspection.h"
#include "requires.h"
#include "typeof.h"

namespace zelda{


namespace result_details {
template<class F>
struct function_object {};

template<class F, class... Args>
struct function_object<F(Args...)> 
{
    typedef F type;
};
ZELDA_HAS_TEMPLATE(result);
ZELDA_HAS_TEMPLATE(enable);
ZELDA_HAS_TYPE(result_type);
ZELDA_HAS_TYPE(type);
}

#if 0
template<class F, class Enable = void>
struct result_of {};

template<class F>
struct result_of<F, ZELDA_CLASS_REQUIRES(result_details::has_result<typename result_details::function_object<F>::type >)>
: result_details::function_object<F>::type::template result<F> {};

template<class F>
struct result_of<F, ZELDA_CLASS_REQUIRES(result_details::has_result_type<typename result_details::function_object<F>::type >)>
{
    typedef typename result_details::function_object<F>::type::result_type type;
};

template<class F, class Enable = void>
struct is_callable : result_details::has_type<typename result_of<F>::type> {};

#endif

template<class F>
struct is_callable {};

template<class F, class... Args>
struct is_callable<F(Args...)>
{
    typedef char yes; 
    typedef long no; 
    template<class T> 
    struct selector {}; 

    template<class U> 
    static yes check( selector<ZELDA_TYPEOF( zelda::declval<U>()(zelda::declval<Args>()...) )> * ); 

    template<class U> 
    static no check(...); 
    
    static const bool value = sizeof(check<F>(0)) == sizeof(yes); 

    typedef boost::mpl::bool_<value> type; 
};

template<class F, class Enable = void>
struct result_of {};

template<class F, class... Args>
struct result_of<F(Args...), ZELDA_CLASS_REQUIRES(is_callable<F(Args...)>)>
: boost::mpl::identity<ZELDA_XTYPEOF(zelda::declval<F>()(zelda::declval<Args>()...))> {};

#define DETAIL_ZELDA_AUTO_RESULT_CLASS_TRANSFORM(r, x) class x
#define DETAIL_ZELDA_AUTO_RESULT_CLASS(seq) PP_SEQ_ENUM(PP_SEQ_TRANSFORM(DETAIL_ZELDA_AUTO_RESULT_CLASS_TRANSFORM, seq))

#define DETAIL_ZELDA_AUTO_RESULT_TEMPLATE_ENUM(r, n, prefix) prefix Zelda_AR_Type ## n
#define DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(...) BOOST_PP_ENUM(PP_HEAD(__VA_ARGS__), DETAIL_ZELDA_AUTO_RESULT_TEMPLATE_ENUM, PP_TAIL(__VA_ARGS__)) 

#define DETAIL_ZELDA_AUTO_RESULT_TEMPLATE_REF_ENUM(r, n, prefix) Zelda_AR_Type ## n &&
#define DETAIL_ZELDA_AUTO_RESULT_TEMPLATE_REF(...) BOOST_PP_ENUM(PP_HEAD(__VA_ARGS__), DETAIL_ZELDA_AUTO_RESULT_TEMPLATE_REF_ENUM, PP_TAIL(__VA_ARGS__)) 

#define DETAIL_ZELDA_AUTO_RESULT_STATIC_REPEAT(r, n, seq) static Zelda_AR_Type ## n && PP_SEQ_ELEM(n, seq);
#define DETAIL_ZELDA_AUTO_RESULT_STATIC(seq) BOOST_PP_REPEAT(PP_SEQ_SIZE(seq), DETAIL_ZELDA_AUTO_RESULT_STATIC_REPEAT, seq) 

#define DETAIL_ZELDA_AUTO_RESULT_ARGS_ENUM(r, n, seq) Zelda_AR_Type ## n && PP_SEQ_ELEM(n, seq)
#define DETAIL_ZELDA_AUTO_RESULT_ARGS(seq) BOOST_PP_ENUM(PP_SEQ_SIZE(seq), DETAIL_ZELDA_AUTO_RESULT_ARGS_ENUM, seq) 

#define DETAIL_ZELDA_AUTO_RESULT_ENABLE(args, requires) \
template<class Zelda_F> struct PP_CAT(private_result_enable, __LINE__) : boost::mpl::bool_<false> {}; \
template<class Zelda_F, DETAIL_ZELDA_AUTO_RESULT_CLASS(args)> \
struct PP_CAT(private_result_enable, __LINE__) <Zelda_F(PP_SEQ_ENUM(args))> \
: ZELDA_DETAIL_REQUIRES_CLAUSE(PP_TUPLE_REM(requires)) {};

#define DETAIL_ZELDA_AUTO_RESULT_DEDUCE(args, requires, expr) \
template<class Zelda_F, DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(PP_SEQ_SIZE(args), class)> \
struct result<Zelda_F(DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(PP_SEQ_SIZE(args))), \
typename zelda::mpl::if_<PP_CAT(private_result_enable, __LINE__) <Zelda_F(DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(PP_SEQ_SIZE(args)))> >::type> \
{ \
    DETAIL_ZELDA_AUTO_RESULT_STATIC(args) \
    typedef ZELDA_XTYPEOF(expr) type;\
};

#define DETAIL_ZELDA_AUTO_RESULT_FUNCTION(args, expr) \
template<DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(PP_SEQ_SIZE(args), class)> \
typename zelda::mpl::if_< \
PP_CAT(private_result_enable, __LINE__) <void(DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(PP_SEQ_SIZE(args)))>, \
zelda::mpl::lazy<result<void(DETAIL_ZELDA_AUTO_RESULT_TEMPLATE(PP_SEQ_SIZE(args)))> > \
>::type \
operator()( DETAIL_ZELDA_AUTO_RESULT_ARGS(args) ) \
{ \
    return expr; \
}


#define DETAIL_ZELDA_AUTO_RESULT_KERNEL(args, requires, expr) \
DETAIL_ZELDA_AUTO_RESULT_ENABLE(args, requires) \
DETAIL_ZELDA_AUTO_RESULT_DEDUCE(args, requires, expr) \
DETAIL_ZELDA_AUTO_RESULT_FUNCTION(args, expr)

#define DETAIL_ZELDA_AUTO_RESULT_2(seq) \
DETAIL_ZELDA_AUTO_RESULT_KERNEL(PP_TUPLE_TO_SEQ(PP_SEQ_ELEM(0, seq)), (boost::mpl::bool_<true>), PP_SEQ_ELEM(1, seq))

#define DETAIL_ZELDA_AUTO_RESULT_3(seq) \
DETAIL_ZELDA_AUTO_RESULT_KERNEL(PP_TUPLE_TO_SEQ(PP_SEQ_ELEM(0, seq)), PP_SEQ_ELEM(1, seq), PP_SEQ_ELEM(2, seq))

#define ZELDA_AUTO_DECLARE_RESULT() \
template<class Zelda_F, class Zelda_Enable = void> struct result {}; 

#define DETAIL_ZELDA_AUTO_RESULT(seq) PP_CAT(DETAIL_ZELDA_AUTO_RESULT_, PP_SEQ_SIZE(seq))(seq)
//AUTO_RESULT((args)(requires)(expression))
//AUTO_RESULT((args)(expression))
#define ZELDA_AUTO_RESULT(seq) DETAIL_ZELDA_AUTO_RESULT(PP_TUPLE_SEQ(seq))

// template<class F>
// struct is_callable<F, ZELDA_CLASS_REQUIRES(result_details::has_enable<typename result_details::function_object<F>::type>)>
// : result_details::function_object<F>::type::template enable<F>::type {};

// template<class F>
// struct is_callable<F, ZELDA_CLASS_REQUIRES(result_details::has_type<typename result_of<F>::type>)>
// : result_details::has_type< typename result_details::function_object<F>::type::template result<F> >::type {};

}

// struct sum
// {

// ZELDA_AUTO_DECLARE_RESULT()

// ZELDA_AUTO_RESULT((x, y)(boost::is_numeric<x>)(x+y))
// };

// struct sum
// {

// template<class Zelda_F, class Zelda_Enable = void> struct result {}; 
// template<class Zelda_F> struct private_result_enable : boost::mpl::bool_<false> {};

// template<class Zelda_F, class x, class y> 
// struct private_result_enable<Zelda_F(x, y)> 
// : zelda::requires_detail::requires_<boost::mpl::bool_<true> > {}; 

// template<class Zelda_F, class Zelda_AR_Type0 , class Zelda_AR_Type1> 
//     struct result<Zelda_F( Zelda_AR_Type0 , Zelda_AR_Type1), typename zelda::mpl::if_<private_result_enable<Zelda_F( Zelda_AR_Type0 , Zelda_AR_Type1)> >::type> 
//     { 
//         static Zelda_AR_Type0 && x; static Zelda_AR_Type1 && y; 
//         typedef typename zelda::typeof_detail::xtypeof<decltype(zelda::typeof_detail::test((x+y))), decltype((x+y))>::type type;
//     }; 
//     template< class Zelda_AR_Type0 , class Zelda_AR_Type1> 
//     typename result<void( Zelda_AR_Type0 && , Zelda_AR_Type1 &&)>::type 
//     operator()( Zelda_AR_Type0 && x , Zelda_AR_Type1 && y ) { return (x+y); }
// };


#endif