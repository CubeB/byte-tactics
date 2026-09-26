// Decompiled by Haiku. Names are provisional.

class Class_00472d30 {
public:
    char unknown_0[4];
    int* begin;
    int* end;

    int FUN_00472d30();
};

// FUNCTION: 0x472d30
int Class_00472d30::FUN_00472d30()
{
    if (!begin) {
        return 0;
    }
    return end - begin;
}
