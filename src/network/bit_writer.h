// BitWriter: writes bit fields into an array of dwords, lowest bits first. The
// one declaration of the class, for net_stats.cpp (which defines the methods)
// and every file that writes packets; the reader is BitReader.
#ifndef BIT_WRITER_H
#define BIT_WRITER_H

class BitWriter {
public:
    int index;                         // +0x0 current word index
    int bit;                           // +0x4 bits used in the current word
    int capacity;                      // +0x8
    unsigned int* data;                // +0xc
    unsigned int buffer[0x100];        // +0x10

    BitWriter();
    void FreeBuffer();
    void GrowBuffer();
    void WriteBits(int value, int bits);
    void SetByteAt(int offset, unsigned char value);
};

#endif
