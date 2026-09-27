// Decompiled by Opus. Names are provisional.
// Looks up the object's GAF entry by name and copies the first two words
// of frame obj->frame (from FUN_004b7f30) into the object.
struct GafEntry_004b8d40;
struct Gaf_004b8d40;
GafEntry_004b8d40* __stdcall FUN_004b8d40(Gaf_004b8d40* gaf, const char* name);
int __stdcall FUN_004b7f30(unsigned short* param_1, int param_2);

struct Holder_00479bf0 {
    char unknown_0[0xc0];
    Gaf_004b8d40* gaf;                 // +0xc0
};
struct Screen_00479bf0 {
    char unknown_0[4];
    Holder_00479bf0* holder;           // +0x4
};
#pragma pack(push, 1)
struct Game_00479bf0 {
    char unknown_0[0x531];
    Screen_00479bf0* screen;           // +0x531
};
struct Obj_00479bf0 {
    char unknown_0[0x17];
    short x;                           // +0x17
    short y;                           // +0x19
    char unknown_1b[0x2f - 0x1b];
    GafEntry_004b8d40* entry;          // +0x2f
    char unknown_33[0x13b - 0x33];
    unsigned char frame;               // +0x13b
};
#pragma pack(pop)
extern Game_00479bf0* g_game;

// FUNCTION: 0x479bf0
void __stdcall FUN_00479bf0(Obj_00479bf0* obj, char* name)
{
    Holder_00479bf0* h = g_game->screen->holder;
    obj->entry = 0;
    if (h->gaf) {
        GafEntry_004b8d40* e = FUN_004b8d40(h->gaf, name);
        if (e) {
            obj->entry = e;
            short* f = (short*)FUN_004b7f30((unsigned short*)e, obj->frame);
            if (f) {
                obj->x = f[0];
                obj->y = f[1];
            }
        }
    }
}
