// Decompiled by Opus. Names are provisional.

// COM interface (IDirectPlay2-like); slot 9 (+0x24) is DestroyPlayer.
class DirectPlay_4ca780 {
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
    virtual int __stdcall DestroyPlayer(unsigned long id);
};

#pragma pack(push, 1)
struct Net_4ca780 {
    char unknown_0[0x4c5];
    DirectPlay_4ca780* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl HapinetTrace(int);

// FUNCTION: 0x4ca780
int __stdcall HAPINET_removeplayer(Net_4ca780* net, unsigned long id)
{
    HapinetTrace((int)"HAPINET_removeplayer\n");
    if (net->dp != 0) {
        int hr = net->dp->DestroyPlayer(id);
        return hr == 0 ? 1 : 0;
    }
    return 0;
}
