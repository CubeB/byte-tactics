// Decompiled by Opus. Names are provisional.
// Opens a file in "a+b" mode through HAPI_OpenFile (sibling of 0x4bb5b0).

extern void __stdcall HAPI_OpenFile(void* param1, const char* param2);

// FUNCTION: 0x4bb2c0
void __stdcall HAPI_OpenFileAppend(void* param1)
{
    HAPI_OpenFile(param1, "a+b");
}
