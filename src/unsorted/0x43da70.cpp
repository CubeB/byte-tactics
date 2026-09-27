// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

class Class_004b0940 { public: void FUN_004b0940(const char*, int, int); };

#pragma pack(push, 1)
struct Type_0043da70 {
    char unknown_0[0x1ae];
    int field_1ae;                 // +0x1ae
    int field_1b2;                 // +0x1b2
};

struct Unit_0043da70 {
    char unknown_0[0x86];
    int field_86;                  // +0x86
    char unknown_8a[0x92 - 0x8a];
    Type_0043da70* field_92;       // +0x92
    char unknown_96[0x9a - 0x96];
    Class_004b0940* script;        // +0x9a
    char unknown_9e[0x110 - 0x9e];
    unsigned int flags;            // +0x110
};

class Class_0043da70 {
public:
    char unknown_0[0x20];
    int field_20;                  // +0x20
    short field_24;                // +0x24
    char unknown_26[0x2e - 0x26];
    unsigned char field_2e;        // +0x2e
    void FUN_0043da70(Unit_0043da70* unit);
};
#pragma pack(pop)

// FUNCTION: 0x43da70
void Class_0043da70::FUN_0043da70(Unit_0043da70* unit)
{
    int rate;
    if ((field_2e & 4) == 0 && unit->field_86 == 0
        && (field_20 != 0 || field_24 != 0)) {
        if (field_20 <= unit->field_92->field_1ae) {
            rate = 1;
        } else {
            rate = 2 + (field_20 > unit->field_92->field_1b2);
        }
    } else {
        rate = 0;
    }
    if (rate == (int)((unit->flags >> 2) & 3))
        return;
    if (rate == 0) {
        unit->script->FUN_004b0940("StopMoving", rate, 1);
    } else if ((unit->flags & 0xc) == 0) {
        unit->script->FUN_004b0940("StartMoving", 0, 1);
    }
    switch (rate) {
    case 1:
        unit->script->FUN_004b0940("MoveRate1", 0, 1);
        break;
    case 2:
        unit->script->FUN_004b0940("MoveRate2", 0, 1);
        break;
    case 3:
        unit->script->FUN_004b0940("MoveRate3", 0, 1);
        break;
    }
    unit->flags = (unit->flags & 0xfffffff3) | ((rate & 3) << 2);
}
