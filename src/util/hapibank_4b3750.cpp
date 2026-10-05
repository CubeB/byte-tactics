// Decompiled by Haiku. Names are provisional.

class Class_004b3630 {
public:
    void CloseBank(void);
};

extern void* __cdecl GameCalloc(int, int);

class Class_004b3750 {
public:
    void* field_0;
    char unknown_4[4];
    int field_8;

    void NewBank(void);
};

// FUNCTION: 0x4b3750
void Class_004b3750::NewBank(void)
{
    ((Class_004b3630*)this)->CloseBank();
    void* result = GameCalloc(1, 0xc);
    field_0 = result;
    ((int*)result)[2] = -1;
}
