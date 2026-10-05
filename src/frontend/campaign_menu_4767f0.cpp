// Decompiled by Opus. Names are provisional.

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
int __stdcall CountDirectoryEntries(const char* path, int flag);

// Counts the campaign files (camps\*.TDF); compare 0x4769f0.
// FUNCTION: 0x4767f0
void CountCampaignFiles()
{
    char path[0x100];
    BuildDataPath(path, "camps", "*", "TDF");
    CountDirectoryEntries(path, 0);
}
