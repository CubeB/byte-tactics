// Decompiled by Opus. Names are provisional.
// Writes `depth` tabs and then a closing brace line.

struct Class_004bbbe0;

extern unsigned int __stdcall FUN_004bbbe0(Class_004bbbe0* file, void* data, unsigned int size);

// FUNCTION: 0x4acda0
void __stdcall FUN_004acda0(Class_004bbbe0* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        FUN_004bbbe0(file, &tab, 1);
    FUN_004bbbe0(file, "}\n", 2);
}
