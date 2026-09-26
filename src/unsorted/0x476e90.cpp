// Decompiled by Haiku. Names are provisional.

// FUNCTION: 0x476e90
char *__stdcall FUN_00476e90(char *param_1, int param_2, int param_3)
{
    int ebx = param_3;
    if (ebx == 0) {
        return param_1;
    }

    char *edx = param_1;
    int eax = 0;
    unsigned char cl = *edx;
    int esi = 0;

    if (cl == 0) {
        goto done;
    }

    {
        int edi = param_2;

        loop_label:
        cl = *edx;

        if (cl == 0xff) {
            goto done;
        }

        if (eax != 0) {
            goto done;
        }

        edx++;

        if (cl == 0xa) {
            int ecx = ebx;
            esi++;
            ecx = ecx * edi;
            if (esi != ecx) {
            } else {
                eax = 1;
            }
        }

        if (*edx != 0) {
            goto loop_label;
        }
    }

    done:
    eax = -eax;
    eax = eax - (eax != 0 ? 0 : 1);
    eax = eax & (int)edx;

    return (char *)eax;
}
