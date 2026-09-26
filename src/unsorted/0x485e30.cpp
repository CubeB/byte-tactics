// Decompiled by Haiku. Names are provisional.

extern void operator delete(void* p);

class Class_004b06f0 {
public:
    void FUN_004b06f0();
};

class Class_00485e30 {
public:
    void* FUN_00485e30(unsigned char flag);
};

// FUNCTION: 0x485e30
void* Class_00485e30::FUN_00485e30(unsigned char flag)
{
    ((Class_004b06f0*)this)->FUN_004b06f0();
    if (flag & 1) {
        operator delete(this);
    }
    return this;
}
