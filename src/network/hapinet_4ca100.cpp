// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// DirectPlay EnumConnections callback: ignores the session's own service
// providers (the four GUIDs at 0x50a788), records the connection's GUID, copies
// the connection data into a freshly allocated block and stores the name.
#include <stdio.h>
#include <string.h>

struct Guid_4ca100 {
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

struct DPName_4ca100 {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

struct Conn_4ca100 {
    void* data;                // +0x0
    unsigned long size;        // +0x4
};

#pragma pack(push, 1)
struct Net_4ca100 {
    char unknown_0[0x431];
    Guid_4ca100* guids;        // +0x431
    Conn_4ca100* conns;        // +0x435
    char* names;               // +0x439
    char unknown_43d[0x4e5 - 0x43d];
    int field_4e5;             // +0x4e5
};
#pragma pack(pop)

extern Guid_4ca100* DAT_0050a788[4];

void __cdecl HapinetTrace(int);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
char* __stdcall FUN_004b6af0(char* text, int n);

// FUNCTION: 0x4ca100
int __stdcall HAPINET_enumconnections(Guid_4ca100* guid, void* connection, unsigned long size,
                           DPName_4ca100* name, unsigned long flags, Net_4ca100* net)
{
    char local[200];
    HapinetTrace((int)"HAPINET_enumconnections\n");
    for (unsigned int i = 0; i < 4; i++) {
        if (memcmp(guid, DAT_0050a788[i], sizeof(Guid_4ca100)) == 0)
            return 1;
    }
    net->guids[net->field_4e5] = *guid;
    net->conns[net->field_4e5].data = FUN_004d83b0("DPLAY CONNECTION", size);
    if (net->conns[net->field_4e5].data == 0)
        return 0;
    memcpy(net->conns[net->field_4e5].data, connection, size);
    net->conns[net->field_4e5].size = size;
    sprintf(local, "%s", name->lpszShortNameA);
    char* dest = FUN_004b6af0(net->names, net->field_4e5);
    strcpy(dest, local);
    net->field_4e5++;
    return 1;
}
