// Decompiled by Haiku, rewritten by Opus. Names are provisional.
// Copy constructor of a {string handle, int} pair.

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class Class_00437820 {
public:
    Class_004c91a0 handle;
    int field_4;

    Class_00437820(const Class_00437820& other);
};

// FUNCTION: 0x437820
Class_00437820::Class_00437820(const Class_00437820& other)
    : handle(other.handle), field_4(other.field_4)
{
}
