// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4afe90
void __stdcall FUN_004afe90(void* param1, int param2)
{
    int max = *(int*)((char*)param1 + 0xaa);
    if (param2 <= max) {
        void* ptr = *(void**)((char*)param1 + 0xa6);
        int calc = param2 + param2 * 4;  // param2 * 5
        int offset = (param2 + calc * 8) * 4;  // (param2 * 41) * 4 = param2 * 0xa4
        *(unsigned char*)((char*)ptr + offset) = 0;
    }
}
