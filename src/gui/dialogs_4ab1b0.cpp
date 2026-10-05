// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Appends a type-5 GUI entry to the holder's 0x15b-byte entry table and fills
// it in: type 5, the unbounded name at +2, x/y at +0x13/+0x15, the width at
// +0x17 (defaults to entry 0's width minus x minus 5), flags at +0x1b, the
// text via strncpy(0x7f) at +0xb6, and assorted small fields.
//
// Suspected original bug: the entry name is only 0x10 bytes (+0x02..+0x11)
// but it is filled with an unbounded strcpy, and the caller at 0x4abe6b
// passes the literal "Player%dController" (0x5029f8, 18 characters plus the
// terminator), which spills past the name into the x field at +0x13.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004ab1b0 {                 // 0x15b bytes
    unsigned char type;                 // +0x00
    unsigned char group;                // +0x01
    char name[0x10];                    // +0x02 (filled with an unbounded strcpy)
    char unknown_12[0x13 - 0x12];
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short f19;                          // +0x19
    int flags;                          // +0x1b
    int f1f;                            // +0x1f
    int f23;                            // +0x23
    unsigned char f27;                  // +0x27
    unsigned char f28;                  // +0x28
    unsigned char f29;                  // +0x29
    unsigned char f2a;                  // +0x2a
    char unknown_2b[0xb6 - 0x2b];
    union {
        short count;                    // +0xb6 (entry 0 holds the entry count)
        char text[0x80];                // +0xb6
    } u;
    char unknown_136[0x15b - 0x136];
};
#pragma pack(pop)

struct Holder_004ab1b0 {
    int unknown_0;
    Entry_004ab1b0* entries;            // +0x04
};

// FUNCTION: 0x4ab1b0
void __stdcall AddTextGadget(Holder_004ab1b0* obj, char* name, char* text,
                            int x, short y, int w, int flags)
{
    Entry_004ab1b0* entries = obj->entries;
    short n = entries->u.count;
    int index = n + 1;
    n++;
    entries->u.count = n;
    Entry_004ab1b0* e = &obj->entries[index];
    e->type = 5;
    e->x = x;
    e->y = y;
    if (w == -1)
        e->w = entries->w - x - 5;
    else
        e->w = w;
    e->f19 = 0xf;
    e->f1f = 0xf;
    e->flags = flags;
    e->group = 0;
    e->f23 = 0;
    e->f27 = 0;
    e->f28 = 0;
    e->f29 = 1;
    e->f2a = 0;
    strcpy(e->name, name);
    strncpy(e->u.text, text, 0x7f);
    e->u.text[0x7f] = 0;
}
