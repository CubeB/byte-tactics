// Decompiled by Opus. Names are provisional.
// Returns 1 when camps\<name>.TDF can be opened (the file is closed again).

struct File_004bb5d0;

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
File_004bb5d0* __stdcall FUN_004bb2c0(char* path);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);

// FUNCTION: 0x4352d0
int __stdcall FUN_004352d0(char* name)
{
    char path[256];
    FUN_004290f0(path, "camps", name, "TDF");
    File_004bb5d0* file = FUN_004bb2c0(path);
    if (file) {
        FUN_004bb5d0(file);
        return 1;
    }
    return 0;
}
