// Decompiled by Haiku. Names are provisional.

class Class_004b73e0 {
public:
    int FUN_004b73e0(int, int);
};

class Class_004ce690 {
public:
    void SetTrackCategory(int);
};

extern void* g_game;

// FUNCTION: 0x4175e0
void __stdcall CmdMusicMode(void* param_1)
{
    Class_004b73e0* obj = (Class_004b73e0*)param_1;
    int result = obj->FUN_004b73e0(1, 0);
    void* p = *(void**)((char*)g_game + 0x10);
    ((Class_004ce690*)p)->SetTrackCategory(result);
}
