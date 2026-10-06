// Decompiled by Haiku, gathered by DeepSeek V4.1 Flash. Names are provisional.

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
