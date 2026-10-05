// Decompiled by Opus. Names are provisional.

extern unsigned int DAT_00528a04;
extern unsigned int DAT_00528a08;
extern unsigned int DAT_00528a1c;
extern unsigned int DAT_005289dc;
extern unsigned int DAT_005289fc;
extern unsigned int DAT_005289f8;
extern unsigned int DAT_005289d8;

// FUNCTION: 0x4da7d0
void __cdecl CountAlloc(unsigned int size)
{
    DAT_00528a04++;
    DAT_00528a08++;
    if (DAT_00528a08 > DAT_00528a1c)
        DAT_00528a1c = DAT_00528a08;
    DAT_005289dc += size;
    if (DAT_005289dc >= 1000000000) {
        DAT_005289dc -= 1000000000;
        DAT_005289fc++;
    }
    DAT_005289f8 += size;
    if (DAT_005289f8 > DAT_005289d8)
        DAT_005289d8 = DAT_005289f8;
}
