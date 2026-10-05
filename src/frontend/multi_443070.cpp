// Decompiled by Opus. Names are provisional.
// An IDirectPlayLobby::EnumAddress callback (LPDPENUMADDRESSCALLBACK). When
// the chunk is DPAID_Modem (DAT_004fcec8, {f6dcc200-a2fe-11d0-9c4f-00a0c905425e}),
// the data is a double-null-terminated list of modem names; each one is
// copied into the buffer at DAT_00512980 and counted in DAT_00512984.
#include <windows.h>
#include <string.h>

extern const GUID DAT_004fcec8;
extern char* DAT_00512980;
extern int DAT_00512984;

// Where the next name goes: after the first string if the buffer is not empty.
static inline char* NameSlot(char* buffer)
{
    if (strlen(buffer) != 0)
        return buffer + strlen(buffer) + 1;
    return buffer;
}

// FUNCTION: 0x443070
BOOL __stdcall EnumModemAddressCallback(REFGUID guidDataType, DWORD dataSize, LPCVOID data, LPVOID context)
{
    char* name = (char*)data;
    if (IsEqualGUID(guidDataType, DAT_004fcec8)) {
        while (lstrlenA(name) != 0) {
            strcpy(NameSlot(DAT_00512980), name);
            DAT_00512984++;
            name += lstrlenA(name) + 1;
        }
    }
    return TRUE;
}
