// Decompiled by Opus. Names are provisional.
#include <io.h>

// A directory search: a buffer followed by the _findfirst state.
#pragma pack(push, 1)
struct FindFiles {
    char unknown_0[0x200];
    int state;                          // +0x200 (negative while a _findfirst handle is open)
    char unknown_204;
    long handle;                        // +0x205
};
#pragma pack(pop)

void __cdecl FUN_004d85a0(int* param_1);

// Closes the search handle (if open) and frees the search; -1 for no search.
// FUNCTION: 0x4bc8d0
int __stdcall HAPI_FindClose(FindFiles* f)
{
    if (f == (FindFiles*)-1 || f == 0)
        return -1;
    int result;
    if (f->state < 0)
        result = _findclose(f->handle);
    else
        result = 0;
    FUN_004d85a0((int*)f);
    return result;
}
