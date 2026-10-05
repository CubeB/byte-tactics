// Decompiled by Opus. Names are provisional.

void __cdecl FUN_004d85a0(int* param_1);

class CMemoryCache {
public:
    char unknown_0[4];
    int* field_4;                      // +0x4

    void FreeCache();
};

// FUNCTION: 0x437a00
void CMemoryCache::FreeCache()
{
    if (field_4 != 0) {
        FUN_004d85a0(field_4);
        field_4 = 0;
    }
}
