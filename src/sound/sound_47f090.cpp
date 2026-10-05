// Decompiled by Sonnet. Names are provisional.

extern void* g_noDirectSound;
extern char* g_game;

class Class_004d06c0 {
public:
    void StreamSampleDelayed(char* param_1, int param_2, int param_3);
};

// FUNCTION: 0x47f090
void __stdcall StreamSoundDelayed(char* param_1, int param_2, int param_3)
{
    if (g_noDirectSound == 0) {
        Class_004d06c0* obj = *(Class_004d06c0**)((char*)g_game + 0x10);
        obj->StreamSampleDelayed(param_1, param_2, param_3);
    }
}
