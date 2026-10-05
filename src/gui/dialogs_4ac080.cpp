// Decompiled by Sonnet. Names are provisional.

extern char DAT_00502ae8[];

bool __stdcall IsGadgetNamed(int param1, int param2, char* param3);

struct Dialog
{
public:
    int unknown_0[6];
    int* ptr_18;
    int unknown_1c[17];
    int ptr_60;
};

// FUNCTION: 0x4ac080
void __stdcall NotExistDialogHandler(Dialog* obj)
{
    IsGadgetNamed(obj->ptr_18[1], obj->ptr_60, DAT_00502ae8);
}
