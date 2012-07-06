/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    requires.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/


#ifndef ZELDA_REQUIRES_H
#define	ZELDA_REQUIRES_H

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/vector.hpp>
#include <zelda/pp.h>
#include <zelda/static_assert.h>

namespace zelda{namespace requires_detail{
struct not_tag {};

template<class Sequence>
struct requires_
{
    template<class X>
    struct not_state : X {};

    struct op
    {
        template<class X, class Y>
        struct apply
        : boost::mpl::bool_<X::type::value and Y::type::value> {};

        template<class X>
        struct apply<X, not_tag>
        : boost::mpl::identity<not_state<X> > {};

        template<class X, class Y>
        struct apply<not_state<X>, Y>
        : boost::mpl::bool_<X::type::value and not Y::type::value> {};
    };

    typedef typename boost::mpl::fold<Sequence, boost::mpl::bool_<true>, op>::type type;

    static const bool value = type::value;

};

}
}
#define ZELDA_DETAIL_REQUIRES_CLAUSE_KEYWORD_not ()
#define ZELDA_DETAIL_REQUIRES_CLAUSE_KEYWORD_exclude ()


#define ZELDA_DETAIL_REQUIRES_CLAUSE_DISPATCH(tokened, x) BOOST_PP_IIF(ZELDA_PP_IS_PAREN(tokened), (zelda::requires_detail::not_tag)(ZELDA_PP_REM tokened), (x))
#define ZELDA_DETAIL_REQUIRES_CLAUSE_EACH(r, data, x) ZELDA_DETAIL_REQUIRES_CLAUSE_DISPATCH(BOOST_PP_CAT(ZELDA_DETAIL_REQUIRES_CLAUSE_KEYWORD_, x), x)
#define ZELDA_DETAIL_REQUIRES_CLAUSE(...) \
zelda::requires_detail::requires_<boost::mpl::vector<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_FOR_EACH(ZELDA_DETAIL_REQUIRES_CLAUSE_EACH, ~, ZELDA_PP_ARGS_TO_SEQ(__VA_ARGS__)))> >


#ifdef ZELDA_TEST
static_assert(ZELDA_DETAIL_REQUIRES_CLAUSE(boost::mpl::bool_<true>)::type::value, "Failed");
static_assert(ZELDA_DETAIL_REQUIRES_CLAUSE(boost::mpl::bool_<true>, boost::mpl::bool_<true>)::type::value, "Failed");
static_assert(ZELDA_DETAIL_REQUIRES_CLAUSE(boost::mpl::bool_<true>, exclude boost::mpl::bool_<false>)::type::value, "Failed");
static_assert(ZELDA_DETAIL_REQUIRES_CLAUSE(exclude boost::mpl::bool_<false>, boost::mpl::bool_<true>)::type::value, "Failed");
static_assert(ZELDA_DETAIL_REQUIRES_CLAUSE(exclude boost::mpl::bool_<false>)::type::value, "Failed");
#endif

 
#define ZELDA_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE(...) __VA_ARGS__>::type
#define ZELDA_FUNCTION_REQUIRES(...) typename boost::enable_if<ZELDA_DETAIL_REQUIRES_CLAUSE(__VA_ARGS__), ZELDA_ERROR_PARENTHESIS_MUST_BE_PLACED_AROUND_THE_RETURN_TYPE

#define ZELDA_CLASS_REQUIRES(...) typename boost::enable_if<ZELDA_DETAIL_REQUIRES_CLAUSE(__VA_ARGS__)>::type

#define ZELDA_REQUIRES(...) class Zelda_Enable = typename boost::enable_if<ZELDA_DETAIL_REQUIRES_CLAUSE(__VA_ARGS__)>::type


#endif	/* ZELDA_REQUIRES_H */

