// Decompiled by Opus. Names are provisional.
#include <windows.h>

int FUN_004d9f50(void);
HGLOBAL __cdecl FUN_004da480(int id);

// FUNCTION: 0x4da4f0
int __cdecl FUN_004da4f0(int id, HWND parent, DLGPROC proc, LPARAM param)
{
    HGLOBAL mem = FUN_004da480(id);
    if (mem == 0)
        return 0;
    int result = DialogBoxIndirectParamA((HINSTANCE)FUN_004d9f50(), (LPCDLGTEMPLATEA)mem, parent, proc, param);
    GlobalFree(mem);
    return result;
}
