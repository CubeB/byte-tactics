// Decompiled by Opus. Names are provisional.
// Reads a registry value through AccessRegistryValue (see ReadGameRegistryValue).

int __stdcall AccessRegistryValue(void* app, void* key, void* buf, void* size, int a5, int a6);

// FUNCTION: 0x4b6860
int __stdcall ReadRegistryValue(const char* app, const char* key, void* buf, unsigned int* size)
{
    return AccessRegistryValue((void*)app, (void*)key, buf, size, 0, 1);
}
