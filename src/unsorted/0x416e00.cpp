// Decompiled by Haiku. Names are provisional.

extern char* g_game;

// FUNCTION: 0x416e00
void __stdcall FUN_00416e00(int unused) {
    unsigned short* field_ptr = (unsigned short*)((char*)g_game + 0x37f2f);
    unsigned short v = *field_ptr;
    unsigned short t = ~v;
    unsigned char tl = (unsigned char)t;
    unsigned char vl = (unsigned char)v;
    tl ^= vl;
    t = (t & ~0xFF) | tl;
    t &= 4;
    t ^= v;
    *field_ptr = t;
}
