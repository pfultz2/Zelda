/*=============================================================================
    Copyright (c) 2012 Paul Fultz II
    unfold_iterator.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef ZELDA_GUARD_ITERATOR_UNFOLD_ITERATOR_H
#define ZELDA_GUARD_ITERATOR_UNFOLD_ITERATOR_H

#include <boost/assert.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/mpl/if.hpp>
#include <zelda/function/result_of.h>
#include <zelda/function/is_pure.h>

namespace zelda {



template< class State, class Operate, class Next, class Reference=typename result_of<Operate(State const&)>::type::reference_type, class Value=typename boost::remove_reference<Reference>::type >
struct unfold_iterator :
    boost::iterator_facade
    <
        unfold_iterator<State, Next, Operate>,
        Value,
        typename boost::mpl::if_
        <
            is_pure<Operate>,
            boost::forward_traversal_tag,
            boost::single_pass_traversal_tag
        >::type,
        Reference
    >
{
    typedef typename boost::mpl::if_
    <
        is_pure<Operate>,
        boost::forward_traversal_tag,
        boost::single_pass_traversal_tag
    > traversal_category;

    unfold_iterator()
    { }


    unfold_iterator(const State& init=State(), Operate op=Operate(), Next next=Next()) 
    : state(init), op(op), next(next), result(op(init))
    {}

    unfold_iterator(const State& init, Operate op, Next next, typename result_of<Operate(State const&)>::type result) 
    : state(init), op(op), next(next), result(result)
    {}


    State state;
    Operate op;
    Next next;
    typename result_of<Operate(State const&)>::type result;

    bool is_end() const
    {
        return !this->result;
    }

    Reference dereference() const
    {
        BOOST_ASSERT(!is_end());
        return *this->result;
    }

    void increment()
    {
        BOOST_ASSERT(!is_end());
        this->state = this->next(this->state);
        this->result = this->op(this->state);
    }

    bool equal_aux(const unfold_iterator& other, boost::forward_traversal_tag) const
    {
        if (this->is_end() != other.is_end())
            return false;
        else if (is_end() && other.is_end())
            return true;
        else 
        {
            BOOST_ASSERT(!is_end() && !other.is_end());
            return *this->result == *other.result;
        }
    }

    bool equal_aux(const unfold_iterator& other, boost::single_pass_traversal_tag) const
    {
        return this->is_end() == other.is_end();
    }

    bool equal(const unfold_iterator& other) const
    {
        return this->equal_aux(other, traversal_category());
    }
};
}


#endif
