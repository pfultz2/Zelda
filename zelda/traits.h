/* 
 * File:   traits.h
 * Author: pfultz
 *
 * Created on November 17, 2011, 8:48 PM
 */

#ifndef ZELDA_TRAITS_H
#define	ZELDA_TRAITS_H

#include <boost/unordered_map.hpp>
#include <boost/range.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/has_xxx.hpp>
#include <boost/range/iterator.hpp>
#include <boost/utility.hpp>
#include <boost/mpl/logical.hpp>
#include <boost/iterator.hpp>
#include <string>
#include "requires.h"
#include <iso646.h>
#include <iterator>

#include "introspection.h"

namespace zelda {

BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

namespace details
{


namespace range_detail
{



template<class T, class Enabler = void>
struct has_range_iterator_impl
: boost::mpl::false_
{
};

template<class T>
struct has_range_iterator_impl<T, ZELDA_CLASS_REQUIRES(has_type< boost::range_mutable_iterator<T> >)>
: boost::mpl::true_
{
};

template<class T, class Enabler = void>
struct has_range_const_iterator_impl
: boost::mpl::false_
{
};

template<class T>
struct has_range_const_iterator_impl<T, ZELDA_CLASS_REQUIRES(has_type< boost::range_const_iterator<T> >)>
: boost::mpl::true_
{
};

template<class T, class Enabler = void>
struct is_iterator_impl
: boost::mpl::false_
{
};

template<class T>
struct is_iterator_impl<T, ZELDA_CLASS_REQUIRES(has_type< boost::iterator_category<T> >)>
: boost::mpl::true_
{
};



} // namespace range_detail
}

template<class T>
struct has_range_iterator
: details::range_detail::has_range_iterator_impl<T>
{
};

template<class T>
struct has_range_const_iterator
: details::range_detail::has_range_const_iterator_impl<T>
{
};

template<class T>
struct is_range : boost::mpl::or_<has_range_iterator<T>, has_range_const_iterator<T> >
{
};


template<class Range, class Value, class Enable = void>
struct is_range_of : boost::false_type
{
};

template<class Range, class Value>
struct is_range_of<Range, Value, ZELDA_CLASS_REQUIRES(is_range<Range>)> : 
boost::is_same<Value, typename boost::range_value<Range>::type >
{  
};

template<class T>
struct is_char_sequence : is_range_of<T, char>
{
};

template<class T>
struct is_pair : boost::false_type
{
};

template<class K, class V>
struct is_pair<std::pair<K, V> > : boost::true_type
{
};

template<class T, class Enable = void>
struct is_map_range : boost::false_type
{
};

template<class T>
struct is_map_range<T, ZELDA_CLASS_REQUIRES(is_range<T>)> : is_pair<typename boost::range_value<T>::type >
{  
};

template<class T>
struct is_iterator : details::range_detail::is_iterator_impl<T>
{
};

#define ZELDA_DEFINE_CATEGORY_TRAIT(trait, tag) \
template<class T, class Enable = void> \
struct trait##_iterator : boost::false_type \
{\
};\
template<class T> \
struct trait##_iterator<T, ZELDA_CLASS_REQUIRES(is_iterator<T>)> : \
boost::is_same<typename boost::iterator_category<T>::type, tag > \
{\
};\
template<class T, class Enable = void> \
struct trait##_range : boost::false_type \
{\
};\
template<class T> \
struct trait##_range<T, ZELDA_CLASS_REQUIRES(is_range<T>)> : \
boost::is_same<typename boost::iterator_category<typename boost::range_iterator<T>::type >::type, tag > \
{\
};\

ZELDA_DEFINE_CATEGORY_TRAIT(is_input, std::input_iterator_tag);
ZELDA_DEFINE_CATEGORY_TRAIT(is_output, std::output_iterator_tag);
ZELDA_DEFINE_CATEGORY_TRAIT(is_forward, std::forward_iterator_tag);
ZELDA_DEFINE_CATEGORY_TRAIT(is_bidirectional, std::bidirectional_iterator_tag);
ZELDA_DEFINE_CATEGORY_TRAIT(is_random_access, std::random_access_iterator_tag);

#undef ZELDA_DEFINE_CATEGORY_TRAIT

template<class T, class Traversal, class Enable = void>
struct has_iterator_traversal : boost::false_type
{
};

template<class T, class Traversal>
struct has_iterator_traversal<T, Traversal, ZELDA_CLASS_REQUIRES(is_iterator<T>)> : 
boost::is_convertible<typename boost::iterator_traversal<T>::type, Traversal>::type
{
};

template<class T, class Traversal, class Enable = void>
struct has_range_traversal : boost::false_type
{
};

template<class T, class Traversal>
struct has_range_traversal<T, Traversal, ZELDA_CLASS_REQUIRES(is_range<T>)> : 
boost::is_convertible<typename boost::iterator_traversal<typename boost::range_iterator<T>::type>::type, Traversal>::type
{
};



ZELDA_INTROSPECTION_HAS_MEMBER_FUNCTION_TEMPLATE(class T)(find, (T));

ZELDA_INTROSPECTION_HAS_MEMBER_FUNCTION(size);

}

#endif	/* ZELDA_TRAITS_H */

