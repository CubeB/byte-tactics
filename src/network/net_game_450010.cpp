// Decompiled by Haiku. Names are provisional.

struct Class_00450010 {
    char unknown_0[4];
    void* field_4;
    char unknown_8[0x6b];
    unsigned char field_73;
};

// FUNCTION: 0x450010
int __stdcall GetPlayerDpid(Class_00450010* obj)
{
    if (obj != 0 && obj->field_73 != 0) {
        return (int)obj->field_4;
    }
    return -1;
}
