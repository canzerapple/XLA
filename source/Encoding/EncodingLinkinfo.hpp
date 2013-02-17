#ifndef XLA_ENCODINGLINKINFO_INCLUDE
#define XLA_ENCODINGLINKINFO_INCLUDE

#define RET_SHIFT_ILSEQ(n)  (-1-2*(n))
#define RET_ILSEQ           RET_SHIFT_ILSEQ(0)
#define RET_TOOFEW(n)       (-2-2*(n))
#define DECODE_SHIFT_ILSEQ(r)  ((unsigned int)(RET_SHIFT_ILSEQ(0) - (r)) / 2)
#define DECODE_TOOFEW(r)       ((unsigned int)(RET_TOOFEW(0) - (r)) / 2)
#define RET_ILUNI      -1
#define RET_TOOSMALL   -2


typedef unsigned int ucs4_t;
typedef unsigned int state_t;



struct conv_struct {
	state_t istate;
	state_t ostate;
};
typedef conv_struct* conv_t;

typedef conv_struct  conv;

#include "libiconv/utf16le.h"

#define local_mbtowc utf16le_mbtowc
#define local_wctomb utf16le_wctomb

#define CONV_BUFFERTOSMALL( mode ) ((mode < 0) && (( mode & 1 ) == 0 ))

#define ENCODING_DYNLIBRARY

#endif