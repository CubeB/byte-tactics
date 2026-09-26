// Decompiled by Haiku. Names are provisional.
extern void __stdcall FUN_00437a30(void*, int);

struct Class_00437c80 {
public:
    int field_0;
    char unknown_4[8];
    int field_c;

    void FUN_00437c80();
};

// FUNCTION: 0x437c80
void Class_00437c80::FUN_00437c80()
{
    FUN_00437a30((void*)((char*)this + 0xc), field_0 - 8);
}
