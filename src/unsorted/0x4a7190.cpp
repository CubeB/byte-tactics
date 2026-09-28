// Decompiled by deepseek-v4.1-flash. Names are provisional.
#pragma pack(push, 1)

struct Entry_004a7190 {               // 0x15b bytes
    unsigned char type;               // +0x00
    char unknown_01[0x1f - 0x01];
    int colourIndex;                  // +0x1f
    char unknown_23[0x28 - 0x23];
    char group;                       // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                      // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xd6 - 0xb8];
    int id;                           // +0xd6
    char unknown_da[0x138 - 0xda];
    short maxLength;                  // +0x138
    char unknown_13a[0x15b - 0x13a];
};

struct Holder_004a7190 {
    int current;                      // +0x00
    Entry_004a7190* entries;          // +0x04
    char unknown_08[0x20 - 0x08];
    int field_20;                     // +0x20
};

struct Class_004a7190 {
    char unknown_0[0x18];
    Holder_004a7190* holder;          // +0x18
    char unknown_1c[0x8b2 - 0x1c];
    unsigned char colors[16];         // +0x8b2
};
#pragma pack(pop)

extern Holder_004a7190* DAT_0051fba4;

int FUN_004c13f0();
void __stdcall FUN_004c13a0(int colour, int font);
void __stdcall FUN_004c1420(int id);
void FUN_004c1a40();
int __stdcall FUN_0049fc50(Class_004a7190* obj, int index);
void __stdcall FUN_004ab6c0(Class_004a7190* obj, void* param_2, char* text,
                            int maxLength, int clear);

// FUNCTION: 0x4a7190
void __stdcall FUN_004a7190(Class_004a7190* obj, int index)
{
    Entry_004a7190* entries = obj->holder->entries;
    Entry_004a7190* target = &entries[index];

    FUN_004c13a0(obj->colors[target->colourIndex], FUN_004c13f0());

    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->group) {
                FUN_004c1420(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        FUN_004c1420(DAT_0051fba4->current);
    }

    FUN_0049fc50(obj, index);
    obj->holder->field_20 = index;
    FUN_004ab6c0(obj, (void*)index, (char*)((char*)target + 0xb6),
                 target->maxLength, 0);
    FUN_004c1a40();
}
