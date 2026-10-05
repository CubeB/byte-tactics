// Decompiled by deepseek-v4.1-flash. Names are provisional.

// Sums the first three bytes of each 4-byte entry into sums[256], fills
// idx[256] with 0..255, then selection-sorts sums and carries idx along.

struct RGBA {
    unsigned char r;    // +0
    unsigned char g;    // +1
    unsigned char b;    // +2
    unsigned char a;    // +3
};

// FUNCTION: 0x4ba920
void __stdcall SortByBrightness(unsigned char* data, int* sums, unsigned char* idx)
{
    int i;
    RGBA* p = (RGBA*)data;
    for (i = 0; i < 256; i++) {
        sums[i] = p[i].r + p[i].g + p[i].b;
        idx[i] = (unsigned char)i;
    }
    for (int k = 0; k < 256; k++) {
        for (int j = k + 1; j < 256; j++) {
            if (sums[k] > sums[j]) {
                int t = sums[k];
                sums[k] = sums[j];
                sums[j] = t;
                unsigned char tc = idx[k];
                idx[k] = idx[j];
                idx[j] = tc;
            }
        }
    }
}
