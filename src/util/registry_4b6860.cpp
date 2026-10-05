// Decompiled by Opus. Names are provisional.
// Reads a registry value through FUN_004b6880 (see FUN_0042f980).

int __stdcall FUN_004b6880(void* app, void* key, void* buf, void* size, int a5, int a6);

// FUNCTION: 0x4b6860
int __stdcall FUN_004b6860(const char* app, const char* key, void* buf, unsigned int* size)
{
    return FUN_004b6880((void*)app, (void*)key, buf, size, 0, 1);
}
