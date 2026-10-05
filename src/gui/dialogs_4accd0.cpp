// Decompiled by Opus. Names are provisional.
// Writes `depth` tab characters to a file (sibling of 0x4acda0).

struct FileHandle;

extern unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);

// FUNCTION: 0x4accd0
void __stdcall WriteTabs(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
}
