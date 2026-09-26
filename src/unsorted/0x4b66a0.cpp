// Decompiled by Haiku. Names are provisional.

extern int DAT_0051fbd0;
extern unsigned int (*DAT_004fc0dc)();

// FUNCTION: 0x4b66a0
int FUN_004b66a0()
{
    int eax = *(int *)&DAT_0051fbd0;
    int *esi = (int *)((char *)eax + 0x1da);

    eax = DAT_004fc0dc();

    int edx = *(int *)((char *)esi + 4);
    int ecx = eax;
    ecx = ecx - edx;
    edx = *(int *)esi;
    edx = edx + ecx;
    ecx = *(int *)((char *)esi + 8);
    *(int *)((char *)esi + 4) = eax;
    eax = edx;
    ecx++;

    int check = eax;
    *(int *)esi = edx;
    *(int *)((char *)esi + 8) = ecx;

    if (check > 0x7d0) {
        *(int *)esi = 0x3e8;
    }

    eax = *(int *)esi;
    if (eax > 0x3e8) {
        eax = eax - 0x3e8;
        *(int *)((char *)esi + 0xc) = ecx;
        *(int *)esi = eax;
        *(int *)((char *)esi + 8) = 0;
    }

    edx = *(int *)&DAT_0051fbd0;
    return *(int *)((char *)edx + 0x1e6);
}
