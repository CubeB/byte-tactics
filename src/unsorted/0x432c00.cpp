// Decompiled by Haiku. Names are provisional.

extern void operator delete(void*);

class Class_004c9390 {
public:
    void FUN_004c9390();
};

class Class_00432c00 {
public:
    void* FUN_00432c00(unsigned char param_1);
};

// FUNCTION: 0x432c00
void* Class_00432c00::FUN_00432c00(unsigned char param_1)
{
    ((Class_004c9390*)this)->FUN_004c9390();
    if ((param_1 & 1) != 0) {
        operator delete(this);
    }
    return this;
}
