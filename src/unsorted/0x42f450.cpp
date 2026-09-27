// Decompiled by space-bunny-free. Names are provisional.
#include <stdio.h>
#include <string.h>

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

struct Source_0042f450 {
    char unknown_0[4];
    Class_004c48c0* tdf;               // +0x4
};

extern char DAT_005119b8[];
extern char DAT_00504314[];

int __cdecl FUN_004d84a0(int param_1, char* param_2, int param_3);

// FUNCTION: 0x42f450
int __stdcall FUN_0042f450(Source_0042f450* param_1, char* param_2, int* param_3)
{
    char local_180[0x40];
    char local_140[0x40];
    char local_100[0x100];

    if (param_1->tdf->FUN_004c48c0(local_140, param_2, 0x40, DAT_005119b8) != 0) {
        sprintf(local_100, "%s%s", param_2, "text");
        if (param_1->tdf->FUN_004c48c0(local_180, local_100, 0x40, DAT_005119b8) == 0)
            local_180[0] = 0;
        param_3[1] = FUN_004d84a0(param_3[1], DAT_00504314, (param_3[0] + 1) * 0x40);
        param_3[2] = FUN_004d84a0(param_3[2], DAT_00504314, (param_3[0] + 1) * 0x40);
        strcpy((char*)(param_3[1] + param_3[0] * 0x40), local_140);
        strcpy((char*)(param_3[2] + param_3[0] * 0x40), local_180);
        param_3[0]++;
        return 1;
    }
    return 0;
}
