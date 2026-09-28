// Decompiled by space-bunny-free. Names are provisional.
// PARTIAL, 82.6% (646 of 644 bytes; up from 60.4%). Everything below the
// description is settled: the signature was never wrong (`bool __stdcall
// FUN_0048d9a0(int, int)`, and the 60.4% was entirely allocation), and the three
// changes that fixed the frame were:
//   1. the `player` local declared BEFORE the second FUN_00488c50 call and used
//      only in the third loop, while loops 1 and 2 write the address out as
//      `g_game->players[g_game->localPlayer]`. That reproduces the original's
//      frame exactly: the player stored to slot +0x1c before the second call and
//      reloaded from +0x18 in the third loop. 60.4% to 71.1%.
//   2. loop 2 given its OWN block-local copy of the address, `Player* q =
//      &g_game->players[g_game->localPlayer];` inside the match block. That
//      frees esi, so setB moves to ebp and the player address is materialised in
//      esi before the loop guard. 71.1% to 82.2%.
//   3. loop 2's body turned into a `static inline int AnyFlag32(...)` helper
//      with one `return 1` per path, which puts the `found = 1` store out of
//      line after the epilogue and moves the second `found = 0` store into the
//      third loop's preheader, matching 0x48dc13 and 0x48db0e. 82.2% to 82.6%.
//
// What is left, two gaps:
//   - the materialised player address. The original does `lea esi,
//     [ecx + eax*2 + 0x1b63]` and then loads `[esi+0x67]`/`[esi+0x6b]`; every
//     variant here folds the constant and does `lea esi, [ecx + eax*2]` with
//     `[esi+0x1bca]`/`[esi+0x1bce]`. Tried: char* casts, `&players[0] + i`, array
//     decay, const pointers, inlined getters, and the function-level `player`
//     local. MSVC 5 only keeps the 0x1b63 in the register when the value is a
//     REMATERIALISED constant expression rather than a variable, so that is the
//     axis to attack next.
//   - one redundant `mov [esp+0x14], ebx` right after the second call, the
//     duplicated `found = 0` init, which the original does not have; and
//     correspondingly the original's out-of-line `found = 1` block repeats
//     `mov ebx, [esp+0x10]` where this file shares one reload in the merge block.
//
// On the original: the `mov ebx, [esp+0x10]` idiom reloads `cnt` from the saved
// `edi` slot, and the third loop's player pointer is read from `[esp+0x18]`,
// which is written only on loop 3's preheader path. The pre-call-2 store of the
// player address lands on setA's slot +0x1c, so the third loop's bit test can
// read the wrong pointer. Both look like MSVC 5 frame-allocation artefacts rather
// than Cavedog mistakes, so no game bug is claimed. The float-comparison
// inversion noted below is likewise an MSVC 5 idiom, not a Cavedog bug, and the
// same inversion appears in the matched 0x48be00 and 0x48c9b0.
// Three scans over the local player's unit list. The first looks for a unit
// that passes the common test and whose type index (+0xa6) is in the second
// CTRL_F set; the second re-scans the same list for one of those that also
// carries bit 31 of its flags word; the third tags or clears bit 4 of the
// flags word of every unit that matches the given id, counts the tagged ones,
// issues the STOP order, drops the selection and sets order flag 0x10 at
// +0x37ebe, and returns whether anything was tagged. The unit flags at +0x110
// are read as a byte in the first scan and as a dword in the other two, so the
// two unit struct views are separate types (see 0x48dc30, which is the same
// first scan in miniature and matches).
//
// Not matching yet (60.4%). All three loop bodies and both CTRL_F bit tests
// match instruction for instruction. What differs is one allocation decision,
// made before the first loop: the original keeps the player pointer in its
// stack home, recomputes g_game + 331*localPlayer + 0x1b63 after the second
// CTRL_F call and materialises the pointer into esi only where it needs it
// (0x48d9e1, 0x48da14), while this version promotes the same pointer to ebp
// across the call (0x48d9da). That promotion is also what forces the extra
// frame slot at +0x20 and the duplicate store of the counter's zero. Declaring
// the locals in the original's apparent frame order (cnt, found, player, setA)
// player address computation interleaving is load bearing.
// Suspected original bug: MSVC 5 inverts the sense of the float comparison
// against zero, so `== 0.0f` here tags units whose +0x104 value is NOT 0.0f
// (same inversion as 0x48be00 and 0x48c9b0, which match with the same code).

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


static inline int AnyFlag32(Player_0048d9a0* p, int id)
{
    Unit32_0048d9a0* v;
    for (v = (Unit32_0048d9a0*)p->unitsBegin;
         v <= (Unit32_0048d9a0*)p->unitsEnd; v++) {
        if ((v->flags & 0x20) && v->field_104 == 0.0f && v->field_fb == 0
            && (v->owner == 0 || (v->owner->flags & 0x40000000))
            && v->field_ac == id && (v->flags & 0x80000000))
            return 1;
    }
    return 0;
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
    int found = 0;
    
    for (u = g_game->players[g_game->localPlayer].unitsBegin; u <= g_game->players[g_game->localPlayer].unitsEnd; u++) {
        if ((u->flags & 0x20) && u->field_104 == 0.0f && u->field_fb == 0
            && (u->owner == 0 || (u->owner->flags & 0x40000000))
            && u->field_ac == id && TestBit(setB, u->type)) {
                        Player_0048d9a0* q = &g_game->players[g_game->localPlayer];

            found = AnyFlag32(q, id);
            break;
        }
    }
    
    
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
