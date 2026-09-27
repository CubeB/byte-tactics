// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x40d520
void __stdcall FUN_0040d520(unsigned char* dest, unsigned int count, unsigned char* src) {
    if (count > 0) {
        for (unsigned int i = 0; i < count; i++) {
            if (dest != 0) {
                *dest = *src;
            }
            dest++;
        }
    }
}
