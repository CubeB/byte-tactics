// Decompiled by Sonnet. Names are provisional.

void __stdcall RemoveTimer(int param_1);

class Class_004d02a0 {
public:
    char unknown_0[0x288];
    int field288;                 // +0x288
    void OpenSample(char* param_1, int param_2, int param_3, int param_4);
};

extern Class_004d02a0* g_cdPlayer;
extern int DAT_0051ff58;
extern char DAT_0051ff60[];

// FUNCTION: 0x4d0680
void __stdcall OnStreamTimer(int unused1)
{
    RemoveTimer(g_cdPlayer->field288);
    g_cdPlayer->field288 = -1;
    g_cdPlayer->OpenSample(DAT_0051ff60, 2, DAT_0051ff58, 0);
}
