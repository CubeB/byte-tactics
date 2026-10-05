// Decompiled by Haiku. Names are provisional.

extern char* GetDisplay();
extern int __cdecl _chdir(const char*);

// FUNCTION: 0x4bcee0
void RestoreStartDirectory() {
    char* p = GetDisplay();
    _chdir((const char*)(((int)p) + 0x628));
}
