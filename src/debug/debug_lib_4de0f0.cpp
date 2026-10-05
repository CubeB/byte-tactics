// Decompiled by Haiku. Names are provisional.

class MappedFile {
public:
    void CloseMappedFile();
};

extern MappedFile DAT_00528a78;

// FUNCTION: 0x4de0f0
void FUN_004de0f0()
{
    DAT_00528a78.CloseMappedFile();
}
