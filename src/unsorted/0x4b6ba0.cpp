// Decompiled by Opus. Names are provisional.
// Four-byte checksum of a buffer: byte sum, byte xor, sum of (index ^ byte)
// and xor of (index + byte), packed low byte first.

// FUNCTION: 0x4b6ba0
int __stdcall FUN_004b6ba0(unsigned char* data, int len)
{
    unsigned char a = 0, b = 0, c = 0, d = 0;
    for (int i = 0; i < len; i++, data++) {
        a += *data;
        b ^= *data;
        c += i ^ *data;
        d ^= i + *data;
    }
    return (d << 24) | (c << 16) | (b << 8) | a;
}
