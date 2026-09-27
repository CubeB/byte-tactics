// Decompiled by Opus. Names are provisional.
// Writes "<key>=<value>;" on its own line, indented by `depth` tabs.
#include <stdio.h>
#include <string.h>

struct Class_004bbbe0;

extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* data, unsigned int size);

// FUNCTION: 0x4acde0
void __stdcall FUN_004acde0(Class_004bbbe0* file, char* key, char* value, int depth)
{
    char tab = '\t';
    char line[100];
    for (int i = 0; i < depth; i++)
        FUN_004bbbe0(file, &tab, 1);
    sprintf(line, "%s=%s;\n", key, value);
    FUN_004bbbe0(file, line, strlen(line));
}
