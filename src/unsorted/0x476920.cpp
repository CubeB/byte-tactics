// Decompiled by space-bunny-free. Names are provisional.
#include <string.h>

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
int __stdcall FUN_004bc930(const char* path, int flag);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* data);
int __stdcall FUN_004af320(char* path, void* buffer, char* p3, int p4, int p5, int p6);
char* __stdcall FUN_004b6af0(char* text, int n);

// FUNCTION: 0x476920
int __stdcall FUN_00476920(const char* name)
{
    int found;
    char path[256];
    FUN_004290f0(path, "camps", "*", "TDF");
    int count = FUN_004bc930(path, 0);
    char* names = (char*)FUN_004d83b0("CAMPAIGN NAMES", count << 8);
    FUN_004af320(path, names, 0, 0, 1, 2);
    found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(name, FUN_004b6af0(names, i)) == 0) {
            found = 1;
        }
    }
    FUN_004d85a0(names);
    return found;
}
