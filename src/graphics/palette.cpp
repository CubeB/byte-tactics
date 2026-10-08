// Decompiled by Sonnet 5.5, space-bunny-free, deepseek-v4.1-flash, Sonnet, Haiku and Opus. Names are provisional.
// The palette module: the display's palette table and brightness, the alpha,
// shade, light, gray and blue tables, the palette index lookups and the file
// name helpers, in address order (the palette module of data/modules.csv).
#include <windows.h>
#include <ddraw.h>
#include <string.h>

// The display context at g_display, as this module sees it: the GDI and
// DirectDraw palettes, the table buffers at +0xc0 to +0xd0, the flags at
// +0xf0 and the palette table at +0x214. display.cpp has the whole object.
#pragma pack(push, 1)
struct DisplayContext {
    char unknown_0[0x44];
    int field_44;                      // +0x44
    HDC dc;                            // +0x48
    HPALETTE hpalette;                 // +0x4c
    char unknown_50[0x94 - 0x50];
    LPDIRECTDRAWPALETTE ddPalette;     // +0x94
    char unknown_98[0xc0 - 0x98];
    void* alphaTable;                  // +0xc0
    void* shadeTable;                  // +0xc4
    void* lightTable;                  // +0xc8
    void* grayTable;                   // +0xcc
    void* blueTable;                   // +0xd0
    char unknown_d4[0xf0 - 0xd4];
    unsigned short opt : 1;            // +0xf0
    unsigned short bit1 : 1;
    unsigned short gdi : 1;            // bit 2
    unsigned short bit3 : 1;
    unsigned short bit4 : 1;
    unsigned short has_c0 : 1;         // bit 5
    unsigned short has_c4 : 1;         // bit 6
    unsigned short has_c8 : 1;         // bit 7
    unsigned short has_cc : 1;         // bit 8
    unsigned short has_d0 : 1;         // bit 9
    unsigned short bit10_15 : 6;
    char unknown_f2[0x214 - 0xf2];
    unsigned int entries[256];         // +0x214
    int paletteBrightness;             // +0x614, read as a float by SetPaletteColors
};
#pragma pack(pop)

// A palette entry's four bytes, as the table builders and GetPaletteColors
// walk them.
struct RGBA {
    unsigned char r;                   // +0
    unsigned char g;                   // +1
    unsigned char b;                   // +2
    unsigned char a;                   // +3
};

DisplayContext* GetDisplay(void);
void* __cdecl GameAllocIgnoreTag(const char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);
void __stdcall SortByBrightness(unsigned char* data, int* sums, unsigned char* idx);
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* band, unsigned char* order, PALETTEENTRY color);

extern LONG DAT_0052a4e8;
extern LONG DAT_0052a4ec;
extern HANDLE DAT_0052a4f0;
extern char g_alphaTableName[];
extern const char g_shadeTableName[];

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

// Sets `count` palette entries from `start` (4 bytes each: three colour
// bytes and a zero) under the 'MAIN' lock. The entries are stored in the
// display's palette table, scaled by the display's brightness (+0x614) and
// clamped to 255 into a local palette, and that palette is then either
// installed through GDI (a new logical palette plus SetDIBColorTable on the
// display's DC, when the display has a window DC) or handed to the
// DirectDraw palette object, whose failure returns 0.
// FUNCTION: 0x4ba200
int __stdcall SetPaletteColors(unsigned char* src, int start, int count)
{
    LONG held = Lock();
    float brightness;
    DisplayContext* d;
    unsigned char local[0x400];
    unsigned char quad[0x400];
    int i;
    int end;
    d = GetDisplay();
    // Statement order brightness, p, end, s before the copy loop; p and s both
    // advance in the for header.
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
        if (d->hpalette)
            DeleteObject(d->hpalette);
        LOGPALETTE* lp = (LOGPALETTE*)GameAllocIgnoreTag("Palette", 0x408);
        for (i = 0; i < 256; i++) {
            ((unsigned int*)lp->palPalEntry)[i] = ((unsigned int*)local)[i];
            quad[i * 4 + 2] = local[i * 4];
            quad[i * 4 + 1] = local[i * 4 + 1];
            quad[i * 4 + 0] = local[i * 4 + 2];
            quad[i * 4 + 3] = 0;
        }
        lp->palVersion = 0x300;
        lp->palNumEntries = 0x100;
        d->hpalette = CreatePalette(lp);
        SetDIBColorTable(d->dc, 0, 0x100, (RGBQUAD*)quad);
        GameFreeThunk(lp);
    } else if (d->gdi) {
        // Named HRESULT: gives the compare against the hoisted zero.
        HRESULT hr = d->ddPalette->SetEntries(0, start, count, (LPPALETTEENTRY)(local + start * 4));
        if (hr != 0) {
            Unlock(held);
            return 0;
        }
    }
    Unlock(held);
    return 1;
}

// Fetches the current 256-entry palette (from the DirectDraw palette object
// when there is no GDI palette, otherwise through GetPaletteEntries) and
// converts entries [first, first+count) into 4-byte RGB0 pixels written to
// the caller's buffer. Returns 0 if the palette could not be read, 1 otherwise.
// FUNCTION: 0x4ba4c0
int __stdcall GetPaletteColors(unsigned char* dest, int first, int count)
{
    PALETTEENTRY pal[256];
    DisplayContext* d = GetDisplay();
    if (d->gdi) {
        if (d->field_44 != 0) {
            if (GetPaletteEntries(d->hpalette, 0, 0x100, pal) == 0)
                return 0;
        } else {
            HRESULT hr = d->ddPalette->GetEntries(0, 0, 0x100, pal);
            if (hr != 0)
                return 0;
        }
    }
    RGBA* out = (RGBA*)dest;
    count += first;
    for (int i = first; i < count; i++) {
        out[i].r = pal[i].peRed;
        out[i].g = pal[i].peGreen;
        out[i].b = pal[i].peBlue;
        out[i].a = 0;
    }
    return 1;
}

// FUNCTION: 0x4ba590
void __stdcall SetBrightness(int param_1)
{
    DisplayContext* p = GetDisplay();
    p->paletteBrightness = param_1;
    SetPaletteColors((unsigned char*)p->entries, 0, 0x100);
}

// FUNCTION: 0x4ba5c0
int __stdcall AllocAlphaTable(int param_1)
{
    void* result = GameAllocIgnoreTag(g_alphaTableName, 0x10000);
    *(void**)((int)param_1 + 0xc0) = result;
    return 1;
}

// FUNCTION: 0x4ba5f0
void __stdcall FreeAlphaTable(DisplayContext* param)
{
    GameFreeThunk(param->alphaTable);
}

// FUNCTION: 0x4ba610
int __stdcall AllocShadeTable(DisplayContext* param_1) {
    void* result = GameAllocIgnoreTag(g_shadeTableName, 0x2000);
    param_1->shadeTable = result;
    return 1;
}

// FUNCTION: 0x4ba640
void __stdcall FreeShadeTable(DisplayContext* obj)
{
    GameFreeThunk(obj->shadeTable);
}

// FUNCTION: 0x4ba660
int __stdcall AllocLightTable(DisplayContext* obj)
{
    obj->lightTable = GameAllocIgnoreTag("LIGHT TABLE", 0x2000);
    return 1;
}

// FUNCTION: 0x4ba690
void __stdcall FreeLightTable(int param_1)
{
    GameFreeThunk(*(void**)(param_1 + 0xc8));
}

// FUNCTION: 0x4ba6b0
int __stdcall AllocGrayTable(DisplayContext* obj)
{
    obj->grayTable = GameAllocIgnoreTag("GRAY TABLE", 0x100);
    return 1;
}

// FUNCTION: 0x4ba6e0
void __stdcall FreeGrayTable(DisplayContext* obj)
{
    GameFreeThunk(obj->grayTable);
}

// FUNCTION: 0x4ba700
int __stdcall AllocBlueTable(DisplayContext* obj)
{
    obj->blueTable = GameAllocIgnoreTag("BLUE TABLE", 0x100);
    return 1;
}

// FUNCTION: 0x4ba730
void __stdcall FreeBlueTable(DisplayContext* obj)
{
    GameFreeThunk(obj->blueTable);
}

// Builds the 256x256 colour-blend table at obj->alphaTable when has_c0 is
// set: diagonal entries are the palette index itself, off-diagonal entries
// are the palette index closest to the per-channel average of palette entries
// row and col.
// FUNCTION: 0x4ba750
unsigned int* __stdcall BuildAlphaTable(unsigned char* data)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_c0) {
        int sums[256];
        unsigned char idx[256];
        SortByBrightness(data, sums, idx);
        RGBA* pal = (RGBA*)data;
        int row = 0;
        int off = 0;
        for (; off < 0x10000; off += 0x100, row++) {
            for (int col = 0; col < 0x100; col++) {
                if (row == col) {
                    ((unsigned char*)obj->alphaTable)[off + col] = (unsigned char)row;
                } else {
                    PALETTEENTRY color;
                    color.peRed = (pal[row].r + pal[col].r) / 2;
                    color.peGreen = (pal[row].g + pal[col].g) / 2;
                    color.peBlue = (pal[row].b + pal[col].b) / 2;
                    ((unsigned char*)obj->alphaTable)[off + col] = NearestColorInBand((PALETTEENTRY*)data, sums, idx, color);
                }
            }
        }
        return (unsigned int*)obj->alphaTable;
    }
    return 0;
}

// Returns the index of the palette entry closest to `color` (squared RGB
// distance). The colour fields are read in the loop in the source; MSVC
// hoists their widened values out of it, which is why the loop counter,
// not the colour, gets a callee-saved register.
// FUNCTION: 0x4ba880
unsigned char __stdcall NearestColor(PALETTEENTRY* palette, PALETTEENTRY color)
{
    int best = 1000000000;
    unsigned char bestIndex;
    for (int i = 0; i < 256; i++) {
        int dr = palette[i].peRed - color.peRed;
        int dg = palette[i].peGreen - color.peGreen;
        int db = palette[i].peBlue - color.peBlue;
        int d = dr * dr + dg * dg + db * db;
        if (d < best) {
            bestIndex = i;
            best = d;
        }
    }
    return bestIndex;
}

// Sums the first three bytes of each 4-byte entry into sums[256], fills
// idx[256] with 0..255, then selection-sorts sums and carries idx along.
// FUNCTION: 0x4ba920
void __stdcall SortByBrightness(unsigned char* data, int* sums, unsigned char* idx)
{
    int i;
    RGBA* p = (RGBA*)data;
    for (i = 0; i < 256; i++) {
        sums[i] = p[i].r + p[i].g + p[i].b;
        idx[i] = (unsigned char)i;
    }
    for (int k = 0; k < 256; k++) {
        for (int j = k + 1; j < 256; j++) {
            if (sums[k] > sums[j]) {
                int t = sums[k];
                sums[k] = sums[j];
                sums[j] = t;
                unsigned char tc = idx[k];
                idx[k] = idx[j];
                idx[j] = tc;
            }
        }
    }
}

// The shade table's scale factor, declared here rather than at the top: the
// symbol count in front of SortByBrightness is what its allocation needs
// (docs/c2-regalloc.md).
extern double g_shadeStep;

// Finds the palette entry closest to `color` whose brightness band `band[k]`
// is within 40 of the colour's brightness; returns `order[bestIndex]`.
// FUNCTION: 0x4ba9d0
unsigned char __stdcall NearestColorInBand(PALETTEENTRY* palette, int* band, unsigned char* order, PALETTEENTRY color)
{
    int best = 1000000000;
    unsigned char bestIndex;
    int sum = color.peRed + (color.peGreen + color.peBlue);
    int i;
    // Index `band` directly: no local pointer copy.
    for (i = 0; i < 256; i++) {
        if (band[i] >= sum - 40) {
            if (band[i] > sum + 40)
                break;
            int k = order[i];
            // One expression, not three locals: puts the loop counter in edx.
            int d = (palette[k].peRed - color.peRed) * (palette[k].peRed - color.peRed)
                  + (palette[k].peGreen - color.peGreen) * (palette[k].peGreen - color.peGreen)
                  + (palette[k].peBlue - color.peBlue) * (palette[k].peBlue - color.peBlue);
            if (d < best) {
                bestIndex = i;
                best = d;
            }
        }
    }
    if (best == 1000000000)
        bestIndex = i;
    return order[bestIndex];
}

// Sibling of 0x4bab00 and 0x4bab60: copies a 64 KB table into the buffer at
// +0xc0 when bit 5 of the flags word at +0xf0 is set.
// FUNCTION: 0x4baad0
void __stdcall SetAlphaTable(unsigned int* param_1)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_c0) {
        memcpy(obj->alphaTable, param_1, 0x4000 * 4);
    }
}

// FUNCTION: 0x4bab00
void __stdcall SetShadeTable(unsigned int* param_1)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_c4) {
        memcpy(obj->shadeTable, param_1, 0x800 * 4);
    }
}

// Sibling of 0x4bab00: copies an 8 KB table into the buffer at +0xc8 when
// bit 7 of the flags word at +0xf0 is set.
// FUNCTION: 0x4bab30
void __stdcall SetLightTable(unsigned int* param_1)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_c8) {
        memcpy(obj->lightTable, param_1, 0x800 * 4);
    }
}

// Sibling of 0x4bab00: copies a 256-byte table into the buffer at +0xcc
// when bit 8 of the flags word at +0xf0 is set.
// FUNCTION: 0x4bab60
void __stdcall SetGrayTable(unsigned int* param_1)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_cc) {
        memcpy(obj->grayTable, param_1, 0x40 * 4);
    }
}

// FUNCTION: 0x4bab90
void __stdcall NopRetC(int, int, int)
{
}

// Sibling of 0x4bab60: copies a 256-byte table into the buffer at +0xd0
// when bit 9 of the flags word at +0xf0 is set.
// FUNCTION: 0x4baba0
void __stdcall SetBlueTable(unsigned int* param_1)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_d0) {
        memcpy(obj->blueTable, param_1, 0x40 * 4);
    }
}

// FUNCTION: 0x4babd0
unsigned char* __stdcall BuildLightTable(PALETTEENTRY* palette)
{
    DisplayContext* obj = GetDisplay();
    if (obj->has_c8) {
        int sums[256];
        unsigned char order[256];
        SortByBrightness((unsigned char*)palette, sums, order);
        for (int row = 0; row < 32; row++) {
            double factor = 1.0 - row * -0.03333333333333333;
            for (int i = 0; i < 256; i++) {
                int red = (int)(palette[i].peRed * factor);
                int green = (int)(palette[i].peGreen * factor);
                int blue = (int)(palette[i].peBlue * factor);
                PALETTEENTRY color;
                if (red > 255) {
                    red = 255;
                }
                if (green > 255) {
                    green = 255;
                }
                if (blue > 255) {
                    blue = 255;
                }
                color.peRed = red;
                color.peGreen = green;
                color.peBlue = blue;
                ((unsigned char*)obj->lightTable)[i + row * 256] =
                    NearestColorInBand(palette, sums, order, color);
            }
        }
        return (unsigned char*)obj->lightTable;
    }
    return 0;
}

// FUNCTION: 0x4bad30
unsigned char* __stdcall BuildGrayTable(PALETTEENTRY* palette)
{
    DisplayContext* app = GetDisplay();
    if (app->has_cc) {
        PALETTEENTRY color;
        unsigned char order[256];
        int sums[256];
        SortByBrightness((unsigned char*)palette, sums, order);
        for (int i = 0; i < 0x100; i++) {
            unsigned char gray = (unsigned char)((unsigned int)(palette[i].peRed +
                palette[i].peGreen + palette[i].peBlue) / 3);
            color.peGreen = gray;
            color.peBlue = gray;
            color.peRed = gray;
            ((unsigned char*)app->grayTable)[i] = NearestColorInBand(palette, sums, order, color);
        }
        return (unsigned char*)app->grayTable;
    }
    return 0;
}

// Builds the 32 row (x 256 entries) shaded palette table into app->shadeTable.
// The scaling factor starts at 0.0 and grows by 0.06875 (g_shadeStep is
// -0.06875) once per row, so the first row is all black; kept as it is in the
// original.
// FUNCTION: 0x4badf0
unsigned char* __stdcall BuildShadeTable(PALETTEENTRY* palette)
{
    DisplayContext* app = GetDisplay();
    if (app->has_c4) {
        PALETTEENTRY color;
        unsigned char order[256];
        int sums[256];
        SortByBrightness((unsigned char*)palette, sums, order);
        double factor = 0.0;
        int offset = 0;
        do {
            for (int i = 0; i < 256; i++) {
                unsigned short v;
                v = (unsigned short)(palette[i].peRed * factor);
                color.peRed = v;
                if (v > 0xff)
                    color.peRed = 0xff;
                v = (unsigned short)(palette[i].peGreen * factor);
                color.peGreen = v;
                if (v > 0xff)
                    color.peGreen = 0xff;
                v = (unsigned short)(palette[i].peBlue * factor);
                color.peBlue = v;
                if (v > 0xff)
                    color.peBlue = 0xff;
                ((unsigned char*)app->shadeTable)[offset + i] = NearestColorInBand(palette, sums, order, color);
            }
            factor -= g_shadeStep;
            offset += 0x100;
        } while (offset < 0x2000);
        return (unsigned char*)app->shadeTable;
    }
    return 0;
}

// FUNCTION: 0x4baf30
unsigned char* __stdcall BuildBlueTable(PALETTEENTRY* palette)
{
    DisplayContext* app = GetDisplay();
    if (app->has_d0) {
        PALETTEENTRY color;
        unsigned char order[256];
        int sums[256];
        SortByBrightness((unsigned char*)palette, sums, order);
        for (int i = 0; i < 0x100; i++) {
            color.peRed = palette[i].peRed >> 1;
            color.peGreen = palette[i].peGreen >> 1;
            int b = palette[i].peBlue >> 1;
            if (b + 0x3c > 0xff)
                color.peBlue = 0xff;
            else
                color.peBlue = b + 0x32;
            ((unsigned char*)app->blueTable)[i] = NearestColorInBand(palette, sums, order, color);
        }
        return (unsigned char*)app->blueTable;
    }
    return 0;
}

// FUNCTION: 0x4baff0
char* __stdcall ChangeExtension(char* a, char* b, char* c)
{
    strcpy(b, a);
    char* p = b + strlen(b) - 1;
    while (p >= b) {
        if (*p == '\\')
            break;
        if (*p == '.') {
            *p = '\0';
            break;
        }
        p--;
    }
    strcat(b, ".");
    strcat(b, c);
    return b;
}

// Returns 1 if the string contains a '.', else 0.
// FUNCTION: 0x4bb0a0
int __stdcall HasExtension(char* name)
{
    for (unsigned int i = 0; i < strlen(name); i++) {
        if (name[i] == '.')
            return 1;
    }
    return 0;
}

// Cuts a file name at its last '.' ("a\\b.txt" -> "a\\b").
// FUNCTION: 0x4bb0f0
char* __stdcall StripExtension(char* name)
{
    for (int i = strlen(name); i >= 0; i--) {
        if (name[i] == '.') {
            name[i] = '\0';
            break;
        }
    }
    return name;
}

// Cuts a path after its last backslash ("a\\b\\c.txt" -> "a\\b\\").
// The scan starts at strlen, on the terminator, not at strlen - 1: starting
// one lower adds a `dec ecx` that MSVC cannot fold into the strlen sequence.
// FUNCTION: 0x4bb120
char* __stdcall StripFileName(char* path)
{
    for (int i = strlen(path); i >= 0; i--) {
        if (path[i] == '\\') {
            path[i + 1] = '\0';
            break;
        }
    }
    return path;
}

// Strips the directory from a path in place ("a\\b\\c.txt" -> "c.txt");
// compare 0x4bb120, which keeps the directory instead.
// FUNCTION: 0x4bb150
char* __stdcall StripPath(char* path)
{
    int len = strlen(path);
    int i = len - 1;
    while (i >= 0 && path[i] != '\\')
        i--;
    // Two indices into the same array, not pointers: keeps the offset addressing.
    int j = i + 1;
    int k = 0;
    do {
        path[k] = path[j];
        k++;
    } while (path[j++] != 0);
    return path;
}
