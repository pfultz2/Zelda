/* 
 * File:   auto.h
 * Author: pfultz
 *
 * Created on December 8, 2011, 8:19 PM
 */

#ifndef AUTO_H
#define	AUTO_H

#ifdef ZELDA_HAS_AUTO
#define ZELDA_AUTO(var, ...) auto var = __VA_ARGS__
#else
#define ZELDA_AUTO(var, ...) typeof(__VA_ARGS__) var = __VA_ARGS__
#endif

#endif	/* AUTO_H */

