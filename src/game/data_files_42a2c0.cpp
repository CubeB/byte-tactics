// Decompiled by Opus. Names are provisional.
// Loads objects3d\<name>.3DO: aborts with the path if the file is missing,
// then prepares the loaded model.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void* __stdcall Load3do(char* param);
void __stdcall FatalError(char* path);
void __stdcall MirrorObject(void* data);
void __stdcall FUN_0042a140(void* data, const char* name);

// FUNCTION: 0x42a2c0
void* __stdcall LoadObject3d(const char* name)
{
    char path[256];
    BuildDataPath(path, "objects3d", name, "3DO");
    void* data = Load3do(path);
    if (data == 0) {
        FatalError(path);
    }
    MirrorObject(data);
    FUN_0042a140(data, name);
    return data;
}
