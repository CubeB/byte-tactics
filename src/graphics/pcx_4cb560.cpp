// Decompiled by Haiku. Names are provisional.

extern void* __stdcall HAPI_LoadFile(void*, int);
extern void* __stdcall RelocateObject(void*, void*);

// FUNCTION: 0x4cb560
void* __stdcall Load3do(char* param)
{
    void* result = HAPI_LoadFile(param, 0);
    if (result == 0) {
        return 0;
    }
    RelocateObject(result, result);
    return result;
}
