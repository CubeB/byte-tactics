// Decompiled by Haiku. Names are provisional.

#pragma pack(push, 1)
class Class_004388b0 {
public:
    char unknown_0[0x0e];
    void* obj_ptr;
    char unknown_1[0x40];
    int value;

    void FUN_004388b0();
};
#pragma pack(pop)

// FUNCTION: 0x4388b0
void Class_004388b0::FUN_004388b0()
{
    if (value != 0) {
        void* p1 = *(void**)obj_ptr;
        void* p2 = *(void**)p1;
        void* p3 = *(void**)p2;
        void* fn_ptr = *(void**)((char*)p3 + 4);
        ((void (__stdcall*)(int))fn_ptr)(value);
    }
}
