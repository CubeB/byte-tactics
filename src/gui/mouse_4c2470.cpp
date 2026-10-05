// Decompiled by Opus. Names are provisional.
// Increments a hide counter and draws the image at the stored position when
// the counter was zero (unless mode +0x1ce is 1).

#pragma pack(push, 2)
struct Obj_004c2470 {
    char unknown_0[0x1ae];
    int count;                         // +0x1ae
    char unknown_1b2[4];
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    void* image;                       // +0x1be
    char unknown_1c2[0x1ce - 0x1c2];
    int mode;                          // +0x1ce
};
#pragma pack(pop)

Obj_004c2470* GetDisplay(void);
void __stdcall DrawSurface(void* dest, void* image, int x, int y);

// FUNCTION: 0x4c2470
void FUN_004c2470()
{
    Obj_004c2470* o = GetDisplay();
    if (o->mode != 1 && o->count++ == 0)
        DrawSurface(0, o->image, o->x, o->y);
}
