// Decompiled by Sonnet, Haiku and Opus. Names are provisional.
// CobScript: an abstract base class, 0x540 bytes, vtable 0x4fdb00 with
// 21 slots. Slots 0-6 are pure; each is named after the one override that
// fills it, in the vtable of the only derived class (0x4fd698, see
// src/units/units_485e30.cpp). Slots 7-13 are defined in
// src/units/cob_4b1e50.cpp, slots 14-19 below, and slot 20 is the virtual
// destructor (the vtable holds its scalar deleting destructor).

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

extern int GetTickRate();
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4b0610
CobScript::CobScript()
{
    field_8 = 0;
    ptr14 = 0;
    ptr10 = 0;
    for (int i = 0; i < 8; i++) {
        arr[i].value = 0;
    }
    field_53c = 0;
    field_4 = GetTickRate();
}

// FUNCTION: 0x4b0650
void CobScript::AttachUnit(unsigned short, int, int)
{
}

// FUNCTION: 0x4b0660
void CobScript::DropUnit(unsigned short)
{
}

// FUNCTION: 0x4b0670
void CobScript::SetUnitValue(int, int)
{
}

// FUNCTION: 0x4b0680
int CobScript::GetUnitValue(int, int, int, int, int)
{
    return 0;
}

// FUNCTION: 0x4b0690
int CobScript::IsCarryingUnit(int)
{
    return 0;
}

// FUNCTION: 0x4b06a0
int CobScript::GetTransporterId()
{
    return 0;
}

// The compiler-generated scalar deleting destructor (0x4b06b0, vtable slot
// 20) comes from the destructor definition below, with its body inlined.
// FUNCTION: 0x4b06b0 ??_GCobScript@@UAEPAXI@Z
// FUNCTION: 0x4b06f0
CobScript::~CobScript()
{
    if (ptr14) {
        FUN_004d85a0((int*)ptr14);
    }
    if (ptr10) {
        FUN_004d85a0((int*)ptr10);
    }
}
