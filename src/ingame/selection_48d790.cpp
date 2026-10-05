// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, reworked by Claude Sonnet 5.5, verified by GPT-6.1-sol, finished by mimo-v2.6-pro, finished by claude-opus-5-5. Names are provisional.
// The match path in the second loop jumps to the shared `g_game->flags |= flag`
// tail with a goto; MSVC then duplicates that tail into the match block (reloading
// g_game after each store through a pointer), which is the original's layout.
#pragma pack(push, 1)

// The object at +0x86 of a unit. Bit 30 of the flags dword at +0x110 is the
// same bit the first scan tests as byte [owner+0x113] & 0x40.
struct Owner_0048d790 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Unit {
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
    Unit* begin;                       // +0x67
    Unit* end;                         // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game {
    char unknown_0[0x1b63];
    Team_0048d790 teams[10];           // +0x1b63, 0x14b each
    char unknown_2a43[0x2a43 - (0x1b63 + 10 * 0x14b)];
    unsigned char field_2a43;
    char unknown_2a44[0x14357 - 0x2a44];
    Unit* list_begin;                  // +0x14357
    Unit* list_end;                    // +0x1435b
    char unknown_1435f[0x37e9c - 0x1435f];
    short field_37e9c;                 // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags;              // +0x37ebe
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00491d70(int force);

// FUNCTION: 0x48d790
void __stdcall FUN_0048d790(void)
{
    Unit* found = 0;
    Team_0048d790* t = &g_game->teams[g_game->field_2a43];
    Unit* u = t->begin;
    Unit* last = t->end;

    for (; u <= last; u++) {
        if (u->u.flags & 0x20) {
            if (u->field_104 == 0.0f && u->field_fb == 0) {
                Owner_0048d790* owner = u->owner;
                if (owner == 0 || (owner->flags & 0x40000000)) {
                    if (found == 0) {
                        found = u;
                    }
                    if (u->u.bits.bit4) {
                        for (Unit* q = g_game->list_begin;
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
        u++;
        if (u <= t->end) {
            do {
                if ((u->u.flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0) {
                    Owner_0048d790* owner = u->owner;
                    if (owner == 0 || (owner->flags & 0x40000000)) {
                        u->u.flags |= flag;
                        g_game->field_37e9c = 0;
                        goto done;
                    }
                }
                u++;
            } while (u <= t->end);
        }
        found->u.flags |= flag;
    }
done:
    g_game->flags |= flag;
}