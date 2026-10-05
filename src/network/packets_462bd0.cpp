// Decompiled by Haiku. Names are provisional.

class Class_00462ae0;

#pragma pack(push, 1)
class Class_00462bd0 {
public:
    char unknown_0[0xc];
    void* field_c;

    void FUN_00462bd0();
};
#pragma pack(pop)

// Forward declare the method to be called
class Class_00462ae0 {
public:
    void FUN_00462ae0(void* param);
};

// FUNCTION: 0x462bd0
void Class_00462bd0::FUN_00462bd0()
{
    ((Class_00462ae0*)field_c)->FUN_00462ae0(this);
    field_c = 0;
}
