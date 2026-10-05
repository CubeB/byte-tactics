// Decompiled by Opus. Names are provisional.
#include <windows.h>

int GetDebugLibInstance(void);
HGLOBAL __cdecl GetDialogTemplate(int id);

// FUNCTION: 0x4da540
HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param)
{
    HGLOBAL mem = GetDialogTemplate(id);
    if (mem == 0)
        return 0;
    HWND result = CreateDialogIndirectParamA((HINSTANCE)GetDebugLibInstance(), (LPCDLGTEMPLATEA)mem, parent, proc, param);
    GlobalFree(mem);
    return result;
}
