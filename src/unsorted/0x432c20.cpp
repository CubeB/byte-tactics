// Decompiled by Haiku. Names are provisional.

extern void FUN_004c9390(void*);
extern void FUN_004b4f20(void*);

class Class_00432c20 {
public:
    void* FUN_00432c20(unsigned char param);
};

// FUNCTION: 0x432c20
void* Class_00432c20::FUN_00432c20(unsigned char param) {
    FUN_004c9390(this);
    if (param & 1) {
        FUN_004b4f20(this);
    }
    return this;
}
