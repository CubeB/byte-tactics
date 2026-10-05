// Decompiled by Space Bunny Free. Names are provisional.
// Builds the piece-tree "Object State" block of a placed object, called from
// the three places that finish placing a unit. The block comes from
// FUN_0045a950, which also reorders the entries to match the build list, when
// the definition reached through +0x92 has object data at +0x18e; a 0x544-byte
// variable block is allocated for the object first and told about that data.
// Without the data the plain FUN_0045a8d0 builds the block and the owner
// pointer at +0xc is filled in by hand. Both arms store the block at +0x9e,
// and clearing its flag at +0x10 as a shared tail after the if/else is what
// makes MSVC 5 reload the block into eax in both exits and give each exit its
// own copy of the store.
//
// Both calls take the definition object (g_game->definitions[id], in ebx) as
// their argument. The cold arm pushes it at the top of its block, which looks
// like a spare register save but is the argument FUN_0045a8d0 pops itself.

#include <stddef.h>

struct ObjectState_00485d40 {
    char unknown_0[8];
    int field_8;                       // +0x8
    void* field_c;                     // +0xc
    int field_10;                      // +0x10
};

struct Class_0045ae80;                 // object definition, only passed on

#pragma pack(push, 1)
struct Data_00485d40 {                // the definition data at type+0x18e
    char unknown_0[8];
};

struct Unit_00485d40 {
    char unknown_0[0x18e];
    Data_00485d40* data;               // +0x18e
};

struct Game {
    char unknown_0[0x14377];
    Class_0045ae80** definitions;      // +0x14377
};
#pragma pack(pop)

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

    void SetCob(Data_00485d40* data);
};

class UnitScript : public CobScript {
public:
    void* field_540;               // +0x540

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

// The two names below are the ones data/symbols.csv gives 0x480d40 and
// 0x4b0940, but both are called on the variable block here, through casts.
class Class_004b0940 {
public:
    void StartScript(const char* name, int a, int b);
};

class Class_00480d40 {
public:
    char unknown_0[0x540];
    void* field_540;               // +0x540, the state block

    void SetObjectState(ObjectState_00485d40* state);
};

#pragma pack(push, 1)
struct Object_00485d40 {
    char unknown_0[0x92];
    Unit_00485d40* unit;           // +0x92
    char unknown_96[0x9a - 0x96];
    UnitScript* vars;              // +0x9a
    ObjectState_00485d40* state;   // +0x9e
    char unknown_a2[0xa6 - 0xa2];
    unsigned short id;             // +0xa6
};
#pragma pack(pop)

extern Game* g_game;

void* __cdecl operator new(size_t size);
ObjectState_00485d40* __stdcall FUN_0045a950(Class_0045ae80* obj, Data_00485d40* data, int player);
ObjectState_00485d40* __stdcall FUN_0045a8d0(Class_0045ae80* obj);

// FUNCTION: 0x485d40
void __stdcall InitUnitScript(Object_00485d40* self)
{
    Class_0045ae80* obj = g_game->definitions[self->id];
    if (self->unit->data) {
        self->vars = new UnitScript;
        self->vars->SetCob(self->unit->data);
        self->state = FUN_0045a950(obj, self->unit->data, (int)self);   // the owner, as an int
        ((Class_00480d40*)self->vars)->SetObjectState(self->state);
        ((Class_004b0940*)self->vars)->StartScript("Create", 0, 1);
    } else {
        self->vars = 0;
        self->state = FUN_0045a8d0(obj);
        self->state->field_c = self;
    }
    self->state->field_10 = 0;
}
