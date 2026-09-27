// Decompiled by Haiku. Names are provisional.

extern void* DAT_0051e690;
extern char* g_game;

class Class_004d06c0 {
public:
    void FUN_004d06c0(char* param_1, int param_2, int param_3);
};

// FUNCTION: 0x47f090
void __stdcall FUN_0047f090(char* param_1, int param_2, int param_3) {
    if (DAT_0051e690 == 0) {
        void* game_obj = *(void**)g_game;
        Class_004d06c0* obj = (Class_004d06c0*)((char*)game_obj + 0x10);
        obj->FUN_004d06c0(param_1, param_2, param_3);
    }
}
