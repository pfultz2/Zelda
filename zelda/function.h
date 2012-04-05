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
//#include <boost/fusion/algorithm/transformation/join.hpp>
#include "requires.h"
//#include "invoke.h"
#include "tuple.h"
#include "result_of.h"
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

template<bool B, class T>
T static_assert_expr(T x)
{
    static_assert(B, "Assertion failed");
    return x;
}


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

template<class A, class Enable = void>
struct function_adaptor_base
{
    A a;
    function_adaptor_base() {};
    function_adaptor_base(A a) : a(a) {};

    A& get_adaptor() const
    {
        return a;
    }
};

template<class A>
struct function_adaptor_base<A, ZELDA_CLASS_REQUIRES(boost::is_empty<A>)>
{
    function_adaptor_base() {};
    function_adaptor_base(A) {};

    A get_adaptor() const
    {
        return A();
    }
};

template<class A, class F>
struct function_adaptor : function_base<F>, function_adaptor_base<A>
{
    function_adaptor() {};
    function_adaptor(F f) : function_base<F>(f) {};
    function_adaptor(A a) : function_adaptor_base<A>(a) {};
    function_adaptor(F f, A a) : function_base<F>(f), function_adaptor_base<A>(a) {};

    template<class X, class Enable = void>
    struct result {};

    template<class X, class... T>
    struct result<X(T...)>
    : zelda::result_of<A(F, T...)> {};

    template<class... T>
    typename result<void(T...)>::type
    operator()(T && ... x)
    {
        return this->get_adaptor()(this->get_function(), std::forward<T>(x)...);
    }
};

template<class A, class F>
struct function_variadic_adaptor : function_base<F>, function_adaptor_base<A>
{
    function_variadic_adaptor() {};
    function_variadic_adaptor(F f) : function_base<F>(f) {};
    function_variadic_adaptor(A a) : function_adaptor_base<A>(a) {};
    function_variadic_adaptor(F f, A a) : function_base<F>(f), function_adaptor_base<A>(a) {};

    template<class X, class Enable = void>
    struct result {};

    //TODO: Check if it is callable
    template<class X, class... T>
    struct result<X(T...)>
    {
        typedef ZELDA_XTYPEOF(zelda::declval<A>()(zelda::declval<F>(), zelda::declval<zelda::tuple<T&&...>>())) type;
    };

    template<class... T>
    typename result<void(T...)>::type
    operator()(T && ... x)
    {
        return this->get_adaptor()(this->get_function(), zelda::forward_as_tuple(std::forward<T>(x)...));
    }

};

#define DETAIL_ZELDA_FUNCTION_MAKE_ADAPTOR(name, adaptor_type, adaptor_base) \
template<class F> \
struct name ## _adaptor : adaptor_type<adaptor_base, F> \
{ \
    name ## _adaptor() {}; \
    name ## _adaptor(F f) : adaptor_type<F, adaptor_base>(f) {}; \
}; \
template<class F> \
name ## _adaptor<F> name(F f) { return name ## _adaptor<F>(f); }


#define ZELDA_FUNCTION_MAKE_SIMPLE_ADAPTOR(name) DETAIL_ZELDA_FUNCTION_MAKE_ADAPTOR(name, function_adaptor, name ## _adaptor_base)
#define ZELDA_FUNCTION_MAKE_ADAPTOR(name) DETAIL_ZELDA_FUNCTION_MAKE_ADAPTOR(name, function_variadic_adaptor, name ## _adaptor_base)


//TODO: Defer

//fuse
struct fuse_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT((f, seq)(zelda::invoke(f, seq)))
};
ZELDA_FUNCTION_MAKE_SIMPLE_ADAPTOR(fuse)


// template<class F>
// struct fuse_adaptor : function_base<F>
// {
//     fuse_adaptor() {};
//     fuse_adaptor(F f) : function_base<F>(f) {};

//     template<class X>
//     struct result {};

//     template<class X, class Sequence>
//     struct result<X(Sequence)>
//     : result_of_seq<F, Sequence> {};

//     template<class Sequence>
//     auto operator()(Sequence && seq) -> ZELDA_XTYPEOF(zelda::invoke(zelda::declval<F>(), seq))
//     {
//         return zelda::invoke(this->get_function(), seq);
//     }
// };

// template<class F>
// fuse_adaptor<F> fuse(F f)
// {
//     return fuse_adaptor<F>(f);
// }

//variadic
struct variadic_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT((f, seq)(f(seq)))
};
ZELDA_FUNCTION_MAKE_ADAPTOR(variadic)



// template<class F>
// struct variadic_adaptor : function_base<F>
// {
//     variadic_adaptor() {};
//     variadic_adaptor(F f) : function_base<F>(f) {};

//     template<class X>
//     struct result {};

//     template<class X, class... T>
//     struct result<X(T...)>
//     {
//         typedef ZELDA_XTYPEOF(zelda::declval<F>()(zelda::make_tuple(zelda::declval<T>()...))) type;
//     };

//     template<class... T>
//     auto operator()(T && ... x) -> ZELDA_XTYPEOF(zelda::declval<F>()(zelda::make_tuple(std::forward<T>(x)...)))
//     {
//         return  this->get_function()(zelda::make_tuple(std::forward<T>(x)...));
//     }
// };

// template<class F>
// variadic_adaptor<F> variadic(F f)
// {
//     return variadic_adaptor<F>(f);
// }


//Pipable
namespace details {
template<class F, class Sequence>
struct pipe_closure
{
    F f;
    Sequence seq;
    template<class S>
    pipe_closure(S && seq) : seq(std::forward<Sequence>(seq)) {};
    template<class S>
    pipe_closure(F f, S && seq) : f(f), seq(std::forward<Sequence>(seq)) {};

    template<class A>
    static auto push_front(A && a, Sequence && seq)
    ZELDA_RETURNS(zelda::tuple_cat(zelda::tuple<A>(a), seq));

    template<class A>
    friend ZELDA_FUNCTION_REQUIRES(zelda::is_callable_tuple<F, typename zelda::tuple_join<tuple<A&&>, Sequence>::type>)
    (typename zelda::result_of_tuple<F, typename zelda::tuple_join<tuple<A&&>, Sequence>::type>::type) 
    operator|(A && a, pipe_closure<F, Sequence> p)
    
    {
        return zelda::invoke(p.f, zelda::tuple_cat(zelda::forward_as_tuple(std::forward<A>(a)), p.seq));
    }
};

template<class F, class Seq>
auto make_pipe_closure(F f, Seq && seq) ZELDA_RETURNS
( pipe_closure<F, decltype(zelda::unforward_tuple(seq))>(f, std::forward<Seq>(seq)) )

}

struct pipable_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT( (f, seq)(is_callable_tuple<f, seq>) (zelda::invoke(f, seq)) )
    //ZELDA_AUTO_RESULT( (f, seq)(is_callable_seq<f, seq>) (static_assert(false, "is_callable")) )

    ZELDA_AUTO_RESULT( (f, seq)(not is_callable_tuple<f, seq>) (details::make_pipe_closure(f, seq)) )
};

template<class F>
struct pipable_adaptor : function_variadic_adaptor<pipable_adaptor_base, F> 
{ 
    pipable_adaptor() {}; 
    pipable_adaptor(F f) : function_variadic_adaptor<F, pipable_adaptor_base>(f) {}; 

    template<class A, ZELDA_REQUIRES(zelda::is_callable<F(A)>)>
    friend typename zelda::result_of<F(A)>::type operator|(A && a, const pipable_adaptor<F>& p)
    {
        return p.get_function()(std::forward<A>(a));
    }
}; 
template<class F> 
pipable_adaptor<F> pipable(F f) { return pipable_adaptor<F>(f); }


// template<class F>
// struct pipable_adaptor : function_base<F>
// {
//     pipable_adaptor() {};
//     pipable_adaptor(F f) : function_base<F>(f) {};

//     template<class X, class E = void>
//     struct result {};

//     template<class X, class... Args>
//     struct result<X(Args...), ZELDA_CLASS_REQUIRES(not zelda::is_callable<X(Args...)>)>
//     {
//         typedef details::pipe_closure<F, zelda::tuple<Args...>> type;
//     };

//     template<class X, class... Args>
//     struct result<X(Args...), ZELDA_CLASS_REQUIRES(zelda::is_callable<X(Args...)>)>
//     : zelda::result_of<X(Args...)> {};

//     template<class... Args>
//     ZELDA_FUNCTION_REQUIRES(not zelda::is_callable<F(Args...)>)
//     (typename result<F(Args&&...)>::type) operator()(Args &&... args) const
//     {
//         return details::make_pipe_closure(this->get_function(), std::forward<Args>(args)...);
//     }

//     template<class... Args>
//     ZELDA_FUNCTION_REQUIRES(zelda::is_callable<F(Args...)>)
//     (typename result<F(Args&&...)>::type) operator()(Args &&... args) const
//     {
//         return this->get_function()(std::forward<Args>(args)...);
//     }
// };

// template<class F>
// pipable_adaptor<F> pipable(F f)
// {
//     return pipable_adaptor<F>(f);
// }

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
template<class , class>
struct partial_adaptor;

template<class F, class Sequence>
partial_adaptor<F, Sequence> partial(F, Sequence);

namespace partial_details {
template<class F>
struct seq 
: zelda::mpl::identity<zelda::tuple<> > {};

template<class F, class Sequence>
struct seq<partial_adaptor<F, Sequence> >
: zelda::mpl::identity<Sequence > {};

template<class F>
struct fun 
: zelda::mpl::identity<F> {};

template<class F, class Sequence>
struct fun<partial_adaptor<F, Sequence> >
: zelda::mpl::identity<F> {};

template<class F, class Sequence>
Sequence get_tuple(partial_adaptor<F, Sequence> f)
{
    return f.seq;
}

template<class F>
zelda::tuple<> get_tuple(F)
{
    return zelda::tuple<>();
}

template<class F>
F get_function(F f)
{
    return f;
}

template<class F, class Sequence>
F get_function(partial_adaptor<F, Sequence> f)
{
    return f.get_function();
}

template<class F, class Sequence>
auto partial_join(F f, Sequence && seq)
ZELDA_RETURNS(zelda::tuple_cat(get_tuple(f), std::forward<Sequence>(seq)));


template<class F, class Sequence>
struct partial_join_tuple
: zelda::mpl::identity<decltype(partial_join(zelda::declval<F>(), zelda::declval<Sequence>()))> 
{};


}

struct partial_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT( (f, seq)(is_callable_tuple<f, typename partial_details::partial_join_tuple<f, seq>::type>) 
                        (zelda::invoke(partial_details::get_function(f), partial_details::partial_join(f, seq))) )

    ZELDA_AUTO_RESULT( (f, seq)(not is_callable_tuple<f, typename partial_details::partial_join_tuple<f, seq>::type>) 
                        (partial(partial_details::get_function(f), partial_details::partial_join(f, seq))) )
};

template<class F, class Sequence = zelda::tuple<> >
struct partial_adaptor : function_variadic_adaptor<partial_adaptor_base, F>
{
    typedef Sequence sequence;
    Sequence seq;
    partial_adaptor() {}; 
    partial_adaptor(Sequence seq) : seq(seq) {};
    partial_adaptor(F f) : function_variadic_adaptor<F, partial_adaptor_base>(f) {};
    partial_adaptor(Sequence seq, F f) : seq(seq), function_variadic_adaptor<F, partial_adaptor_base>(f) {};


};

template<class F, class Sequence>
auto partial(F f, Sequence && seq) ZELDA_RETURNS
(
    partial_adaptor<F, decltype(zelda::unforward_tuple(seq))>(f, std::forward<Sequence>(seq))
)

template<class F>
partial_adaptor<F> partial(F f)
{
    return partial_adaptor<F>(f);
}

//TODO: poly

//TODO: memoize
}




#endif	/* FUNCTION_H */

