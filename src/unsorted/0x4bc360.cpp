// Decompiled by Haiku. Names are provisional.
extern int _chdir(const char*);

// FUNCTION: 0x4bc360
void __stdcall FUN_004bc360(const char* param_1, int unused)
{
    _chdir(param_1);
}
