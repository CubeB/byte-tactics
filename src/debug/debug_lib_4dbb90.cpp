// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x4dbb90
void __cdecl FUN_004dbb90(unsigned int* param_1, unsigned int* param_2)
{
    *param_1 &= 0xfffff000;
    *param_2 = (*param_2 + 0xfff) & 0xfffff000;
}
