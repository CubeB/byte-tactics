// Decompiled by Sonnet. Names are provisional.
// The `push ecx` reserves a 4-byte local (guide: "push ecx as the first
// instruction usually just reserves stack space"); the compiler never
// initialises it before its one byte is copied into field_0.

class Class_00410830 {
public:
    unsigned char field_0;
    int field_4;
    int field_8;
    int field_c;

    Class_00410830();
};

// FUNCTION: 0x410830
Class_00410830::Class_00410830()
{
    unsigned char local;
    field_0 = local;
    field_4 = 0;
    field_8 = 0;
    field_c = 0;
}
