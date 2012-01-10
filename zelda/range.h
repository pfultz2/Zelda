/* 
 * File:   range.h
 * Author: pfultz
 *
 * Created on November 17, 2011, 9:08 PM
 */

#ifndef ZELDA_RANGE_H
#define	ZELDA_RANGE_H

#include <boost/range.hpp>
#include <boost/iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>
#include <boost/iterator/zip_iterator.hpp>
#include <boost/type_traits.hpp>
#include "traits.h"
#include "pp.h"

namespace zelda {

template<class Iterator>
class iterator_range
{
    Iterator b;
    Iterator e;
    
public:
    typedef Iterator   iterator;
    typedef Iterator   const_iterator;
    typedef boost::iterator_difference<iterator>::type difference_type;
        
    iterator_range(Iterator b, Iterator e) : b(b), e(e)
    {

    }
    
    Iterator begin() const
    {
        return b;
    }
    Iterator end() const
    {
        return e;
    }
};

template<class Iterator>
ZELDA_FUNCTION_REQUIRES(is_iterator<Iterator>)
(iterator_range<Iterator>) range(Iterator b, Iterator e)
{
    return boost::iterator_range<Iterator>(b, e);
}


template<class Number>
ZELDA_FUNCTION_REQUIRES(boost::is_arithmetic<Number>)
(iterator_range<boost::counting_iterator<Number> >) range(Number b, Number e)
{
    typedef boost::counting_iterator<Number> iterator;
    return iterator_range<iterator >(iterator(b), iterator(e));
}

template<class Number>
ZELDA_FUNCTION_REQUIRES(boost::is_arithmetic<Number>)
(iterator_range<boost::counting_iterator<Number> >) range(Number e)
{
    typedef boost::counting_iterator<Number> iterator;
    return iterator_range<iterator >(iterator(0), iterator(e));
}

//Reverse
template<class Iterator>
class reverse_iterator_range : public iterator_range < boost::reverse_iterator< Iterator > >
{
public:
    typedef iterator_range<iterator> base;

    reverse_iterator_range(Iterator b, Iterator e) : base(iterator(b), iterator(e))
    {
    }
};
;

template<class Range> 
ZELDA_FUNCTION_REQUIRES(is_range<Range>)
(reverse_iterator_range < typename boost::range_iterator < Range > ::type >) reverse_iterator_range(Range& r)
{
    return reverse_iterator_range <typename boost::range_iterator<Range>::type > (boost::begin(r), boost::end(r));
};

//Filter
template<class Iterator, class Predicate>
class filter_iterator_range : public iterator_range < boost::filter_iterator< Iterator, Predicate > >
{
public:
    typedef iterator_range<iterator> base;

    filter_iterator_range(Iterator b, Iterator e, Predicate p) : base(iterator(p, b, e), iterator(e))
    {
    }
};
;

template<class Range, class Predicate > 
ZELDA_FUNCTION_REQUIRES(is_range<Range>)
(filter_iterator_range < typename boost::range_iterator < Range > ::type, Predicate >) filter_iterator_range(Range& r, Predicate p)
{
    return filter_iterator_range <typename boost::range_iterator<Range>::type, Predicate > 
            (boost::begin(r), boost::end(r), p);
};

//Transform
template<class Iterator, class Transformer>
class transform_iterator_range : public iterator_range < boost::transform_iterator< Transformer > >
{
public:
    typedef iterator_range<iterator> base;

    transform_iterator_range(Iterator b, Iterator e, Transformer t) : base(iterator(b, t), iterator(e, t))
    {
    }
};
;

template<class Range, class Transformer > 
ZELDA_FUNCTION_REQUIRES(is_range<Range>)
(transform_iterator_range < typename boost::range_iterator < Range > ::type, Transformer >) 
transform_iterator_range(Range& r, Transformer t)
{
    return transform_iterator_range <typename boost::range_iterator<Range>::type, Transformer > 
            (boost::begin(r), boost::end(r), t);
};

#define ZELDA_RANGE_ZIP_RANGE_EACH(r, data, x) typename boost::range_iterator < x > ::type
#define ZELDA_RANGE_ZIP_RANGE(...) iterator_range< boost::zip_iterator< boost::tuple< \
PP_ARGS_TRANSFORM(ZELDA_RANGE_ZIP_RANGE_EACH, data, __VA_ARGS__) > > >

#define ZELDA_RANGE_ZIP_MAKE_EACH(r, data, x) data(x)
#define ZELDA_RANGE_ZIP_MAKE(call, ...) boost::make_zip_iterator(boost::make_tuple(PP_ARGS_TRANSFORM(ZELDA_RANGE_ZIP_MAKE_EACH, call, __VA_ARGS__)))

template<class Range1, class Range2>
ZELDA_FUNCTION_REQUIRES(is_range<Range1>, is_range<Range2>)
(ZELDA_RANGE_ZIP_RANGE(Range1, Range2))
zip(Range1& r1, Range2& r2)
{
    return ZELDA_RANGE_ZIP_RANGE(Range1, Range2)
            (
            ZELDA_RANGE_ZIP_MAKE(boost::begin, r1, r2),
            ZELDA_RANGE_ZIP_MAKE(boost::end, r1, r2)
            );
}

template<class Range1, class Range2, class Range3>
ZELDA_FUNCTION_REQUIRES(is_range<Range1>, is_range<Range2>, is_range<Range3>)
(ZELDA_RANGE_ZIP_RANGE(Range1, Range2, Range3))
zip(Range1& r1, Range2& r2, Range3& r3)
{
    return ZELDA_RANGE_ZIP_RANGE(Range1, Range2, Range3)
            (
            ZELDA_RANGE_ZIP_MAKE(boost::begin, r1, r2, r3),
            ZELDA_RANGE_ZIP_MAKE(boost::end, r1, r2, r3)
            );
}

template<class Range1, class Range2, class Range3, class Range4>
ZELDA_FUNCTION_REQUIRES(is_range<Range1>, is_range<Range2>, is_range<Range3>, is_range<Range4>)
(ZELDA_RANGE_ZIP_RANGE(Range1, Range2, Range3, Range4))
zip(Range1& r1, Range2& r2, Range3& r3, Range4& r4)
{
    return ZELDA_RANGE_ZIP_RANGE(Range1, Range2, Range3, Range4)
            (
            ZELDA_RANGE_ZIP_MAKE(boost::begin, r1, r2, r3, r4),
            ZELDA_RANGE_ZIP_MAKE(boost::end, r1, r2, r3, r4)
            );
}


template<class Iterator>
ZELDA_FUNCTION_REQUIRES(has_iterator_traversal<Iterator, boost::bidirectional_traversal_tag>)
(typename boost::iterator_value<Iterator>::type) iterator_at(Iterator b, Iterator e, int index)
{
    if (index < 0) 
    {
        std::advance(e, index);
        return e;
    }
    else
    {
        std::advance(b, index);
        return b;
    }
}

template<class Iterator>
ZELDA_FUNCTION_REQUIRES(has_iterator_traversal<Iterator, boost::forward_traversal_tag>, ZELDA_EXCLUDE(has_iterator_traversal<Iterator, boost::bidirectional_traversal_tag>))
(typename boost::iterator_value<Iterator>::type) iterator_at(Iterator b, Iterator e, int index)
{
    //TODO: Assert index >= 0
    std::advance(b, index);
    return b;
}

template<class Range>
ZELDA_FUNCTION_REQUIRES(has_range_traversal<Iterator, boost::forward_traversal_tag>)
(typename boost::iterator_value<typename boost::range_iterator < Range > ::type>::type) 
iterator_at(Range& r, int index)
{
    return iterator_at(boost::begin(r), boost::end(r));
}





//
//#define ZELDA_DETAIL_RANGE_DEFINE_ITERATOR_RANGE_CLASS_BASE(...) (__VA_ARGS__) \
//    { \
//    } \
//};
//
//#define ZELDA_DETAIL_RANGE_DEFINE_ITERATOR_RANGE_CLASS_CONSTRUCTOR(...) \
//(__VA_ARGS__) : base ZELDA_DETAIL_RANGE_DEFINE_ITERATOR_RANGE_CLASS_BASE 
//
//#define ZELDA_RANGE_DEFINE_ITERATOR_RANGE_CLASS(name, ...) \
//class BOOST_PP_CAT(name, _iterator_range) : public iterator_range \
//< \
//__VA_ARGS__ \
//> \
//{ \
//public: \
//    typedef iterator_range<iterator> base; \
//    BOOST_PP_CAT(name, _iterator_range) ZELDA_DETAIL_RANGE_DEFINE_ITERATOR_RANGE_CLASS_CONSTRUCTOR
//
//
//#define DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_PARAMS(...) , PP_ARGS_PAIR(__VA_ARGS__)
//#define DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_FORWARD(...) , PP_ARGS_PAIR_NAME(__VA_ARGS__)
//
//#define DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN(...) \
//ZELDA_FUNCTION_REQUIRES(is_range<Range>) \
//(BOOST_PP_CAT(PP_OPTIONAL_FIRST(__VA_ARGS__), _iterator_range)<typename boost::range_iterator<Range>::type>) \
//BOOST_PP_CAT(PP_OPTIONAL_FIRST(__VA_ARGS__), _iterator_range)\
//(Range& r PP_OPTIONAL_WHEN(DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_PARAMS, __VA_ARGS__) ) \
//{ \
//    return BOOST_PP_CAT(PP_OPTIONAL_FIRST(__VA_ARGS__), _iterator_range)<typename boost::range_iterator<Range>::type> \
//            (boost::begin(r), boost::end(r) PP_OPTIONAL_WHEN(DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_FORWARD, __VA_ARGS__)); \
//} \
//
//#define ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN template<class Range> DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN
//#define ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_TEMPLATE(...) template<class Range, __VA_ARGS__> DETAIL_ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN
//
//
//
//
////template<class Iterator>
////class reverse_iterator_range : public iterator_range
////<
////boost::reverse_iterator< Iterator >
////>
////{
////public:
////    typedef iterator_range<iterator> base;
////    reverse_iterator_range(Iterator b, Iterator e) : base(iterator(b), iterator(e))
////    {
////        
////    }
////};
////
////template<class Range>
////ZELDA_FUNCTION_REQUIRES(is_iterator<Range>)
////(reverse_iterator_range<typename boost::range_iterator<Range>::type>) reverse_range(Range& r)
////{
////    return reverse_iterator_range<typename boost::range_iterator<Range>::type>
////            (boost::begin(r), boost::end(r));
////}
//
//template<class Iterator>
//ZELDA_RANGE_DEFINE_ITERATOR_RANGE_CLASS(reverse, boost::reverse_iterator< Iterator >)
//(Iterator b, Iterator e)(iterator(b), iterator(e));
//
//ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN(reverse);
//
//template<class Iterator, class Predicate>
//ZELDA_RANGE_DEFINE_ITERATOR_RANGE_CLASS(filter, boost::filter_iterator< Iterator, Predicate >)
//(Iterator b, Iterator e, Predicate p)(iterator(p, b, e), iterator(e));
//
//ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_TEMPLATE(class Predicate)(filter, (Predicate) p);
//
//template<class Iterator, class Transformer>
//ZELDA_RANGE_DEFINE_ITERATOR_RANGE_CLASS(transform, boost::transform_iterator< Transformer >)
//(Iterator b, Iterator e, Transformer t)(iterator(b, t), iterator(e, t));
//
//ZELDA_RANGE_DEFINE_ITERATOR_RANGE_FN_TEMPLATE(class Transformer)(transform, (Transformer) t);



}

#endif	/* RANGE_H */
