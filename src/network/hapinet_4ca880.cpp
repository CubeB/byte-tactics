// Decompiled by Opus. Names are provisional.
// HAPINET_createcompoundaddress: IDirectPlayLobby2::CreateCompoundAddress
// (+0x38) on the lobby interface at +0x4cd; returns 0x88770140 when there
// is none.

// COM interface (IDirectPlayLobby2-like); slot 14 (+0x38) is CreateCompoundAddress.
class DirectPlayLobby_4ca880 {
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
    virtual int __stdcall CreateCompoundAddress(void* elements, unsigned long count, void* address,
                                                unsigned long* size);
};

#pragma pack(push, 1)
struct Net_4ca880 {
    char unknown_0[0x4cd];
    DirectPlayLobby_4ca880* lobby;   // +0x4cd
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);

// FUNCTION: 0x4ca880
int __stdcall FUN_004ca880(Net_4ca880* net, void* elements, unsigned long count, void* address,
                           unsigned long* size)
{
    FUN_004c9740((int)"HAPINET_createcompoundaddress\n");
    if (net->lobby != 0) {
        return net->lobby->CreateCompoundAddress(elements, count, address, size);
    }
    return 0x88770140;
}
