// Decompiled by Sonnet. Names are provisional.

class Class_00470eb0
{
public:
    char unknown_0[0x14];
    void* field_14;
    char unknown_18[0x4];
    int field_1c;
    int field_20;

    int AllocSlot(int unused);
};

// FUNCTION: 0x470eb0
int Class_00470eb0::AllocSlot(int unused)
{
    int edx = field_20;
    int esi = field_1c;
    int eax = 0;

    if (edx < esi) {
        void* ptr = field_14;
        edx++;
        eax = *(int*)((char*)ptr + edx * 4 - 4);
        field_20 = edx;
    }

    return eax;
}
