// Decompiled by Opus. Names are provisional.
// HAPINET_setplayername: IDirectPlay2::SetPlayerName (+0x78); returns
// 0x88770140 when there is no DirectPlay interface.

// COM interface (IDirectPlay2-like); slot 30 (+0x78) is SetPlayerName.
class DirectPlay_4ca800 {
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
    virtual int __stdcall Slot22();
    virtual int __stdcall Slot23();
    virtual int __stdcall Slot24();
    virtual int __stdcall Slot25();
    virtual int __stdcall Slot26();
    virtual int __stdcall Slot27();
    virtual int __stdcall Slot28();
    virtual int __stdcall Slot29();
    virtual int __stdcall SetPlayerName(unsigned long id, void* name, unsigned long flags);
};

#pragma pack(push, 1)
struct Net_4ca800 {
    char unknown_0[0x4c5];
    DirectPlay_4ca800* dp;           // +0x4c5
};
#pragma pack(pop)

void FUN_004c9740(int);

// FUNCTION: 0x4ca800
int __stdcall FUN_004ca800(Net_4ca800* net, unsigned long id, void* name, unsigned long flags)
{
    FUN_004c9740((int)"HAPINET_setplayername\n");
    if (net->dp != 0) {
        return net->dp->SetPlayerName(id, name, flags);
    }
    return 0x88770140;
}
