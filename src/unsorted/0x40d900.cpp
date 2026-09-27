// Decompiled by deepseek-v4.1-flash. Names are provisional.
//
// Partial (72.7%): clears the "kind" byte of each dirty 8-cell group.
// The structure, offsets and constants match; what differs is which register
// holds the dirty mask. The original loads it into ebx in the main loop and
// copies edx->ebx in the tail (`mov ebx, edx` before computing p); here it is
// edx in the main loop and ebx in the tail, and the tail computes p through
// eax instead of edi. A guarded do/while outer loop plus an internally
// guarded ClearBits helper (reproducing the duplicated entry test) raised
// this from 57.1% to 72.7%.

class Class_0040d900 {
public:
    char unknown_0[0x1c];
    unsigned char* field_0x1c;         // +0x1c cell array
    char unknown_1[0x08];
    int field_0x28;                    // +0x28 cell count
    unsigned int* field_0x2c;          // +0x2c dirty-cell masks

    void ClearBits(unsigned char* p, unsigned int bits)
    {
        if (bits != 0) {
            while (bits != 0) {
                if (bits & 1) {
                    unsigned char* q = p;
                    int n = 8;
                    do {
                        *q = 0;
                        q += 4;
                    } while (--n);
                }
                bits >>= 1;
                p += 0x20;
            }
        }
    }
    void FUN_0040d900();
};

// FUNCTION: 0x40d900
void Class_0040d900::FUN_0040d900()
{
    int nfull = ((field_0x28 + 0xff) >> 8) - 1;
    int row = 0;
    if (nfull > 0) {
        row = 0;
        do {
            unsigned int bits = field_0x2c[row];
            if (bits != 0) {
                field_0x2c[row] = 0;
                unsigned char* p = field_0x1c + row * 0x400;
                ClearBits(p, bits);
            }
            row++;
        } while (row < nfull);
    }
    {
        unsigned int bits = field_0x2c[row];
        if (bits != 0) {
            field_0x2c[row] = 0;
            unsigned char* p = field_0x1c + row * 0x400;
            while (bits != 0) {
                if (bits & 1) {
                    int col = row << 8;
                    unsigned char* q = p;
                    int n = 8;
                    do {
                        if (col < field_0x28)
                            *q = 0;
                        col++;
                        q += 4;
                    } while (--n);
                }
                bits >>= 1;
                p += 0x20;
            }
        }
    }
}
