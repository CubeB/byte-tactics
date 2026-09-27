// Decompiled by Haiku. Names are provisional.

struct Class_004e2d00;

int Class_004e2d00_FUN_004e2d00(Class_004e2d00* this_obj, const char* param1, int param2, int param3, unsigned char param4);

// FUNCTION: 0x4e2cc0
bool __stdcall FUN_004e2cc0(const char* param_1, unsigned int param_2)
{
    int eax = param_2;
    eax &= 0xff;
    int result = Class_004e2d00_FUN_004e2d00(0, param_1, 0, 1, (unsigned char)eax);
    return result != 0;
}
