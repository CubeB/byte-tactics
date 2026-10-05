// Decompiled by deepseek-v4.1-flash. Names are provisional.

class Mission {
public:
    int FUN_00435100();
};

void __stdcall FUN_00491c80(int n);
void __stdcall OpenReportDialog(unsigned int* a, unsigned int* b);
void __stdcall OpenMessageBox(void* p, char* text, int a, int b, int c);
void __stdcall LoadPictureCached(char* name, int a, int b, int c);
void __stdcall RunWhileScreenNamed(void* p, char* name);
int __stdcall FUN_0046bf30(unsigned int* a, unsigned int* b);
char* __stdcall Translate(char* text);
void __stdcall SetOffscreenSurface(int x);

extern char* g_game;
extern unsigned int DAT_005054a8;
extern unsigned int DAT_00512788;

// FUNCTION: 0x4436e0
int InitScoreReporting(void)
{
    if ((*(Mission**)(g_game + 0x391e9))->FUN_00435100() != 3)
        return 0;
    int saved = *(signed char*)(g_game + 0x2cbe);
    FUN_00491c80(0x14);
    int r = FUN_0046bf30(&DAT_005054a8, &DAT_00512788);
    if (r == 0) {
        if (DAT_005054a8 > 0) {
            FUN_00491c80(0x13);
            OpenReportDialog(&DAT_005054a8, &DAT_00512788);
            FUN_00491c80(saved);
            return 1;
        }
    } else if (r != 4) {
        SetOffscreenSurface(*(int*)(g_game + 0x37e1b));
        OpenMessageBox(g_game + 0x519, Translate("Unable to initialize scores reporting."), 0x190, 1, 0);
        LoadPictureCached("ReportError", 0, 1, 0);
        RunWhileScreenNamed(g_game + 0x519, "MSGBOX.GUI");
    }
    FUN_00491c80(saved);
    return 0;
}
