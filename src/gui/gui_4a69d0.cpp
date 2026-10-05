// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Entry_004a69d0 {
    unsigned char state;               // +0x0
    char unknown_1[0xb6 - 0x1];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x138 - 0xb8];
    short field_138;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};
#pragma pack(pop)

struct Holder_004a69d0 {
    char unknown_0[4];
    Entry_004a69d0* entries;           // +0x4
};

#pragma pack(push, 1)
struct Class_004a69d0 {
    char unknown_0[0x18];
    Holder_004a69d0* holder;           // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

void __stdcall FUN_004a5f40(Class_004a69d0* param_1, int param_2);

// FUNCTION: 0x4a69d0
void __stdcall FUN_004a69d0(Class_004a69d0* param_1)
{
    Entry_004a69d0* entries = param_1->holder->entries;
    Entry_004a69d0* e = &entries[1];
    for (int i = 1; i < entries->count + 1; i++, e++) {
        if (e->state == 1 && e->field_138 != 0) {
            e->field_138 = 0;
            FUN_004a5f40(param_1, i);
            param_1->field_cca = 1;
        }
    }
}
