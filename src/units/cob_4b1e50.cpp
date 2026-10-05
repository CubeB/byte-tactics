// Decompiled by Haiku and Opus. Names are provisional.
// Vtable slots 7-13 of CobScript; the class, its constructor, slots
// 14-19 and its destructor are in src/units/cob_4b0610.cpp.

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

// FUNCTION: 0x4b1e50
int CobScript::IsPieceVisible(int)
{
    return 0;
}

// FUNCTION: 0x4b1e60
int CobScript::IsPieceCached(int)
{
    return 0;
}

// FUNCTION: 0x4b1e70
int CobScript::IsPieceShaded(int)
{
    return 0;
}

// FUNCTION: 0x4b1e80
void CobScript::FUN_004b1e80(int, int, int)
{
}

// FUNCTION: 0x4b1e90
void CobScript::FUN_004b1e90(int)
{
}

// FUNCTION: 0x4b1ea0
void CobScript::EmitSfx(int, int)
{
}

// FUNCTION: 0x4b1eb0
void CobScript::ExplodePiece(int, unsigned int)
{
}
