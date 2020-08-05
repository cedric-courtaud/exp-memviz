#ifndef __MEMVIZ_BITFIELD_H__
#define __MEMVIZ_BITFIELD_H__

#include "pub_tool_basics.h"

typedef struct {
    ULong * __bits;
    ULong capacity;
} Bitfield;

void initBitField(Bitfield * b, ULong capacity);

void setBit(Bitfield *b,  UInt pos);
void unsetBit(Bitfield * b, UInt pos);
Bool isSet(Bitfield * b, UInt pos);

static inline ULong getCapacity(Bitfield * b) {
    return b->capacity;
}

#endif