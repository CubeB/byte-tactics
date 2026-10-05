// Decompiled by Opus. Names are provisional.
// Returns 1 when camps\<name>.TDF can be opened (the file is closed again).

struct FileHandle;

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
FileHandle* __stdcall HAPI_OpenFileAppend(char* path);
int __stdcall HAPI_CloseFile(FileHandle* file);

// FUNCTION: 0x4352d0
int __stdcall CampaignFileExists(char* name)
{
    char path[256];
    BuildDataPath(path, "camps", name, "TDF");
    FileHandle* file = HAPI_OpenFileAppend(path);
    if (file) {
        HAPI_CloseFile(file);
        return 1;
    }
    return 0;
}
