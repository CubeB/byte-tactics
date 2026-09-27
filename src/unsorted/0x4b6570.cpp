// Decompiled by space-bunny-free. Names are provisional.
// Frame rate counter: adds the time since the last call to the counter, and once
// a second stores the number of frames in that second as the frame rate. When
// there is no offscreen GDI device context it draws "FRATE <rate>" over the
// frame through the surface's device context.
//
// 96.7%, 294 of 294 bytes, so every instruction is the right size and the
// frame, the six reloads and the register choices are right. All three
// differences are the same thing: where the reload of the address-taken local
// `dc` sits inside the argument block of its call, the original lets it sink
// one or two instructions further down than ours:
//   +0x95 the original loads the hoisted SelectObject import into esi before
//        reloading dc, ours reloads dc first;
//   +0xdc the original emits `lea ecx,[buf]`, `push ebx` and only then reloads
//        dc, ours reloads dc before the lea;
//   +0xf1 the original pushes the old font before reloading dc, ours reloads
//        dc first.
// The registers and the displacements agree in all three places (edx/eax and
// [esp+0xc] shifted by 4 for each push), so this is a scheduling tie-break,
// not a different source value. The other three calls (SetBkMode at +0x83,
// SetTextColor at +0xc0, EndDraw at +0x105) put the reload first in both.
//
// What the source below already gets right, and what the three diffs must not
// disturb: the frame is exactly filled, 4 bytes of `dc` at +0xc plus the
// 128-byte buffer at +0x10 leaves the 132 bytes the prologue reserves, so
// there is no room for a third local; the address-taken `dc` is reloaded after
// every one of the six calls; the SelectObject import is hoisted into esi
// because it is called twice; the wsprintf result is kept in ebx; and the
// vtable load `mov edx,[eax]` comes after the pushes of both __stdcall
// methods, which is why those two slots must be __stdcall.
//
// A /Fa listing of this file shows that MSVC 5.0 emits the whole argument
// block of a call under one statement, and that in this build it does not move
// anything inside a block: our order is the plain source order, the reload of
// `dc` first, in all six calls. So the original's compiler had three of those
// reloads at a lower priority than the neighbouring import load, buffer lea
// and register push. Nothing in this function's source shape moved them.
// Compiled and diffed instruction by instruction, all byte-identical to this
// file: swapping the two local declarations; putting both locals in one local
// struct and using its fields; reading the local through a `HDC*`; an inline
// helper taking `HDC&` for the whole drawing block; inline helpers for the
// font read, the rate read, SetBkMode, TextOut, the GetDC call and the
// `&dc`; about fifteen spellings (`== 0` against `!`, TRANSPARENT against 1,
// RGB(255,255,0) against 0xffff, wsprintf against wsprintfA, TextOut against
// TextOutA, split declarations and assignments, `== NULL` for the offscreen
// test, DWORD against unsigned int, `+=` against `= ... +`); 0 to 12 and 500
// extern declarations, 2000 unused prototypes and an unrelated function placed
// ahead of this one; and all 128 header sets headers.py tries.
//
// The three reloads are therefore very likely decided by compiler state this
// function's own source cannot reach, most likely the register allocator's
// state left by whatever Cavedog compiled just before it in the same
// translation unit, which docs/AGENTS.md expects to be recoverable when
// functions are regrouped into their original translation units.
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
