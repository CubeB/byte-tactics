// Decompiled by GPT-5.6-Terra and GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// The single test is the loop's pre-test: write the loop body with a fresh
// char* built from `entries + i*0x15b + 0x140` inside the body so MSVC strength
// reduces it into the rotated preheader (add ebp,0x29b). Splitting the first
// float ratio into `float ratio = a * b; result = (int)(ratio / c);` fixes the
// x87 operand-staging order.
#include <string.h>
#include <windows.h>

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
    int i = 1;
    char* me = (char*)entries + param_2 * 0x15b;
    int type = *(unsigned char*)me;
    int field_1b = *(int*)(me + 0x1b);
    for (; i < (short)entries->count + 1; i++) {
        char* entry = (char*)entries + i * 0x15b + 0x140;
        if (i != param_2) {
            if (entry[-0x13f] == me[1]) {
                switch ((unsigned char)entry[-0x140]) {
                case 2:
                    if (type == 2) {
                        *(short*)(entry - 0x84) = *(short*)(me + 0xbc);
                        *(short*)(entry - 0x86) = *(short*)(me + 0xba);
                        FUN_004a1b40(param_1, i);
                    } else if (type == 4) {
                        int esi_val;
                        if (*(unsigned char*)(entry - 0x125) & 0x20) {
                            esi_val = *(short*)(me + 0x136) / (*(short*)(entry - 0x82) + 1);
                        } else {
                            esi_val = 0;
                        }
                        short rows = *(short*)(entry - 0x66);
                        if (rows != 0) {
                            int edx_val = *(short*)(entry - 0x80) -
                                *(short*)(entry - 0x127) / rows;
                            int eax_val = *(short*)(me + 0x140) + esi_val;
                            int result = (int)((float)edx_val * eax_val /
                                (*(short*)(me + 0x136) - 1));
                            *(short*)(entry - 0x84) = result;
                        }
                        FUN_004a1b40(param_1, i);
                    }
                    break;
                case 3:
                    if (type == 2 && field_1b & 8) {
                        char* line = FUN_004b6af0(*(char**)(me + 0xc2), *(short*)(me + 0xba));
                        strcpy(entry - 0x8a, line);
                        FUN_004a4d70(param_1, i);
                    }
                    break;
                case 4:
                    if (type == 2) {
                        if (*(short*)(me + 0xc0) > 1) {
                            int result;
                            if (*(short*)(me + 0xbe) != 0) {
                                short scale = *(short*)(me + 0xbc);
                                short height = *(short*)(entry - 0xa);
                                short count = *(short*)(me + 0xbe);
                                float ratio = (float)scale * height;
                                result = (int)(ratio / count);
                            } else {
                                result = 0;
                            }
                            if (*(short*)entry != result) {
                                *(short*)entry = result;
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
