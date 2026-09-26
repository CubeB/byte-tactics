// Decompiled by Haiku. Names are provisional.

extern "C" int DAT_004fc988;

class Class_00407930 {
public:
    void FUN_00407930(void *param_1, void *param_2, void *param_3, void *param_4);
};

// FUNCTION: 0x407930
void Class_00407930::FUN_00407930(void *param_1, void *param_2, void *param_3, void *param_4)
{
    *(void **)((char *)this + 8) = param_2;
    *(void **)((char *)this + 4) = param_1;
    *(int *)((char *)this + 0xc) = 0;
    *(unsigned int *)((char *)this + 0x10) = *(unsigned char *)((char *)param_1 + 4);
    *(void **)((char *)this + 0x1c) = param_4;
    *(void **)((char *)this + 0x20) = param_3;
    *(int *)this = (int)&DAT_004fc988;
    *(int *)((char *)this + 0x24) = 0;
    *(int *)((char *)this + 0x18) = 6;
    *(int *)((char *)this + 0x14) = 3;
}
