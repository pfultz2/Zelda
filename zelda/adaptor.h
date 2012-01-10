/* 
 * File:   adaptor.h
 * Author: pfultz
 *
 * Created on November 20, 2011, 8:11 PM
 */

#ifndef ADAPTOR_H
#define	ADAPTOR_H

#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/front.hpp>

#include "traits.h"
#include "requires.h"
#include "expression.h"
#include "auto.h"

namespace zelda {

namespace adaptor {
template<class Adaptor, class Kernel>
struct kernel
{   

    template<class A>
    struct add_adaptor
    {
        typedef kernel<A, kernel<Adaptor,  Kernel> > type;
    };
    
    template<class Range>
    struct range_result
    {
        typedef typename Adaptor::range_result<Range>::type adaptor_result;
        typedef typename Kernel::range_result<adaptor_result>::type type;
    };
    template<class A>
    typename add_adaptor<Adaptor>::type
    operator| (const kernel& k, const A& a)
    {
        return typename add_adaptor<A>::type();
    }
    
    template<class Range>
    friend ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator| (Range& r, const kernel& k)
    {
        return k(r);
    }
    
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator()(Range& r)
    {
        return r | Adaptor() | Kernel();
    }
};

struct terminal
{
    template<class Range>
    struct range_result
    {
        typedef Range type;
    };
    
    template<class Range>
    friend ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator| (Range& r, const kernel& k)
    {
        return k(r);
    }
    
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type&)
    operator()(Range& r)
    {
        return r;
    }
};

struct make_adaptor_t
{
    template<class A> 
    adaptor::kernel<A, adaptor::terminal>
    operator| (const A& a)
    {
        return adaptor::kernel<A, adaptor::terminal>();
    }
};

}

adaptor::make_adaptor_t make_adaptor;

//Reverse
namespace details {
struct reverse_adaptor
{
    template<class Range>
    struct range_result
    {
        typedef reverse_iterator_range < typename boost::range_iterator < Range > ::type > type;
    };
    
    template<class Range>
    friend ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator| (Range& r, const reverse_adaptor& k)
    {
        return k(r);
    }
    
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator()(Range& r)
    {
        return typename range_result<Range>::type 
                (boost::begin(r), boost::end(r));
    }
};
}
details::reverse_adaptor reverse;

//Slice
namespace details {
struct slice_adaptor
{
    int start;
    int end;
    slice_adaptor(int start, int end=0) : start(start), end(end) {};
    template<class Range>
    struct range_result
    {
        typedef iterator_range < typename boost::range_iterator < Range > ::type > type;
    };
    
    template<class Range>
    friend ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator| (Range& r, const slice_adaptor& k)
    {
        return k(r);
    }
    
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator()(Range& r)
    {
        typedef typename boost::range_iterator < Range > ::type iterator;
        iterator e;
        if (index == 0) e = boost::end(r);
        else e = iterator_at(boost::end(r), end);
        return typename range_result<Range>::type 
                (iterator_at(boost::begin(r), start), e);
    }
    
};
}
details::slice_adaptor slice(int start, int end=0)
{
    return details::slice_adaptor(start, end);
}

template<class Range>
ZELDA_FUNCTION_REQUIRES(is_range<Range>)
(details::slice_adaptor) slice(Range& r, int start, int end=0)
{
    return details::slice_adaptor(start, end)(r);
}

//Select
namespace details {
template<class Transformer>
struct select_adaptor
{
    Transformer t;
    select_adaptor(Transformer t) : t(t) {};
    template<class Range>
    struct range_result
    {
        typedef transform_iterator_range < typename boost::range_iterator < Range > ::type, Transformer > type;
    };
    
    template<class Range>
    friend ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator| (Range& r, const slice_adaptor& k)
    {
        return k(r);
    }
    
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator()(Range& r)
    {
        return typename range_result<Range>::type 
                (boost::begin(r), boost::end(r), t);
    }
    
};
}
template<class Transformer>
details::select_adaptor select(Transformer t)
{
    return details::select_adaptor(t);
}

template<class Range, class Transformer>
ZELDA_FUNCTION_REQUIRES(is_range<Range>)
(details::select_adaptor) select(Range& r, Transformer t)
{
    return details::select_adaptor(t)(r);
}

//Where
namespace details {
template<class Predicate>
struct where_adaptor
{
    Predicate p;
    select_adaptor(Predicate p) : p(p) {};
    template<class Range>
    struct range_result
    {
        typedef filter_iterator_range < typename boost::range_iterator < Range > ::type, Predicate > type;
    };
    
    template<class Range>
    friend ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator| (Range& r, const slice_adaptor& k)
    {
        return k(r);
    }
    
    template<class Range>
    ZELDA_FUNCTION_REQUIRES(is_range<Range>)
    (typename range_result<Range>::type)
    operator()(Range& r)
    {
        return typename range_result<Range>::type 
                (boost::begin(r), boost::end(r), p);
    }
    
};
}
template<class Predicate>
details::where_adaptor where(Predicate p)
{
    return details::where_adaptor(p);
}

template<class Range, class Predicate>
ZELDA_FUNCTION_REQUIRES(is_range<Range>)
(details::where_adaptor) where(Range& r, Predicate p)
{
    return details::where_adaptor(p)(r);
}


//TODO: Stride, split, and join

namespace adaptors
{

ZELDA_EXPR(x)(x.first) key;
ZELDA_EXPR(x)(x.second) value;
ZELDA_EXPR(x)(*x) deref;
}

ZELDA_AUTO(keys, make_adaptor | select(adaptors::key));
ZELDA_AUTO(values, make_adaptor | select(adaptors::value));
ZELDA_AUTO(deref, make_adaptor | select(adaptors::deref));


}

#endif	/* ADAPTOR_H */

