// Decompiled by Haiku. Names are provisional.

class Class_00462ae0;

#pragma pack(push, 1)
class Class_00462bd0 {
public:
    char unknown_0[0xc];
    void* field_c;

    void RemoveFromBuffer();
};
#pragma pack(pop)

// Forward declare the method to be called
class Class_00462ae0 {
public:
    void RemovePacket(void* param);
};

// FUNCTION: 0x462bd0
void Class_00462bd0::RemoveFromBuffer()
{
    ((Class_00462ae0*)field_c)->RemovePacket(this);
    field_c = 0;
}
