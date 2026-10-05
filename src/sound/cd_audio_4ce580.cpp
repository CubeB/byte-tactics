// Decompiled by Opus. Names are provisional.
// Sets the value at +0x204 (read back by FUN_004ce5a0) unless it exceeds the
// limit at +0x200.

class Class_004ce580 {
public:
    char unknown_0[0x200];
    int limit;                         // +0x200
    int value;                         // +0x204

    void FUN_004ce580(int v);
};

// FUNCTION: 0x4ce580
void Class_004ce580::FUN_004ce580(int v)
{
    if (v <= limit) {
        value = v;
    }
}
