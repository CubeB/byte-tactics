// Decompiled by Sonnet. Names are provisional.

extern void* g_game;

struct Item_004794d0
{
    int type;               // +0x0
    char unknown_4[0x14];   // sizeof == 0x18
};

// FUNCTION: 0x4794d0
int FUN_004794d0()
{
    int count = 0;
    int num_items = *(int*)((char*)g_game + 0x38d81);
    if (num_items > 0) {
        Item_004794d0* ptr = *(Item_004794d0**)((char*)g_game + 0x29a0);
        do {
            if (ptr->type == 2) {
                count++;
            }
            ptr++;
        } while (--num_items);
    }
    return count;
}
