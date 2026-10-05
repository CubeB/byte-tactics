// Decompiled by Haiku. Names are provisional.

// The timer's elapsed-time getter (0x4e1e30), a method on the same object.
class Class_004e1e30 {
public:
    double FUN_004e1e30();
};

struct Class_004e2150 {
public:
    double field_0;
    char unknown_8[0x40];
    unsigned char field_48;
    
    void FUN_004e2150();
};

// FUNCTION: 0x4e2150
void Class_004e2150::FUN_004e2150()
{
    field_0 = ((Class_004e1e30*)this)->FUN_004e1e30();
    field_48 = 1;
}
