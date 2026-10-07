// Decompiled by Space Bunny Free, finished by Sonnet 5.5. Names are provisional.
// Frame rate counter: adds the time since the last call to the counter, and once
// a second stores the number of frames in that second as the frame rate. When
// there is no offscreen GDI device context it draws "FRATE <rate>" over the
// frame through the surface's device context.
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

extern App_4b6570* g_display;

// FUNCTION: 0x4b6570
int __stdcall DrawFrameRate()
{
    FrameCounter_4b6570* c = &g_display->counter;
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

    if (!g_display->offscreenDC) {
        HDC dc;
        char buf[128];
        if (g_display->surface->GetDC(&dc) == 0) {
            SetBkMode(dc, TRANSPARENT);
            HGDIOBJ oldFont = SelectObject(dc, g_display->font);
            int len = wsprintf(buf, "FRATE %d", g_display->counter.rate);
            SetTextColor(dc, RGB(255, 255, 0));
            TextOut(dc, 0, 0, buf, len);
            SelectObject(dc, oldFont);
            g_display->surface->EndDraw(dc);
        }
    }

    return g_display->counter.rate;
}
