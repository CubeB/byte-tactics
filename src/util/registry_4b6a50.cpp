// Decompiled by Opus. Names are provisional.
// Stores a DWORD value (type 4) through AccessRegistryValue, compare 0x4b6a20.

extern int __stdcall AccessRegistryValue(void*, void*, void*, void*, int, int);

// FUNCTION: 0x4b6a50
void __stdcall WriteRegistryDword(char* key, char* name, int value)
{
    unsigned int size = 4;
    AccessRegistryValue(key, name, &value, &size, 4, 0);
}
