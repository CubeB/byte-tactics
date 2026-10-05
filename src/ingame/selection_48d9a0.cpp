// Decompiled by space-bunny-free, finished by GPT-6, finished by GPT-6.1-sol, edited by deepseek-v4.1-flash, finished by mimo-v2.6-pro. Names are provisional.
// MATCH. The one thing that fixed the last diff (the +0x1b63 player-base
// materialization in esi at 0x48da14): the outer scan must read its bounds
// through its own Player pointer local while the nested scan reads through a
// SECOND, separate Player pointer local. A pointer local whose accesses all
// fold into raw addressing (the outer loads sit where g_game+i and i*165 are
// still live in ecx/eax) still makes MSVC emit the element address as a kept
// `lea esi, [ecx+eax*2+0x1b63]` (the 0x4679a0 dead-lea pattern), and the
// nested scan's folded accesses then rebase their displacements onto that
// register (+0x67/+0x6b) instead of onto the partial base. Sharing one local
// between the two scans makes MSVC merge the nested bounds loads with the
// outer ones; using no local for the outer scan gives it no kept lea and the
// nested loads fold 0x1b63 into +0x1bca (the old 79.1% shape). Compare the
// matched siblings 0x48bf30 (head through one local, end through another) and
// 0x48be00 (one local for both scans, everything through the kept lea).
//
// Notes on the rest of the match: at 0x48d9d3 and 0x48d9e1 a string argument
// is still pushed, so [esp+0x14] is the counter's normal +0x10 slot and
// [esp+0x1c] the player's normal +0x18 slot; these are allocated locals, not
// saved registers. The first set (setA) stays at +0x1c and the final loop
// tests it through that slot. The floating comparison tests x87 C3 and
// accepts equality with zero as the source does. The original writes found =
// 0 at 0x48db0e and found = 1 at 0x48dc17; a shared selection label gives
// both paths their own store and the out-of-line success block. The counter
// lives in ebx through the final loop and is spilled around the bit-test
// masks that clobber it.

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

struct Game {
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

extern Game* g_game;

void SelectStopOrder(void);

class UnitTypeSet {
public:
    int bits[16];
};

UnitTypeSet* __stdcall GetCategoryMask(char* name);

static inline int TestBit(UnitTypeSet* set, unsigned short n)
{
    return set->bits[n >> 5] & (1 << (n & 0x1f));
}

// FUNCTION: 0x48d9a0
bool __stdcall SelectSquad(int id, int param_2)
{
    UnitTypeSet* setA = GetCategoryMask("CTRL_F");
    int cnt = 0;
    Player_0048d9a0* player = &g_game->players[g_game->localPlayer];
    UnitTypeSet* setB = GetCategoryMask("CTRL_F");

    Unit8_0048d9a0* u;
    Unit32_0048d9a0* v;
    Unit32_0048d9a0* w;
    int found;
    Player_0048d9a0* q = &g_game->players[g_game->localPlayer];
    for (u = q->unitsBegin; u <= q->unitsEnd; u++) {
        if ((u->flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->owner == 0 || (u->owner->flags & 0x40000000))
            && u->field_ac == id && TestBit(setB, u->type)) {
            {
                Player_0048d9a0* r = &g_game->players[g_game->localPlayer];
                for (v = (Unit32_0048d9a0*)r->unitsBegin;
                     v <= (Unit32_0048d9a0*)r->unitsEnd; v++) {
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
    SelectStopOrder();
    g_game->field_37e9c = 0;
    g_game->orderFlag = 1;
    return cnt > 0;
}
