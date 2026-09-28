// Decompiled by deepseek-v4.1-flash. Names are provisional.
// GLOBAL: 0x51fdb0
extern int DAT_0051fdb0;

// FUNCTION: 0x4b9f50
char* __stdcall FUN_004b9f50(char* out, int count, unsigned char a, unsigned char b)
{
    if (a == b) {
        do {
            int n = count;
            if (n > 0x7f) {
                n = 0x7f;
            }
            count -= n;
            unsigned char header = (unsigned char)n;
            header <<= 1;
            header |= 1;
            if (out != 0) {
                *out++ = header;
            }
            DAT_0051fdb0++;
        } while (count > 0);
    } else {
        int value = a;
        do {
            int n = count;
            if (n > 0x40) {
                n = 0x40;
            }
            count -= n;
            unsigned char header = (unsigned char)(n + 0x3f);
            header <<= 2;
            header |= 2;
            if (out != 0) {
                *out++ = header;
            }
            DAT_0051fdb0++;
            for (unsigned int shift = 0; shift < 8; shift += 8) {
                if (out != 0) {
                    *out++ = (char)(value >> shift);
                }
                DAT_0051fdb0++;
            }
        } while (count > 0);
    }
    return out;
}
