// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 2)
struct Rec14_0046fb40 {
    int a;
    int b;
    int c;
    short d;
};
#pragma pack(pop)

// FUNCTION: 0x46fb40
void __stdcall FUN_0046fb40(Rec14_0046fb40* dst, unsigned int count, Rec14_0046fb40* src)
{
    for (; count > 0; count--) {
        if (dst != 0) {
            *dst = *src;
        }
        dst++;
    }
}
