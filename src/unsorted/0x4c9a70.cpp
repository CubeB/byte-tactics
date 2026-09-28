// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

// DirectPlay session description (DPSESSIONDESC2).
struct DPSessionDesc_4c9a70 {
    unsigned long size;              // +0x00
    unsigned long flags;             // +0x04
    char guidInstance[0x10];         // +0x08
    char guidApplication[0x10];      // +0x18
    unsigned long maxPlayers;        // +0x28
    unsigned long currentPlayers;    // +0x2c
    char* sessionName;               // +0x30
    char* password;                  // +0x34
    unsigned long reserved1;         // +0x38
    unsigned long reserved2;         // +0x3c
    unsigned long user1;             // +0x40
    unsigned long user2;             // +0x44
    unsigned long user3;             // +0x48
    unsigned long user4;             // +0x4c
};

// Buffer holding the connection settings; +8 points at the session description.
struct Connection_4c9a70 {
    char unknown_0[8];
    DPSessionDesc_4c9a70* desc;      // +0x08
};

// IDirectPlayLobby-like interface; slot 3 (+0xc) is Connect, slot 12 (+0x30)
// is SetConnectionSettings.
class Lobby_4c9a70 {
public:
    virtual int __stdcall Slot00();
    virtual int __stdcall Slot01();
    virtual int __stdcall Slot02();
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
struct Net_4c9a70 {
    char unknown_0[0x45d];
    DPSessionDesc_4c9a70 desc;       // +0x45d
    char unknown_4ad[0x4c5 - 0x4ad];
    void* dp;                        // +0x4c5
    void* field_4c9;                 // +0x4c9
    Lobby_4c9a70* lobby;             // +0x4cd
    Connection_4c9a70* connection;   // +0x4d1
    int connection_size;             // +0x4d5
};
#pragma pack(pop)

void FUN_004c9740(int);

// FUNCTION: 0x4c9a70
int __stdcall FUN_004c9a70(Net_4c9a70* net, char* password, unsigned long maxPlayers,
                           unsigned long user1, unsigned long user2, unsigned long user3,
                           unsigned long user4)
{
    FUN_004c9740((int)"HAPINET_createorjoinlobbygame\n");
    int hr = 0;
    if (password != 0 || maxPlayers != 0 || user1 != 0 || user2 != 0 || user3 != 0 || user4 != 0) {
        DPSessionDesc_4c9a70* d = net->connection->desc;
        if (password != 0) {
            d->password[0] = 0;
            strncat(d->password, password, 0x10);
        }
        if (d->maxPlayers == 0) d->maxPlayers = maxPlayers;
        if (d->user1 == 0) d->user1 = user1;
        if (d->user2 == 0) d->user2 = user2;
        if (d->user3 == 0) d->user3 = user3;
        if (d->user4 == 0) d->user4 = user4;
        hr = net->lobby->SetConnectionSettings(0, 0, net->connection);
    }
    if (hr >= 0) {
        net->desc = *net->connection->desc;
        hr = net->lobby->Connect(0, (unsigned long*)&net->dp, 0);
    }
    return hr >= 0;
}
