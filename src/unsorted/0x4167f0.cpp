// Decompiled by Haiku. Names are provisional.

extern void* g_game;

class Class_004b73e0 {
public:
    int FUN_004b73e0(int, int);
};

extern void FUN_004ceb60(void*, int);

// FUNCTION: 0x4167f0
void __stdcall FUN_004167f0(void* param_1)
{
    Class_004b73e0* obj = (Class_004b73e0*)param_1;
    int result = obj->FUN_004b73e0(1, 0);

    void* p_game = g_game;
    void* ptr = *(void**)((char*)p_game + 0x10);
    FUN_004ceb60(ptr, result);
}
