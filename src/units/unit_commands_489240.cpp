// Decompiled by Opus. Names are provisional.
// Assignment of a record holding a reference-counted string handle (assigned
// by 0x4c93b0) and an int.

struct Class_004c93b0 {
    char* ptr;

    Class_004c93b0* Assign(Class_004c93b0* param_1);
};

class Class_00489240 {
public:
    Class_004c93b0 name;               // +0x0
    int value;                         // +0x4

    Class_00489240* FUN_00489240(Class_00489240* other);
};

// FUNCTION: 0x489240
Class_00489240* Class_00489240::FUN_00489240(Class_00489240* other)
{
    name.Assign(&other->name);
    value = other->value;
    return this;
}
