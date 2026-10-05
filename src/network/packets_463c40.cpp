// Decompiled by Haiku. Names are provisional.

void __cdecl operator delete(void* p);

#pragma pack(push, 1)
class Class_00463c40 {
public:
    char unknown_0[0x27];
    void* field_27;
    char unknown_2b[0x51];
    void* field_7c;

    void FUN_00463c40();
};
#pragma pack(pop)

// FUNCTION: 0x463c40
void Class_00463c40::FUN_00463c40()
{
    delete field_27;
    delete field_7c;
}
