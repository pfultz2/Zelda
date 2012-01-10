/* 
 * File:   expression.h
 * Author: pfultz
 *
 * Created on November 21, 2011, 6:59 PM
 */

#ifndef EXPRESSION_H
#define	EXPRESSION_H

#include "pp.h"


#define ZELDA_DETAIL_NAMED_EXPRESSION_EACH(r, i, data) ZELDA_TEMPLATE_TYPE ## i BOOST_PP_SEQ_ELEM(i, data)

#define ZELDA_DETAIL_NAMED_EXPRESSION_EACH_VAR(r, data, i, x) static ZELDA_TEMPLATE_TYPE ## i x;

#ifdef ZELDA_HAS_DECLTYPE
#define ZELDA_NAMED_EXPRESSION(name, ...) \
struct name \
{\
template<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), class ZELDA_TEMPLATE_TYPE)>\
auto operator()(BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_NAMED_EXPRESSION_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__)) )\
-> ZELDA_DETAIL_NAMED_EXPRESSION

#define ZELDA_DETAIL_NAMED_EXPRESSION(...) decltype(__VA_ARGS__) { return __VA_ARGS__; }\
}
#else
#define ZELDA_NAMED_EXPRESSION(name, ...) \
struct name \
{\
template<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), class ZELDA_TEMPLATE_TYPE)>\
struct deducer\
{\
PP_ARGS_FOREACH_I(ZELDA_DETAIL_NAMED_EXPRESSION_EACH_VAR, data, __VA_ARGS__)\
struct deduce;\
};\
template<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), class ZELDA_TEMPLATE_TYPE)> \
typename deducer<BOOST_PP_ENUM_PARAMS(PP_NARGS(__VA_ARGS__), ZELDA_TEMPLATE_TYPE)>::deduce::type \
operator()(BOOST_PP_ENUM(PP_NARGS(__VA_ARGS__), ZELDA_DETAIL_NAMED_EXPRESSION_EACH, PP_ARGS_TO_SEQ(__VA_ARGS__)) )\
BOOST_PP_CAT(ZELDA_DETAIL_NAMED_EXPRESSION_II_, PP_NARGS(__VA_ARGS__))


#define ZELDA_DETAIL_NAMED_EXPRESSION_III(n, ...) { return __VA_ARGS__; } \
template<BOOST_PP_ENUM_PARAMS(n, class ZELDA_TEMPLATE_TYPE)>\
struct deducer<BOOST_PP_ENUM_PARAMS(n, ZELDA_TEMPLATE_TYPE)>::deduce\
{\
typedef typeof(__VA_ARGS__) type;\
};\
}\

#define ZELDA_DETAIL_NAMED_EXPRESSION_II_0(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(0, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_1(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(1, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_2(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(2, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_3(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(3, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_4(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(4, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_5(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(5, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_6(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(6, __VA_ARGS__)
#define ZELDA_DETAIL_NAMED_EXPRESSION_II_7(...) ZELDA_DETAIL_NAMED_EXPRESSION_III(7, __VA_ARGS__)


#endif

//PP_UNIQUE_LOCAL_NAME(zelda_private_expression_)
#define ZELDA_EXPR(...) ZELDA_NAMED_EXPRESSION(, __VA_ARGS__)

//Post operators
#define ZELDA_OPERATOR_FROM_NAME_post_increment ++
#define ZELDA_OPERATOR_FROM_NAME_post_decrement --
//Unary operators
#define ZELDA_OPERATOR_FROM_NAME_increment ++
#define ZELDA_OPERATOR_FROM_NAME_decrement --
#define ZELDA_OPERATOR_FROM_NAME_unary_plus +
#define ZELDA_OPERATOR_FROM_NAME_unary_minus -
#define ZELDA_OPERATOR_FROM_NAME_not !
#define ZELDA_OPERATOR_FROM_NAME_bitnot ~
#define ZELDA_OPERATOR_FROM_NAME_indirection *
//Binary operators
#define ZELDA_OPERATOR_FROM_NAME_multiply *
#define ZELDA_OPERATOR_FROM_NAME_divide /
#define ZELDA_OPERATOR_FROM_NAME_remainder %
#define ZELDA_OPERATOR_FROM_NAME_plus +
#define ZELDA_OPERATOR_FROM_NAME_minus -
#define ZELDA_OPERATOR_FROM_NAME_shift_left <<
#define ZELDA_OPERATOR_FROM_NAME_shift_right >>
#define ZELDA_OPERATOR_FROM_NAME_less <
#define ZELDA_OPERATOR_FROM_NAME_less_equal <=
#define ZELDA_OPERATOR_FROM_NAME_greater >
#define ZELDA_OPERATOR_FROM_NAME_greater_equal >=
#define ZELDA_OPERATOR_FROM_NAME_equal ==
#define ZELDA_OPERATOR_FROM_NAME_not_equal !=
#define ZELDA_OPERATOR_FROM_NAME_bitand &
#define ZELDA_OPERATOR_FROM_NAME_xor ^
#define ZELDA_OPERATOR_FROM_NAME_bitor |
#define ZELDA_OPERATOR_FROM_NAME_and &&
#define ZELDA_OPERATOR_FROM_NAME_or ||
#define ZELDA_OPERATOR_FROM_NAME_assign =
#define ZELDA_OPERATOR_FROM_NAME_plus_assign +=
#define ZELDA_OPERATOR_FROM_NAME_minus_assign -=
#define ZELDA_OPERATOR_FROM_NAME_multiply_assign *=
#define ZELDA_OPERATOR_FROM_NAME_divide_assign /=
#define ZELDA_OPERATOR_FROM_NAME_shift_left_assign <<=
#define ZELDA_OPERATOR_FROM_NAME_shift_right_assign >>=
#define ZELDA_OPERATOR_FROM_NAME_and_assign &=
#define ZELDA_OPERATOR_FROM_NAME_xor_assign ^=
#define ZELDA_OPERATOR_FROM_NAME_or_assign |=
#define ZELDA_OPERATOR_FROM_NAME_comma ,

#define ZELDA_OPERATOR_FROM_NAME(name) ZELDA_OPERATOR_FROM_NAME_ ## name


//namespace zelda {
//
//template<class Base, class Operator, class Parameter = void>
//struct expression
//{
//    typedef Base base_type;
//    typedef Operator op_type;
//    typedef Parameter type;
//    
//    base_type& base;
//    type& x;
//    expression(base_type& b, type& x) : b(b), x(x) {}
//};
//
//template<class Base, class Operator>
//struct expression<Base, Operator, void>
//{
//    typedef Base base_type;
//    typedef Operator op_type;
//    typedef void type;
//    
//    base_type& base;
//    expression(base_type& b) : b(b) {}
//};
//
//#define ZELDA_DEFINE_OPERATOR(op) BOOST_PP_IF(ZELDA_IS_UNARY_OPERATOR(op), ZELDA_DEFINE_UNARY_OPERATOR, ZELDA_DEFINE_BINARY_OPERATOR)(op)
//
//
//#define ZELDA_DEFINE_BINARY_OPERATOR(op) \
//struct op ## _operator \
//{\
//    ZELDA_NAMED_EXPRESSION(eval, x, y)(x ZELDA_OPERATOR_FROM_NAME(op) y);\
//    ZELDA_NAMED_EXPRESSION(quote, x, y)(x + BOOST_PP_STRINGIZE(ZELDA_OPERATOR_FROM_NAME(op)) + y);\
//    ZELDA_NAMED_EXPRESSION(quote_with, x, y, map)(x + map[BOOST_PP_STRINGIZE(op)] + y);\
//};
//
//#define ZELDA_DEFINE_UNARY_OPERATOR(op) \
//struct op ## _operator \
//{\
//    ZELDA_NAMED_EXPRESSION(eval, x)(ZELDA_OPERATOR_FROM_NAME(op) x);\
//    ZELDA_NAMED_EXPRESSION(quote, x)(BOOST_PP_STRINGIZE(ZELDA_OPERATOR_FROM_NAME(op)) + x);\
//    ZELDA_NAMED_EXPRESSION(quote_with, x, map)(map[BOOST_PP_STRINGIZE(op)] + x);\
//};
//
//#define ZELDA_DEFINE_POST_OPERATOR(op) \
//struct op ## _operator \
//{\
//    ZELDA_NAMED_EXPRESSION(eval, x)(x ZELDA_OPERATOR_FROM_NAME(op));\
//    ZELDA_NAMED_EXPRESSION(quote, x)(x + BOOST_PP_STRINGIZE(ZELDA_OPERATOR_FROM_NAME(op)));\
//    ZELDA_NAMED_EXPRESSION(quote_with, x, map)(x + map[BOOST_PP_STRINGIZE(op)]);\
//};
//
//#define ZELDA_DEFINE_UNARY_OPERATORS_EACH(r, data, x) ZELDA_DEFINE_UNARY_OPERATOR(x)
//#define ZELDA_DEFINE_UNARY_OPERATORS(...) PP_ARGS_FOREACH(ZELDA_DEFINE_UNARY_OPERATORS_EACH, data, __VA_ARGS__)
//
//#define ZELDA_DEFINE_BINARY_OPERATORS_EACH(r, data, x) ZELDA_DEFINE_BINARY_OPERATOR(x)
//#define ZELDA_DEFINE_BINARY_OPERATORS(...) PP_ARGS_FOREACH(ZELDA_DEFINE_BINARY_OPERATORS_EACH, data, __VA_ARGS__)
//
//#define ZELDA_DEFINE_POST_OPERATORS_EACH(r, data, x) ZELDA_DEFINE_POST_OPERATOR(x)
//#define ZELDA_DEFINE_POST_OPERATORS(...) PP_ARGS_FOREACH(ZELDA_DEFINE_POST_OPERATORS_EACH, data, __VA_ARGS__)
//
//ZELDA_DEFINE_POST_OPERATORS(post_increment, post_decrement)
//
//ZELDA_DEFINE_UNARY_OPERATORS
//(
//increment ,
//decrement ,
//unary_plus ,
//unary_minus ,
//not ,
//bitnot ,
//indirection
//)
//ZELDA_DEFINE_BINARY_OPERATORS
//(
//multiply ,
//divide ,
//remainder ,
//plus ,
//minus ,
//shift_left ,
//shift_right ,
//less ,
//less_equal ,
//greater ,
//greater_equal ,
//equal ,
//not_equal ,
//bitand ,
//xor ,
//bitor ,
//and ,
//or
//)
//ZELDA_DEFINE_BINARY_OPERATORS
//(
//assign ,
//plus_assign ,
//minus_assign ,
//multiply_assign ,
//divide_assign ,
//shift_left_assign ,
//shift_right_assign ,
//and_assign ,
//xor_assign ,
//or_assign ,
//comma
//)
//
//#define ZELDA_KERNEL_DEFINE_BINARY_OPERATOR(op) \
//template<class T> \
//typename add_expression<op ## _operator, T>::type operator ZELDA_OPERATOR_FROM_NAME(op)(T& x) \
//{ \
//    typedef typename add_expression<op ## _operator, T>::expr expr; \
//    return typename add_expression<op ## _operator, T>::type(expr(*this, x)); \
//} \
//template<class T> \
//typename add_expression<op ## _operator, const T>::type operator ZELDA_OPERATOR_FROM_NAME(op)(const T& x) \
//{ \
//    typedef typename add_expression<op ## _operator, T>::expr expr; \
//    return typename add_expression<op ## _operator, const T>::type(expr(*this, x)); \
//} \
//
//#define ZELDA_KERNEL_DEFINE_UNARY_OPERATOR(op) \
//template<class T> \
//typename add_expression<op ## _operator>::type operator ZELDA_OPERATOR_FROM_NAME(op)() \
//{ \
//    typedef typename add_expression<op ## _operator, T>::expr expr; \
//    return typename add_expression<op ## _operator>::type(expr(*this)); \
//} \
//
//#define ZELDA_KERNEL_DEFINE_POST_OPERATOR(op) \
//template<class T> \
//typename add_expression<op ## _operator>::type operator ZELDA_OPERATOR_FROM_NAME(op)(int) \
//{ \
//    typedef typename add_expression<op ## _operator, T>::expr expr; \
//    return typename add_expression<op ## _operator>::type(expr(*this)); \
//} \
//
//#define ZELDA_KERNEL_DEFINE_BINARY_OPERATORS_EACH(r, data, x) ZELDA_KERNEL_DEFINE_BINARY_OPERATOR(x)
//#define ZELDA_KERNEL_DEFINE_BINARY_OPERATORS(...) PP_ARGS_FOREACH(ZELDA_KERNEL_DEFINE_BINARY_OPERATORS_EACH, data, __VA_ARGS__)
//
//#define ZELDA_KERNEL_DEFINE_UNARY_OPERATORS_EACH(r, data, x) ZELDA_KERNEL_DEFINE_UNARY_OPERATOR(x)
//#define ZELDA_KERNEL_DEFINE_UNARY_OPERATORS(...) PP_ARGS_FOREACH(ZELDA_KERNEL_DEFINE_UNARY_OPERATORS_EACH, data, __VA_ARGS__)
//
//#define ZELDA_KERNEL_DEFINE_POST_OPERATORS_EACH(r, data, x) ZELDA_KERNEL_DEFINE_POST_OPERATOR(x)
//#define ZELDA_KERNEL_DEFINE_POST_OPERATORS(...) PP_ARGS_FOREACH(ZELDA_KERNEL_DEFINE_POST_OPERATORS_EACH, data, __VA_ARGS__)
//
//template<class Expr>
//struct kernel
//{
//    Expr e;
//    kernel() {}
//    kernel(Expr e) : e(e) {}
//    
//    
//    template<class Op, class Param = void>
//    struct add_expression
//    {
//        typedef expression<kernel<Expr>, Op, Param> expr;
//        typedef kernel<expression<kernel<Expr>, Op, Param> > type;
//    };
//    
//    ZELDA_KERNEL_DEFINE_POST_OPERATORS(post_increment, post_decrement)
//
//
//    ZELDA_KERNEL_DEFINE_UNARY_OPERATORS
//    (
//    increment ,
//    decrement ,
//    unary_plus ,
//    unary_minus ,
//    not ,
//    bitnot ,
//    indirection
//    )
//    ZELDA_KERNEL_DEFINE_BINARY_OPERATORS
//    (
//    multiply ,
//    divide ,
//    remainder ,
//    plus ,
//    minus ,
//    shift_left ,
//    shift_right ,
//    less ,
//    less_equal ,
//    greater ,
//    greater_equal ,
//    equal ,
//    not_equal ,
//    bitand ,
//    xor ,
//    bitor ,
//    and ,
//    or
//    )
//    ZELDA_KERNEL_DEFINE_BINARY_OPERATORS
//    (
//    assign ,
//    plus_assign ,
//    minus_assign ,
//    multiply_assign ,
//    divide_assign ,
//    shift_left_assign ,
//    shift_right_assign ,
//    and_assign ,
//    xor_assign ,
//    or_assign ,
//    comma
//    )
//};
//
//
//template<class Expr, class Evaluator>
//void eval(kernel<Expr> & x, Evaluator evaluator)
//{
//    eval(x.e, evaluator);
//}
//
//template<class Base, class Operator, class Parameter, class Evaluator>
//void eval(expression<Base, Operator, Parameter> & x, Evaluator evaluator)
//{
//    Operator::eval()(eval(x.base, evaluator), eval(x.x, evaluator));
//}
//
//template<class Base, class Operator, class Evaluator>
//void eval(expression<Base, Operator, void> & x, Evaluator evaluator)
//{
//    Operator::eval()(eval(x.base, evaluator));
//}
//
//template<class T, class Evaluator>
//void eval(T& x, Evaluator evaluator)
//{
//    evaluator(x);
//}
//}

#endif	/* EXPRESSION_H */

