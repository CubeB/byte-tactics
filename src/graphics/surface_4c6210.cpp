// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Locks the screen into a local surface, blits the bitmap at display+0x98
// into it at (0, 0), then unlocks. Both objects at arg+4 and arg+8 are
// COM-like interfaces; slot 27 (+0x6c) is tested first and its non-zero
// result aborts the blit. The unlock is UnlockScreen inlined (see
// 0x4c6c50.cpp); the result variable, zero on the success path, is reused
// as the zero arguments.
#include <ddraw.h>

struct Surface {
    int data[12];
};

// COM interface (slot 27 = +0x6c is the tested method).
class Intf_004c6210 {
public:
    virtual int __stdcall Slot00();
    virtual int __stdcall Slot01();
    virtual int __stdcall Slot02();
    virtual int __stdcall Slot03();
    virtual int __stdcall Slot04();
    virtual int __stdcall Slot05();
    virtual int __stdcall Slot06();
    virtual int __stdcall Slot07();
    virtual int __stdcall Slot08();
    virtual int __stdcall Slot09();
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall Slot12();
    virtual int __stdcall Slot13();
    virtual int __stdcall Slot14();
    virtual int __stdcall Slot15();
    virtual int __stdcall Slot16();
    virtual int __stdcall Slot17();
    virtual int __stdcall Slot18();
    virtual int __stdcall Slot19();
    virtual int __stdcall Slot20();
    virtual int __stdcall Slot21();
    virtual int __stdcall Slot22();
    virtual int __stdcall Slot23();
    virtual int __stdcall Slot24();
    virtual int __stdcall Slot25();
    virtual int __stdcall Slot26();
    virtual int __stdcall Slot27();
};

struct Screen_004c6210 {
    char unknown_0[0xc];
    IDirectDrawSurface* surface;       // +0xc

    void UnlockRect(LPRECT p) { surface->Unlock(p); }
};

struct Display_004c6210 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    char unknown_48[0x80 - 0x48];
    Screen_004c6210 screen;            // +0x80
    char unknown_90[0x98 - 0x90];
    Surface* field_98;                 // +0x98
    char unknown_9c[0xdc - 0x9c];
    int field_dc;                      // +0xdc
};

struct Arg_004c6210 {
    int unknown_0;                     // +0x0
    Intf_004c6210* field_4;            // +0x4
    Intf_004c6210* field_8;            // +0x8
};

extern int g_screenLockCount;

Display_004c6210* GetDisplay(void);
int __stdcall LockScreen(Surface* out);
void __cdecl BlitSurface(Surface* dst, Surface* src, int x, int y);

// FUNCTION: 0x4c6210
int __stdcall RestoreSurfaces(Arg_004c6210* arg)
{
    Display_004c6210* d = GetDisplay();
    if (d->field_44 != 0)
        return 0;

    int r = arg->field_4->Slot27();
    if (r == 0) {
        r = arg->field_8->Slot27();
        if (r == 0) {
            Surface screen;
            LockScreen(&screen);
            BlitSurface(&screen, d->field_98, r, r);

            Display_004c6210* d2 = GetDisplay();
            if (d2->field_44 == 0 && d2->field_dc == 0 && d2->screen.surface != 0) {
                d2->screen.UnlockRect((LPRECT)r);
                if (g_screenLockCount > 0)
                    g_screenLockCount--;
            }
        }
    }
    return r;
}
