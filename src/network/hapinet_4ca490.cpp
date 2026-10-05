// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// HAPINET_initlobbiedconnection: creates the DirectPlay lobby (DPLAYX ordinal
// 4), queries IID_IDirectPlayLobby2A from it, then copies the connection
// settings into a malloc'd buffer. The GUID at 0x4fce18 is
// IID_IDirectPlayLobby2A; slot 8 (+0x20) of that interface is
// GetConnectionSettings, which first reports DPERR_BUFFERTOOSMALL (0x8877001e)
// with a null buffer and the required size.
#include <windows.h>

// IDirectPlayLobbyA and IDirectPlayLobby2A (DirectX 5 dplobby.h).
class DirectPlayLobby_4ca490 {
public:
    virtual int __stdcall QueryInterface(GUID* riid, void** ppvObject);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual int __stdcall Connect(unsigned long flags, void** dp, void* outer);
    virtual int __stdcall CreateAddress(GUID* sp, GUID* device, const void* address,
                                        unsigned long addressSize, void* dest,
                                        unsigned long* destSize);
    virtual int __stdcall EnumAddress(void* callback, const void* address,
                                      unsigned long length, void* context);
    virtual int __stdcall EnumAddressTypes(void* callback, GUID* sp, void* context,
                                           unsigned long flags);
    virtual int __stdcall EnumLocalApplications(void* callback, void* context,
                                                unsigned long flags);
    virtual int __stdcall GetConnectionSettings(unsigned long appID, void* connection,
                                                unsigned long* dataSize);
};

#pragma pack(push, 1)
struct Net_4ca490 {
    char unknown_0[0x4c9];
    DirectPlayLobby_4ca490* created;   // +0x4c9
    DirectPlayLobby_4ca490* lobby;     // +0x4cd
    void* settings;                    // +0x4d1
    unsigned long settingsSize;        // +0x4d5
};
#pragma pack(pop)

extern GUID DAT_004fce18;              // IID_IDirectPlayLobby2A
extern int g_guaranteePackets;

void __cdecl HapinetTrace(int);
void* __cdecl FUN_004d83b0(unsigned int name, unsigned int size);
extern "C" HRESULT __stdcall DirectPlayLobbyCreateA(GUID* lpGUID,
                                                    DirectPlayLobby_4ca490** lplpDPL,
                                                    void* lpUnkOuter, void* lpReserved,
                                                    unsigned long dwFlags);

// FUNCTION: 0x4ca490
int __stdcall HAPINET_initlobbiedconnection(Net_4ca490* net)
{
    HapinetTrace((int)"HAPINET_initlobbiedconnection\n");
    if (net->lobby != 0)
        return 0;
    net->created = 0;
    net->lobby = 0;
    net->settings = 0;
    net->settingsSize = 0;
    int hr = DirectPlayLobbyCreateA(0, &net->created, 0, 0, 0);
    if (hr >= 0) {
        hr = net->created->QueryInterface(&DAT_004fce18, (void**)&net->lobby);
        if (hr >= 0) {
            unsigned long size = 0;
            hr = net->lobby->GetConnectionSettings(0, 0, &size);
            if (hr == (int)0x8877001e) {
                net->settings = FUN_004d83b0((unsigned int)"DPLAY LOBBY CONNECTION", size);
                if (net->settings != 0) {
                    hr = net->lobby->GetConnectionSettings(0, net->settings, &size);
                    if (hr >= 0)
                        net->settingsSize = size;
                } else {
                    hr = (int)0x8007000e;
                }
            }
            HapinetTrace((int)"HAPINET_guaranteepackets\n");
            g_guaranteePackets = 1;
        }
    }
    return hr >= 0;
}
