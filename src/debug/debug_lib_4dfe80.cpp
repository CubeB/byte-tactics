// Decompiled by Haiku. Names are provisional.

extern void GetPerformanceWindow();
extern void ShowPerformanceStatus();
extern const char* __cdecl FindCommandLineSwitch(const char*);

// FUNCTION: 0x4dfe80
void StartPerformanceStatus() {
    GetPerformanceWindow();
    const char* result = FindCommandLineSwitch("-performancestatus");
    if (result != 0) {
        ShowPerformanceStatus();
    }
}
