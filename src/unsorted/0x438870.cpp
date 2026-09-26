// Decompiled by Haiku. Names are provisional.

struct Class_00438870 {
    char unknown_0[0x4c];
    unsigned short unknown_0x4c;
    int field_0x4e;

    void FUN_00438870(unsigned int param_1);
};

// FUNCTION: 0x438870
void Class_00438870::FUN_00438870(unsigned int param_1)
{
    int* ptr = (int*)((char*)this + 0x4e);
    *ptr |= param_1;
}
