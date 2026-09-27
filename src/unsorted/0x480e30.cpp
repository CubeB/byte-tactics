// Decompiled by Haiku. Names are provisional.

class Class_00480e30 {
public:
    char unknown_0[0x540];
    void* buffer;

    int FUN_00480e30(int param_1);
};

// FUNCTION: 0x480e30
int Class_00480e30::FUN_00480e30(int param_1)
{
    unsigned char* buf = (unsigned char*)buffer;
    int idx = param_1;
    idx = idx + idx * 2;
    idx = idx + idx * 8;
    unsigned char val = buf[idx * 2 + 0x4a];
    return val & 1;
}
