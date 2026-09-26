// Decompiled by Haiku. Names are provisional.

extern void* DAT_00512eec;
extern void(__stdcall* DAT_004fc0c0)(void*);

// FUNCTION: 0x45b640
void FUN_0045b640(void)
{
    if (DAT_00512eec != 0) {
        DAT_004fc0c0(DAT_00512eec);
        DAT_00512eec = 0;
    }
}
