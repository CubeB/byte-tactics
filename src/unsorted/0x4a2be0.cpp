// Decompiled by LongCat 2.5 Preview Free. Names are provisional.
// GAVE UP: Register allocation: compiler uses ebx for walking pointer, original uses ebp.
// Also redundant check after add ebp,0x29b. 39.4% match.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a2be0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x19 - 0x01];      // +0x01
    short field_19;                    // +0x19
    int field_1b;                      // +0x1b (dword)
    char unknown_1f[0xb6 - 0x1f];      // +0x1f
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];      // +0xb8
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    char unknown_c6[0xd6 - 0xc6];      // +0xc6
    int id;                            // +0xd6
    char unknown_d8[0xda - 0xd8];      // +0xd8
    short field_da;                    // +0xda
    char unknown_dc[0x136 - 0xdc];     // +0xdc
    short field_136;                   // +0x136
    char unknown_138[0x140 - 0x138];   // +0x138
    short field_140;                   // +0x140
    char unknown_142[0x15b - 0x142];   // +0x142
};
#pragma pack(pop)

struct Holder_004a2be0 {
    int current;                       // +0x00
    Entry_004a2be0* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    void* list;                        // +0x14
};

struct Class_004a2be0 {
    char unknown_0[0x18];
    Holder_004a2be0* holder;           // +0x18
};

void __stdcall FUN_004a1b40(Class_004a2be0* param_1, int param_2);
void __stdcall FUN_004a2580(Class_004a2be0* param_1, int param_2);
void __stdcall FUN_004a4d70(Class_004a2be0* param_1, int param_2);
char* __stdcall FUN_004b6af0(char* text, int line);

// FUNCTION: 0x4a2be0
void __stdcall FUN_004a2be0(Class_004a2be0* param_1, int param_2)
{
    Entry_004a2be0* entries = param_1->holder->entries;
    Entry_004a2be0* me = &entries[param_2];
    int type = me->type;
    int field_1b = me->field_1b;

    int i = 1;
    if ((short)entries->count + 1 > 1) {
        Entry_004a2be0* entry = (Entry_004a2be0*)((char*)entries + 0x29b);
        for (; i < (short)entries->count + 1; i++, entry = (Entry_004a2be0*)((char*)entry + 0x15b)) {
            if (i != param_2) {
                if (entry->unknown_01[0] == me->unknown_01[0]) {
                    switch (entry->type) {
                    case 2:
                        if (type == 2) {
                            entry->field_bc = me->field_bc;
                            entry->field_ba = me->field_ba;
                            FUN_004a1b40(param_1, i);
                        } else if (type == 4) {
                            int esi_val;
                            if (*(unsigned char*)&entry->field_1b & 0x20) {
                                esi_val = me->field_136 / (entry->field_be + 1);
                            } else {
                                esi_val = 0;
                            }
                            if (entry->field_da != 0) {
                                int edx_val = entry->field_c0 - entry->field_19 / entry->field_da;
                                int eax_val = me->field_140 + esi_val;
                                int result = (int)((float)edx_val * eax_val / (me->field_136 - 1));
                                entry->field_bc = result;
                            }
                            FUN_004a1b40(param_1, i);
                        }
                        break;
                    case 3:
                        if (type == 2 && field_1b & 8) {
                            char* line = FUN_004b6af0(me->field_c2, me->field_ba);
                            strcpy((char*)entry + 0xb6, line);
                            FUN_004a4d70(param_1, i);
                        }
                        break;
                    case 4:
                        if (type == 2) {
                            if (me->field_c0 > 1) {
                                int result;
                                if (me->field_be != 0) {
                                    result = (int)((float)me->field_bc * entry->field_136 / me->field_be);
                                } else {
                                    result = 0;
                                }
                                if (entry->field_140 != result) {
                                    entry->field_140 = result;
                                    FUN_004a2580(param_1, i);
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}
