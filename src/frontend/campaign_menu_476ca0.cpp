// Decompiled by Opus. Names are provisional.
// Sibling of 0x476c70, with name slot 8 instead of 3.

extern char* g_game;

class Class_004356c0 {
public:
    int FUN_004356c0(int param_1);
};

void __stdcall FUN_0047f090(char* param_1, int param_2, int param_3);

// FUNCTION: 0x476ca0
void FUN_00476ca0()
{
    if (*(int*)(g_game + 0x391f1) != 6) {
        char* name = (char*)(*(Class_004356c0**)(g_game + 0x391e9))->FUN_004356c0(8);
        if (name) {
            FUN_0047f090(name, 0, 0x3c);
        }
    }
}
