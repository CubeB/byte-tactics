// Decompiled by Opus, Sonnet, Haiku and DeepSeek V4.1 Flash. Names are provisional.
// The HAPINET DirectPlay wrapper: the provider and player calls, the lobby
// and address calls, the interface setup and teardown, and the report
// interface stubs.

#include <windows.h>
#include <stdio.h>
#include <string.h>

// A GUID (16 bytes), copied whole.
struct Guid_4ca330 {
    unsigned long data1;
    unsigned short data2;
    unsigned short data3;
    unsigned char data4[8];
};

// The first two dwords of a DirectPlay session description (DPSESSIONDESC2);
// only the flags at +0x4 are read here.
struct Desc_004ca450 {
    int unknown_0;
    unsigned int flags;                // +0x4
};

// The connection settings buffer; +8 points at the session description.
struct Connection_004ca450 {
    char unknown_0[8];
    Desc_004ca450* desc;               // +0x8
};

// DirectPlay DPNAME (the toolchain's DirectX 3 <dplay.h> lacks it).
struct DPName_004ca6a0 {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

#pragma pack(push, 1)
// The player data block HAPINET_addplayer passes to CreatePlayer.
struct PlayerData_004ca6a0 {
    char name[17];                     // +0x00
    short field_11;                    // +0x11
    short field_13;                    // +0x13
};
#pragma pack(pop)

// IDirectPlay2/3-like COM interface: the slots this module calls.
class DirectPlay_4ca5d0 {
public:
    virtual int __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual int __stdcall Slot03();
    virtual int __stdcall Slot04();
    virtual int __stdcall Slot05();
    virtual int __stdcall CreatePlayer(unsigned long* id, DPName_004ca6a0* name, void* event,
                                       void* data, unsigned long size, unsigned long flags);   // +0x18
    virtual int __stdcall Slot07();
    virtual int __stdcall Slot08();
    virtual int __stdcall DestroyPlayer(unsigned long id);                                    // +0x24
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall EnumPlayers(void* session, void* callback, void* context, unsigned long flags);   // +0x30
    virtual int __stdcall Slot13();
    virtual int __stdcall Slot14();
    virtual int __stdcall Slot15();
    virtual int __stdcall Slot16();
    virtual int __stdcall Slot17();
    virtual int __stdcall GetPlayerAddress(unsigned long player, void* data, unsigned long* size);   // +0x48
    virtual int __stdcall Slot19();
    virtual int __stdcall Slot20();
    virtual int __stdcall GetPlayerName(unsigned long id, void* data, unsigned long* size);   // +0x54
    virtual int __stdcall Slot22();
    virtual int __stdcall Slot23();
    virtual int __stdcall Slot24();
    virtual int __stdcall Slot25();
    virtual int __stdcall Slot26();
    virtual int __stdcall Slot27();
    virtual int __stdcall Slot28();
    virtual int __stdcall Slot29();
    virtual int __stdcall SetPlayerName(unsigned long id, void* name, unsigned long flags);   // +0x78
    virtual int __stdcall Slot31();
    virtual int __stdcall Slot32();
    virtual int __stdcall Slot33();
    virtual int __stdcall Slot34();
    virtual int __stdcall Slot35();
    virtual int __stdcall Slot36();
    virtual int __stdcall Slot37();
    virtual int __stdcall Slot38(int val, int flag);                                          // +0x98
};

// IDirectPlayLobby-like COM interface: the slots this module calls.
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
                                                unsigned long* dataSize);                 // +0x20
    virtual int __stdcall Slot09();
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall Slot12();
    virtual int __stdcall Slot13();
    virtual int __stdcall CreateCompoundAddress(void* elements, unsigned long count, void* address,
                                                unsigned long* size);                     // +0x38
};

#pragma pack(push, 1)
// The HAPINET network object: the player names, the enumeration buffers, the
// application and service provider GUIDs, the connection settings and the
// DirectPlay and lobby interfaces.
struct Net_4ca330 {
    char unknown_0[0x11];             // +0x00
    char longName[16];                // +0x11
    char shortName[16];               // +0x21
    char unknown_31[0x431 - 0x31];    // +0x31
    Guid_4ca330* guids;               // +0x431
    char* conns;                      // +0x435
    char* names;                      // +0x439
    Guid_4ca330 application;          // +0x43d
    Guid_4ca330 sp;                   // +0x44d
    char unknown_45d[0x461 - 0x45d];  // +0x45d
    unsigned int flags;               // +0x461
    char unknown_465[0x4ad - 0x465];  // +0x465
    int field_4ad;                    // +0x4ad
    char unknown_4b1[0x4c1 - 0x4b1];  // +0x4b1
    DirectPlay_4ca5d0* dp1;           // +0x4c1
    DirectPlay_4ca5d0* dp;            // +0x4c5
    DirectPlayLobby_4ca490* created;  // +0x4c9
    DirectPlayLobby_4ca490* lobby;    // +0x4cd
    Connection_004ca450* connection;  // +0x4d1
    int connection_size;              // +0x4d5
    char unknown_4d9[0x4e5 - 0x4d9];  // +0x4d9
    int field_4e5;                    // +0x4e5
};
#pragma pack(pop)

// The DirectPlay interface pair HAPINET_createdplayinterface fills: the base
// object at +0x0 and the IDirectPlay3 interface at +0x4.
struct Net_004ca900 {
    DirectPlay_4ca5d0* dp;            // +0x0
    DirectPlay_4ca5d0* dp3;           // +0x4
};

// DPLAYX.dll ordinal 2 (DirectPlayEnumerateA from the DirectX 5 dplay.h).
typedef BOOL (__stdcall* EnumSpCallback_4ca400)(Guid_4ca330* sp, char* name, DWORD major, DWORD minor, Net_4ca330* context);

extern int g_guaranteePackets;
extern GUID DAT_004fce18;              // IID_IDirectPlayLobby2A
extern GUID DAT_004fcd78;              // IID_IDirectPlay3A
extern int DAT_0051ff08;
extern int DAT_0051ff04;

void __cdecl HapinetTrace(int);
char* __stdcall SkipTextLines(char* text, int n);
void* __cdecl FUN_004d83b0(unsigned int name, unsigned int size);
int GetDisplay();
extern "C" HRESULT __stdcall DirectPlayCreate(GUID* sp, DirectPlay_4ca5d0** dp, void* unknown);
extern "C" HRESULT __stdcall DirectPlayLobbyCreateA(GUID* lpGUID,
                                                    DirectPlayLobby_4ca490** lplpDPL,
                                                    void* lpUnkOuter, void* lpReserved,
                                                    unsigned long dwFlags);
extern "C" HRESULT __stdcall DirectPlayEnumerateA(EnumSpCallback_4ca400 callback, void* context);

// DirectPlay EnumProviders callback: records the provider GUID in the array at
// +0x431 and the "name major.minor" string in the text buffer at +0x439.
// FUNCTION: 0x4ca330
int __stdcall HAPINET_enumproviders(Guid_4ca330* guid, char* name, unsigned long major,
                           unsigned long minor, Net_4ca330* net)
{
    char local[200];
    HapinetTrace((int)"HAPINET_enumproviders\n");
    net->guids[net->field_4e5] = *guid;
    sprintf(local, "%s %d.%d", name, major, minor);
    char* slot = SkipTextLines(net->names, net->field_4e5);
    strcpy(slot, local);
    net->field_4e5++;
    return 1;
}

// HAPINET_getproviders: enumerates the service providers into the GUID array
// at +0x431 and the name buffer at +0x439.
// FUNCTION: 0x4ca400
int __stdcall HAPINET_getproviders(Net_4ca330* net, int a, int c)
{
    HapinetTrace((int)"HAPINET_getproviders\n");
    net->field_4e5 = 0;
    net->guids = (Guid_4ca330*)a;
    net->names = (char*)c;
    DirectPlayEnumerateA(HAPINET_enumproviders, net);
    return 1;
}

// HAPINET_passwordrequired: tests flag 0x400 in the current connection's
// description, or in the local copy when there is no connection.
// FUNCTION: 0x4ca450
unsigned int __stdcall HAPINET_passwordrequired(Net_4ca330* net)
{
    HapinetTrace((int)"HAPINET_passwordrequired\n");
    if (net->connection != 0)
        return net->connection->desc->flags & 0x400;
    return net->flags & 0x400;
}

// HAPINET_initlobbiedconnection: creates the DirectPlay lobby (DPLAYX ordinal
// 4), queries IID_IDirectPlayLobby2A from it, then copies the connection
// settings into a malloc'd buffer. The GUID at 0x4fce18 is
// IID_IDirectPlayLobby2A; slot 8 (+0x20) of that interface is
// GetConnectionSettings, which first reports DPERR_BUFFERTOOSMALL (0x8877001e)
// with a null buffer and the required size.
// FUNCTION: 0x4ca490
int __stdcall HAPINET_initlobbiedconnection(Net_4ca330* net)
{
    HapinetTrace((int)"HAPINET_initlobbiedconnection\n");
    if (net->lobby != 0)
        return 0;
    net->created = 0;
    net->lobby = 0;
    net->connection = 0;
    net->connection_size = 0;
    int hr = DirectPlayLobbyCreateA(0, &net->created, 0, 0, 0);
    if (hr >= 0) {
        hr = net->created->QueryInterface(&DAT_004fce18, (void**)&net->lobby);
        if (hr >= 0) {
            unsigned long size = 0;
            hr = net->lobby->GetConnectionSettings(0, 0, &size);
            if (hr == (int)0x8877001e) {
                net->connection = (Connection_004ca450*)FUN_004d83b0((unsigned int)"DPLAY LOBBY CONNECTION", size);
                if (net->connection != 0) {
                    hr = net->lobby->GetConnectionSettings(0, net->connection, &size);
                    if (hr >= 0)
                        net->connection_size = size;
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

// HAPINET_initconnection: opens the session through slot 38 of the DirectPlay
// interface at +0x4c5.
// FUNCTION: 0x4ca590
int __stdcall HAPINET_initconnection(Net_4ca330* param_1, int* param_2)
{
    HapinetTrace((int)"HAPINET_initconnection\n");
    return param_1->dp->Slot38(*param_2, 0) >= 0;
}

// HAPINET_initmultiplay: creates the DirectPlay object from the service
// provider GUID in sp, stores the application GUID and answers whether the
// queried DirectPlay interface came back.
// FUNCTION: 0x4ca5d0
int __stdcall HAPINET_initmultiplay(Net_4ca330* net, Guid_4ca330* sp, Guid_4ca330* application)
{
    HapinetTrace((int)"HAPINET_initmultiplay\n");
    g_guaranteePackets = 0;
    net->field_4ad = *(int*)(GetDisplay() + 0x40);
    net->application = *application;
    net->created = 0;
    net->lobby = 0;
    net->connection = 0;
    net->connection_size = 0;
    net->dp1 = 0;
    net->dp = 0;
    net->sp = *sp;
    int hr = DirectPlayCreate((GUID*)&net->sp, &net->dp1, 0);
    if (hr != 0)
        return 0;
    hr = net->dp1->QueryInterface(&DAT_004fcd78, (void**)&net->dp);
    return hr == 0 ? 1 : 0;
}

// FUNCTION: 0x4ca6a0
int __stdcall HAPINET_addplayer(Net_4ca330* net, unsigned long* id, char* shortName, char* longName,
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

// FUNCTION: 0x4ca780
int __stdcall HAPINET_removeplayer(Net_4ca330* net, unsigned long id)
{
    HapinetTrace((int)"HAPINET_removeplayer\n");
    if (net->dp != 0) {
        int hr = net->dp->DestroyPlayer(id);
        return hr == 0 ? 1 : 0;
    }
    return 0;
}

// HAPINET_getplayername: IDirectPlay2::GetPlayerName (+0x54); returns
// 0x88770140 when there is no DirectPlay interface.
// FUNCTION: 0x4ca7c0
int __stdcall HAPINET_getplayername(Net_4ca330* net, unsigned long id, void* data, unsigned long* size)
{
    HapinetTrace((int)"HAPINET_getplayername\n");
    if (net->dp != 0) {
        return net->dp->GetPlayerName(id, data, size);
    }
    return 0x88770140;
}

// HAPINET_setplayername: IDirectPlay2::SetPlayerName (+0x78); returns
// 0x88770140 when there is no DirectPlay interface.
// FUNCTION: 0x4ca800
int __stdcall HAPINET_setplayername(Net_4ca330* net, unsigned long id, void* name, unsigned long flags)
{
    HapinetTrace((int)"HAPINET_setplayername\n");
    if (net->dp != 0) {
        return net->dp->SetPlayerName(id, name, flags);
    }
    return 0x88770140;
}

// HAPINET_enumplayers: IDirectPlay2::EnumPlayers (+0x30); returns 0x88770140
// when there is no DirectPlay interface.
// FUNCTION: 0x4ca840
int __stdcall HAPINET_enumplayers(Net_4ca330* net, void* session, void* callback, void* context, unsigned long flags)
{
    HapinetTrace((int)"HAPINET_enumplayers\n");
    if (net->dp != 0) {
        return net->dp->EnumPlayers(session, callback, context, flags);
    }
    return 0x88770140;
}

// HAPINET_createcompoundaddress: IDirectPlayLobby2::CreateCompoundAddress
// (+0x38) on the lobby interface at +0x4cd; returns 0x88770140 when there
// is none.
// FUNCTION: 0x4ca880
int __stdcall HAPINET_createcompoundaddress(Net_4ca330* net, void* elements, unsigned long count, void* address,
                           unsigned long* size)
{
    HapinetTrace((int)"HAPINET_createcompoundaddress\n");
    if (net->lobby != 0) {
        return net->lobby->CreateCompoundAddress(elements, count, address, size);
    }
    return 0x88770140;
}

// HAPINET_enumaddress: IDirectPlayLobby::EnumAddress (+0x14) on the lobby
// interface; returns 0x88770140 when there is none. Same shape as 0x4ca840.
// FUNCTION: 0x4ca8c0
int __stdcall HAPINET_enumaddress(Net_4ca330* net, void* callback, void* address, unsigned long size, void* context)
{
    HapinetTrace((int)"HAPINET_enumaddress\n");
    if (net->lobby != 0) {
        return net->lobby->EnumAddress(callback, address, size, context);
    }
    return 0x88770140;
}

// HAPINET_createdplayinterface: DirectPlayCreate (DPLAYX ordinal 1), then
// QueryInterface for IDirectPlay3A (the GUID at 0x4fcd78) into the second
// slot.
// FUNCTION: 0x4ca900
HRESULT __stdcall HAPINET_createdplayinterface(GUID* sp, Net_004ca900* net)
{
    HapinetTrace((int)"HAPINET_createdplayinterface\n");
    memset(net, 0, sizeof(Net_004ca900));
    HRESULT hr = DirectPlayCreate(sp, &net->dp, 0);
    if (hr >= 0) {
        hr = net->dp->QueryInterface(&DAT_004fcd78, (void**)&net->dp3);
    }
    return hr;
}

// Releases the IDirectPlay3 interface, then the DirectPlay object.
// FUNCTION: 0x4ca940
int __stdcall HAPINET_releasedplayinterface(Net_004ca900* net)
{
    HapinetTrace((int)"HAPINET_releasedplayinterface\n");
    int result = 0;
    if (net->dp3 != 0) {
        result = net->dp3->Release();
        net->dp3 = 0;
    }
    if (net->dp != 0) {
        net->dp->Release();
        net->dp = 0;
    }
    return result;
}

// DirectX 5's DPERR_UNINITIALIZED, MAKE_DPHRESULT(320); the toolchain's
// DirectX 3 <dplay.h> does not define it.
#define DPERR_UNINITIALIZED_004ca990 0x88770140

// HAPINET_getplayeraddress: IDirectPlay2::GetPlayerAddress (+0x48) on the
// interface at +0x4.
// FUNCTION: 0x4ca990
int __stdcall HAPINET_getplayeraddress(Net_004ca900* net, unsigned long player, void* data, unsigned long* size)
{
    HapinetTrace((int)"HAPINET_getplayeraddress\n");
    if (net->dp3 != 0)
        return net->dp3->GetPlayerAddress(player, data, size);
    return DPERR_UNINITIALIZED_004ca990;
}

// Returns the lobby object at +0x4c9.
// FUNCTION: 0x4ca9d0
int __stdcall FUN_004ca9d0(Net_4ca330* param_1)
{
    return (int)param_1->created;
}

// FUNCTION: 0x4ca9e0
int __stdcall FUN_004ca9e0(int param_1)
{
    return param_1 + 0x465;
}

// FUNCTION: 0x4ca9f0
int __stdcall FUN_004ca9f0(int)
{
    return 0;
}

// FUNCTION: 0x4caa00
int __stdcall RIReportGameChat(int arg1, int arg2)
{
    return 1;
}

// FUNCTION: 0x4caa10
int __stdcall RIReport(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10)
{
    return 1;
}

// FUNCTION: 0x4caa20
void __stdcall RISetCallbacks(int param_1, int param_2)
{
    DAT_0051ff08 = param_1;
    DAT_0051ff04 = param_2;
}
