// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: check.py says 67.6%. What is solved: the team index multiply
// (element size 0x14b), the float and field_fb tests, the 0xffffff2f clearing
// loop (an inlined 0x48bd00), "shr ecx,4; test cl,1" for the 0x10 bit (a
// 1-bit bitfield at bit 4 of the flags dword), and, most importantly, the
// owner test: BOTH scans test `owner->flags & 0x40000000`, and MSVC narrows
// the first one to a byte test (`test byte ptr [eax+0x113], 0x40`) on its own.
// With that the first-match pointer stays in ebx across the call, the 0x10
// mask lands in edx, and the clearing loop uses edi as its temporary, exactly
// as the original.
// What still differs:
//  1. The team pointer: the original materialises ebp as base+0x1b63
//     ("lea ebp,[edx+eax*2+0x1b63]") and reads the second scan's end at
//     [ebp+0x6b]; MSVC here materialises the bare base ("lea ebp,[edx+eax*2]")
//     and reads [ebp+0x1bce]. The two spellings are the same address, so this
//     is a CSE tie-break: no declaration order, reference, char* cast or
//     inline helper tried (about 25 forms) flips it.
//  2. The two exits are emitted in the other physical order: the original puts
//     the second scan's in-loop `return` block (esi path) before the
//     `found->flags |= 0x10` block (ebx path); here the ebx path comes first
//     and falls into the shared `g_game->flags |= 0x10`. The pop scheduling in
//     the esi path differs with it.
#pragma pack(push, 1)

// The object at +0x86 of a unit. Bit 30 of the flags dword at +0x110 is the
// same bit the first scan tests as byte [owner+0x113] & 0x40.
struct Owner_0048d790 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
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
                if (owner == 0 || (owner->flags & 0x40000000)) {
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
                if (owner == 0 || (owner->flags & 0x40000000)) {
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
