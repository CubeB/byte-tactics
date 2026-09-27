// Decompiled by Opus. Names are provisional.

// DirectX 5's DPERR_UNINITIALIZED, MAKE_DPHRESULT(320); the toolchain's
// DirectX 3 <dplay.h> does not define it.
#define DPERR_UNINITIALIZED_004ca990 0x88770140

// COM interface (IDirectPlay2-like); slot 18 (+0x48) is GetPlayerAddress.
class DirectPlay_004ca990 {
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
    virtual int __stdcall GetPlayerAddress(unsigned long player, void* data, unsigned long* size);
};

struct Net_004ca990 {
    char unknown_0[0x4];
    DirectPlay_004ca990* dp;           // +0x4
};

void FUN_004c9740(int);

// FUNCTION: 0x4ca990
int __stdcall FUN_004ca990(Net_004ca990* net, unsigned long player, void* data, unsigned long* size)
{
    FUN_004c9740((int)"HAPINET_getplayeraddress\n");
    if (net->dp != 0)
        return net->dp->GetPlayerAddress(player, data, size);
    return DPERR_UNINITIALIZED_004ca990;
}
