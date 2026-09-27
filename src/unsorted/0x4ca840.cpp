// Decompiled by Opus. Names are provisional.
// HAPINET_enumplayers: IDirectPlay2::EnumPlayers (+0x30); returns 0x88770140
// when there is no DirectPlay interface.

// COM interface (IDirectPlay2-like); slot 12 (+0x30) is EnumPlayers.
class DirectPlay_4ca840 {
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
    virtual int __stdcall EnumPlayers(void* session, void* callback, void* context, unsigned long flags);
};

#pragma pack(push, 1)
struct Net_4ca840 {
    char unknown_0[0x4c5];
    DirectPlay_4ca840* dp;           // +0x4c5
};
#pragma pack(pop)

void FUN_004c9740(int);

// FUNCTION: 0x4ca840
int __stdcall FUN_004ca840(Net_4ca840* net, void* session, void* callback, void* context, unsigned long flags)
{
    FUN_004c9740((int)"HAPINET_enumplayers\n");
    if (net->dp != 0) {
        return net->dp->EnumPlayers(session, callback, context, flags);
    }
    return 0x88770140;
}
