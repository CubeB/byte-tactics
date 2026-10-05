// Decompiled by Opus. Names are provisional.
// Copy constructor of a {string handle, int} record.

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class UnitCategory {
public:
    Class_004c91a0 handle;
    int field_4;

    UnitCategory(const UnitCategory& other);
};

// FUNCTION: 0x489260
UnitCategory::UnitCategory(const UnitCategory& other)
    : handle(other.handle), field_4(other.field_4)
{
}
