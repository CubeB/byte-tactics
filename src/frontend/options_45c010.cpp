// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <stdio.h>

extern char* g_game;

struct Object_004a0bf0;

void __stdcall FUN_004a0bf0(Object_004a0bf0* obj, char* name, int param_3, int param_4);

#pragma pack(push, 1)
struct Entry_0045c010 {
    char* format;   // +0x0
    int min;        // +0x4
};
#pragma pack(pop)

// FUNCTION: 0x45c010
void __stdcall FUN_0045c010(Entry_0045c010* table, char* name, int value)
{
    char buf[100];
    if (table->format == 0)
        return;
    while (table->format != 0) {
        if (value <= table->min) {
            sprintf(buf, table->format, value);
            FUN_004a0bf0((Object_004a0bf0*)(g_game + 0x519), name, (int)buf, 0);
            return;
        }
        table++;
    }
}
