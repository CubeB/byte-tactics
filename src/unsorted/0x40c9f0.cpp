// Decompiled by Sonnet. Names are provisional.

struct Class_0040c9f0 {
    char unknown_0[8];
    int* finish;   // +0x8

    int* FUN_0040c9f0(int* dest, int* first);
};

// FUNCTION: 0x40c9f0
int* Class_0040c9f0::FUN_0040c9f0(int* dest, int* first)
{
    int* old_finish = finish;
    while (first != old_finish) {
        *dest = *first;
        dest++;
        first++;
    }
    finish = dest;
    return old_finish;
}
