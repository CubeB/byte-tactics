// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// DirectPlay EnumProviders callback: records the provider GUID in the array at
// +0x431 and the "name major.minor" string in the text buffer at +0x439.
#include <stdio.h>
#include <string.h>

struct Guid_4ca330 {
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

#pragma pack(push, 1)
struct Net_4ca330 {
    char unknown_0[0x431];
    Guid_4ca330* guids;        // +0x431
    char* conns;               // +0x435
    char* names;               // +0x439
    char unknown_43d[0x4e5 - 0x43d];
    int field_4e5;             // +0x4e5
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);
char* __stdcall FUN_004b6af0(char* text, int n);

// FUNCTION: 0x4ca330
int __stdcall FUN_004ca330(Guid_4ca330* guid, char* name, unsigned long major,
                           unsigned long minor, Net_4ca330* net)
{
    char local[200];
    FUN_004c9740((int)"HAPINET_enumproviders\n");
    net->guids[net->field_4e5] = *guid;
    sprintf(local, "%s %d.%d", name, major, minor);
    char* slot = FUN_004b6af0(net->names, net->field_4e5);
    strcpy(slot, local);
    net->field_4e5++;
    return 1;
}
