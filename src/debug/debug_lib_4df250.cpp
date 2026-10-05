// Decompiled by Sonnet. Names are provisional.
#include <windows.h>

HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK PerformanceDlgProc(HWND, UINT, WPARAM, LPARAM);

class PerformanceDialog {
public:
    void CreatePerformanceDialog(void);
};

// FUNCTION: 0x4df250
void PerformanceDialog::CreatePerformanceDialog(void)
{
    if (CreateDialogFromTemplate(0x67, GetDesktopWindow(), (DLGPROC)PerformanceDlgProc, (LPARAM)this) == 0) {
        MessageBoxA(0, "Performance dialog failed to open", "Cavedog", 0);
    }
}
