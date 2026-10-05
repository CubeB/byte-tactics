// Decompiled by Opus. Names are provisional.

// GLOBAL: 0x51fcb0
extern unsigned char DAT_0051fcb0[];
// GLOBAL: 0x51fdb0
extern int DAT_0051fdb0;

// FUNCTION: 0x4b9ed0
void __stdcall FUN_004b9ed0(char* out, int count)
{
    int index = 0;
    do {
        int n = count;
        if (n > 0x40) {
            n = 0x40;
        }
        count -= n;
        unsigned char header = (unsigned char)(n + 0x3f);
        header <<= 2;
        if (out != 0) {
            *out++ = header;
        }
        DAT_0051fdb0++;
        for (int i = 0; i < n; i++) {
            int value = DAT_0051fcb0[index++];
            for (unsigned int shift = 0; shift < 8; shift += 8) {
                if (out != 0) {
                    *out++ = (char)(value >> shift);
                }
                DAT_0051fdb0++;
            }
        }
    } while (count > 0);
}
