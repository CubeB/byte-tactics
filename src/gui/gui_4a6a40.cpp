// Decompiled by Opus. Names are provisional.

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
};

#pragma pack(push, 1)
struct Dialog {
    char unknown_0[0x18];
    Holder_004a6a40* holder;           // +0x18
    char unknown_1c[0xcca - 0x1c];
    int field_cca;                     // +0xcca
};
#pragma pack(pop)

void __stdcall DrawButton(Dialog* param_1, int param_2);

// Sibling of 0x4a69d0, limited to the entries on the same team as `index`.
// FUNCTION: 0x4a6a40
void __stdcall FUN_004a6a40(Dialog* param_1, int index)
{
    Entry_004a6a40* entries = param_1->holder->entries;
    Entry_004a6a40* e = &entries[1];
    char team = entries[index].team;
    for (int i = 1; i < entries->count + 1; i++, e++) {
        if (e->state == 1 && e->team == team && e->field_138 != 0) {
            e->field_138 = 0;
            DrawButton(param_1, i);
            param_1->field_cca = 1;
        }
    }
}
