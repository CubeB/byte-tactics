// Decompiled by Haiku. Names are provisional.

extern void FUN_004b7f90(void*, int, int, int);

// FUNCTION: 0x467a20
void FUN_00467a20(void* param_1, int* param_2, int param_3, int param_4) {
    short* data = (short*)param_2;
    int y_offset = (int)data[2];
    int x_offset = (int)data[3];
    FUN_004b7f90(param_1, (int)data[3] + param_3, (int)data[2] + param_4, param_4);
}
