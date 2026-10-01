// Decompiled by deepseek-v4.1, finished by space-bunny-free, finished by GPT-6.1-sol. Names are provisional.
// PARTIAL 80.9% (1202 bytes vs the original 1199). Frame, the three cell loops,
// the inlined FUN_0047cb60 owner surgery, the (g_game+0x38a47) store, the
// 0x20000000 mask path, both FUN_00483210/FUN_00440a40 calls and the epilogue
// all match. What still differs is ONE block, the two bounds tests at the top
// (0x47cc57..0x47cca9), which is 3 bytes long and therefore shifts every
// forward jump target in the rest of the function by 3:
//   1) The original keeps pos.x in AX and pos.y in CX for the two negative
//      tests, then accumulates each sum in the register that held the
//      POSITION: mov ebp,g_game / movsx ebx,dx / movsx edx,ax /
//      mov eax,[width] / add edx,ebx / cmp edx,eax / mov [esp+0x2c],ebx /
//      jge, then movsx edi,[esp+0x16] / movsx eax,cx / mov ecx,[height] /
//      add eax,edi / cmp eax,ecx / jge.  We get the same movsx order but
//      MSVC builds the sum through a copy of size.x (mov eax,edi / add
//      eax,ebx for y, then mov ecx,ebx / add ecx,edx for x), loads width
//      into EDX instead of EAX, spills size.x before the cmp instead of
//      after it, and hoists the y sum above the x test.  Separate `if`s
//      give the right AX/CX but move the `remove` block next to the top
//      block, which turns both `jl remove` into 2-byte short jumps and
//      costs more than it wins.
//   2) The owner index does lea edx,[esi+0x6a]; mov ecx,[esi+0x6a] and
//      computes (p.z>>23)*cols with p.z in EDX; the original does
//      lea ecx,[esi+0x6a]; mov edx,ecx, indexes everything through edx and
//      keeps p.z in EAX so the multiply is `imul eax,[ebp+0x142a3]`.
// Second session (space-bunny-free, timeboxed): no new score, still 80.9%. Third session (GPT-6.1-sol, timeboxed): owner-index left-accumulator rewrites in both operand orders and independently split negative/bounds guards produced no gain; split guards scored 72.8% and were reverted. Best remains 80.9%.
// New analysis: in every commutative op the original accumulates the LEFT
// operand (add edx,ebx with edx=pos.x, imul eax,[cols] with eax=z>>23) while
// ours accumulates the RIGHT one (mov ecx,ebx / add ecx,edx, mov eax,[cols] /
// imul eax,edx), so the fix is likely one source shape that flips that
// choice, fixing both diff regions at once. Unscored scratch variants v1..v8
// under build/scratch/0x47cc30/ try separate vs combined ifs, both operand
// orders and an int sx = pos.x; sx += size.x accumulator form.
// Tried and all WORSE or equal: `obj->pos.x + size.x` in both operand
// orders (MSVC 5 canonicalises them identically), `g_game->width <= ...`,
// named sx/sy locals at function scope and inside a block, `sx = pos.x;
// sx += size.x`, a pointer to the position struct instead of a copy
// (drops the dead store of p.y and costs 13 points), the reversed
// `(p.z>>23)*cols + (p.x>>23)`, and every combination of combined `||`
// versus separate ifs for the negative and the sum tests.
#pragma pack(push, 1)

struct Obj_0047cc30;

struct Owner_0047cc30 {
    char unknown_0[6];
    Obj_0047cc30* first;                // +0x6
};

struct Unit_0047cc30 {
    char unknown_0[0x14e];
    unsigned char* mask;                // +0x14e
};

struct Player_0047cc30 {
    int active;                         // +0x0
    char unknown_4[0x73 - 0x4];
    unsigned char type;                 // +0x73
};

struct UnitRec_0047cc30 {               // 0x118 bytes
    char unknown_0[0x92];
    void* def;                          // +0x92
    Player_0047cc30* owner;             // +0x96
    char unknown_9a[0x110 - 0x9a];
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Cell_0047cc30 {
    unsigned short field_0;             // +0x0
    unsigned short field_2;             // +0x2
    char unknown_4[0xc - 0x4];
    unsigned char field_c;              // +0xc
};

struct Point_0047cc30 {
    short x;
    short y;
};

struct Position_0047cc30 {
    int x;
    int y;
    int z;
};

union Flags_0047cc30 {
    struct {
        unsigned int unknown_0 : 27;
        unsigned int flag27 : 1;
        unsigned int unknown_1 : 4;
    } bits;
    int all;
};

struct Obj_0047cc30 {
    unsigned char* field_0;             // +0x0
    char unknown_4[0x26 - 0x4];
    int field_26;                       // +0x26
    char unknown_2a[0x6a - 0x2a];
    Position_0047cc30 position;         // +0x6a
    Point_0047cc30 pos;                 // +0x76
    char unknown_7a[4];
    Point_0047cc30 size;                // +0x7e
    Owner_0047cc30* owner;              // +0x82
    int field_86;                       // +0x86
    char unknown_8a[4];
    Obj_0047cc30* next;                 // +0x8e
    Unit_0047cc30* unit;                // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short field_a8;            // +0xa8
    char unknown_aa[0x10f - 0xaa];
    unsigned char bit0 : 1;             // +0x10f
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    Flags_0047cc30 flags;               // +0x110
};

struct Game_0047cc30 {
    char unknown_0[0x14233];
    int width;                          // +0x14233
    int height;                         // +0x14237
    char unknown_1423b[0x14287 - 0x1423b];
    Cell_0047cc30* cells;               // +0x14287
    char unknown_1428b[0x1429f - 0x1428b];
    Owner_0047cc30* owners;             // +0x1429f
    int ownerCols;                      // +0x142a3
    char unknown_142a7[0x142b7 - 0x142a7];
    Owner_0047cc30* defaultOwner;       // +0x142b7
    char unknown_142bb[0x14357 - 0x142bb];
    UnitRec_0047cc30* units;            // +0x14357
    char unknown_1435b[0x38a47 - 0x1435b];
    int field_38a47;                    // +0x38a47
};
#pragma pack(pop)

extern Game_0047cc30* g_game;

void __stdcall FUN_00483210(Point_0047cc30 pos, Point_0047cc30 size);
void __stdcall FUN_00440a40(Point_0047cc30 pos, Point_0047cc30 size);

static void SetOwner_0047cc30(Obj_0047cc30* obj, Owner_0047cc30* nw)
{
    if (nw != obj->owner) {
        if (obj->field_86 == 0) {
            Owner_0047cc30* old = obj->owner;
            if (old != 0) {
                Obj_0047cc30** pp = &old->first;
                while (*pp != obj)
                    pp = &(*pp)->next;
                *pp = obj->next;
                obj->next = 0;
            }
            obj->next = nw->first;
            nw->first = obj;
        }
        obj->owner = nw;
    }
}

// FUNCTION: 0x47cc30
void __stdcall FUN_0047cc30(Obj_0047cc30* obj)
{
    Point_0047cc30 size = obj->size;
    if (obj->field_0 != 0)
        *(int*)(obj->field_0 + 0x26) = g_game->field_38a47;
    int sx, sy;
    if (obj->pos.x < 0 || obj->pos.y < 0)
        goto remove;
    sx = obj->pos.x + size.x;
    sy = obj->pos.y + size.y;
    if (sx >= g_game->width || sy >= g_game->height)
        goto remove;

    {
        Position_0047cc30 p = obj->position;
        SetOwner_0047cc30(obj,
            &g_game->owners[(p.x >> 23) + (p.z >> 23) * g_game->ownerCols]);
    }
    {
        Cell_0047cc30* cell = &g_game->cells[g_game->width * obj->pos.y + obj->pos.x];
        unsigned int f = obj->flags.all;
        int index = 0;

        if (f & 0x20000000) {
            for (int y = size.y; y > 0; y--) {
                for (int x = size.x; x > 0; x--) {
                    unsigned char m = obj->unit->mask[index++];
                    if (m & (obj->bit2 ? 2 : 4)) {
                        unsigned short id = cell->field_0;
                        if (id != 0) {
                            UnitRec_0047cc30* rec = &g_game->units[id];
if (rec->owner->active == 0) {
                                goto a_bad;
                            } else if (rec->owner->type != 3) {
                                goto a_bad;
                            }
                            rec->flags |= 0x8000000;
                            obj->flags.all |= 0x4000000;
                            goto a_write;
                            a_bad:
                            rec->flags |= 0x4000000;
                            obj->flags.all |= 0x8000000;
                            goto a_next;
                            a_write: ;

                        }
                        cell->field_0 = obj->field_a8;
                    }
                a_next:
                    if (m & 1)
                        cell->field_c |= 2;
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            Point_0047cc30 grown;
            grown.x = size.x + 2;
            grown.y = size.y + 2;
            Point_0047cc30 pad;
            pad.x = obj->pos.x - 1;
            pad.y = obj->pos.y - 1;
            FUN_00483210(pad, grown);
            FUN_00440a40(obj->pos, obj->size);
            return;
        }
        if ((f & 3) == 1) {
            for (int y = size.y; y > 0; y--) {
                for (int x = size.x; x > 0; x--) {
                    unsigned short id = cell->field_0;
                    if (id != 0) {
                        UnitRec_0047cc30* rec = &g_game->units[id];
if (rec->owner->active == 0) {
                            goto b_bad;
                        } else if (rec->owner->type != 3) {
                            goto b_bad;
                        }
                        rec->flags |= 0x8000000;
                        obj->flags.all |= 0x4000000;
                        goto b_write;
                        b_bad:
                        rec->flags |= 0x4000000;
                        obj->flags.all |= 0x8000000;
                        goto b_next;
                        b_write: ;

                    }
                    cell->field_0 = obj->field_a8;
                b_next:
                    cell++;
                }
                cell += g_game->width - size.x;
            }
            return;
        }
        if ((f & 3) == 2) {
            for (int y = size.y; y > 0; y--) {
                for (int x = size.x; x > 0; x--) {
                    unsigned short id = cell->field_2;
                    if (id != 0) {
                        UnitRec_0047cc30* rec = &g_game->units[id];
if (rec->owner->active == 0) {
                            goto c_bad;
                        } else if (rec->owner->type != 3) {
                            goto c_bad;
                        }
                        rec->flags |= 0x8000000;
                        obj->flags.all |= 0x4000000;
                        goto c_write;
                        c_bad:
                        rec->flags |= 0x4000000;
                        obj->flags.all |= 0x8000000;
                        goto c_next;
                        c_write: ;

                    }
                    cell->field_2 = obj->field_a8;
                c_next:
                    cell++;
                }
                cell += g_game->width - size.x;
            }
        }
    }
    return;

remove:
    SetOwner_0047cc30(obj, g_game->defaultOwner);
}
