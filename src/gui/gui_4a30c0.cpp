// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Refreshes GUI entry `index` (0x15b-byte entries, entry 0 holds the count at
// +0xb6): clears the two words at +0xba/+0xbc, selects the entry of type 7
// whose group number matches this entry's group and makes its id current, then
// quantises this entry's height (+0x19) down to a multiple of the current font
// line step and stamps the current time into +0xb6.

#pragma pack(push, 1)
struct Entry_004a30c0 {                // 0x15b bytes
    unsigned char type;               // +0x00
    char unknown_01[0x19 - 0x01];
    short height;                     // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                       // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                      // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                   // +0xba
    short field_bc;                   // +0xbc
    char unknown_be[0xd6 - 0xbe];
    int id;                           // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct Holder_004a30c0 {
    char unknown_0[4];
    Entry_004a30c0* entries;           // +0x04
};

struct Class_004a30c0 {
    char unknown_0[0x18];
    Holder_004a30c0* holder;           // +0x18
};

struct Font_004a30c0 {
    char unknown_0[0xc];
    void* glyphs;                      // +0x0c
};

struct Dialog {
    int group;                         // +0x00
    char unknown_04[0x14 - 0x04];
    Font_004a30c0* font;               // +0x14
};

extern Dialog* g_guiContext;

void __stdcall SetFont(int id);
int GetFontHeight();
void* __stdcall GetGafFrame(void* a, int b);
unsigned int __cdecl GetTicks();

// FUNCTION: 0x4a30c0
void __stdcall FUN_004a30c0(Class_004a30c0* obj, int index)
{
    Entry_004a30c0* entries = obj->holder->entries;
    Entry_004a30c0* e = &entries[index];
    e->field_bc = 0;
    e->field_ba = 0;
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == e->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        SetFont(g_guiContext->group);
    }
    int step;
    if (g_guiContext->font == 0) {
        step = GetFontHeight();
    } else {
        step = *(unsigned short*)((char*)GetGafFrame(g_guiContext->font->glyphs, 0x49) + 2) + 2;
    }
    int h = e->height;
    e->height = h - h % (step + 2);
    *(int*)&e->count = GetTicks();
}
