// BitReader: reads bit fields from an array of dwords, lowest bits first. The
// one declaration of the class, for net_stats.cpp (which defines ReadBits and
// ReadSignedBits) and every file that reads packets; the writer is BitWriter.
#ifndef BIT_READER_H
#define BIT_READER_H

class BitReader {
public:
    unsigned int* data;                // +0x00
    int index;                         // +0x04
    int bit;                           // +0x08

    int ReadBits(int bits);
    int ReadSignedBits(int bits);

    int ReadBit()
    {
        int r = (data[index] & (1 << bit)) != 0;
        if (++bit == 32) {
            bit = 0;
            index++;
        }
        return r;
    }
};

#endif
