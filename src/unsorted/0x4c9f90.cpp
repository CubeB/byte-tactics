// Decompiled by Opus. Names are provisional.

// COM interface (IDirectPlay-like); slot 4 (+0x10) is Close.
class DirectPlay_4c9f90 {
public:
    virtual int __stdcall Slot00();
    virtual int __stdcall Slot01();
    virtual int __stdcall Slot02();
    virtual int __stdcall Slot03();
    virtual int __stdcall Close();
};

#pragma pack(push, 1)
struct Net_4c9f90 {
    char unknown_0[0x4c5];
    DirectPlay_4c9f90* dp;           // +0x4c5
};
#pragma pack(pop)

void FUN_004c9740(int);

// FUNCTION: 0x4c9f90
int __stdcall FUN_004c9f90(Net_4c9f90* net)
{
    FUN_004c9740((int)"HAPINET_quitgame\n");
    if (net->dp != 0 && net->dp->Close() != 0) {
        return 0;
    }
    return 1;
}
