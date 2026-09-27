// Decompiled by Opus. Names are provisional.
// Clears flag 0x2000 of an order and, if it was set, posts message kind 5
// with the given text for the order's unit.

struct Unit_00438880;

void __stdcall FUN_0047f780(Unit_00438880* unit, int kind, char* text);

#pragma pack(push, 1)
class Class_00438880 {
public:
    char unknown_0[0xe];
    Unit_00438880* unit;               // +0xe
    char unknown_12[0x42 - 0x12];
    unsigned int flags;                // +0x42

    void FUN_00438880(char* text);
};
#pragma pack(pop)

// FUNCTION: 0x438880
void Class_00438880::FUN_00438880(char* text)
{
    if (flags & 0x2000) {
        flags &= ~0x2000;
        FUN_0047f780(unit, 5, text);
    }
}
