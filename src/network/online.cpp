// Decompiled by Haiku, gathered by DeepSeek V4.1 Flash. Names are provisional.
// The online.dll glue. 0x45b250, 0x45b490 and 0x45b670 are gap code
// (data/functions.csv), so they stay in online_45b250.cpp, online_45b490.cpp
// and online_45b670.cpp: tools/gapcheck.py sizes a gap region by the file it
// is in.

#include <windows.h>

extern int g_onlineConfigLoaded;
extern HMODULE g_onlineDll;

void __stdcall Translate(unsigned char* param_1);

// FUNCTION: 0x45b480
void __stdcall OnlineTranslate(unsigned char* param_1)
{
    Translate(param_1);
}

// FUNCTION: 0x45b640
void OnlineUnload(void)
{
    if (g_onlineDll != 0) {
        FreeLibrary(g_onlineDll);
        g_onlineDll = 0;
    }
}

// FUNCTION: 0x45b660
int IsOnlineConfigLoaded(void)
{
    return g_onlineConfigLoaded;
}
