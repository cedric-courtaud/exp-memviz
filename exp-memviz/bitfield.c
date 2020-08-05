#include "bitfield.h"
#include "pub_tool_mallocfree.h"

void initBitField(Bitfield * b, ULong capacity) {
    b->capacity = capacity;
    ULong size = capacity / sizeof(ULong);

    b->__bits = VG_(calloc)("initBitField", size, sizeof(ULong));

}

#define bit_sizeof(type) (sizeof(type) * 8)

static inline ULong * getWordAddr(Bitfield * b, UInt pos) {
    return &b->__bits[pos / bit_sizeof(*b->__bits)];
}

static inline ULong getMask(Bitfield * b, UInt pos) {
    pos %= bit_sizeof(*b->__bits);

    return 1 << pos;
}

static inline void adaptCapacity(Bitfield * b, UInt pos) {
    if (pos >= b->capacity) {
        ULong old_size = b->capacity / sizeof(ULong);

        while (pos >= b->capacity) {
            b->capacity *= 2;
        }
        
        ULong new_size = b->capacity / sizeof(ULong);

        b->__bits = VG_(realloc)("initBitField", b->__bits, new_size);

        for (int i = old_size; i < new_size; i++) {
            b->__bits[i] = 0;
        }
    }
}

void setBit(Bitfield *b,  UInt pos) {
    adaptCapacity(b, pos);

    ULong * word = getWordAddr(b, pos);
    ULong mask = getMask(b, pos);

   *word |= mask;
}

void unsetBit(Bitfield * b, UInt pos) {
    adaptCapacity(b, pos);

    ULong * word = getWordAddr(b, pos);
    ULong mask = getMask(b, pos);

   *word &= ~mask;
}

Bool isSet(Bitfield * b, UInt pos) {
    if (pos >= b->capacity) return False;

    ULong * word = getWordAddr(b, pos);
    ULong mask = getMask(b, pos);

    return (*word & mask) != 0;
}