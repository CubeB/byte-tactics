// Decompiled by Haiku. Names are provisional.

class CMemoryCache {
public:
    void FreeBuffer();
};

extern CMemoryCache DAT_00511f80;

// FUNCTION: 0x420610
void FUN_00420610()
{
    DAT_00511f80.FreeBuffer();
}
