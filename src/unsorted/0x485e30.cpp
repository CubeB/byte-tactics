// Decompiled by Haiku. Names are provisional.
// The scalar deleting destructor of a Class_004b06f0 subclass: run the
// destructor, then free the object if bit 0 of the flag is set.

extern void operator delete(void* p);

class Class_004b06f0 {
public:
    ~Class_004b06f0();
};

class Class_00485e30 {
public:
    void* FUN_00485e30(unsigned char flag);
};

// FUNCTION: 0x485e30
void* Class_00485e30::FUN_00485e30(unsigned char flag)
{
    ((Class_004b06f0*)this)->~Class_004b06f0();
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}
