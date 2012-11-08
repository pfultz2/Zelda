/*=============================================================================
    Copyright (c) 2005-2007 Shunsuke Sogame
    fusion_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ITERATOR_FUSION_ITERATOR_H
#define ZELDA_GUARD_ITERATOR_FUSION_ITERATOR_H

#include <cstddef>
#include <stdexcept>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/utility/addressof.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <zelda/pp.h>

#ifndef ZELDA_FUSION_ITERATOR_MAX
#define ZELDA_FUSION_ITERATOR_MAX 20
#endif

namespace zelda { 



template< class Sequence, class Reference, int Size = boost::fusion::result_of::size<Sequence>::value >
struct fusion_iterator_dereference;

#define ZELDA_ITERATOR_FUSION_RETURN_AT(z, n, _) case n: return boost::fusion::at_c<n>(seq);

#define ZELDA_ITERATOR_FUSION_DEREF(z, n, data) \
template< class Sequence, class Reference > \
struct fusion_iterator_dereference<Sequence, Reference, n> \
{ \
    static Reference call(Sequence& seq, int index) \
    { \
        switch (index) \
        { \
            BOOST_PP_REPEAT_ ## z(n, ZELDA_ITERATOR_FUSION_RETURN_AT, ~) \
        default: \
            throw std::out_of_range("Fusion iterator out of range access"); \
        } \
    } \
};
BOOST_PP_REPEAT_1(ZELDA_FUSION_ITERATOR_MAX, ZELDA_ITERATOR_FUSION_DEREF, ~)
#undef ZELDA_ITERATOR_FUSION_RETURN_AT
#undef ZELDA_ITERATOR_FUSION_DEREF

//
// fusion_iterator
//

template< class Sequence, class Reference, class Value >
struct fusion_iterator;

namespace detail {
template< class Sequence, class Reference, class Value >
struct fusion_iterator_super
{
    typedef
        boost::iterator_adaptor<
            // Pass everything; fusion_iterator_base isn't an iterator.
            fusion_iterator<Sequence, Reference, Value>,
            std::size_t,
            Value,
            boost::random_access_traversal_tag,
            Reference,
            std::ptrdiff_t
        >
    type;
};
}

template< class Sequence, class Reference, class Value=typename boost::remove_reference<Reference>::type >
struct fusion_iterator :
    detail::fusion_iterator_super<Sequence, Reference, Value>::type
{
    typedef typename detail::fusion_iterator_super<Sequence, Reference, Value>::type super;


    fusion_iterator()
    { }

    fusion_iterator(Sequence& seq, std::size_t i) :
        super(i), seq(boost::addressof(seq))
    { }

    Sequence *seq;

    Reference dereference() const
    {
        assert(seq);
        return fusion_iterator_dereference<Sequence, Reference>::call(*seq, this->base());
    }
};

}

#endif
