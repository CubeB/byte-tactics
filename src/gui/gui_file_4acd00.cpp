// Decompiled by Opus. Names are provisional.
// Writes a section header: "[name]" on its own line indented by depth - 1
// tabs, then "{" indented by depth tabs. The two tab runs are the inlined
// tab writer (0x4accd0), each with its own tab character.
#include <stdio.h>
#include <string.h>

struct FileHandle;

extern unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);

static inline void WriteTabs(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
}

// FUNCTION: 0x4acd00
void __stdcall WriteSectionHeader(FileHandle* file, char* name, int depth)
{
    char line[100];
    sprintf(line, "[%s]", name);
    WriteTabs(file, depth - 1);
    HAPI_WriteFile(file, line, strlen(line));
    HAPI_WriteFile(file, "\n", 1);
    WriteTabs(file, depth);
    HAPI_WriteFile(file, "{\n", 2);
}
