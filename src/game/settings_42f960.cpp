// Decompiled by Opus. Names are provisional.
// Writes a value under the game's registry key (see FUN_004b6a00); the
// sibling of ReadGameRegistryValue.

void __stdcall FUN_004b6a00(void* param_1, void* param_2, void* param_3, int unused);

// FUNCTION: 0x42f960
void __stdcall WriteGameRegistryValue(void* key, void* buf, int value)
{
    FUN_004b6a00("Total Annihilation", key, buf, value);
}
