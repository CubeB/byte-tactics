// Decompiled by space-bunny-free. Names are provisional.
// Fills a surface with one colour byte. With no surface it uses the one the
// screen object was given by SetOffscreenSurface (+0xbc, when +0xdc is set), else its
// own framebuffer, unless bit 1 of the flags byte at +0xf0 sends the work to
// the driver's function table, which then takes the object as its first
// argument. Returns 0 only when that call reports a failure.
#include <string.h>

struct Surface {                        // the same class as in 0x4c6a60.cpp
    int width;                          // +0x0
    int height;                         // +0x4
    int field_8;                        // +0x8
    char* pixels;                       // +0xc
    char unknown_10[0x2c - 0x10];
    unsigned int flag0 : 1;             // +0x2c bit 0
    unsigned int flag1 : 1;             // +0x2c bit 1
};

// What the driver is handed: 0x64 bytes, of which two fields are set.
struct Fade_004c6890 {
    int amount;                         // +0x0
    char unknown_4[0x50 - 0x4];
    int colour;                         // +0x50
    char unknown_54[0x64 - 0x54];       // never touched
};

// The driver's table of __stdcall function pointers, each of which takes the
// driver object itself as its first argument.
struct Table_004c6890 {
    void* slot0;                        // +0x00
    void* slot1;                        // +0x04
    void* slot2;                        // +0x08
    void* slot3;                        // +0x0c
    void* slot4;                        // +0x10
    int (__stdcall* fade)(void* self, int a, int b, int c, int colour, Fade_004c6890* fade);  // +0x14
};

struct Driver_004c6890 {
    Table_004c6890* table;              // +0x0
};

struct Class_004c6890 {
    char unknown_0[0x54];
    int field_54;                       // +0x54
    int field_58;                       // +0x58
    char* pixels;                       // +0x5c
    char unknown_60[0x8c - 0x60];
    Driver_004c6890* driver;            // +0x8c
    char unknown_90[0xbc - 0x90];
    Surface* surface;                   // +0xbc
    char unknown_c0[0xdc - 0xc0];
    int field_dc;                       // +0xdc
    char unknown_e0[0xf0 - 0xe0];
    unsigned char flags;                // +0xf0
};

// The three fills are written through these little helpers because the order in
// which the count and the fill value are evaluated decides which of the two
// count operands the inlined imul takes, and the original has a different
// order in each of the three branches.
static void set_mem(char* p, int count, int colour)
{
    memset(p, colour, count);
}

static void clear_surface(Surface* s, int colour)
{
    memset(s->pixels, colour, s->height * s->field_8);
}

static void clear_screen(Class_004c6890* o, int colour)
{
    memset(o->pixels, colour, o->field_54 * o->field_58);
}

extern Class_004c6890* GetDisplay(void);

// FUNCTION: 0x4c6890
int __stdcall FillSurface(Surface* surface, int colour)
{
    Class_004c6890* obj = GetDisplay();
    int ret = 1;
    if (surface == 0) {
        if (obj->field_dc != 0) {
            Surface* s = obj->surface;
            set_mem(s->pixels, s->height * s->field_8, colour);
        } else if (!(obj->flags & 2)) {
            clear_screen(obj, colour);
        } else {
            Fade_004c6890 fade;
            fade.amount = 100;
            fade.colour = colour;
            if (obj->driver->table->fade(obj->driver, 0, 0, 0, 0x1000400, &fade) != 0)
                ret = 0;
            return ret;
        }
    } else if (surface->flag0) {
        clear_surface(surface, colour);
    } else {
        Fade_004c6890 fade;
        fade.amount = 100;
        fade.colour = colour;
        if (obj->driver->table->fade(obj->driver, 0, 0, 0, 0x1000400, &fade) != 0)
            ret = 0;
    }
    return ret;
}
