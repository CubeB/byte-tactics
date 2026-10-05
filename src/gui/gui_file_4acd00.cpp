// Decompiled by Opus. Names are provisional.
// Writes a section header: "[name]" on its own line indented by depth - 1
// tabs, then "{" indented by depth tabs. The two tab runs are the inlined
// tab writer (0x4accd0), each with its own tab character.
#include <stdio.h>
#include <string.h>

struct Class_004bbbe0;

extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* data, unsigned int size);

static inline void WriteTabs(Class_004bbbe0* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        FUN_004bbbe0(file, &tab, 1);
}

// FUNCTION: 0x4acd00
void __stdcall FUN_004acd00(Class_004bbbe0* file, char* name, int depth)
{
    char line[100];
    sprintf(line, "[%s]", name);
    WriteTabs(file, depth - 1);
    FUN_004bbbe0(file, line, strlen(line));
    FUN_004bbbe0(file, "\n", 1);
    WriteTabs(file, depth);
    FUN_004bbbe0(file, "{\n", 2);
}
