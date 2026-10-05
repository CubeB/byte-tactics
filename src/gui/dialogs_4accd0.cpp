// Decompiled by Opus. Names are provisional.
// Writes `depth` tab characters to a file (sibling of 0x4acda0).

struct Class_004bbbe0;

extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* data, unsigned int size);

// FUNCTION: 0x4accd0
void __stdcall WriteTabs(Class_004bbbe0* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        FUN_004bbbe0(file, &tab, 1);
}
