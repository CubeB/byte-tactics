// Decompiled by Opus. Names are provisional.
// Copy constructor of a reference-counted string handle: the handle points at
// character data whose reference count is stored just before it. The
// assignment operator of the same handle is at 0x4c93b0.

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

// FUNCTION: 0x4c91a0
Class_004c91a0::Class_004c91a0(const Class_004c91a0& other)
{
    ptr = other.ptr;
    ((int*)ptr)[-1]++;
}
