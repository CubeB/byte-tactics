// Decompiled by Haiku. Names are provisional.

extern void* g_game;

class Class_004b73e0 {
public:
    unsigned char FUN_004b73e0(int arg1, int arg2);
};

extern void SaveSettings(void);

// FUNCTION: 0x416cf0
void __stdcall CmdScrollSpeed(void* param_1)
{
    unsigned char result = ((Class_004b73e0*)param_1)->FUN_004b73e0(1, 0);
    *(unsigned char*)((char*)g_game + 0x1434d) = result;
    SaveSettings();
}
