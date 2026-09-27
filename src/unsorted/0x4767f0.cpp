// Decompiled by Opus. Names are provisional.

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bc930(const char* path, int flag);

// Counts the campaign files (camps\*.TDF); compare 0x4769f0.
// FUNCTION: 0x4767f0
void FUN_004767f0()
{
    char path[0x100];
    FUN_004290f0(path, "camps", "*", "TDF");
    FUN_004bc930(path, 0);
}
