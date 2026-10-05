// Decompiled by Opus. Names are provisional.
// Slot 14 of UnitScript (vtable 0x4fd698), overriding
// CobScript::AttachUnit; the class views are those of
// src/units/units_485e30.cpp.

#pragma pack(push, 1)
struct Unit {
    char unknown_0[0x86];
    int owner;                         // +0x86
    char unknown_8a[0x110 - 0x8a];
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct Game {
    char unknown_0[0x14357];
    Unit* units;                       // +0x14357
};
#pragma pack(pop)

extern Game* g_game;

struct Player_00481340 {
    char unknown_0[0xc];
    int id;                            // +0x0c
};

void __stdcall AttachUnitToPiece(Unit* unit, int player, int a, int b);

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

class CobScript {
public:
    int field_4;                   // +0x4
    int field_8;                   // +0x8
    char unknown_c[0x10 - 0xc];
    void* ptr10;                   // +0x10
    void* ptr14;                   // +0x14
    char unknown_18[0x1c - 0x18];
    Elem_4b0610 arr[8];            // +0x1c
    int field_53c;                 // +0x53c

    CobScript();

    virtual void SetPieceTranslation(int, int, int) = 0;  // slot 0
    virtual void SetPieceRotation(int, int, int) = 0;  // slot 1
    virtual void SetPieceVisible(int, int) = 0;       // slot 2
    virtual void SetPieceCached(int, int) = 0;        // slot 3
    virtual void SetPieceShaded(int, int) = 0;        // slot 4
    virtual int GetPieceTranslation(int, int) = 0;    // slot 5
    virtual int GetPieceRotation(int, int) = 0;       // slot 6
    virtual int IsPieceVisible(int);                  // slot 7
    virtual int IsPieceCached(int);                   // slot 8
    virtual int IsPieceShaded(int);                   // slot 9
    virtual void FUN_004b1e80(int, int, int);         // slot 10
    virtual void FUN_004b1e90(int);                   // slot 11
    virtual void EmitSfx(int, int);                   // slot 12
    virtual void ExplodePiece(int, unsigned int);     // slot 13
    virtual void AttachUnit(unsigned short, int, int); // slot 14
    virtual void DropUnit(unsigned short);            // slot 15
    virtual void SetUnitValue(int, int);              // slot 16
    virtual int GetUnitValue(int, int, int, int, int); // slot 17
    virtual int IsCarryingUnit(int);                  // slot 18
    virtual int GetTransporterId();                   // slot 19
    virtual ~CobScript();                             // slot 20
};

class UnitScript : public CobScript {
public:
    Player_00481340* player;       // +0x540

    virtual void SetPieceTranslation(int, int, int);  // slot 0
    virtual void SetPieceRotation(int, int, int);     // slot 1
    virtual void SetPieceVisible(int, int);           // slot 2
    virtual void SetPieceCached(int, int);            // slot 3
    virtual void SetPieceShaded(int, int);            // slot 4
    virtual int GetPieceTranslation(int, int);        // slot 5
    virtual int GetPieceRotation(int, int);           // slot 6
    virtual int IsPieceVisible(int);                  // slot 7, 0x480e30
    virtual int IsPieceCached(int);                   // slot 8, 0x480e50
    virtual int IsPieceShaded(int);                   // slot 9, 0x480e70
    virtual void FUN_004b1e80(int, int, int);         // slot 10, 0x480e90
    virtual void FUN_004b1e90(int);                   // slot 11, 0x480ea0
    virtual void EmitSfx(int, int);                   // slot 12, 0x480eb0
    virtual void ExplodePiece(int, unsigned int);     // slot 13, 0x481140
    virtual void AttachUnit(unsigned short, int, int); // slot 14, 0x481340
    virtual void DropUnit(unsigned short);            // slot 15, 0x4813b0
    virtual void SetUnitValue(int, int);              // slot 16, 0x480b20
    virtual int GetUnitValue(int, int, int, int, int); // slot 17, 0x480770
    virtual int IsCarryingUnit(int);                  // slot 18, 0x481430
    virtual int GetTransporterId();                   // slot 19, 0x481470
};

// FUNCTION: 0x481340
void UnitScript::AttachUnit(unsigned short id, int a, int b)
{
    Unit* u = GetUnit(id);
    if (u != 0 && (u->flags & 0x10000000)
        && (u->owner == 0 || u->owner == player->id)) {
        AttachUnitToPiece(u, player->id, a, b);
    }
}
