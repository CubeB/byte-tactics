// Decompiled by Opus. Names are provisional.
// HAPINET_getplayername: IDirectPlay2::GetPlayerName (+0x54); returns
// 0x88770140 when there is no DirectPlay interface.

// COM interface (IDirectPlay2-like); slot 21 (+0x54) is GetPlayerName.
class DirectPlay_4ca7c0 {
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
    virtual int __stdcall GetPlayerName(unsigned long id, void* data, unsigned long* size);
};

#pragma pack(push, 1)
struct Net_4ca7c0 {
    char unknown_0[0x4c5];
    DirectPlay_4ca7c0* dp;           // +0x4c5
};
#pragma pack(pop)

void FUN_004c9740(int);

// FUNCTION: 0x4ca7c0
int __stdcall FUN_004ca7c0(Net_4ca7c0* net, unsigned long id, void* data, unsigned long* size)
{
    FUN_004c9740((int)"HAPINET_getplayername\n");
    if (net->dp != 0) {
        return net->dp->GetPlayerName(id, data, size);
    }
    return 0x88770140;
}
