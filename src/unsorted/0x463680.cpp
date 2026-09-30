// Decompiled by Sonnet. Names are provisional.

extern void __cdecl operator delete(void* p);

class Class_00463680 {
public:
    char unknown_0[0x14];
    void* field_14;                    // +0x14
    char unknown_18[0x24 - 0x18];
    void* field_24;                    // +0x24
    void* field_28;                    // +0x28
    ~Class_00463680();
};

// FUNCTION: 0x463680
Class_00463680::~Class_00463680()
{
    operator delete(field_14);
    operator delete(field_28);
    operator delete(field_24);
}
