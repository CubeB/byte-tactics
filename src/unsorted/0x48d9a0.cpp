// Decompiled by space-bunny-free, finished by GPT-6. Names are provisional.
// PARTIAL 79.1%, 646 of 644 bytes. The earlier 83.6% version left found
// uninitialized on a failed scan. The original writes zero at 0x48db0e and
// one at 0x48dc17. A shared selection label restores both paths, the original
// four local slots and the separate out-of-line successful-scan block.
// The remaining address materialization differs: the original keeps the
// player base in esi (0x48da14), then reads +0x67/+0x6b at 0x48da9e.
// Ours folds 0x1b63 into those field offsets, increasing the length by two
// bytes and shifting later branch targets. All 768 header sets, references,
// bounds spellings, helper forms and cached bounds failed to improve it.
//
// Earlier stack diagnoses were incorrect. At 0x48d9d3 and 0x48d9e1 a
// string argument is still pushed: [esp+0x14] is the counter's normal +0x10
// slot and [esp+0x1c] is the player's normal +0x18 slot. The first set stays
// at +0x1c. These are allocated locals, not saved registers, and none of the
// original counter, player or set reads are uninitialized. The floating
// comparison tests x87 C3 and accepts equality with zero as the source does.

#pragma pack(push, 1)

struct Team_0048d9a0 {
    char unknown_0[0x110];
    unsigned int flags;                // +0x110
};

struct Unit8_0048d9a0 {               // 0x118 bytes, flags read as a byte
    char unknown_0[0x86];
    Team_0048d9a0* owner;              // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short type;               // +0xa6
    char unknown_a8[0xac - 0xa8];
    int field_ac;                      // +0xac
    char unknown_b0[0xfb - 0xb0];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned char flags;               // +0x110
    char unknown_111[0x118 - 0x111];
};

struct Unit32_0048d9a0 {              // 0x118 bytes, flags read as a dword
    char unknown_0[0x86];
    Team_0048d9a0* owner;              // +0x86
    char unknown_8a[0xa6 - 0x8a];
    unsigned short type;               // +0xa6
    char unknown_a8[0xac - 0xa8];
    int field_ac;                      // +0xac
    char unknown_b0[0xfb - 0xb0];
    int field_fb;                      // +0xfb
    char unknown_ff[0x104 - 0xff];
    float field_104;                   // +0x104
    char unknown_108[0x110 - 0x108];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Player_0048d9a0 {              // 0x14b bytes
    char unknown_0[0x67];
    Unit8_0048d9a0* unitsBegin;        // +0x67
    Unit8_0048d9a0* unitsEnd;          // +0x6b
    char unknown_6f[0x14b - 0x6f];
};

struct Game_0048d9a0 {
    char unknown_0[0x1b63];
    Player_0048d9a0 players[10];       // +0x1b63
    char unknown_2851[0x2a42 - 0x2851];
    unsigned char localPlayer;         // +0x2a42
    char unknown_2a43[0x37e9c - 0x2a43];
    unsigned short field_37e9c;        // +0x37e9c
    char unknown_37e9e[0x37ebe - 0x37e9e];
    unsigned short flags_0 : 4;        // +0x37ebe
    unsigned short orderFlag : 1;      // +0x37ebe, bit 4
    unsigned short flags_5 : 11;
};
#pragma pack(pop)

extern Game_0048d9a0* g_game;

void FUN_00495860(void);

class Class_00488d30 {
public:
    int bits[16];
};

Class_00488d30* __stdcall FUN_00488c50(char* name);

static inline int TestBit(Class_00488d30* set, unsigned short n)
{
    return set->bits[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x48d9a0
bool __stdcall FUN_0048d9a0(int id, int param_2)
{
    Class_00488d30* setA = FUN_00488c50("CTRL_F");
    int cnt = 0;
    Player_0048d9a0* player = &g_game->players[g_game->localPlayer];
    Class_00488d30* setB = FUN_00488c50("CTRL_F");

    Unit8_0048d9a0* u;
    Unit32_0048d9a0* v;
    Unit32_0048d9a0* w;
    int found;
    for (u = g_game->players[g_game->localPlayer].unitsBegin; u <= g_game->players[g_game->localPlayer].unitsEnd; u++) {
        if ((u->flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->owner == 0 || (u->owner->flags & 0x40000000))
            && u->field_ac == id && TestBit(setB, u->type)) {
            {
                Player_0048d9a0* q = &g_game->players[g_game->localPlayer];
                for (v = (Unit32_0048d9a0*)q->unitsBegin;
                     v <= (Unit32_0048d9a0*)q->unitsEnd; v++) {
                    if ((v->flags & 0x20) && v->field_104 == 0.0f && v->field_fb == 0
                        && (v->owner == 0 || (v->owner->flags & 0x40000000))
                        && v->field_ac == id && (v->flags & 0x80000000)) {
                        goto found_match;
                    }
                }
            }
            break;
        }
    }

    found = 0;
    goto select_units;
found_match:
    found = 1;
select_units:
    for (w = (Unit32_0048d9a0*)player->unitsBegin;
         w <= (Unit32_0048d9a0*)player->unitsEnd; w++) {
        if ((w->flags & 0x20) && w->field_104 == 0.0f && w->field_fb == 0
            && (w->owner == 0 || (w->owner->flags & 0x40000000))) {
            if (w->field_ac == id) {
                if (found) {
                    if (TestBit(setA, w->type)) {
                        w->flags &= ~0x10;
                    } else {
                        w->flags |= 0x10;
                        cnt++;
                    }
                } else {
                    w->flags |= 0x10;
                    cnt++;
                }
            } else if (param_2 == 0) {
                w->flags &= ~0x10;
            }
        }
    }
    FUN_00495860();
    g_game->field_37e9c = 0;
    g_game->orderFlag = 1;
    return cnt > 0;
}
