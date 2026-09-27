// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x433a60
int __fastcall FUN_00433a60(void* this_ptr)
{
    int start = *(int*)((char*)this_ptr + 0x4);
    if (start == 0) {
        return 0;
    }
    int end = *(int*)((char*)this_ptr + 0x8);
    return (end - start) >> 2;
}
