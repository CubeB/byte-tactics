// Decompiled by Haiku. Names are provisional.

class Class_004b3630 {
public:
    void FUN_004b3630(void);
};

extern void* __cdecl FUN_004d8460(int, int);

class Class_004b3750 {
public:
    void* field_0;
    char unknown_4[4];
    int field_8;

    void FUN_004b3750(void);
};

// FUNCTION: 0x4b3750
void Class_004b3750::FUN_004b3750(void)
{
    ((Class_004b3630*)this)->FUN_004b3630();
    void* result = FUN_004d8460(1, 0xc);
    field_0 = result;
    ((int*)result)[2] = -1;
}
