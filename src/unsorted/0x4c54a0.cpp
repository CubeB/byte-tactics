// Decompiled by Haiku. Names are provisional.
// Copy constructor of a pair of reference-counted string handles.

class Class_004c91a0 {
public:
    char* ptr;

    Class_004c91a0(const Class_004c91a0& other);
};

class Class_004c54a0 {
public:
    Class_004c91a0 first;    // +0x0
    Class_004c91a0 second;   // +0x4

    Class_004c54a0(const Class_004c54a0& other);
};

// FUNCTION: 0x4c54a0
Class_004c54a0::Class_004c54a0(const Class_004c54a0& other)
    : first(other.first), second(other.second)
{
}
