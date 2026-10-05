// Decompiled by Opus. Names are provisional.
// Grows the bit writer's buffer (the same class as 0x415b60, 0x415b90 and
// 0x415c10; see docs/consolidation.md). The original allocates with
// `new unsigned int(n)` (one dword holding n) instead of `new unsigned int[n]`.

class BitWriter {
public:
    int bit;                           // +0x0
    int index;                         // +0x4
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    void GrowBuffer();
};

static inline void CopyWords(unsigned int* first, unsigned int* last, unsigned int* dst)
{
    for (; first != last; first++) {
        *dst = *first;
        dst++;
    }
}

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
