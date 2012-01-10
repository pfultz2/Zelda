/* 
 * File:   curry.h
 * Author: pfultz
 *
 * Created on November 19, 2011, 7:18 PM
 */

#ifndef CURRY_H
#define	CURRY_H

#include "pp.h"

namespace zelda {


template<class F, class Sig>
struct curry_front;

template<class F, void (int, int)>
struct curried_back
{
    int i;
    F f;
    curried(int i) : i(i) {};
    
    void operator[](int j)
    {
        return f(i, j);
    }
};

template<class F, void (int, int)>
struct curry_back
{
    curried_back<F, void (int, int)> operator()(int i)
    {
        return curried_back<F, void (int, int)>(i);
    }
    
    void operator()(int i, int j)
    {
        return F()(i, j);
    }
};
}

#endif	/* CURRY_H */

