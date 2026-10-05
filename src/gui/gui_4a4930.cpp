// Decompiled by Sonnet. Names are provisional.

struct ElemArray_4a4930 {
    char unknown_0[4];
    char* base;               // +4
};

struct GameState_4a4930 {
    char unknown_0[0x18];
    ElemArray_4a4930* arr;    // +0x18
};

// FUNCTION: 0x4a4930
void __stdcall FUN_004a4930(GameState_4a4930* param_1, int index)
{
    char* e = param_1->arr->base + index * 0x15b;
    *(int*)(e + 0xb6) = 0;
    *(int*)(e + 0xbe) = 0;
    *(int*)(e + 0xc2) = 0;
    *(short*)(e + 0xc6) = 0;
}
