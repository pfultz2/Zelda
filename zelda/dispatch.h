#ifndef ZELDA_DISPATCH_H
#define ZELDA_DISPATCH_H

#include "pp.h"

//#define ZELDA_DISPATCH_fun macro, mode, 

#define ZELDA_DISPATCH(macro) macro, 1, 
#define ZELDA_DISPATCH_ALL(macro) macro, 2, 

#define DETAIL_ZELDA_DISPATCH_MODE(...) PP_CHECK(__VA_ARGS__)
#define DETAIL_ZELDA_DISPATCH_MACRO(...) PP_HEAD(__VA_ARGS__)
#define DETAIL_ZELDA_DISPATCH_CONTENT_I(x,n, ...) __VA_ARGS__
#define DETAIL_ZELDA_DISPATCH_CONTENT(...) DETAIL_ZELDA_DISPATCH_CONTENT_I(__VA_ARGS__)

#define DETAIL_ZELDA_0(macro, tail, all) PP_CAT(ZELDA_ERROR_UNRECOGNIZED_SYMBOL_, PP_HEAD all)
#define DETAIL_ZELDA_1(macro, tail, all) macro tail
#define DETAIL_ZELDA_2(macro, tail, all) macro all
  
#define DETAIL_ZELDA(x, all) PP_CAT(DETAIL_ZELDA_, PP_CHECK x)(PP_HEAD x, (DETAIL_ZELDA_DISPATCH_CONTENT tail), all)
#define ZELDA(...) DETAIL_ZELDA(\
(PP_CAT(ZELDA_DISPATCH_, PP_HEAD(__VA_ARGS__)) PP_TAIL(__VA_ARGS__)), \
(__VA_ARGS__))


#endif