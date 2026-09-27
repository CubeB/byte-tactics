// Decompiled by Opus. Names are provisional.
// Stores a DWORD value (type 4) through FUN_004b6880, compare 0x4b6a20.

extern int __stdcall FUN_004b6880(void*, void*, void*, void*, int, int);

// FUNCTION: 0x4b6a50
void __stdcall FUN_004b6a50(char* key, char* name, int value)
{
    unsigned int size = 4;
    FUN_004b6880(key, name, &value, &size, 4, 0);
}
