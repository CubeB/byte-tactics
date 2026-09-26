// Decompiled by Sonnet. Names are provisional.

struct Image_004b96a0 {
    unsigned short width;   // +0x0
    unsigned short height;  // +0x2
    char unknown_4[4];      // +0x4
    char colorKey;          // +0x8
    char unknown_9[7];      // +0x9
    char* data;             // +0x10
};

// FUNCTION: 0x4b96a0
void __stdcall FUN_004b96a0(Image_004b96a0* param_1)
{
    int count = param_1->height * param_1->width;
    char* p = param_1->data;
    int i = count;

    if (count != 0) {
        do {
            if (*p != param_1->colorKey) {
                *p = 0;
            }
            p++;
        } while (--i != 0);
    }
}
