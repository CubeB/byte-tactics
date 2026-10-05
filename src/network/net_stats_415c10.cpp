// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Bit writer (same class as 0x415b60, 0x415b90 and 0x415bb0; see
// docs/consolidation.md). Appends the low `bits` bits of `value`, lowest bits
// first. When the word is full it may spill into the next word and double the
// buffer; the read counterpart is 0x415dc0.
// The buffer is grown with `new unsigned int(newCapacity)`, one dword rather
// than an array, and freed with `delete` rather than `delete[]`; kept as the
// original does (see docs/bugs.md, 0x415bb0 also inlined here).

#include <string.h>

class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    void WriteBits(int value, int bits);
};

static inline void CopyWords(unsigned int* first, unsigned int* last, unsigned int* dst)
{
    for (; first != last; first++) {
        *dst = *first;
        dst++;
    }
}

static inline void Grow_00415c10(BitWriter* s)
{
    int newCapacity = s->capacity * 2;
    unsigned int* grown = new unsigned int(newCapacity);
    CopyWords(s->data, s->data + s->capacity, grown);
    if (s->data != s->buffer) {
        delete s->data;
    }
    s->capacity = newCapacity;
    s->data = grown;
}

// FUNCTION: 0x415c10
void BitWriter::WriteBits(int value, int bits)
{
    if (bits + index < 0x20) {
        data[bit] |= (value & ((1 << bits) - 1)) << index;
        index += bits;
        return;
    }
    if (index == 0) {
        data[bit] = value;
        bit++;
        if (bit == capacity) {
            Grow_00415c10(this);
        }
        data[bit] = 0;
        return;
    }
    int n = 0x20 - index;
    data[bit] |= (value & ((1 << n) - 1)) << index;
    bit++;
    if (bit == capacity) {
        Grow_00415c10(this);
    }
    index = bits - n;
    data[bit] = ((unsigned int)value >> n) & ((1 << index) - 1);
}
