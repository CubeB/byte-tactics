// Decompiled by Haiku. Names are provisional.

extern void __cdecl ReportException(int, const char*);
extern const char DAT_0050d2d0[];

// FUNCTION: 0x4da2a0
void __stdcall UnhandledExceptionHandler(int param_1)
{
    ReportException(param_1, DAT_0050d2d0);
}
