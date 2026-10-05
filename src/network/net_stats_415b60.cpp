// Decompiled by Opus. Names are provisional.
// Constructor of a bit writer with a 0x100-dword inline buffer (a 0x410-byte
// stack object in 0x48b710; WriteBits writes to it and FreeBuffer frees
// a grown buffer).

class Class_00415b60 {
public:
    int bit;                           // +0x0
    int index;                         // +0x4
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    Class_00415b60();
};

// FUNCTION: 0x415b60
Class_00415b60::Class_00415b60()
{
    bit = 0;
    index = 0;
    capacity = 0x100;
    data = buffer;
    *data = 0;
}
