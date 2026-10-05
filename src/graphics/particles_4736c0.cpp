// Decompiled by Haiku. Names are provisional.

class Class_004736c0 {
public:
    char unknown_0[0x30];
    int field_30;

    int IsExpired(int param_1);
};

// FUNCTION: 0x4736c0
int Class_004736c0::IsExpired(int param_1) {
    return param_1 > field_30;
}
