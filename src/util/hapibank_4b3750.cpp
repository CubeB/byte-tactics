// Decompiled by Haiku. Names are provisional.

extern void* __cdecl GameCalloc(int, int);

class HapiBank {
public:
    void* field_0;
    char unknown_4[4];
    int field_8;

    void NewBank(void);
    void CloseBank(void);
};

// FUNCTION: 0x4b3750
void HapiBank::NewBank(void)
{
    ((HapiBank*)this)->CloseBank();
    void* result = GameCalloc(1, 0xc);
    field_0 = result;
    ((int*)result)[2] = -1;
}
