// Decompiled by Sonnet. Names are provisional.

extern char DAT_0050341c[]; // "TDF"
extern char DAT_0050372c[]; // "*"
extern char DAT_00504a64[]; // "camps"

extern void __stdcall BuildDataPath(char* buf, const char* name, const char* p2, const char* p3);
extern int __stdcall CountDirectoryEntries(const char* path, int flag);

// FUNCTION: 0x4774d0
int FUN_004774d0(void)
{
    char local_100[256];
    BuildDataPath(local_100, DAT_00504a64, DAT_0050372c, DAT_0050341c);
    int n = CountDirectoryEntries(local_100, 0);
    return n <= 2;
}
