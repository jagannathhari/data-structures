#ifndef INCLUDE_BITSET_H

#include <limits.h>
#define BITSET(n) calloc((n+CHAR_BIT-1)/CHAR_BIT,CHAR_BIT)
#define BIT_RESET(bset,pos)  bset[(pos)/CHAR_BIT]&= ~(1<<((pos)%CHAR_BIT))
#define BIT_SET(bset,pos)  bset[(pos)/CHAR_BIT]|= (1<<((pos)%CHAR_BIT))
#define BIT_GET(bset,pos)  bset[(pos)/CHAR_BIT]&(1<<((pos)%CHAR_BIT))

#endif
