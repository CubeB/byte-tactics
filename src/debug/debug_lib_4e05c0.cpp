// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK MemoryStatusDlgProc(HWND, UINT, WPARAM, LPARAM);

extern char DAT_0050d6b4[]; // "Performance dialog failed to open"
extern char DAT_0050c8ac[]; // "Cavedog"

class Class_004e05c0 {
public:
    void CreateMemoryStatusDialog();
};

// FUNCTION: 0x4e05c0
void Class_004e05c0::CreateMemoryStatusDialog()
{
    HWND result = CreateDialogFromTemplate(0x66, GetDesktopWindow(), (DLGPROC)MemoryStatusDlgProc, (LPARAM)this);
    if (result == 0)
        MessageBoxA(0, DAT_0050d6b4, DAT_0050c8ac, 0);
}
