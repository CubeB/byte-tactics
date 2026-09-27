// Decompiled by Haiku. Names are provisional.

struct Class_00433b00 {
    char unknown_0[4];
    int _First;
    int _Last;
};

// FUNCTION: 0x433b00
int __fastcall FUN_00433b00(Class_00433b00* obj)
{
    if (obj->_First == 0) {
        return 0;
    }
    return (obj->_Last - obj->_First) >> 4;
}
