// Decompiled by Opus. Names are provisional.
// Writes "<key>=<value>;" on its own line, indented by `depth` tabs.
#include <stdio.h>
#include <string.h>

struct FileHandle;

extern unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);

// FUNCTION: 0x4acde0
void __stdcall WriteKeyValue(FileHandle* file, char* key, char* value, int depth)
{
    char tab = '\t';
    char line[100];
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
    sprintf(line, "%s=%s;\n", key, value);
    HAPI_WriteFile(file, line, strlen(line));
}
