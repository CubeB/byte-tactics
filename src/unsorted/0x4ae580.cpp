// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Writes the "nuttin" int field (offset 0xb6) as "nuttin=<n>;" on its own
// line, indented by the given number of tabs; the reading counterpart is
// 0x4ae610. Byte-for-byte twin of 0x4ae380.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Class_004bbbe0 {
    FILE* file;                        // +0x0
    int error;                         // +0x4
};

#pragma pack(push, 1)
struct Obj_004ae580 {
    char unknown_0[0xb6];
    int field_b6;                      // +0xb6
};
#pragma pack(pop)

unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* param_1, void* param_2, unsigned int param_3);

// FUNCTION: 0x4ae580
void __stdcall FUN_004ae580(Obj_004ae580* obj, Class_004bbbe0* out, int indent)
{
    char tab;
    char line[100];
    char num[100];
    char* value = _itoa(obj->field_b6, num, 10);
    tab = '\t';
    for (int i = 0; i < indent; i++)
        FUN_004bbbe0(out, &tab, 1);
    sprintf(line, "%s=%s;\n", "nuttin", value);
    FUN_004bbbe0(out, line, strlen(line));
}
