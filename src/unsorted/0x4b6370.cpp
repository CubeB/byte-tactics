// Decompiled by Haiku. Names are provisional.

extern int DAT_0051fbd0;
extern int DAT_0051fc84;
extern int DAT_0051fc80;
extern int DAT_0051fbe0;
extern int (*DAT_004fc0dc)();

// FUNCTION: 0x4b6370
int FUN_004b6370()
{
    int (*GetTickCount)() = DAT_004fc0dc;

    int esi = (int)GetTickCount;
    int edi;

    int eax = GetTickCount();
    int edx = *(int *)&DAT_0051fbd0;
    int ecx = eax;
    ecx = ecx * *(int *)((char *)edx + 0xe8);

    int magic = 0x10624dd3;
    eax = magic;

    edx = (int)((unsigned __int64)(ecx * (unsigned int)eax) >> 32);

    ecx = *(int *)&DAT_0051fc84;
    edx = edx >> 6;
    edx = edx - ecx;
    edi = edx;

    eax = GetTickCount();
    edx = *(int *)&DAT_0051fbd0;
    ecx = eax;
    eax = magic;
    int temp_esi = 0x51fbe0;

    ecx = ecx * *(int *)((char *)edx + 0xe8);
    edx = (int)((unsigned __int64)(ecx * (unsigned int)eax) >> 32);
    edx = edx >> 6;
    *(int *)&DAT_0051fc84 = edx;

    int *piVar5 = (int *)temp_esi;
    int result = 0;

    while ((int)piVar5 < (int)&DAT_0051fc80) {
        if (*piVar5 >= 0) {
            ecx = piVar5[1];
            ecx = ecx - edi;
            eax = ecx;
            piVar5[1] = ecx;

            if (eax <= 0) {
                eax = piVar5[-1];
                int (*fn)(int) = (int (*)(int))piVar5[-2];
                result = fn(eax);
                ecx = *piVar5;
                piVar5[1] = ecx;
            }
        }
        piVar5 += 4;
    }

    return result;
}
