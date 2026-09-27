// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_00489a70 {
public:
    char unknown_0[0x8a];
    int field_8a;

    int FUN_00489a70();
};
#pragma pack(pop)

// FUNCTION: 0x489a70
int Class_00489a70::FUN_00489a70() {
    int count = 0;
    int node = field_8a;
    while (node != 0) {
        if (*(int*)((char*)node + 0x86) == (int)this) {
            count++;
        }
        node = *(int*)((char*)node + 0x8e);
    }
    return count;
}
