// Decompiled by Haiku. Names are provisional.

extern void* g_game;

class Class_00471d70 {
public:
    char unknown_0[4];
    int field_4;

    void SetLifetime(int param_1);
};

// FUNCTION: 0x471d70
void Class_00471d70::SetLifetime(int param_1)
{
    int edx = *(int*)((char*)g_game + 0x38a47);
    edx += param_1;
    field_4 = edx;
}
