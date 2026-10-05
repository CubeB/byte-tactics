// Decompiled by Opus. Names are provisional.
// Writes a value under the game's registry key (see WriteRegistryBinary); the
// sibling of ReadGameRegistryValue.

void __stdcall WriteRegistryBinary(void* param_1, void* param_2, void* param_3, int unused);

// FUNCTION: 0x42f960
void __stdcall WriteGameRegistryValue(void* key, void* buf, int value)
{
    WriteRegistryBinary("Total Annihilation", key, buf, value);
}
