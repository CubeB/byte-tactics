// Decompiled by Haiku. Names are provisional.

extern void* g_game;
extern void SaveSettings();

class Class_004b73e0 {
public:
    void* FUN_004b73e0(int a, int b);
};

// FUNCTION: 0x416d20
void __stdcall CmdIFace(void* param_1)
{
    void* result = ((Class_004b73e0*)param_1)->FUN_004b73e0(1, 0);
    void* g = g_game;
    *(void**)((char*)g + 0x37efa) = result;
    SaveSettings();
}
