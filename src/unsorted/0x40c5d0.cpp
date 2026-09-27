// Decompiled by Haiku. Names are provisional.

extern void FUN_004b4f20(void*);

class Class_0040c5d0 {
public:
    char unknown_0[4];
    void* field_4;
    int field_8;
    int field_c;

    void FUN_0040c5d0();
};

// FUNCTION: 0x40c5d0
void Class_0040c5d0::FUN_0040c5d0() {
    void* ptr = field_4;
    FUN_004b4f20(ptr);
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
}
