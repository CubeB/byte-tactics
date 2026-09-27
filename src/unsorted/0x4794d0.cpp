// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;

// FUNCTION: 0x4794d0
int FUN_004794d0() {
    int count = 0;
    int num_items = *(int*)((char*)&DAT_00511de8 + 0x38d81);
    if (num_items > 0) {
        int* ptr = (int*)*(int*)((char*)&DAT_00511de8 + 0x29a0);
        for (int i = 0; i < num_items; i++) {
            if (*ptr == 2) {
                count++;
            }
            ptr = (int*)((char*)ptr + 0x18);
        }
    }
    return count;
}
