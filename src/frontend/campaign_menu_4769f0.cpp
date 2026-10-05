// Decompiled by Opus. Names are provisional.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bc930(const char* path, int flag);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall ScanDirectory(char* path, void* buffer, char* p3, int p4, int p5, int p6);

// Lists the campaign files (camps\*.TDF) into a buffer of 256-byte names and
// returns how many there are.
// FUNCTION: 0x4769f0
int __stdcall ListCampaignFiles(void** names)
{
    char path[0x100];
    BuildDataPath(path, "camps", "*", "TDF");
    int count = FUN_004bc930(path, 0);
    void* buffer = FUN_004d83b0("CAMPAIGN NAMES", count << 8);
    *names = buffer;
    ScanDirectory(path, buffer, 0, 0, 1, 2);
    return count;
}
