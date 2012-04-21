/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    partial.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_FUNCTION_PARTIAL_H
#define ZELDA_GUARD_FUNCTION_PARTIAL_H

#include <zelda/function/adaptor.h>

namespace zelda { 

//TODO: Partial
template<class , class>
struct partial_adaptor;

template<class F, class Sequence>
auto partial(F f, Sequence && seq) -> partial_adaptor<F, decltype(zelda::unforward_tuple(seq))>;

namespace partial_details {
template<class F>
struct seq 
: zelda::tpl::identity<zelda::tuple<> > {};

template<class F, class Sequence>
struct seq<partial_adaptor<F, Sequence> >
: zelda::tpl::identity<Sequence > {};

template<class F>
struct fun 
: zelda::tpl::identity<F> {};

template<class F, class Sequence>
struct fun<partial_adaptor<F, Sequence> >
: zelda::tpl::identity<F> {};

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

// template<class F, class Sequence>
// auto partial_join(F f, Sequence && seq)
// ZELDA_RETURNS(zelda::tuple_cat(get_tuple(f), std::forward<Sequence>(seq)));


// template<class F, class Sequence>
// struct partial_join_tuple
// : zelda::tpl::identity<decltype(partial_join(zelda::declval<F>(), zelda::declval<Sequence>()))> 
// {};

template<class F, class T, class U>
struct partial_is_callable
: is_callable_tuple<F, typename tuple_cat_result<typename std::remove_reference<T>::type::tuple, U>::type> 
{};


}

struct partial_adaptor_base
{
    ZELDA_AUTO_DECLARE_RESULT()

    ZELDA_AUTO_RESULT( (self, f, seq)(partial_details::partial_is_callable<f, self, seq>) 
                        ( zelda::invoke(f, zelda::tuple_cat(self.get_tuple(), std::move(seq))) ) )

    ZELDA_AUTO_RESULT( (self, f, seq)(not partial_details::partial_is_callable<f, self, seq>) 
                        ( partial(f, zelda::tuple_cat(self.get_tuple(), std::move(seq))) ) )
};

template<class F, class Sequence = zelda::tuple<> >
struct partial_adaptor : function_variadic_adaptor<partial_adaptor<F, Sequence>, partial_adaptor_base, F>
{
    typedef function_variadic_adaptor<partial_adaptor<F, Sequence>, partial_adaptor_base, F> base;
    typedef Sequence tuple;
    Sequence seq;
    partial_adaptor() {}; 
    partial_adaptor(Sequence seq) : seq(seq) {};
    partial_adaptor(F f) : base(f) {};
    partial_adaptor(F f, Sequence seq) : seq(seq), base(f) {};

    Sequence& get_tuple()
    {
        return seq;
    }

    const Sequence& get_tuple() const
    {
        return seq;
    }


};

template<class F, class Sequence>
auto partial(F f, Sequence && seq) -> partial_adaptor<F, decltype(zelda::unforward_tuple(seq))>
{
    return {f, std::forward<Sequence>(seq)};
}

template<class F>
partial_adaptor<F> partial(F f)
{
    return partial_adaptor<F>(f);
}


}

#endif
