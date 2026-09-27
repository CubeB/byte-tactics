// Decompiled by Space Bunny Free. Names are provisional.
// Frame rate counter: samples the clock, updates the frames-per-second value
// and draws it over the frame when there is no offscreen GDI device context.
//
// Still differs (96.7%, 294 of 294 bytes, all three diffs are pure scheduling
// of the same instructions inside the GDI block, same registers, same sizes):
//   +0x95 the original loads the hoisted SelectObject import into esi before
//        reloading dc, ours reloads dc first;
//   +0xdc the original emits `lea ecx,[buf]`, `push ebx` and only then reloads
//        dc, ours reloads dc before the lea;
//   +0xf1 the original pushes the old font before reloading dc, ours reloads
//        dc first.
// So the original puts the reload of the address-taken local dc one slot later
// in those three statements. Phrasing the statements differently (temporaries,
// comma operators, static inline helpers, block scopes, dead statements, other
// field types, pack(1) instead of pack(2), a different buffer size) never moved
// it; every variant compiles to the same order. The vtable methods must be
// __stdcall (that is what produces `push this` plus `mov edx,[eax]`), and
// headers.py reports no header set that changes anything.
// A second pass followed the guide's "copy them into locals just before the
// call; the order of those copies decides which load MSVC hoists": a fresh
// HDC copy immediately before each of the six calls, the same for a three-call
// subset, one copy for the whole block, SetTextColor moved ahead of the
// wsprintf, and the font and rate read into locals before any drawing call.
// The per-call copies still give 96.7% with the same three diffs; the other
// three fall to 59 to 70%, so they change more than they fix. The order
// between a hoisted import load and a stack-slot reload is the scheduler's
// choice and does not follow from the source.
#include <windows.h>

// The surface at +0x8c is used through its vtable. Only two slots matter here:
// 0x44 hands out a GDI device context, 0x68 gives it back after the drawing.
class Surface_4b6570 {
public:
    virtual int Slot_00();
    virtual int Slot_04();
    virtual int Slot_08();
    virtual int Slot_0c();
    virtual int Slot_10();
    virtual int Slot_14();
    virtual int Slot_18();
    virtual int Slot_1c();
    virtual int Slot_20();
    virtual int Slot_24();
    virtual int Slot_28();
    virtual int Slot_2c();
    virtual int Slot_30();
    virtual int Slot_34();
    virtual int Slot_38();
    virtual int Slot_3c();
    virtual int Slot_40();
    virtual int __stdcall GetDC(HDC* dc);  // +0x44
    virtual int Slot_48();
    virtual int Slot_4c();
    virtual int Slot_50();
    virtual int Slot_54();
    virtual int Slot_58();
    virtual int Slot_5c();
    virtual int Slot_60();
    virtual int Slot_64();
    virtual int __stdcall EndDraw(HDC dc); // +0x68
};

struct FrameCounter_4b6570 {
    int accum;                       // +0x0  ms accumulated since the last sample
    unsigned int lastTick;           // +0x4
    int frames;                      // +0x8  frames since the last sample
    int rate;                        // +0xc  frames counted in the last second
};

#pragma pack(push, 2)
struct App_4b6570 {
    char unknown_0[0x44];
    HDC offscreenDC;                 // +0x44
    char unknown_48[0x8c - 0x48];
    Surface_4b6570* surface;         // +0x8c
    char unknown_90[0xb0 - 0x90];
    HFONT font;                      // +0xb0
    char unknown_b4[0x1da - 0xb4];
    FrameCounter_4b6570 counter;     // +0x1da
};
#pragma pack(pop)

extern App_4b6570* DAT_0051fbd0;

// FUNCTION: 0x4b6570
int FUN_004b6570()
{
    FrameCounter_4b6570* c = &DAT_0051fbd0->counter;
    unsigned int now = GetTickCount();
    c->accum += now - c->lastTick;
    c->lastTick = now;
    c->frames++;
    if (c->accum > 2000) {
        c->accum = 1000;
    }
    if (c->accum > 1000) {
        c->rate = c->frames;
        c->accum -= 1000;
        c->frames = 0;
    }

    if (!DAT_0051fbd0->offscreenDC) {
        HDC dc;
        char buf[128];
        if (DAT_0051fbd0->surface->GetDC(&dc) == 0) {
            SetBkMode(dc, TRANSPARENT);
            HGDIOBJ oldFont = SelectObject(dc, DAT_0051fbd0->font);
            int len = wsprintf(buf, "FRATE %d", DAT_0051fbd0->counter.rate);
            SetTextColor(dc, RGB(255, 255, 0));
            TextOut(dc, 0, 0, buf, len);
            SelectObject(dc, oldFont);
            DAT_0051fbd0->surface->EndDraw(dc);
        }
    }

    return DAT_0051fbd0->counter.rate;
}
