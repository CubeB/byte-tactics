// Decompiled by deepseek-v4.1-flash, Space Bunny Free, Opus, Haiku and Sonnet. Names are provisional.
// UnitScript: the COB script of a unit, the one class derived from CobScript
// (vtable 0x4fd698; its scalar deleting destructor is in units_485e30.cpp).
// Its slots move and query the pieces of the unit's model and read and set
// the unit's properties for the script.

// Unused, but <windows.h> and <string> must stay: they change register use
// in GetUnitValue and ExplodePiece.
#include <windows.h>
#include <string>
#include <math.h>
#include <string.h>

struct Vec3 {
    int x, y, z;
};

#pragma pack(push, 1)
struct UnitDef {
    char unknown_0[0x16e];
    int modelMaxY;                      // +0x16e
    char unknown_172[0x1fa - 0x172];
    unsigned int maxHealth;             // +0x1fa
};

struct Unit {
    char unknown_0[0x66];
    short heading;                     // +0x66
    char unknown_68[0x6a - 0x68];
    Vec3 pos;                           // +0x6a
    int cell;                       // +0x76
    char unknown_7a[0x86 - 0x7a];
    Unit* transporter;                  // +0x86
    Unit* carried;                      // +0x8a, the first unit it carries
    Unit* nextCarried;                  // +0x8e
    UnitDef* def;                       // +0x92
    char unknown_96[0xa8 - 0x96];
    unsigned short id;                  // +0xa8
    char unknown_aa[0xba - 0xaa];
    unsigned short unknown_ba : 2;      // +0xba
    // A 1-bit bitfield, not a byte: gives a single `or` in SetUnitValue.
    unsigned short dirty : 1;
    unsigned short unknown_ba_3 : 13;
    char unknown_bc[0x104 - 0xbc];
    float buildLeft;                    // +0x104
    short health;                       // +0x108
    char unknown_10a[0x10e - 0x10a];
    unsigned char on : 1;               // +0x10e bit 0
    unsigned char on2 : 1;              // +0x10e bit 1
    unsigned char unknown_10e : 6;
    unsigned char bit0 : 1;             // +0x10f bit 0
    unsigned char bit1 : 1;
    unsigned char bit2 : 1;
    unsigned char bit3 : 1;
    unsigned char unknown_10f : 4;
    unsigned int flags;                 // +0x110
    char unknown_114[0x118 - 0x114];

    void SetStateBits(int which, int on);
};

// The animation state of one piece of a unit's model.
struct PieceState {                     // 0x36 bytes
    int field_0;                        // +0x00
    int translation[3];                 // +0x04
    unsigned short rotation[3];         // +0x10
    Vec3 pos;                           // +0x16
    int* sfxOffset;                     // +0x22, two points EmitSfx emits between
    short value;                        // +0x26, cleared when the piece changes
    unsigned short visible : 1;         // +0x28 bit 0
    unsigned short cached : 1;          // +0x28 bit 1
    unsigned short shaded : 1;          // +0x28 bit 2
    char unknown_2a[0x36 - 0x2a];
};

// A unit's model instance: the pieces its script moves.
struct ObjectState {
    int unknown_0;                      // +0x00
    int unknown_4;                      // +0x04, cleared when a cached piece changes
    int dirty;                          // +0x08
    Unit* unit;                         // +0x0c
    int field_10;                       // +0x10
    char unknown_14[0x22 - 0x14];
    PieceState pieces[1];               // +0x22
};

struct Player {
    char unknown_0[0x14b];
};

struct Game {
    char unknown_0[0x1b63];
    Player players[10];                 // +0x1b63
    char unknown_2851[0x2a43 - 0x2851];
    unsigned char playerIndex;          // +0x2a43
    char unknown_2a44[0x1427f - 0x2a44];
    unsigned char limitY;               // +0x1427f
    char unknown_14280[0x14357 - 0x14280];
    Unit* units;                        // +0x14357
    char unknown_1435b[0x147f7 - 0x1435b];
    void* sources[6];                   // +0x147f7
};

// The 0x30-byte record StartExplodePiece consumes.
struct Header_00481140 {
    void* obj;                         // +0x00
    int index;                         // +0x04
    int r1;                            // +0x08
    int r2;                            // +0x0c
    int r3;                            // +0x10
    int x;                             // +0x14
    int y;                             // +0x18
    int z;                             // +0x1c
    int f20;                           // +0x20
    int f24;                           // +0x24
    unsigned int bits;                 // +0x28
    void* field_2c;                    // +0x2c
};
#pragma pack(pop)

extern Game* g_game;

Vec3 __stdcall GetPiecePosition(Unit* obj, int param);
int __stdcall GetGroundHeight(Vec3* pos);
int __cdecl FUN_004b715a(int a, int b);
void __stdcall SetYardOpen(Unit* unit, int flag);
int __stdcall IsUnitVisibleToPlayer(Player* player, Unit* unit);
void __stdcall UpdateObjectState(Unit* unit);
void __stdcall EmitThrustParticles(int, int, int, int, short);
void __stdcall EmitWakeParticles(int, int, int, short);
void __stdcall EmitBubbles(int, int, int, short);
void __stdcall EmitWhiteSmoke(int, short);
void __stdcall EmitBlackSmoke(int, short);
int __stdcall RandomInt(int range);
void __stdcall StartExplodePiece(Header_00481140* h);
void __stdcall AddExplosionEffect(void* pos, void* src, int index, int flag);
void __stdcall AttachUnitToPiece(Unit* unit, Unit* transporter, int a, int b);
int __stdcall CanPlaceUnitFootprint(UnitDef* type, short a, int position, int b);

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
    virtual void ExplodeLegacy(int, int, int);        // slot 10
    virtual void PlaySoundNoop(int);                  // slot 11
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
    ObjectState* state;            // +0x540

    virtual void SetPieceTranslation(int index, int slot, int v);
    virtual void SetPieceRotation(int index, int slot, int v);
    virtual void SetPieceVisible(int index, int flag);
    virtual void SetPieceCached(int index, int flag);
    virtual void SetPieceShaded(int index, int flag);
    virtual int GetPieceTranslation(int index, int slot);
    virtual int GetPieceRotation(int index, int slot);
    virtual int IsPieceVisible(int index);
    virtual int IsPieceCached(int index);
    virtual int IsPieceShaded(int index);
    virtual void ExplodeLegacy(int, int, int);
    virtual void PlaySoundNoop(int);
    virtual void EmitSfx(int a, int b);
    virtual void ExplodePiece(int a, unsigned int b);
    virtual void AttachUnit(unsigned short id, int a, int b);
    virtual void DropUnit(unsigned short id);
    virtual void SetUnitValue(int which, int value);
    virtual int GetUnitValue(int which, int a, int b, int c, int d);
    virtual int IsCarryingUnit(int id);
    virtual int GetTransporterId();
    void SetObjectState(int state);
};

// Slot 17 of UnitScript (vtable 0x4fd698). The switch over the property id
// 1 to 20 matches the COB script "get" list (ACTIVATION, STANDINGMOVEORDERS,
// ..., ARMORED), with the property argument in the first stack parameter. The base class slot (0x4b0680)
// returns 0.

// FUNCTION: 0x480770
int UnitScript::GetUnitValue(int which, int a, int b, int c, int d)
{
    Unit* unit = state->unit;
    switch (which) {
    case 1:
        return unit->on;
    case 2:
        return (unit->flags >> 18) & 3;
    case 3:
        return (unit->flags >> 20) & 3;
    case 4:
        return unit->health * 100 / unit->def->maxHealth;
    case 5:
        return unit->bit0;
    case 6:
        return unit->bit1;
    case 7: {
        Vec3 v;
        v = GetPiecePosition(unit, a);
        return (v.x & 0xffff0000) + (v.z >> 16);
    }
    case 8: {
        Vec3 v;
        v = GetPiecePosition(unit, a);
        return v.y;
    }
    case 9: {
        Unit* u = GetUnit((unsigned short)a);
        if (u != 0 && (u->flags & 0x10000000))
            return (u->pos.z >> 16) + (u->pos.x & 0xffff0000);
        break;
    }
    case 10: {
        Unit* u = GetUnit((unsigned short)a);
        if (u != 0 && (u->flags & 0x10000000))
            return u->pos.y;
        break;
    }
    case 11: {
        Unit* u = GetUnit((unsigned short)a);
        if (u != 0 && (u->flags & 0x10000000))
            return u->def->modelMaxY;
        break;
    }
    case 12: {
        int hi = a & 0xffff0000;
        int lo = a << 16;
        if (lo < 0)
            hi += 0x10000;
        return (unsigned short)(FUN_004b715a(hi, lo) - unit->heading);
    }
    case 13: {
        int hi = a & 0xffff0000;
        int lo = a << 16;
        if (lo < 0)
            hi += 0x10000;
        return (int)_hypot((double)hi, (double)lo);
    }
    case 14:
        return FUN_004b715a(a, b) & 0xffff;
    case 15:
        return (int)_hypot((double)a, (double)b);
    case 16: {
        Vec3 v;
        v.x = a & 0xffff0000;
        v.z = a << 16;
        if (v.z < 0)
            v.x += 0x10000;
        return GetGroundHeight(&v) << 16;
    }
    case 17: {
        int result;
        if (unit->buildLeft == 0.0f)
            result = 0;
        else
            result = 1 - (int)(unit->buildLeft * -99.0f);
        return result;
    }
    case 18:
        return unit->bit2;
    case 19:
        return unit->bit3;
    case 20:
        return unit->on2;
    }
    return 0;
}

// Slot 16 of UnitScript (vtable 0x4fd698); see src/units/units_485e30.cpp
// and the sibling slots 0x480ce0, 0x480d50, 0x480db0, 0x480df0.
// FUNCTION: 0x480b20
void UnitScript::SetUnitValue(int which, int value)
{
    Unit* unit = state->unit;
    switch (which) {
    case 1:
        ((Unit*)unit)->SetStateBits(1, value);
        break;
    case 5:
        unit->bit0 = value;
        break;
    case 6:
        unit->bit1 = value;
        break;
    case 18:
        SetYardOpen(unit, value);
        break;
    case 19:
        unit->bit3 = value;
        break;
    case 20:
        ((Unit*)unit)->SetStateBits(2, value);
        break;
    }
    // After the switch, not inside the cases.
    unit->dirty = true;
}

// The piece array starts at +0x22 of the object state.
// FUNCTION: 0x480c30
int UnitScript::GetPieceTranslation(int index, int slot)
{
    return state->pieces[index].translation[slot];
}

// FUNCTION: 0x480c50
void UnitScript::SetPieceTranslation(int index, int slot, int v)
{
    if (state->pieces[index].translation[slot] != v) {
        state->pieces[index].translation[slot] = v;
        state->pieces[index].value = 0;
        state->dirty = 1;
        if (state->pieces[index].cached) {
            state->unknown_4 = 0;
        }
    }
}

// FUNCTION: 0x480cb0
int UnitScript::GetPieceRotation(int index, int slot)
{
    return state->pieces[index].rotation[slot];
}

// Slot 1 of UnitScript (vtable 0x4fd698).
// FUNCTION: 0x480ce0
void UnitScript::SetPieceRotation(int index, int slot, int v)
{
    if (state->pieces[index].rotation[slot] != (unsigned short)v) {
        state->pieces[index].rotation[slot] = v;
        state->pieces[index].value = 0;
        state->dirty = 1;
        if (state->pieces[index].cached) {
            state->unknown_4 = 0;
        }
    }
}

// FUNCTION: 0x480d40
void UnitScript::SetObjectState(int param_1)
{
    state = (ObjectState*)param_1;
}

// FUNCTION: 0x480d50
void UnitScript::SetPieceVisible(int index, int flag)
{
    if (state->pieces[index].visible != flag) {
        state->pieces[index].visible = flag;
        state->pieces[index].value = 0;
        if (state->pieces[index].cached) {
            state->unknown_4 = 0;
        }
    }
}

// Slot 3 of UnitScript (see 0x485e30.cpp); the same as 0x480df0 (slot 4)
// but for bit 1 of the entry flags.
// FUNCTION: 0x480db0
void UnitScript::SetPieceCached(int index, int flag)
{
    state->pieces[index].cached = flag;
    state->field_10 = 0;
}

// Slot 4 of UnitScript (see 0x485e30.cpp); compare 0x480d50.
// FUNCTION: 0x480df0
void UnitScript::SetPieceShaded(int index, int flag)
{
    state->pieces[index].shaded = flag;
    state->field_10 = 0;
}

// Slot 7 of UnitScript (vtable 0x4fd698), overriding
// CobScript::IsPieceVisible.
// FUNCTION: 0x480e30
int UnitScript::IsPieceVisible(int index)
{
    return state->pieces[index].visible;
}

// Slot 8 of UnitScript (vtable 0x4fd698), overriding
// CobScript::IsPieceCached.
// FUNCTION: 0x480e50
int UnitScript::IsPieceCached(int index)
{
    return state->pieces[index].cached;
}

// Slot 9 of UnitScript (vtable 0x4fd698), overriding
// CobScript::IsPieceShaded.
// FUNCTION: 0x480e70
int UnitScript::IsPieceShaded(int index)
{
    return state->pieces[index].shaded;
}

// Slot 10 of UnitScript (vtable 0x4fd698), overriding
// CobScript::ExplodeLegacy.
// FUNCTION: 0x480e90
void UnitScript::ExplodeLegacy(int, int, int)
{
}

// Slot 11 of UnitScript (vtable 0x4fd698), overriding
// CobScript::PlaySoundNoop.
// FUNCTION: 0x480ea0
void UnitScript::PlaySoundNoop(int)
{
}

// Slot 12 of UnitScript (vtable 0x4fd698).
//
// First the unit's visibility against the local player's map is tested; if the
// unit is not visible nothing happens. Otherwise the unit's state is snapped
// (UpdateObjectState) and a two-position record is built: with bit 0x100 of `b`
// set, only one position is needed, otherwise two (the second from the six
// dwords the piece's sfxOffset points at). The message id `b` then selects
// which list-append helper receives the pair.
// FUNCTION: 0x480eb0
void UnitScript::EmitSfx(int a, int b)
{
    if (!IsUnitVisibleToPlayer(&g_game->players[g_game->playerIndex], state->unit))
        return;
    UpdateObjectState(state->unit);

    Vec3 v1;
    Vec3 v2;

    if (b & 0x100) {
        v1 = state->unit->pos;
        v1.x += state->pieces[a].pos.x;
        v1.y += state->pieces[a].pos.y;
        v1.z -= state->pieces[a].pos.z;
    } else {
        // Binding the unit once is load bearing: without it MSVC reloads
        // state->unit for the second copy and the whole allocation shifts.
        Unit* u = state->unit;
        v2 = u->pos;
        v1 = u->pos;
        v1.x += state->pieces[a].sfxOffset[0];
        v1.y += state->pieces[a].sfxOffset[1];
        v1.z -= state->pieces[a].sfxOffset[2];
        v2.x += state->pieces[a].sfxOffset[3];
        v2.y += state->pieces[a].sfxOffset[4];
        v2.z -= state->pieces[a].sfxOffset[5];
    }

    switch (b) {
    case 0:
        EmitThrustParticles((int)&v1, (int)&v2, 1, 6, 7);
        break;
    case 1:
        EmitThrustParticles((int)&v1, (int)&v2, 1, 7, 7);
        break;
    case 2:
        EmitWakeParticles((int)&v1, (int)&v2, 0x10, 2);
        break;
    case 3:
        EmitWakeParticles((int)&v1, (int)&v2, 8, 2);
        break;
    case 4:
        EmitWakeParticles((int)&v2, (int)&v1, 0x10, 2);
        break;
    case 5:
        EmitWakeParticles((int)&v2, (int)&v1, 8, 2);
        break;
    case 0x101:
        EmitWhiteSmoke((int)&v1, 9);
        break;
    case 0x102:
        EmitBlackSmoke((int)&v1, 9);
        break;
    case 0x103:
        v2.x = v1.x;
        v2.y = v1.y;
        v2.z = v1.z;
        v2.y = g_game->limitY << 16;
        EmitBubbles((int)&v1, (int)&v2, 8, 7);
        break;
    }
}

// Slot 13 of UnitScript (vtable 0x4fd698), overriding
// CobScript::ExplodePiece.
//
// Two independent blocks selected by bits of the second argument `b`:
//  - !(b & 0x20): builds a 0x30-byte header on the stack (the same record
//    StartExplodePiece consumes) from the state->unit pointer at +0x0c, the first
//    argument and six RandomInt random draws, then hands it to
//    StartExplodePiece.
//  - (b & 0x3f00): computes the unit's position with GetPiecePosition and appends
//    it to up to six tables in g_game (+0x147f7, a six-pointer array) with
//    AddExplosionEffect(&v, table, 2, 0).
//
// Suspected original bug: h.bits is read before it is ever written (the
// first `h.bits & ~0x30` in each arm, and the three merge assignments, all
// load the uninitialised local). Only bits 6 and up survive the masks, so
// whatever the stack held leaks into the record StartExplodePiece copies.
// `b` must stay unsigned (shr, not sar) and the header's flag dword a plain
// unsigned int: a 1-bit field would truncate `(b & 2) << 4`.
// FUNCTION: 0x481140
void UnitScript::ExplodePiece(int a, unsigned int b)
{
    if (!(b & 0x20)) {
        Header_00481140 h;
        h.obj = state->unit;
        h.index = a;
        h.r1 = RandomInt(3000);
        h.r2 = RandomInt(3000);
        h.r3 = RandomInt(3000);
        h.x = (0x14 - RandomInt(0x28)) << 14;
        h.y = RandomInt(10) << 16;
        h.z = (0x14 - RandomInt(0x28)) << 14;
        h.f20 = 900;
        // h.f24 is stored after the OR in this arm and before it in the else arm.
        if (b & 1) {
            h.bits = (h.bits & ~0x30) | ((b & 2) << 4);
            h.f24 = 1;
            h.bits |= 4;
        } else {
            h.f24 = 0;
            h.bits = (h.bits & ~0x34) | ((b & 2) << 3);
        }
        h.bits = (h.bits & ~2) | ((b >> 2) & 2);
        h.bits = (h.bits & ~1) | ((b >> 4) & 1);
        h.bits = (h.bits & ~8) | ((b & 4) << 1);
        StartExplodePiece(&h);
    }
    if (b & 0x3f00) {
        Vec3 v;
        v = GetPiecePosition(state->unit, a);
        if (b & 0x100)
            AddExplosionEffect(&v, g_game->sources[0], 2, 0);
        if (b & 0x200)
            AddExplosionEffect(&v, g_game->sources[1], 2, 0);
        if (b & 0x400)
            AddExplosionEffect(&v, g_game->sources[2], 2, 0);
        if (b & 0x800)
            AddExplosionEffect(&v, g_game->sources[3], 2, 0);
        if (b & 0x1000)
            AddExplosionEffect(&v, g_game->sources[4], 2, 0);
        if (b & 0x2000)
            AddExplosionEffect(&v, g_game->sources[5], 2, 0);
    }
}

// Slot 14 of UnitScript (vtable 0x4fd698), overriding
// CobScript::AttachUnit.
// FUNCTION: 0x481340
void UnitScript::AttachUnit(unsigned short id, int a, int b)
{
    Unit* u = GetUnit(id);
    if (u != 0 && (u->flags & 0x10000000)
        && (u->transporter == 0 || u->transporter == state->unit)) {
        AttachUnitToPiece(u, state->unit, a, b);
    }
}

// Slot 15 of UnitScript (vtable 0x4fd698), overriding
// CobScript::DropUnit.
// FUNCTION: 0x4813b0
void UnitScript::DropUnit(unsigned short id)
{
    Unit* u = GetUnit(id);
    if (u != 0 && (u->flags & 0x10000000) && u->transporter == state->unit) {
        if (CanPlaceUnitFootprint(u->def, u->id, u->cell, 1)) {
            AttachUnitToPiece(u, 0, -1, 1);
        }
    }
}

// Slot 18 of UnitScript (vtable 0x4fd698), overriding
// CobScript::IsCarryingUnit.
// Returns whether the list at +0x8a of the object two links down holds an
// item with the given id.
// FUNCTION: 0x481430
int UnitScript::IsCarryingUnit(int id)
{
    Unit* p = state->unit->carried;
    while (p) {
        if (p->id == id)
            return 1;
        p = p->nextCarried;
    }
    return 0;
}

// Slot 19 of UnitScript (vtable 0x4fd698), overriding
// CobScript::GetTransporterId.
// FUNCTION: 0x481470
int UnitScript::GetTransporterId()
{
    Unit* t = state->unit->transporter;
    return t ? t->id : 0;
}
