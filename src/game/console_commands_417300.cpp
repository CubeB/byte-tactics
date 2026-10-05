// Decompiled by Sonnet. Names are provisional.
// Toggles one flag bit in the same 16-bit flags word as 0x416e30, 0x417060
// and 0x418ca0 (mask 0x40 = bit 6), then calls SaveSettings.

extern void* g_game;
extern void SaveSettings();

struct Flags_00417300
{
    unsigned short low : 6;
    unsigned short flag : 1;
    unsigned short rest : 9;
};

// FUNCTION: 0x417300
void __stdcall CmdClock(int unused)
{
    Flags_00417300* f = (Flags_00417300*)((char*)g_game + 0x37f2f);
    f->flag = !f->flag;
    SaveSettings();
}
