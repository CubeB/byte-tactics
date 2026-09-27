// Decompiled by Haiku. Names are provisional.

extern char* FUN_004b6220();
extern int __cdecl _chdir(const char*);

// FUNCTION: 0x4bcee0
void FUN_004bcee0() {
    char* p = FUN_004b6220();
    _chdir((const char*)(((int)p) + 0x628));
}
