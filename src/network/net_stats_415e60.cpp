// Decompiled by Opus. Names are provisional.
// Reads a signed bit field: ReadBits's value, sign-extended from `bits`.

class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08
    int ReadBits(int bits);
    int ReadSignedBits(int bits);
};

// FUNCTION: 0x415e60
int BitReader::ReadSignedBits(int bits)
{
    int r = ReadBits(bits);
    if (r & (1 << (bits - 1))) {
        r |= -1 << bits;
    }
    return r;
}
