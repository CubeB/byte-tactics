// Decompiled by Opus, Haiku and deepseek-v4.1-flash. Names are provisional.
// A bit writer with a 0x100-dword inline buffer (a 0x410-byte stack object in
// 0x48b710). The read counterpart is 0x415dc0.

#include <string.h>

class BitWriter {
public:
    int bit;                           // +0x0 current word index
    int index;                         // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    BitWriter();
    void FreeBuffer();
    void GrowBuffer();
    void WriteBits(int value, int bits);
    void SetByteAt(int offset, unsigned char value);
};

// FUNCTION: 0x415b60
BitWriter::BitWriter()
{
    bit = 0;
    index = 0;
    capacity = 0x100;
    data = buffer;
    *data = 0;
}

// FUNCTION: 0x415b90
void BitWriter::FreeBuffer()
{
    if (data != buffer) {
        operator delete(data);
    }
}

static inline void CopyWords(unsigned int* first, unsigned int* last, unsigned int* dst)
{
    for (; first != last; first++) {
        *dst = *first;
        dst++;
    }
}

// The original allocates with `new unsigned int(n)` (one dword holding n)
// instead of `new unsigned int[n]`, and frees with `delete` rather than
// `delete[]` (docs/bugs.md).
// FUNCTION: 0x415bb0
void BitWriter::GrowBuffer()
{
    int newCapacity = capacity * 2;
    unsigned int* grown = new unsigned int(newCapacity);
    CopyWords(data, data + capacity, grown);
    if (data != buffer) {
        delete data;
    }
    capacity = newCapacity;
    data = grown;
}

// Appends the low `bits` bits of `value`, lowest bits first. When the word is
// full it may spill into the next word and double the buffer.
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
            GrowBuffer();
        }
        data[bit] = 0;
        return;
    }
    int n = 0x20 - index;
    data[bit] |= (value & ((1 << n) - 1)) << index;
    bit++;
    if (bit == capacity) {
        GrowBuffer();
    }
    index = bits - n;
    data[bit] = ((unsigned int)value >> n) & ((1 << index) - 1);
}

// FUNCTION: 0x415da0
void BitWriter::SetByteAt(int offset, unsigned char value)
{
    ((unsigned char*)data)[offset] = value;
}
