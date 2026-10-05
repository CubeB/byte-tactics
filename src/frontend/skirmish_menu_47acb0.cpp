// Decompiled by Haiku. Names are provisional.

extern void* __stdcall FindGadgetIndex(void*, const char*, int);

// FUNCTION: 0x47acb0
void __stdcall FUN_0047acb0(void* obj, void* unused)
{
    void* ptr = *(void**)((char*)obj + 0x18);
    void* val = *(void**)((char*)ptr + 4);
    void* result = FindGadgetIndex(val, "LOGOS", 2);
    *(void**)((char*)obj + 0x60) = result;
}
