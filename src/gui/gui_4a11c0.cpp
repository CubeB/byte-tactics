// Decompiled by Sonnet. Names are provisional.

void __stdcall FUN_004a0340(void* param_1, int param_2);

struct Layer_004a11c0 {
    char unknown_0[4];
    char* data;                         // +4
};

struct Obj_004a11c0 {
    char unknown_0[0x18];
    Layer_004a11c0* layer;              // +0x18
};

static inline char* GetData(Obj_004a11c0* obj) { return obj->layer->data; }

// FUNCTION: 0x4a11c0
void __stdcall SetGadgetStatus(Obj_004a11c0* param_1, int param_2, short param_3)
{
    *(short*)(GetData(param_1) + param_2 * 347 + 0x138) = param_3;
    FUN_004a0340(param_1, param_2);
}
