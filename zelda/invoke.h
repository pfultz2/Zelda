#ifndef ZELDA_INVOKE_H
#define ZELDA_INVOKE_H

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include "result_of.h"
#include "requires.h"
#include "typeof.h"

namespace zelda {

namespace details{

template<class F, class Iterator, class End, class... Args>
ZELDA_FUNCTION_REQUIRES(ZELDA_EXCLUDE(boost::fusion::result_of::equal_to<Iterator, End>))
(void) invoke_impl(F f, Iterator it, End e, Args && ... args)
{
    invoke_impl(f, boost::fusion::next(it), e, boost::fusion::deref(it), std::forward<Args>(args)...);
}

template<class F, class Iterator, class End, class... Args>
ZELDA_FUNCTION_REQUIRES(boost::fusion::result_of::equal_to<Iterator, End>)
(zelda::result_of<F(Args && ...)>) invoke_impl(F f, Iterator it, End e, Args && ... args)
{
    return f(std::forward<Args>(args)...);
}
}

template<class F, class Sequence, ZELDA_REQUIRES(boost::fusion::traits::is_sequence<Sequence>)>
auto invoke(F f, const Sequence& seq) ZELDA_RETURNS(details::invoke_impl(f, boost::fusion::begin(seq), boost::fusion::end(seq)))

}

#endif