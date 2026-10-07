// Decompiled by Haiku and Opus. Names are provisional.
// The compiler-generated scalar deleting destructor of UnitScript, the
// only class derived from CobScript (src/units/cob.cpp). Its
// vtable at 0x4fd698 has the base's 21 slots, all overridden; slot 20 holds
// this function. The derived class has no destructor of its own, so the
// implicit one only calls the base destructor.
//
// The overrides live at 0x480770-0x481470. Slots 7-19 here carry the base
// names, with their addresses (from 0x4fd698) noted beside them. All 20 are
// defined as members of UnitScript (src/units/unit_script.cpp).
//
// InitUnitScript builds the object (`new` of 0x544 bytes, then this class's
// vtable).
// The global below exists only to emit the vtable and this COMDAT, as in
// src/game/data_files_42a870.cpp.

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
    virtual void ExplodeLegacy(int, int, int);        // slot 10, 0x480e90
    virtual void PlaySoundNoop(int);                  // slot 11, 0x480ea0
    virtual void EmitSfx(int, int);                   // slot 12, 0x480eb0
    virtual void ExplodePiece(int, unsigned int);     // slot 13, 0x481140
    virtual void AttachUnit(unsigned short, int, int); // slot 14, 0x481340
    virtual void DropUnit(unsigned short);            // slot 15, 0x4813b0
    virtual void SetUnitValue(int, int);              // slot 16, 0x480b20
    virtual int GetUnitValue(int, int, int, int, int); // slot 17, 0x480770
    virtual int IsCarryingUnit(int);                  // slot 18, 0x481430
    virtual int GetTransporterId();                   // slot 19, 0x481470
};

// FUNCTION: 0x485e30 ??_GUnitScript@@UAEPAXI@Z
static UnitScript* s_object;
// A namespace-scope `new` would construct the object during CRT init, and the
// base constructor reads a global that is not set until later.
UnitScript* emit_00485e30() { return new UnitScript; }
