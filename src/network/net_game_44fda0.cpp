// Decompiled by Sonnet. Names are provisional.

typedef void (__stdcall *FuncPtr)(void*);
extern FuncPtr DAT_00512a20[];

class Class_0044fda0 {
public:
    void FUN_0044fda0();
};

// FUNCTION: 0x44fda0
void Class_0044fda0::FUN_0044fda0()
{
    unsigned int idx = 0;
    idx = *(unsigned char*)this;
    DAT_00512a20[idx](this);
}
