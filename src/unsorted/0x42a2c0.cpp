// Decompiled by Opus. Names are provisional.
// Loads objects3d\<name>.3DO: aborts with the path if the file is missing,
// then prepares the loaded model.

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall FUN_004cb560(char* param);
void __stdcall FUN_004b6290(char* path);
void __stdcall FUN_004cb590(void* data);
void __stdcall FUN_0042a140(void* data, const char* name);

// FUNCTION: 0x42a2c0
void* __stdcall FUN_0042a2c0(const char* name)
{
    char path[256];
    FUN_004290f0(path, "objects3d", name, "3DO");
    void* data = FUN_004cb560(path);
    if (data == 0) {
        FUN_004b6290(path);
    }
    FUN_004cb590(data);
    FUN_0042a140(data, name);
    return data;
}
