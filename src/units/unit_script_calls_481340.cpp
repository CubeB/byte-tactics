// Decompiled by Opus. Names are provisional.
// Slot 14 of Class_00485e30 (vtable 0x4fd698), overriding
// Class_004b0610::FUN_004b0650; the class views are those of
// src/units/units_485e30.cpp.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x86];
    int owner;                         // +0x86
    char unknown_8a[0x110 - 0x8a];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game_00481340 {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
};
#pragma pack(pop)

extern Game_00481340* g_game;

struct Player_00481340 {
    char unknown_0[0xc];
    int id;                            // +0x0c
};

void __stdcall FUN_0048aac0(Unit* unit, int player, int a, int b);

static inline Unit* GetUnit(unsigned short id)
{
    if (id == 0)
        return 0;
    return &g_game->units[id];
}

struct Elem_4b0610 {
    int value;         // +0x0
    char pad[0xa0];    // pad to stride 0xa4
};

class Class_004b0610 {
public:
    int field_4;                   // +0x4
    int field_8;                   // +0x8
    char unknown_c[0x10 - 0xc];
    void* ptr10;                   // +0x10
    void* ptr14;                   // +0x14
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];            // +0x1c
    int field_53c;                 // +0x53c

    Class_004b0610();

    virtual void FUN_00480c50(int, int, int) = 0;     // slot 0
    virtual void FUN_00480ce0(int, int, int) = 0;     // slot 1
    virtual void FUN_00480d50(int, int) = 0;          // slot 2
    virtual void FUN_00480db0(int, int) = 0;          // slot 3
    virtual void FUN_00480df0(int, int) = 0;          // slot 4
    virtual int FUN_00480c30(int, int) = 0;           // slot 5
    virtual int FUN_00480cb0(int, int) = 0;           // slot 6
    virtual int FUN_004b1e50(int);                    // slot 7
    virtual int FUN_004b1e60(int);                    // slot 8
    virtual int FUN_004b1e70(int);                    // slot 9
    virtual void FUN_004b1e80(int, int, int);         // slot 10
    virtual void FUN_004b1e90(int);                   // slot 11
    virtual void FUN_004b1ea0(int, int);              // slot 12
    virtual void FUN_004b1eb0(int, unsigned int);     // slot 13
    virtual void FUN_004b0650(unsigned short, int, int); // slot 14
    virtual void FUN_004b0660(unsigned short);        // slot 15
    virtual void FUN_004b0670(int, int);              // slot 16
    virtual int FUN_004b0680(int, int, int, int, int); // slot 17
    virtual int FUN_004b0690(int);                    // slot 18
    virtual int FUN_004b06a0();                       // slot 19
    virtual ~Class_004b0610();                        // slot 20
};

class Class_00485e30 : public Class_004b0610 {
public:
    Player_00481340* player;       // +0x540

    virtual void FUN_00480c50(int, int, int);         // slot 0
    virtual void FUN_00480ce0(int, int, int);         // slot 1
    virtual void FUN_00480d50(int, int);              // slot 2
    virtual void FUN_00480db0(int, int);              // slot 3
    virtual void FUN_00480df0(int, int);              // slot 4
    virtual int FUN_00480c30(int, int);               // slot 5
    virtual int FUN_00480cb0(int, int);               // slot 6
    virtual int FUN_004b1e50(int);                    // slot 7, 0x480e30
    virtual int FUN_004b1e60(int);                    // slot 8, 0x480e50
    virtual int FUN_004b1e70(int);                    // slot 9, 0x480e70
    virtual void FUN_004b1e80(int, int, int);         // slot 10, 0x480e90
    virtual void FUN_004b1e90(int);                   // slot 11, 0x480ea0
    virtual void FUN_004b1ea0(int, int);              // slot 12, 0x480eb0
    virtual void FUN_004b1eb0(int, unsigned int);     // slot 13, 0x481140
    virtual void FUN_004b0650(unsigned short, int, int); // slot 14, 0x481340
    virtual void FUN_004b0660(unsigned short);        // slot 15, 0x4813b0
    virtual void FUN_004b0670(int, int);              // slot 16, 0x480b20
    virtual int FUN_004b0680(int, int, int, int, int); // slot 17, 0x480770
    virtual int FUN_004b0690(int);                    // slot 18, 0x481430
    virtual int FUN_004b06a0();                       // slot 19, 0x481470
};

// FUNCTION: 0x481340
void Class_00485e30::FUN_004b0650(unsigned short id, int a, int b)
{
    Unit* u = GetUnit(id);
    if (u != 0 && (u->flags & 0x10000000)
        && (u->owner == 0 || u->owner == player->id)) {
        FUN_0048aac0(u, player->id, a, b);
    }
}
