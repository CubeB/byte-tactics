// Decompiled by Sonnet. Names are provisional.
// FUN_0046d6c0 reads fields of the same object (offsets +0x14, +0x58, +0x5c,
// +0x64) that this function's "this" belongs to; no `mov ecx` appears before
// the call, so ecx (this) flows through unchanged from FUN_0046d500's own
// thiscall "this" into FUN_0046d6c0's.

class Class_0046d6c0 {
public:
    void FUN_0046d6c0(void* param_1, int param_2);
};

class Class_0046d500 {
public:
    void FUN_0046d500(void* param_1, int param_2);
};

// FUNCTION: 0x46d500
void Class_0046d500::FUN_0046d500(void* param_1, int param_2)
{
    unsigned char b = *((unsigned char*)param_1 + 1);
    *(int*)((char*)param_1 + 2) = 0;
    if (b < 0x64) {
        ((Class_0046d6c0*)this)->FUN_0046d6c0(param_1, param_2);
    }
}
