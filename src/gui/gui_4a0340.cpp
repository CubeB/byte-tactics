// Decompiled by space-bunny-free. Names are provisional.
// Same entry table as 0x4a69d0/0x4a6a40: clears the field_138 flag of every
// other active entry on the same team as entry `index`, and marks the holder
// dirty. Only runs when entry `index` itself has a non-zero team.

#pragma pack(push, 1)
struct Entry_004a6a40 {
    unsigned char state;               // +0x0
    char team;                         // +0x1
    char unknown_2[0xb6 - 0x2];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0x138 - 0xb8];
    short field_138;                   // +0x138
    char unknown_13a[0x15b - 0x13a];
};
#pragma pack(pop)

struct Holder_004a6a40 {
    char unknown_0[4];
    Entry_004a6a40* entries;           // +0x4
    char unknown_8[0x14 - 0x8];
    int field_14;                      // +0x14
};

#pragma pack(push, 1)
struct Class_004a6a40 {
    char unknown_0[0x18];
    Holder_004a6a40* holder;           // +0x18
};
#pragma pack(pop)

void __stdcall FUN_004a5f40(Class_004a6a40* param_1, int param_2);

// FUNCTION: 0x4a0340
void __stdcall FUN_004a0340(Class_004a6a40* param_1, int index)
{
    Entry_004a6a40* entries = param_1->holder->entries;
    Entry_004a6a40* me = &entries[index];
    if (me->team != 0) {
        Entry_004a6a40* e = &entries[1];
        for (int i = 1; i < entries->count + 1; i++, e++) {
            if (e->state == 1 && i != index && e->team == me->team
                && e->field_138 != 0) {
                e->field_138 = 0;
                FUN_004a5f40(param_1, i);
                if (param_1->holder != 0) {
                    param_1->holder->field_14 = 1;
                }
            }
        }
    }
}
