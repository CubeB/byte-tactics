// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x406c40
void FUN_00406c40(int* dest, int count, int* src) {
    for (; count > 0; count--) {
        if (dest != 0) {
            *dest = *src;
        }
        dest++;
        src++;
    }
}
