// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL: check.py says 57.1% (many of the lost lines are only shifted jump
// targets). What is solved: the team index multiply (element size is 0x14b, not
// 0x14a: that is what gives "add edx,ecx" plus the eax*2 scale), the float
// and field_fb tests, the 0xffffff2f clearing loop (an inlined 0x48bd00), the
// second scan with the dword owner test, and "shr ecx,4; test cl,1" for the
// 0x10 bit (a 1-bit bitfield at +0x114 of the flags dword, not a mask: a mask
// gives "test cl,0x10").
// What still differs, all of it register allocation:
//  1. The "first match" pointer is spilled (extra "push ecx", [esp+0x10] load
//     and store) instead of living in ebx, so the function has 5 prologue
//     pushes. Removing the `t` local makes MSVC put the pointer in ebp and drop
//     the stack slot, but then the "lea ebp,[edx+eax*2+0x1b63]" and the
//     [ebp+0x6b] end load of the second loop disappear, so it is not a win.
//  2. Because the pointer is spilled, MSVC hoists the 0x40 mask out of the
//     first loop into bl ("mov bl,0x40") and uses 0x40000000 out of the second
//     loop into edx; the original keeps both as immediates because all four
//     callee-saved registers are taken (ebx first match, ebp team, esi cursor,
//     edi g_game). Writing the same 0x40 test in the second loop as well (so
//     the constant has two uses) stops the hoist, but then the second loop gets
//     a byte test instead of the original's dword test.
//  3. My "lea ebp,[edx+eax*2]" folds the 0x1b63 into the member access
//     ([ebp+0x1bce]); the original keeps 0x1b63 in the lea and +0x6b in the
//     access. No declaration order of the three cursor statements changes it.
//  4. In the clearing loop the original uses edi as the value temporary (so
//     g_game has to be reloaded into edi after the call), mine uses edx.
//  5. The 0x10 mask: the original materialises it in edx at the merge point of
//     the two scans ("mov edx,0x10", then "or eax,edx" and
//     "or word ptr [edi+0x37ebe],dx"); mine rematerialises the immediate, which
//     also narrows the game flag store to a byte. This follows from (1): with
//     ebx free, MSVC gives it to the 0x10 mask instead.
#pragma pack(push, 1)

// The object at +0x86 of a unit. Both loops test the same bit (bit 30 of the
// dword at +0x110), but the first one reads it as a byte, so the field is
// spelled both ways here.
struct Owner_0048d790 {
    char unknown_0[0x110];
    union {
        unsigned int flags;                            // +0x110
        struct {
            unsigned char unknown_111[3];
            unsigned char bit30;                       // +0x113
        } bytes;
    } u;
};

struct Unit_0048d790 {
    char unknown_0[0x86];
    Owner_0048d790* owner;             // +0x86
    char unknown_8a[0xfb - 0x8a];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    union {
        unsigned int flags;                            // +0x110
        struct {
            unsigned int low : 4;
            unsigned int bit4 : 1;                     // the 0x10 bit
            unsigned int high : 27;
        } bits;
    } u;
    char unknown_114[0x118 - 0x114];
};

struct Team_0048d790 {
    char unknown_0[0x67];
    Unit_0048d790* begin;              // +0x67
    Unit_0048d790* end;                // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048d790 {
    char unknown_0[0x1b63];
    Team_0048d790 teams[10];           // +0x1b63, 0x14b each
    char unknown_2a43[0x2a43 - (0x1b63 + 10 * 0x14b)];
    unsigned char field_2a43;
    char unknown_2a44[0x14357 - 0x2a44];
    Unit_0048d790* list_begin;         // +0x14357
    Unit_0048d790* list_end;           // +0x1435b
    char unknown_1435f[0x37e9c - 0x1435f];
    short field_37e9c;                 // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags;              // +0x37ebe
};
#pragma pack(pop)

extern Game_0048d790* g_game;

int __stdcall FUN_00491d70(int force);

// FUNCTION: 0x48d790
void __stdcall FUN_0048d790(void)
{
    Unit_0048d790* found = 0;
    Unit_0048d790* u = g_game->teams[g_game->field_2a43].begin;
    Team_0048d790* t = &g_game->teams[g_game->field_2a43];
    Unit_0048d790* last = g_game->teams[g_game->field_2a43].end;

    for (; u <= last; u++) {
        if (u->u.flags & 0x20) {
            if (u->field_104 == 0.0f && u->field_fb == 0) {
                Owner_0048d790* owner = u->owner;
                if (owner == 0 || (owner->u.bytes.bit30 & 0x40)) {
                    if (found == 0) {
                        found = u;
                    }
                    if (u->u.bits.bit4) {
                        for (Unit_0048d790* q = g_game->list_begin;
                             q <= g_game->list_end; q++) {
                            q->u.flags &= 0xffffff2f;
                        }
                        FUN_00491d70(0);
                        break;
                    }
                }
            }
        }
    }

    unsigned short flag = 0x10;
    if (found != 0) {
        for (u++; u <= t->end; u++) {
            if ((u->u.flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0) {
                Owner_0048d790* owner = u->owner;
                if (owner == 0 || (owner->u.flags & 0x40000000)) {
                    u->u.flags |= flag;
                    g_game->field_37e9c = 0;
                    g_game->flags |= flag;
                    return;
                }
            }
        }
        found->u.flags |= flag;
    }
    g_game->flags |= flag;
}
