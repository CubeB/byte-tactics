// Decompiled by Opus. Names are provisional.
// HAPINET_enumaddress: IDirectPlayLobby::EnumAddress (+0x14) on the lobby
// interface; returns 0x88770140 when there is none. Same shape as 0x4ca840.

// COM interface (IDirectPlayLobby); slot 5 (+0x14) is EnumAddress.
class DirectPlayLobby_4ca8c0 {
public:
    virtual int __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual int __stdcall Connect(unsigned long flags, void** dp, void* outer);
    virtual int __stdcall CreateAddress(void* sp, void* type, void* data, unsigned long dataSize, void* address, unsigned long* addressSize);
    virtual int __stdcall EnumAddress(void* callback, void* address, unsigned long size, void* context);
};

#pragma pack(push, 1)
struct Net_4ca8c0 {
    char unknown_0[0x4cd];
    DirectPlayLobby_4ca8c0* lobby;   // +0x4cd
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);

// FUNCTION: 0x4ca8c0
int __stdcall FUN_004ca8c0(Net_4ca8c0* net, void* callback, void* address, unsigned long size, void* context)
{
    FUN_004c9740((int)"HAPINET_enumaddress\n");
    if (net->lobby != 0) {
        return net->lobby->EnumAddress(callback, address, size, context);
    }
    return 0x88770140;
}
