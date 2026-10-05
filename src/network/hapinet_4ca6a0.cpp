// Decompiled by Opus. Names are provisional.
#include <string.h>

// DirectPlay DPNAME (the toolchain's DirectX 3 <dplay.h> lacks it).
struct DPName_004ca6a0 {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

#pragma pack(push, 1)
struct PlayerData_004ca6a0 {
    char name[17];                     // +0x00
    short field_11;                    // +0x11
    short field_13;                    // +0x13
};
#pragma pack(pop)

// COM interface (IDirectPlay2-like); slot 6 (+0x18) is CreatePlayer.
class DirectPlay_004ca6a0 {
public:
    virtual int __stdcall Slot00();
    virtual int __stdcall Slot01();
    virtual int __stdcall Slot02();
    virtual int __stdcall Slot03();
    virtual int __stdcall Slot04();
    virtual int __stdcall Slot05();
    virtual int __stdcall CreatePlayer(unsigned long* id, DPName_004ca6a0* name, void* event,
                                       void* data, unsigned long size, unsigned long flags);
};

#pragma pack(push, 1)
struct Net_004ca6a0 {
    char unknown_0[0x11];
    char longName[16];                 // +0x11
    char shortName[16];                // +0x21
    char unknown_31[0x4c5 - 0x31];
    DirectPlay_004ca6a0* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl HapinetTrace(int);

// FUNCTION: 0x4ca6a0
int __stdcall HAPINET_addplayer(Net_004ca6a0* net, unsigned long* id, char* shortName, char* longName,
                           char* name, short field_11, short field_13)
{
    HapinetTrace((int)"HAPINET_addplayer\n");
    if (net->dp != 0) {
        DPName_004ca6a0 dpname;
        dpname.dwSize = sizeof(DPName_004ca6a0);
        dpname.dwFlags = 0;
        dpname.lpszShortNameA = shortName;
        dpname.lpszLongNameA = longName;
        strncpy(net->longName, longName, 16);
        strncpy(net->shortName, shortName, 16);
        PlayerData_004ca6a0 data;
        memset(&data, 0, sizeof(data));
        strncpy(data.name, name, 16);
        data.field_11 = field_11;
        data.field_13 = field_13;
        int hr = net->dp->CreatePlayer(id, &dpname, 0, &data, sizeof(data), 0);
        return hr == 0 ? 1 : 0;
    }
    return 0;
}
