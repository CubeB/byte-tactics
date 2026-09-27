// Decompiled by Haiku. Names are provisional.

extern int DAT_00511de8;
extern void FUN_00430f00();

// FUNCTION: 0x417300
void FUN_00417300() {
    int* game = (int*)&DAT_00511de8;
    unsigned short val = *(unsigned short*)((char*)game + 0x37f2f);
    val = val ^ 0x40;
    *(unsigned short*)((char*)game + 0x37f2f) = val;
    FUN_00430f00();
}
