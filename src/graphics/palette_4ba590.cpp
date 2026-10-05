// Decompiled by Sonnet. Names are provisional.

extern int GetDisplay();
extern void __stdcall SetPaletteColors(unsigned char* palette, int first, int count);

struct Obj_004ba590
{
    char unknown_0[0x214];
    unsigned char palette[0x400];   // +0x214, 256 entries
    int field_614;                  // +0x614
};

// FUNCTION: 0x4ba590
void __stdcall SetBrightness(int param_1)
{
    Obj_004ba590* p = (Obj_004ba590*)GetDisplay();
    p->field_614 = param_1;
    SetPaletteColors(p->palette, 0, 0x100);
}
