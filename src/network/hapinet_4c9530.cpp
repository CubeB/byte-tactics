// Decompiled by space-bunny-free, Haiku, Opus, Sonnet and DeepSeek V4.1 Flash. Names are provisional.
// The HAPINET DirectPlay wrapper: the error strings, the packet send and
// receive calls, the session and lobby setup and the enumeration callbacks.

#include <windows.h>
#include <stdio.h>
#include <string.h>

// A GUID (16 bytes), copied and compared whole.
struct GUID_004c9920 {
    unsigned long data[4];
};

// DirectPlay session description (DPSESSIONDESC2).
struct SessionDesc_4c9890 {
    unsigned long dwSize;            // +0x00
    unsigned long dwFlags;           // +0x04
    GUID_004c9920 guidInstance;      // +0x08
    GUID_004c9920 guidApplication;   // +0x18
    unsigned long dwMaxPlayers;      // +0x28
    unsigned long dwCurrentPlayers;  // +0x2c
    char* lpszSessionName;           // +0x30
    char* lpszPassword;              // +0x34
    unsigned long dwReserved1;       // +0x38
    unsigned long dwReserved2;       // +0x3c
    unsigned long dwUser1;           // +0x40
    unsigned long dwUser2;           // +0x44
    unsigned long dwUser3;           // +0x48
    unsigned long dwUser4;           // +0x4c
};

// One game in the list the EnumSessions callback fills.
struct GameRec_4c9c50 {
    int unknown_0;                   // +0x00
    int user1;                       // +0x04
    int user2;                       // +0x08
    int user3;                       // +0x0c
    int user4;                       // +0x10
    int maxPlayers;                  // +0x14
    char name[0x20];                 // +0x18
    GUID_004c9920 guidInstance;      // +0x38
    char unknown_48[0xc];            // +0x48
};

// A connection the EnumConnections callback records.
struct Conn_4ca100 {
    void* data;                      // +0x0
    unsigned long size;              // +0x4
};

// The buffer holding the connection settings; +8 points at the session
// description.
struct Connection_4c9a70 {
    char unknown_0[8];
    SessionDesc_4c9890* desc;        // +0x08
};

// IDirectPlay3-like COM interface: the slots this module calls.
class DirectPlay_4c97b0 {
public:
    virtual int __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual int __stdcall AddPlayerToGroup(unsigned long group, unsigned long player);
    virtual int __stdcall Close();
    virtual int __stdcall Slot05();
    virtual int __stdcall Slot06();
    virtual int __stdcall Slot07();
    virtual int __stdcall Slot08();
    virtual int __stdcall Slot09();
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall Slot12();
    virtual int __stdcall EnumSessions(void* desc, unsigned long timeout, void* callback, void* context, unsigned long flags);
    virtual int __stdcall Slot14();
    virtual int __stdcall Slot15();
    virtual int __stdcall Slot16();
    virtual int __stdcall Slot17();
    virtual int __stdcall Slot18();
    virtual int __stdcall Slot19();
    virtual int __stdcall GetPlayerData(unsigned long player, void* data, unsigned long* size, unsigned long flags);
    virtual int __stdcall Slot21();
    virtual int __stdcall GetSessionDesc(void* data, unsigned long* size);
    virtual int __stdcall Slot23();
    virtual int __stdcall Slot24(void* desc, unsigned long flags);
    virtual int __stdcall Receive(unsigned long* from, unsigned long* to, unsigned long flags, void* data, unsigned long* size);
    virtual int __stdcall Send(unsigned long from, unsigned long to, unsigned long flags, void* data, unsigned long size);
    virtual int __stdcall Slot27();
    virtual int __stdcall Slot28();
    virtual int __stdcall SetPlayerData(unsigned long player, void* data, unsigned long size, unsigned long flags);
    virtual int __stdcall Slot30();
    virtual int __stdcall SetSessionDesc(void* desc, unsigned long flags);
    virtual int __stdcall Slot32();
    virtual int __stdcall Slot33();
    virtual int __stdcall Slot34();
    virtual int __stdcall EnumConnections(GUID_004c9920* application, void* callback, void* context, unsigned long flags);
};

// IDirectPlayLobby-like COM interface: Connect and the connection settings.
class Lobby_4c9a70 {
public:
    virtual int __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual int __stdcall Connect(unsigned long flags, unsigned long* appId, int conn);
    virtual int __stdcall Slot04();
    virtual int __stdcall Slot05();
    virtual int __stdcall Slot06();
    virtual int __stdcall Slot07();
    virtual int __stdcall Slot08();
    virtual int __stdcall Slot09();
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall SetConnectionSettings(unsigned long a, unsigned long b, Connection_4c9a70* conn);
};

#pragma pack(push, 1)
// The HAPINET network object: the session name, the application GUID, the
// session description, the DirectPlay and lobby interfaces and the
// enumeration buffers.
struct Net_4c97b0 {
    char name[0x10];                  // +0x0
    char unknown_10[0x431 - 0x10];    // +0x10
    GUID_004c9920* guids;             // +0x431
    Conn_4ca100* conns;               // +0x435
    char* names;                      // +0x439
    GUID_004c9920 application;        // +0x43d
    char unknown_44d[0x10];           // +0x44d
    SessionDesc_4c9890 desc;          // +0x45d
    char unknown_4ad[8];              // +0x4ad
    unsigned long from;               // +0x4b5
    unsigned long to;                 // +0x4b9
    GameRec_4c9c50* games;            // +0x4bd
    DirectPlay_4c97b0* dp1;           // +0x4c1
    DirectPlay_4c97b0* dp;            // +0x4c5
    Lobby_4c9a70* lobby2;             // +0x4c9
    Lobby_4c9a70* lobby1;             // +0x4cd
    Connection_4c9a70* connection;    // +0x4d1
    int connection_size;              // +0x4d5
    char unknown_4d9[4];              // +0x4d9
    int maxPlayers;                   // +0x4dd
    int field_4e1;                    // +0x4e1
    int field_4e5;                    // +0x4e5
    int count;                        // +0x4e9
};
#pragma pack(pop)

// A connection's name, from the EnumConnections callback.
struct DPName_4ca100 {
    unsigned long dwSize;
    unsigned long dwFlags;
    char* lpszShortNameA;
    char* lpszLongNameA;
};

extern int g_enumSessionsResult;
extern int g_guaranteePackets;
extern GUID_004c9920* DAT_0050a788[4];
extern GUID_004c9920 DAT_004fdaf0;

void __cdecl HapinetTrace(const char*);
void __cdecl HapinetTrace(int);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
char* __stdcall Translate(char* text);
int __cdecl PeekKey();
int __cdecl PopKey();
char* __stdcall SkipTextLines(char* text, int n);
int __stdcall HAPINET_initmultiplay(Net_4c97b0* net, GUID_004c9920* sp, GUID_004c9920* application);

// HAPINET_GetDPErrorString: maps a DirectPlay HRESULT (the DPERR_* values of
// dplay.h, 0x88770000 + the enum ordinal) to the message TA prints. DP_OK and
// the undocumented default are Cavedog's own additions. HapinetTrace is the
// trace stub, so the argument is re-read from the stack after the call.
// FUNCTION: 0x4c9530
char* __stdcall HAPINET_GetDPErrorString(int error)
{
    HapinetTrace("HAPINET_GetDPErrorString\n");
    switch (error)
    {
    case 0x80004001:
        return "DPERR_UNSUPPORTED - The function is not available in this implementation. ";
    case 0x80004005:
        return "DPERR_GENERIC - An undefined error condition occurred. ";
    case 0x8007000e:
        return "DPERR_OUTOFMEMORY - There is insufficient memory to perform the requested operation. ";
    case 0x80070057:
        return "DPERR_INVALIDPARAMS - One or more of the parameters passed to the function are invalid.";
    case 0x88770005:
        return "DPERR_ALREADYINITIALIZED - This object is already initialized. ";
    case 0x8877000a:
        return "DPERR_ACCESSDENIED - The session is full or an incorrect password was supplied.";
    case 0x88770014:
        return "DPERR_ACTIVEPLAYERS - The requested operation cannot be performed because there are existing active players.";
    case 0x8877001e:
        return "DPERR_BUFFERTOOSMALL - The supplied buffer is not large enough to contain the requested data.";
    case 0x88770028:
        return "DPERR_CANTADDPLAYER - The player cannot be added to the session.";
    case 0x8877003c:
        return "DPERR_CANTCREATEPLAYER - A new player cannot be created. ";
    case 0x88770050:
        return "DPERR_CAPSNOTAVAILABLEYET - The capabilities of the DirectPlay object have not been determined yet.";
    case 0x8877005a:
        return "DPERR_EXCEPTION - An exception occurred when processing the request. ";
    case 0x88770078:
        return "DPERR_INVALIDFLAGS - The flags passed to this function are invalid. ";
    case 0x88770082:
        return "DPERR_INVALIDOBJECT - The DirectPlay object pointer is invalid. ";
    case 0x88770096:
        return "DPERR_INVALIDPLAYER - The player ID is not recognized as a valid player ID for this game session.";
    case 0x887700a0:
        return "DPERR_NOCAPS - The communication link underneath DirectPlay is not capable of this function.";
    case 0x887700aa:
        return "DPERR_NOCONNECTION - No communication link was established. ";
    case 0x887700be:
        return "DPERR_NOMESSAGES - There are no messages to be received. ";
    case 0x887700c8:
        return "DPERR_NONAMESERVERFOUND - No name server could be found or created. A name server must exist in order to create a player. ";
    case 0x887700d2:
        return "DPERR_NOPLAYERS - There are no active players in the session. ";
    case 0x887700dc:
        return "DPERR_NOSESSIONS - There are no existing sessions for this game. ";
    case 0x887700e6:
        return "DPERR_SENDTOOBIG - The message buffer passed to the IDirectPlay::Send method is larger than allowed. ";
    case 0x887700f0:
        return "DPERR_TIMEOUT - The operation could not be completed in the specified time. ";
    case 0x887700fa:
        return "DPERR_UNAVAILABLE - The requested service provider or session is not available. ";
    case 0x8877010e:
        return "DPERR_BUSY - The DirectPlay message queue is full. ";
    case 0x88770118:
        return "DPERR_USERCANCEL - The user canceled the connection process during a call to the IDirectPlay::Open method.";
    case 0x88770136:
        return "DPERR_SESSIONLOST - The session was lost. ";
    case 0x88770140:
        return "DPERR_UNINITIALIZED - An object was not initialized. ";
    case 0x00000000:
        return "DP_OK - The request completed successfully.";
    }
    return "DPERR_IHAVENOBLOODYIDEA - DirectPlay returned an undocumented return value.";
}

// The trace stub is empty, so the compiler would inline it into every caller
// in this file; the original calls it.
#pragma auto_inline(off)
// FUNCTION: 0x4c9740
void __cdecl HapinetTrace(int)
{
}
#pragma auto_inline(on)

// Returns the (translated) description of the last DirectPlay error: the
// text after " - " in HAPINET_GetDPErrorString's message, falling back to
// DPERR_GENERIC (0x80004005).
// FUNCTION: 0x4c9750
char* GetEnumSessionsErrorText()
{
    char* s = HAPINET_GetDPErrorString(g_enumSessionsResult);
    if (s == 0)
        s = HAPINET_GetDPErrorString(0x80004005);
    s = strstr(s, " - ");
    if (s == 0)
        return HAPINET_GetDPErrorString(0x80004005);
    return Translate(s + 3);
}

// FUNCTION: 0x4c9790
int __stdcall HAPINET_guaranteepackets(int param_1)
{
    HapinetTrace("HAPINET_guaranteepackets\n");
    int result = g_guaranteePackets;
    g_guaranteePackets = param_1;
    return result;
}

// HAPINET_sendpacket: IDirectPlay2::Send (+0x68); flags is
// DPSEND_GUARANTEED when the guaranteed-packets global is set. Returns
// 0x887700aa when there is no DirectPlay interface.
// FUNCTION: 0x4c97b0
int __stdcall HAPINET_sendpacket(Net_4c97b0* net, unsigned long from, unsigned long to, void* data, unsigned long size)
{
    HapinetTrace((int)"HAPINET_sendpacket\n");
    int result = 0x887700aa;
    if (net->dp != 0) {
        result = net->dp->Send(from, to, g_guaranteePackets != 0, data, size);
    }
    return result;
}

// HAPINET_sendpacketguaranteed: IDirectPlay2::Send (+0x68) with
// DPSEND_GUARANTEED; returns 0x887700aa when there is no DirectPlay
// interface.
// FUNCTION: 0x4c9800
int __stdcall HAPINET_sendpacketguaranteed(Net_4c97b0* net, unsigned long from, unsigned long to, void* data, unsigned long size)
{
    HapinetTrace((int)"HAPINET_sendpacketguaranteed\n");
    int result = 0x887700aa;
    if (net->dp != 0) {
        result = net->dp->Send(from, to, 1, data, size);
    }
    return result;
}

// HAPINET_receivepacket: IDirectPlay2::Receive (+0x64) with DPRECEIVE_ALL into
// the from/to player ids kept in the network object; returns
// 0x887700aa when there is no DirectPlay interface.
// FUNCTION: 0x4c9840
int __stdcall HAPINET_receivepacket(Net_4c97b0* net, void* data, unsigned long* size)
{
    HapinetTrace((int)"HAPINET_receivepacket\n");
    int result = 0x887700aa;
    if (net->dp != 0) {
        result = net->dp->Receive(&net->from, &net->to, 1, data, size);
    }
    return result;
}

// HAPINET_updategameinfo: copies the game name into the network object, fills
// the session description (DPSESSIONDESC2 at +0x45d: name and the four user
// dwords) and applies it with IDirectPlay2::SetSessionDesc (+0x7c). Returns 1
// on success.
// FUNCTION: 0x4c9890
int __stdcall HAPINET_updategameinfo(Net_4c97b0* net, char* name, char* data, int d, int c, int b, int a)
{
    HapinetTrace("HAPINET_updategameinfo\n");
    if (net->dp != 0) {
        strncpy(net->name, name, 0x10);
        net->desc.dwSize = 0x50;
        net->desc.dwUser4 = a;
        net->desc.dwUser3 = b;
        net->desc.dwUser2 = c;
        net->desc.dwUser1 = d;
        net->desc.lpszSessionName = name;
        if (net->dp->SetSessionDesc(&net->desc, 0) == 0)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x4c9920
int __stdcall HAPINET_createnewgame(Net_4c97b0* net, char* name, int a3, int a4, int a5, int a6, int a7)
{
    HapinetTrace((int)"HAPINET_createnewgame\n");
    if (net->dp != 0) {
        strncpy((char*)net, name, 0x10);
        memset(&net->desc, 0, 0x50);
        net->desc.dwMaxPlayers = net->maxPlayers;
        net->desc.guidApplication = net->application;
        net->desc.dwSize = 0x50;
        net->desc.lpszSessionName = name;
        net->desc.dwUser4 = a7;
        net->desc.dwUser3 = a6;
        net->desc.dwUser2 = a5;
        net->desc.dwUser1 = a4;
        net->desc.lpszPassword = (char*)a3;
        int r = net->dp->Slot24(&net->desc, 2);
        unsigned long size = 0x50;
        int hr = net->dp->GetSessionDesc(0, &size);
        if (hr == (int)0x8877001e) {
            void* buf = FUN_004d83b0("DP SESSION DATA2", size);
            if (buf != 0) {
                if (net->dp->GetSessionDesc(buf, &size) == 0)
                    net->desc.guidInstance = *(GUID_004c9920*)((char*)buf + 8);
                FUN_004d85a0(buf);
            }
        }
        if (r == 0)
            return 1;
    }
    return 0;
}

// FUNCTION: 0x4c9a70
int __stdcall HAPINET_createorjoinlobbygame(Net_4c97b0* net, char* password, unsigned long maxPlayers,
                           unsigned long user1, unsigned long user2, unsigned long user3,
                           unsigned long user4)
{
    HapinetTrace((int)"HAPINET_createorjoinlobbygame\n");
    int hr = 0;
    if (password != 0 || maxPlayers != 0 || user1 != 0 || user2 != 0 || user3 != 0 || user4 != 0) {
        SessionDesc_4c9890* d = net->connection->desc;
        if (password != 0) {
            d->lpszPassword[0] = 0;
            strncat(d->lpszPassword, password, 0x10);
        }
        if (d->dwMaxPlayers == 0) d->dwMaxPlayers = maxPlayers;
        if (d->dwUser1 == 0) d->dwUser1 = user1;
        if (d->dwUser2 == 0) d->dwUser2 = user2;
        if (d->dwUser3 == 0) d->dwUser3 = user3;
        if (d->dwUser4 == 0) d->dwUser4 = user4;
        hr = net->lobby1->SetConnectionSettings(0, 0, net->connection);
    }
    if (hr >= 0) {
        net->desc = *net->connection->desc;
        hr = net->lobby1->Connect(0, (unsigned long*)&net->dp, 0);
    }
    return hr >= 0;
}

// HAPINET_uninitmultiplay: closes and releases the two DirectPlay interfaces,
// releases the two lobby interfaces and frees the connection buffer.
// FUNCTION: 0x4c9b70
void __stdcall HAPINET_uninitmultiplay(Net_4c97b0* net)
{
    HapinetTrace((int)"HAPINET_uninitmultiplay\n");
    if (net->dp != 0) {
        net->dp->Close();
        net->dp->Release();
        net->dp = 0;
    }
    if (net->dp1 != 0) {
        net->dp1->Close();
        net->dp1->Release();
        net->dp1 = 0;
    }
    if (net->lobby1 != 0) {
        net->lobby1->Release();
        net->lobby1 = 0;
    }
    if (net->lobby2 != 0) {
        net->lobby2->Release();
        net->lobby2 = 0;
    }
    if (net->connection != 0) {
        FUN_004d85a0(net->connection);
        net->connection = 0;
        net->connection_size = 0;
    }
}

// HAPINET_initmultiplaydefaults: sets two network defaults (0x10 and 1500).
// FUNCTION: 0x4c9c20
void __stdcall HAPINET_initmultiplaydefaults(Net_4c97b0* net)
{
    HapinetTrace((int)"HAPINET_initmultiplaydefaults\n");
    net->maxPlayers = 0x10;
    net->field_4e1 = 0x5dc;
}

// DPENUMSESSIONSCALLBACK2 for IDirectPlay3::EnumSessions (HAPINET_getgamescallback).
// Copies a session description into the next slot of the context's game array;
// returns FALSE on DPESC_TIMEDOUT (0x1) to stop the enumeration.
// FUNCTION: 0x4c9c50
int __stdcall HAPINET_getgamescallback(SessionDesc_4c9890* lpsd, int* lpdwTimeout, unsigned int dwFlags, Net_4c97b0* ctx)
{
    HapinetTrace((int)"HAPINET_getgamescallback\n");
    if ((dwFlags & 1) == 0)
    {
        GameRec_4c9c50* rec = &ctx->games[ctx->count];
        strcpy(rec->name, lpsd->lpszSessionName);
        rec->user1 = lpsd->dwUser1;
        rec->user2 = lpsd->dwUser2;
        rec->user3 = lpsd->dwUser3;
        rec->user4 = lpsd->dwUser4;
        rec->guidInstance = lpsd->guidInstance;
        rec->maxPlayers = lpsd->dwMaxPlayers;
        ctx->count++;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4c9d10
int __stdcall HAPINET_justone(int p1, int p2, int p3, int p4, void* p5)
{
    HapinetTrace("HAPINET_justone\n");
    int* addr = (int*)p5;
    addr[0x12] = p1;
    return 0;
}

// FUNCTION: 0x4c9d30
int __stdcall HAPINET_setplayerdatabuffer(Net_4c97b0* net, unsigned long player, void* data, unsigned long size)
{
    HapinetTrace((int)"HAPINET_setplayerdatabuffer\n");
    if (net->dp != 0 && net->dp->SetPlayerData(player, data, size, 2) == 0) {
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4c9d80
int __stdcall HAPINET_getplayerdatabuffer(Net_4c97b0* net, unsigned long player, void* data, unsigned long* size)
{
    HapinetTrace((int)"HAPINET_getplayerdatabuffer\n");
    if (net->dp != 0 && net->dp->GetPlayerData(player, data, size, 0) == 0) {
        return 1;
    }
    return 0;
}

// Returns the session's current player count (DPSESSIONDESC2 dwCurrentPlayers,
// +0x2c), reading the session description into a temporary buffer.
// The HRESULT goes through a local: a direct test gives `test eax, eax`
// instead of the original's `cmp eax, ebx`.
// FUNCTION: 0x4c9dd0
int __stdcall HAPINET_getcurrentplayers(Net_4c97b0* net)
{
    int hr;
    HapinetTrace((int)"HAPINET_getcurrentplayers\n");
    int players = 0;
    if (net->dp != 0) {
        unsigned long size = 0;
        hr = net->dp->GetSessionDesc(0, &size);
        if (hr == (int)0x8877001e) {
            SessionDesc_4c9890* desc = (SessionDesc_4c9890*)FUN_004d83b0("DP SESSION DATA", size);
            hr = net->dp->GetSessionDesc(desc, &size);
            if (hr == 0)
                players = desc->dwCurrentPlayers;
            FUN_004d85a0(desc);
        }
    }
    return players;
}

// HAPINET_getgames: enumerates the available DirectPlay sessions. The
// EnumSessions callback (HAPINET_getgamescallback) appends each session to the buffer at
// net+0x4bd and bumps the counter at net+0x4e9. The Windows message queue is
// pumped while the enumeration runs; ESC aborts it. Returns the number of
// sessions collected, or -1 when there is no DirectPlay interface or the
// enumeration failed.
// FUNCTION: 0x4c9e50
int __stdcall HAPINET_getgames(Net_4c97b0* net, void* sessions, int unused)
{
    HapinetTrace("HAPINET_getgames\n");
    net->count = 0;
    if (net->dp == 0)
        return -1;
    SessionDesc_4c9890 desc;
    memset(&desc, 0, sizeof(desc));
    desc.dwSize = 0x50;
    desc.guidApplication = net->application;
    desc.guidInstance = net->desc.guidInstance;
    net->games = (GameRec_4c9c50*)sessions;
    int result;
    do {
        result = net->dp->EnumSessions(&desc, net->field_4e1, HAPINET_getgamescallback, net, 0x81);
        g_enumSessionsResult = result;
        MSG msg;
        if (PeekMessageA(&msg, 0, 0, 0, 0)) {
            GetMessageA(&msg, 0, 0, 0);
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if (PeekKey()) {
            if (PopKey() == 0x1b)
                break;
        }
    } while (result == 0x8877015e);
    if (result == 0)
        return net->count;
    return -1;
}

// FUNCTION: 0x4c9f90
int __stdcall HAPINET_quitgame(Net_4c97b0* net)
{
    HapinetTrace((int)"HAPINET_quitgame\n");
    if (net->dp != 0 && net->dp->Close() != 0) {
        return 0;
    }
    return 1;
}

// HAPINET_joingame: fills the session description at +0x45d from the stored
// application GUID (+0x43d) and the requested instance GUID, opens it, then
// re-reads the negotiated description and copies the session name into the
// object's name buffer.
// FUNCTION: 0x4c9fd0
int __stdcall HAPINET_joingame(Net_4c97b0* net, GUID_004c9920 guid)
{
    HapinetTrace("HAPINET_joingame\n");
    // Nested with one trailing return 0: early returns give `size` its own slot.
    if (net->dp != 0) {
        memset(&net->desc, 0, 0x50);
        net->desc.dwSize = 0x50;
        net->desc.guidApplication = net->application;
        net->desc.guidInstance = guid;
        if (net->dp->Slot24(&net->desc, 1) == 0) {
            unsigned long size = 0x50;
            if (net->dp->GetSessionDesc(0, &size) == (int)0x8877001e) {
                SessionDesc_4c9890* p = (SessionDesc_4c9890*)FUN_004d83b0("DP SESSION DATA2", size);
                if (p != 0) {
                    if (net->dp->GetSessionDesc(p, &size) == 0) {
                        net->desc.guidInstance = p->guidInstance;
                        if (p->lpszSessionName != 0)
                            lstrcpynA(net->name, p->lpszSessionName, 0x11);
                    }
                    FUN_004d85a0(p);
                }
            }
            return 1;
        }
    }
    return 0;
}

// DirectPlay EnumConnections callback: ignores the session's own service
// providers (the four GUIDs at 0x50a788), records the connection's GUID, copies
// the connection data into a freshly allocated block and stores the name.
// FUNCTION: 0x4ca100
int __stdcall HAPINET_enumconnections(GUID_004c9920* guid, void* connection, unsigned long size,
                           DPName_4ca100* name, unsigned long flags, Net_4c97b0* net)
{
    char local[200];
    HapinetTrace((int)"HAPINET_enumconnections\n");
    for (unsigned int i = 0; i < 4; i++) {
        if (memcmp(guid, DAT_0050a788[i], sizeof(GUID_004c9920)) == 0)
            return 1;
    }
    net->guids[net->field_4e5] = *guid;
    net->conns[net->field_4e5].data = FUN_004d83b0("DPLAY CONNECTION", size);
    if (net->conns[net->field_4e5].data == 0)
        return 0;
    memcpy(net->conns[net->field_4e5].data, connection, size);
    net->conns[net->field_4e5].size = size;
    sprintf(local, "%s", name->lpszShortNameA);
    char* dest = SkipTextLines(net->names, net->field_4e5);
    strcpy(dest, local);
    net->field_4e5++;
    return 1;
}

// FUNCTION: 0x4ca250
int __stdcall HAPINET_getconnections(Net_4c97b0* net, int a, int b, int c, GUID_004c9920* application)
{
    HapinetTrace((int)"HAPINET_getconnections\n");
    if (net->dp == 0) {
        GUID_004c9920 sp = DAT_004fdaf0;
        GUID_004c9920 app = *application;
        if (!HAPINET_initmultiplay(net, &sp, &app)) {
            return 0;
        }
    }
    net->guids = (GUID_004c9920*)a;
    net->field_4e5 = 0;
    net->conns = (Conn_4ca100*)b;
    net->names = (char*)c;
    int hr = net->dp->EnumConnections(application, HAPINET_enumconnections, net, 1);
    return hr == 0 ? 1 : 0;
}
