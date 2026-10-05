// Decompiled by Opus. Names are provisional.
// Lazily creates the global NameTable object, allocated with
// FUN_004e1a80 (a GlobalAlloc wrapper), and returns it. It is a placement
// new into that block: the constructor is 0x4e17c0.

inline void* __cdecl operator new(unsigned int, void* p) { return p; }

class NameTable {
public:
    char unknown_0[0x14];
    NameTable();
};

void* __cdecl FUN_004e1a80(unsigned int size);

extern NameTable* DAT_00529e7c;

// FUNCTION: 0x4e1a90
NameTable* GetNameTable()
{
    if (DAT_00529e7c == 0)
        DAT_00529e7c = new (FUN_004e1a80(sizeof(NameTable))) NameTable;
    return DAT_00529e7c;
}
