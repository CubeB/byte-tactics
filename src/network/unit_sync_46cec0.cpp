// Decompiled by Opus. Names are provisional.
// A method that ignores `this`: its callers (0x46dad0) set ecx to the object
// at +0x2c before each call. Clears the packet's +2 field and sends the
// 0xe-byte packet. 0x46d530, 0x46d5b0 and 0x46d630 inline a copy of it.

extern "C" int __cdecl FUN_0044fe00();
extern "C" void __stdcall FUN_00451bc0(int a, unsigned int b, void* c, int d);

class Class_0046cec0 {
public:
    void FUN_0046cec0(unsigned int param_1, void* param_2);
};

// FUNCTION: 0x46cec0
void Class_0046cec0::FUN_0046cec0(unsigned int param_1, void* param_2)
{
    *(int*)((char*)param_2 + 2) = 0;
    FUN_00451bc0(FUN_0044fe00(), param_1, param_2, 0xe);
}
