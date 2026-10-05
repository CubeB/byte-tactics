// Decompiled by Sonnet. Names are provisional.

void __cdecl WalkFrameChain(int* param_1, int* param_2, int param_3, int param_4,
                           void* this_, int flag, int* field_78, int* field_7c,
                           int size, int* field_207c);

class Class_004d9c60 {
public:
    char pad0[0x78];
    int field_78;
    int field_7c;
    char pad1[0x207c - 0x80];
    int field_207c;
    int* field_2080;

    void CaptureStack(int* param_1, int* param_2, int param_3, int param_4);
};

// FUNCTION: 0x4d9c60
void Class_004d9c60::CaptureStack(int* param_1, int* param_2, int param_3, int param_4)
{
    field_2080 = param_2;
    WalkFrameChain(param_1, param_2, param_3, param_4, this, 0x1e, &field_78, &field_7c, 0x800, &field_207c);
}
