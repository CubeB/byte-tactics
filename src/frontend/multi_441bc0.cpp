// Decompiled by Opus. Names are provisional.
// Maps the selected DirectPlay service provider GUID to an index:
// 0 modem, 1 TCP/IP, 2 IPX, 3 serial, 4 anything else. The four globals hold
// DPSPGUID_MODEM, DPSPGUID_TCPIP, DPSPGUID_IPX and DPSPGUID_SERIAL.
#include <string.h>

struct Guid_00441bc0 {
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

extern char* g_game;

extern Guid_00441bc0 DAT_004fcdc8;
extern Guid_00441bc0 DAT_004fcda8;
extern Guid_00441bc0 DAT_004fcd98;
extern Guid_00441bc0 DAT_004fcdb8;

// FUNCTION: 0x441bc0
int GetServiceProviderIndex()
{
    Guid_00441bc0* guid = (Guid_00441bc0*)(g_game + 0x39201);
    if (memcmp(guid, &DAT_004fcdc8, sizeof(Guid_00441bc0)) == 0) {
        return 0;
    }
    if (memcmp(guid, &DAT_004fcda8, sizeof(Guid_00441bc0)) == 0) {
        return 1;
    }
    if (memcmp(guid, &DAT_004fcd98, sizeof(Guid_00441bc0)) == 0) {
        return 2;
    }
    if (memcmp(guid, &DAT_004fcdb8, sizeof(Guid_00441bc0)) == 0) {
        return 3;
    }
    return 4;
}
