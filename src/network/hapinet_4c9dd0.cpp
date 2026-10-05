// Decompiled by Opus. Names are provisional.
// Returns the session's current player count (DPSESSIONDESC2 dwCurrentPlayers,
// +0x2c), reading the session description into a temporary buffer.
// The HRESULT goes through a local: a direct test gives `test eax, eax`
// instead of the original's `cmp eax, ebx`.

// COM interface (IDirectPlay3-like); slot 22 (+0x58) is GetSessionDesc.
class DirectPlay_4c9dd0 {
public:
    virtual int __stdcall Slot00();
    virtual int __stdcall Slot01();
    virtual int __stdcall Slot02();
    virtual int __stdcall Slot03();
    virtual int __stdcall Slot04();
    virtual int __stdcall Slot05();
    virtual int __stdcall Slot06();
    virtual int __stdcall Slot07();
    virtual int __stdcall Slot08();
    virtual int __stdcall Slot09();
    virtual int __stdcall Slot10();
    virtual int __stdcall Slot11();
    virtual int __stdcall Slot12();
    virtual int __stdcall Slot13();
    virtual int __stdcall Slot14();
    virtual int __stdcall Slot15();
    virtual int __stdcall Slot16();
    virtual int __stdcall Slot17();
    virtual int __stdcall Slot18();
    virtual int __stdcall Slot19();
    virtual int __stdcall Slot20();
    virtual int __stdcall Slot21();
    virtual int __stdcall GetSessionDesc(void* data, unsigned long* size);
};

struct SessionDesc_4c9dd0 {
    char unknown_0[0x2c];
    int currentPlayers;              // +0x2c
};

#pragma pack(push, 1)
struct Net_4c9dd0 {
    char unknown_0[0x4c5];
    DirectPlay_4c9dd0* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl HapinetTrace(int);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4c9dd0
int __stdcall HAPINET_getcurrentplayers(Net_4c9dd0* net)
{
    int hr;
    HapinetTrace((int)"HAPINET_getcurrentplayers\n");
    int players = 0;
    if (net->dp != 0) {
        unsigned long size = 0;
        hr = net->dp->GetSessionDesc(0, &size);
        if (hr == (int)0x8877001e) {
            SessionDesc_4c9dd0* desc = (SessionDesc_4c9dd0*)FUN_004d83b0("DP SESSION DATA", size);
            hr = net->dp->GetSessionDesc(desc, &size);
            if (hr == 0)
                players = desc->currentPlayers;
            FUN_004d85a0(desc);
        }
    }
    return players;
}
