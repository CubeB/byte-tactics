// Decompiled by Sonnet 5.5, finished by space-bunny-free, finished by Sonnet 5.5. Names are provisional.
// Sets `count` palette entries from `start` (4 bytes each: three colour
// bytes and a zero) under the 'MAIN' lock. The entries are stored in the
// display's palette table, scaled by the display's brightness (+0x614) and
// clamped to 255 into a local palette, and that palette is then either
// installed through GDI (a new logical palette plus SetDIBColorTable on the
// display's DC, when the display has a window DC) or handed to the
// DirectDraw palette object, whose failure returns 0.
//
// MATCHED. What finally worked (after 66.2%): the destination pointer `p` and
// a second walking source pointer `s` (both advanced in the for header), an
// `end = start + count` local, and the statement order brightness, p, end, s
// before the copy loop. Declaring the SetEntries result in a named HRESULT
// gives the original's `cmp eax, edi` against the hoisted zero.
#include <windows.h>
#include <ddraw.h>

#pragma pack(push, 1)
struct Display_004ba200 {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    HDC dc;                            // +0x48
    HPALETTE palette;                  // +0x4c
    char unknown_50[0x94 - 0x50];
    LPDIRECTDRAWPALETTE ddPalette;     // +0x94
    char unknown_98[0xf0 - 0x98];
    unsigned short pad_f0 : 2;         // +0xf0
    unsigned short bit2 : 1;
    unsigned short rest_f0 : 13;
    char unknown_f2[0x214 - 0xf2];
    unsigned int entries[256];         // +0x214
    char unknown_614[0];
};
#pragma pack(pop)

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;

Display_004ba200* FUN_004b6220(void);
void* FUN_004d83b0(const char* name, unsigned int size);
void FUN_004d85a0(void* p);

static inline LONG Lock()
{
    while (1) {
        LONG r = InterlockedExchange(&DAT_0052a4e8, 0x4d41494e);
        if (r == 0) {
            DAT_0052a4ec = 0x4d41494e;
            return 0;
        }
        if (DAT_0052a4ec == 0x4d41494e)
            return r;
        WaitForSingleObject(DAT_0052a4f0, INFINITE);
    }
}

static inline void Unlock(LONG held)
{
    if (held == 0) {
        DAT_0052a4ec = 0;
        InterlockedExchange(&DAT_0052a4e8, 0);
        SetEvent(DAT_0052a4f0);
    }
}

// FUNCTION: 0x4ba200
int __stdcall FUN_004ba200(unsigned char* src, int start, int count)
{
    LONG held = Lock();
    float brightness;
    Display_004ba200* d;
    unsigned char local[0x400];
    unsigned char quad[0x400];
    int i;
    int end;
    d = FUN_004b6220();
    brightness = *(float*)((char*)d + 0x614);
    unsigned int* p = d->entries + start;
    end = start + count;
    unsigned char* s = src;
    for (i = start; i < end; i++, p++, s += 4)
        *p = *(unsigned int*)s;
    for (i = start; i < end; i++) {
        float v0 = src[i * 4] * brightness;
        if (v0 > 255.0)
            v0 = 255.0f;
        local[i * 4] = (unsigned char)(int)v0;
        float v1 = src[i * 4 + 1] * brightness;
        if (v1 > 255.0)
            v1 = 255.0f;
        local[i * 4 + 1] = (unsigned char)(int)v1;
        float v2 = src[i * 4 + 2] * brightness;
        if (v2 > 255.0)
            v2 = 255.0f;
        local[i * 4 + 2] = (unsigned char)(int)v2;
        local[i * 4 + 3] = 0;
    }

    if (d->field_44 != 0) {
        if (d->palette)
            DeleteObject(d->palette);
        LOGPALETTE* lp = (LOGPALETTE*)FUN_004d83b0("Palette", 0x408);
        for (i = 0; i < 256; i++) {
            ((unsigned int*)lp->palPalEntry)[i] = ((unsigned int*)local)[i];
            quad[i * 4 + 2] = local[i * 4];
            quad[i * 4 + 1] = local[i * 4 + 1];
            quad[i * 4 + 0] = local[i * 4 + 2];
            quad[i * 4 + 3] = 0;
        }
        lp->palVersion = 0x300;
        lp->palNumEntries = 0x100;
        d->palette = CreatePalette(lp);
        SetDIBColorTable(d->dc, 0, 0x100, (RGBQUAD*)quad);
        FUN_004d85a0(lp);
    } else if (d->bit2) {
        HRESULT hr = d->ddPalette->SetEntries(0, start, count, (LPPALETTEENTRY)(local + start * 4));
        if (hr != 0) {
            Unlock(held);
            return 0;
        }
    }
    Unlock(held);
    return 1;
}
