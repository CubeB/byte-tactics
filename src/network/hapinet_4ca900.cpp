// Decompiled by Opus. Names are provisional.
// HAPINET_createdplayinterface: DirectPlayCreate (DPLAYX ordinal 1), then
// QueryInterface for IDirectPlay3A (the GUID at 0x4fcd78) into the second
// slot.
#include <string.h>
#include <windows.h>
#include <dplay.h>

struct Net_004ca900 {
    IDirectPlay* dp;                   // +0x0
    void* dp3;                         // +0x4
};

extern GUID DAT_004fcd78;              // IID_IDirectPlay3A

void __cdecl HapinetTrace(int);

// FUNCTION: 0x4ca900
HRESULT __stdcall HAPINET_createdplayinterface(GUID* sp, Net_004ca900* net)
{
    HapinetTrace((int)"HAPINET_createdplayinterface\n");
    memset(net, 0, sizeof(Net_004ca900));
    HRESULT hr = DirectPlayCreate(sp, &net->dp, 0);
    if (hr >= 0) {
        hr = net->dp->QueryInterface(DAT_004fcd78, &net->dp3);
    }
    return hr;
}
