// Decompiled by Opus. Names are provisional.

class Class_0044f940 {
public:
    void FUN_0044f940(void* data, int size);
};

class Class_0044fc10 {
public:
    int FUN_0044fc10(void* session, int from);
};

#pragma pack(push, 1)
class Class_004626e0 {
public:
    char unknown_0[0x21];
    int field_21;                      // +0x21

    void FUN_004626e0(void* session, int from, int value, void* data, int size);
};
#pragma pack(pop)

// FUNCTION: 0x4626e0
void Class_004626e0::FUN_004626e0(void* session, int from, int value, void* data, int size)
{
    field_21 = value;
    ((Class_0044f940*)this)->FUN_0044f940(data, size);
    ((Class_0044fc10*)this)->FUN_0044fc10(session, from);
}
