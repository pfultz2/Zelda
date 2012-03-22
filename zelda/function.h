/* 
 * File:   function.h
 * Author: paul
 *
 * Created on February 29, 2012, 12:17 AM
 */

#ifndef FUNCTION_H
#define	FUNCTION_H

#include <boost/phoenix/function/function.hpp>
#include <boost/phoenix/core/is_actor.hpp>
#include "requires.h"
#include "invoke.h"
#include "tuple.h"
#include "pp.h"

#define ZELDA_DETAIL_FO_DEFINE(tmpl, enable, ret, name, params) \
template<class Zelda_F, PP_TUPLE_REM(tmpl)> \
struct result<Zelda_F(PP_TUPLE_REM(params)), ZELDA_CLASS_REQUIRES(PP_TUPLE_REM(enable))> \
{ \
    typedef PP_TUPLE_REM(ret) type; \
}; \
template<PP_TUPLE_REM(tmpl)> \
typename result<void(PP_TUPLE_REM(params))>::type \
operator() params const \

namespace zelda {

namespace details{
template<class F, class Iterator, class End, class... T>
struct is_callable_seq_impl;

template<class F, class I, class End, class D, class... T>
struct is_callable_seq_impl_next
{
    typedef is_callable_seq_impl<F, typename I::type, End, typename D::type, T...> type;
};

template<class F, class Iterator, class End, class... T>
struct is_callable_seq_impl 
: zelda::mpl::if_<boost::is_same<Iterator, End>,
    zelda::is_callable<F(T...) >,
    zelda::mpl::lazy<is_callable_seq_impl_next<F, boost::fusion::result_of::next<Iterator>, End, boost::fusion::result_of::deref<Iterator>, T...> >
    >::type {};
}
template<class F, class Seq>
struct is_callable_seq : details::is_callable_seq_impl<F, typename boost::fusion::result_of::begin<Seq>::type, typename boost::fusion::result_of::end<Seq>::type>
{};

//Optimization for stateless functions
template<class F, class Enable = void>
struct function_base
{
    F f;
    function_base() {};
    function_base(F f) : f(f) {};

    F get_function() const
    {
        return f;
    }
};

template<class F>
struct function_base<F, ZELDA_CLASS_REQUIRES(boost::is_empty<F>)>
{
    function_base() {};
    function_base(F) {};

    F get_function() const
    {
        return F();
    }
};

//TODO: Defer

//Pipable
namespace details {
template<class F, class Sequence>
struct pipe_closure
{
    F f;
    const Sequence& seq;
    pipe_closure(const Sequence& seq) : seq(seq) {};
    pipe_closure(const Sequence& seq, F f) : seq(seq), f(f) {};

    template<class A, ZELDA_REQUIRES(zelda::is_callable_seq<F, typename boost::fusion::result_of::push_front<Sequence, A&&>::type>)>
    friend auto operator|(A && a, const pipe_closure<F, Sequence>& p) ZELDA_RETURNS(zelda::invoke(p.f, boost::fusion::push_front(p.seq, a)))
};

template<class F, class... Args>
auto make_pipe_closure(F f, Args && ... args) 
ZELDA_RETURNS(pipe_closure<F, zelda::tuple<Args && ...>>(f, zelda::tuple<Args && ...>(std::forward<Args>(args)...)))
}

template<class F>
struct pipable_adaptor : function_base<F>
{
    pipable_adaptor() {};
    pipable_adaptor(F f) : function_base<F>(f) {};

    template<class X, class E = void>
    struct result {};

    template<class X, class... Args>
    struct result<X(Args...), ZELDA_CLASS_REQUIRES(not zelda::is_callable<X(Args...)>)>
    {
        typedef details::pipe_closure<F, zelda::tuple<Args...>> type;
    };

    template<class X, class... Args>
    struct result<X(Args...), ZELDA_CLASS_REQUIRES(zelda::is_callable<X(Args...)>)>
    : zelda::result_of<X(Args...)> {};

    template<class... Args>
    ZELDA_FUNCTION_REQUIRES(not zelda::is_callable<F(Args...)>)
    (typename result<F(Args&&...)>::type) operator()(Args &&... args) const
    {
        return details::make_pipe_closure(this->get_function(), std::forward<Args>(args)...);
    }

    template<class... Args>
    ZELDA_FUNCTION_REQUIRES(zelda::is_callable<F(Args...)>)
    (typename result<F(Args&&...)>::type) operator()(Args &&... args) const
    {
        return this->get_function()(std::forward<Args>(args)...);
    }
};

template<class F>
pipable_adaptor<F> pipable(F f)
{
    return pipable_adaptor<F>(f);
}

//lazy
//TODO: Use boost::phoenix::detail::expression::function_eval instead
template<class F>
boost::phoenix::function<F> lazy(F f)
{
    return boost::phoenix::function<F>(f);
}

//general
namespace details{
template<class... T>
struct is_phoenix_expression
: zelda::mpl::or_<T...> {};
}
template<class F>
struct general_adaptor : function_base<F>
{
    general_adaptor() {};
    general_adaptor(F f) : function_base<F>(f) {};

    template<class X, class E = void>
    struct result {};

    template<class X, class... Args>
    struct result<X(Args...), ZELDA_CLASS_REQUIRES(zelda::details::is_phoenix_expression<Args...>)>
    {
        typedef ZELDA_XTYPEOF(lazy(zelda::declval<F>())(zelda::declval<Args>()...)) type;
    };

    template<class X, class... Args>
    struct result<X(Args...), ZELDA_CLASS_REQUIRES(not zelda::details::is_phoenix_expression<Args...>)>
    : zelda::result_of<X(Args...)> {};

    template<class... Args>
    ZELDA_FUNCTION_REQUIRES(zelda::details::is_phoenix_expression<Args...>)
    (typename result<void(Args&&...)>::type) operator()(Args&&... args) const
    {
        return lazy(this->get_function())(std::forward<Args>(args)...);
    }

    template<class... Args>
    ZELDA_FUNCTION_REQUIRES(not zelda::details::is_phoenix_expression<Args...>)
    (typename result<void(Args&&...)>::type) operator()(Args&&... args) const
    {
        return this->get_function()(std::forward<Args>(args)...);
    }

};

//TODO: Partial

//fuse
template<class F>
struct fuse_adaptor : function_base<F>
{
    fuse_adaptor() {};
    fuse_adaptor(F f) : function_base<F>(f) {};

    template<class Sequence>
    auto operator()(Sequence && seq) -> ZELDA_XTYPEOF(zelda::invoke(zelda::declval<F>(), seq))
    {
        return zelda::invoke(this->get_function(), seq);
    }
};

template<class F>
fuse_adaptor<F> fuse(F f)
{
    return fuse_adaptor<F>(f);
}

//unfuse
template<class F>
struct unfuse_adaptor : function_base<F>
{
    unfuse_adaptor() {};
    unfuse_adaptor(F f) : function_base<F>(f) {};

    template<class... T>
    auto operator()(T && ... x) -> ZELDA_XTYPEOF(zelda::declval<F>()(zelda::make_tuple(std::forward<T>(x)...)))
    {
        return  this->get_function()(zelda::make_tuple(std::forward<T>(x)...));
    }
};

template<class F>
unfuse_adaptor<F> unfuse(F f)
{
    return unfuse_adaptor<F>(f);
}

//TODO: memoize
}

#endif	/* FUNCTION_H */

