// Decompiled by Haiku. Names are provisional.

extern void __stdcall HAPI_OpenFile(void* param1, const char* param2);

// FUNCTION: 0x4bb5b0
void __stdcall HAPI_OpenFileRead(void* param1)
{
    HAPI_OpenFile(param1, "rb");
}
