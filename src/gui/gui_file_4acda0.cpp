// Decompiled by Opus. Names are provisional.
// Writes `depth` tabs and then a closing brace line.

struct FileHandle;

extern unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);

// FUNCTION: 0x4acda0
void __stdcall WriteSectionEnd(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
    HAPI_WriteFile(file, "}\n", 2);
}
