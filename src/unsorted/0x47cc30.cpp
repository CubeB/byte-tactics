// Decompiled by deepseek-v4.1. Names are provisional.
// PARTIAL 75.1% (best kept here). Body, frame, loop registers and the inlined
// FUN_0047cb60 owner surgery all match; what still differs:
// 1) The two bounds tests at the top: the original accumulates the sum in edx
//    (movsx ebx,dx / movsx edx,ax / mov eax,[ebp+0x14233] / add edx,ebx /
//    cmp edx,eax) and spills size.x after the cmp. MSVC always builds the sum
//    in a fresh eax and loads width into edx here, 2 bytes longer. Plain
//    `pos.x + size.x >= g_game->width`, two separate ifs and named sx/sy locals
//    were all tried. Any variant must declare sx/sy before the first
//    `goto remove` (C2362 otherwise).
// 2) The three rec blocks: the original emits `cmp [ecx],0 / je BAD(next insn) /
//    cmp [ecx+0x73],3 / je GOOD / BAD...jmp NEXT / GOOD...WRITE / NEXT` (the
//    else block laid out first, single merged write). MSVC lays the then block
//    first (`je BAD / jne BAD / GOOD / jmp WRITE / BAD / WRITE / NEXT`), so
//    every forward jump in the tail sits a few bytes early and the mask
//    branch's else block is sunk to the end of the function. Plain
//    `active == 0 || type != 3`, nested ifs, and duplicating the write in the
//    else were all tried (tested standalone: same layout).
// 3) `test bl,al` is emitted as `test al,bl`; operand order could not be steered.
// 4) The owner index does `lea eax,[esi+0x6a]; mov ecx,[esi+0x6a]` while the
//    original does `lea ecx,[esi+0x6a]; mov edx,ecx` then indexes through edx.
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
                            if (rec->owner->active != 0 && rec->owner->type == 3) {
                                rec->flags |= 0x8000000;
                                obj->flags.all |= 0x4000000;
                            } else {
                                rec->flags |= 0x4000000;
                                obj->flags.all |= 0x8000000;
                                goto a_next;
                            }
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
                        if (rec->owner->active != 0 && rec->owner->type == 3) {
                            rec->flags |= 0x8000000;
                            obj->flags.all |= 0x4000000;
                        } else {
                            rec->flags |= 0x4000000;
                            obj->flags.all |= 0x8000000;
                            goto b_next;
                        }
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
                        if (rec->owner->active != 0 && rec->owner->type == 3) {
                            rec->flags |= 0x8000000;
                            obj->flags.all |= 0x4000000;
                        } else {
                            rec->flags |= 0x4000000;
                            obj->flags.all |= 0x8000000;
                            goto c_next;
                        }
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
