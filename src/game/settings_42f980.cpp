// Decompiled by Opus. Names are provisional.
// Reads a value from the game's registry key (see FUN_004b6860/0x4b6880).

int __stdcall FUN_004b6860(const char* app, const char* key, void* buf, unsigned int* size);

// FUNCTION: 0x42f980
int __stdcall ReadGameRegistryValue(const char* key, void* buf, unsigned int* size)
{
    return FUN_004b6860("Total Annihilation", key, buf, size);
}
