#ifndef ZELDA_INVOKE_H
#define ZELDA_INVOKE_H

#include <boost/fusion/iterator/deref.hpp>
#include <boost/fusion/iterator/value_of.hpp>
#include <boost/fusion/iterator/distance.hpp>
#include <boost/fusion/iterator/advance.hpp>
#include <boost/fusion/iterator.hpp>
#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/functional/invocation/invoke.hpp>
#include "result_of.h"
#include "requires.h"
#include "typeof.h"

namespace zz {

namespace  details {
template<class F, class Iterator, class End, class... T>
struct seq_to_function_impl;

template<class F, class I, class End, class D, class... T>
struct seq_to_function_impl_next
{
    typedef seq_to_function_impl<F, typename I::type, End, typename D::type, T...> type;
};

template<class F, class Iterator, class End, class... T>
struct seq_to_function_impl 
: zelda::mpl::if_<boost::is_same<Iterator, End>,
    boost::mpl::identity<F(T...)>,
    zelda::mpl::lazy<seq_to_function_impl_next<F, boost::fusion::result_of::next<Iterator>, End, boost::fusion::result_of::deref<Iterator>, T...> >
    >::type {};

template<class F, class Seq>
struct seq_to_function
: seq_to_function_impl<F, typename boost::fusion::result_of::begin<Seq>::type, typename boost::fusion::result_of::end<Seq>::type>
{};

}


template<class F, class Seq>
struct is_callable_seq : zelda::is_callable<typename details::seq_to_function<F, Seq>::type>
{};

template<class F, class Seq>
struct result_of_seq : zelda::result_of<typename details::seq_to_function<F, Seq>::type>
{};



namespace details{

template<class R, class F, class Iterator, class End, class... Args>
ZELDA_FUNCTION_REQUIRES(boost::is_same<Iterator, End>)
(R) invoke_impl(F f, Iterator it, End e, Args && ... args)
{
    return f(std::forward<Args>(args)...);
}

template<class R, class F, class Iterator, class End, class... Args>
ZELDA_FUNCTION_REQUIRES(not boost::is_same<Iterator, End>)
(R) invoke_impl(F f, Iterator it, End e, Args && ... args)
{
    return invoke_impl<R>(f, boost::fusion::next(it), e, boost::fusion::deref(it), std::forward<Args>(args)...);
}


}

//using boost::fusion::invoke;

template<class F, class Sequence, ZELDA_REQUIRES(boost::fusion::traits::is_sequence<Sequence>)>
typename result_of_seq<F, Sequence>::type invoke(F f, Sequence&& seq)
{
    return details::invoke_impl<typename result_of_seq<F, Sequence>::type>(f, boost::fusion::begin(seq), boost::fusion::end(seq));
}




}

#endif