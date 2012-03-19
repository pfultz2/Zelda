



template< typename T , typename fallback_ = boost::mpl::bool_< false > > class has_test
{
    template< typename U > struct has_test_introspect
    {
        template< template< typename V0 > class V > struct has_test_substitute0 { };
        template< template< typename V0 , typename V1 > class V > struct has_test_substitute1 { };
        template< template< typename V0 , typename V1 , typename V2 > class V > struct has_test_substitute2 { };
        template< template< typename V0 , typename V1 , typename V2 , typename V3 > class V > struct has_test_substitute3 { };
        template< template< typename V0 , typename V1 , typename V2 , typename V3 , typename V4 > class V > struct has_test_substitute4 { };
        template< typename V > static boost::mpl::aux::no_tag has_test_test(...);
        template< typename V > static boost::mpl::aux::yes_tag has_test_test( boost::mpl::aux::type_wrapper< V > const volatile* , has_test_substitute0 < V::template test >* = 0 );
        template< typename V > static boost::mpl::aux::yes_tag has_test_test( boost::mpl::aux::type_wrapper< V > const volatile* , has_test_substitute1 < V::template test >* = 0 );
        template< typename V > static boost::mpl::aux::yes_tag has_test_test( boost::mpl::aux::type_wrapper< V > const volatile* , has_test_substitute2 < V::template test >* = 0 );
        template< typename V > static boost::mpl::aux::yes_tag has_test_test( boost::mpl::aux::type_wrapper< V > const volatile* , has_test_substitute3 < V::template test >* = 0 );
        template< typename V > static boost::mpl::aux::yes_tag has_test_test( boost::mpl::aux::type_wrapper< V > const volatile* , has_test_substitute4 < V::template test >* = 0 );
        static const bool value = sizeof(has_test_test< U >(0)) == sizeof(boost::mpl::aux::yes_tag);
        typedef boost::mpl::bool_< value > type;
    };
public:
    static const bool value = has_test_introspect< T >::value;
    typedef typename has_test_introspect< T >::type type;
};



template<class Zelda_I_C, class Zelda_I_T> struct has_method
{
    typedef char yes;
    typedef long no;
    template<class Zelda_I_TT, class Zelda_I_Enable = typename boost::enable_if<boost::mpl::and_<boost::is_convertible<Zelda_I_TT, Zelda_I_T>, boost::mpl::bool_<true> > >::type> struct selector {};
    template<class Zelda_I_U> static yes check( selector<typeof( zelda::declval<Zelda_I_U>.method )> * );
    template<class Zelda_I_U> static no check(...);
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes);
    typedef boost::mpl::bool_<value> type;
    \
};
template<class Zelda_I_C, class Zelda_I_R, class... Zelda_I_Args> struct has_method<Zelda_I_C, Zelda_I_R(Zelda_I_Args...)>
{
    typedef char yes;
    typedef long no;
    template<class Zelda_I_TT, class Zelda_I_Enable = typename boost::enable_if<boost::mpl::and_<boost::is_convertible<Zelda_I_TT, Zelda_I_R>, boost::mpl::bool_<true> > >::type> struct selector {};
    template<class Zelda_I_U> static yes check( selector<typeof( zelda::declval<Zelda_I_U>.method(zelda::declval<Zelda_I_Args>()...) )> * );
    template<class Zelda_I_U> static no check(...);
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes);
    typedef boost::mpl::bool_<value> type;
    \
};;
template<class Zelda_I_C, class Zelda_I_T> struct has_var
{
    typedef char yes;
    typedef long no;
    template<class Zelda_I_TT, class Zelda_I_Enable = typename boost::enable_if<boost::mpl::and_<boost::is_convertible<Zelda_I_TT, Zelda_I_T>, boost::mpl::bool_<true> > >::type> struct selector {};
    template<class Zelda_I_U> static yes check( selector<typeof( zelda::declval<Zelda_I_U>.var )> * );
    template<class Zelda_I_U> static no check(...);
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes);
    typedef boost::mpl::bool_<value> type;
    \
};
template<class Zelda_I_C, class Zelda_I_R, class... Zelda_I_Args> struct has_var<Zelda_I_C, Zelda_I_R(Zelda_I_Args...)>
{
    typedef char yes;
    typedef long no;
    template<class Zelda_I_TT, class Zelda_I_Enable = typename boost::enable_if<boost::mpl::and_<boost::is_convertible<Zelda_I_TT, Zelda_I_R>, boost::mpl::bool_<true> > >::type> struct selector {};
    template<class Zelda_I_U> static yes check( selector<typeof( zelda::declval<Zelda_I_U>.var(zelda::declval<Zelda_I_Args>()...) )> * );
    template<class Zelda_I_U> static no check(...);
    static const bool value = sizeof(check<Zelda_I_C>(0)) == sizeof(yes);
    typedef boost::mpl::bool_<value> type;
    \
};;
template<class Zelda_I_C> struct has_tpl
{
    typedef char yes;
    typedef long no;
    template<template<class...> class Zelda_I_TT> struct selector {};
    template<class Zelda_I_U> static yes check(selector<Zelda_I_U::template tpl> *);
    template<class Zelda_I_U> static no check(...);
    static const bool value=sizeof(check<Zelda_I_C>(0))==sizeof(yes)); typedef boost::mpl::bool_<value> type;
};;
template<class Zelda_I_C, class Zelda_I_T = zelda::introspection::void_> struct has_type
{
    typedef char yes;
    typedef long no;
    template<class Zelda_I_TT, class Zelda_I_Enable = typename boost::enable_if<boost::mpl::and_<boost::is_convertible<Zelda_I_TT, Zelda_I_T>, boost::mpl::bool_<true> > >::type> struct selector {};
    template<class Zelda_I_U> static yes check(selector<typename Zelda_I_U::name> *);
    template<class Zelda_I_U> static no check(...);
    static const bool value=sizeof(check<Zelda_I_C>(0))==sizeof(yes)); typedef boost::mpl::bool_<value> type;
};;
