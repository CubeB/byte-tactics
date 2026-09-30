// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// HAPINET_initmultiplay: creates the DirectPlay object from the service
// provider GUID in sp, stores the application GUID and answers whether the
// queried DirectPlay interface came back.

#include <windows.h>

struct Guid_4ca5d0 {
    unsigned long data[4];
};

// COM interface (IDirectPlay-like); slot 0 (+0x00) is QueryInterface.
class DirectPlay_4ca5d0 {
public:
    virtual int __stdcall QueryInterface(const void* iid, void** out);
};

#pragma pack(push, 1)
struct Net_4ca5d0 {
    char unknown_0[0x43d];
    Guid_4ca5d0 application;           // +0x43d
    Guid_4ca5d0 sp;                    // +0x44d
    char unknown_45d[0x4ad - 0x45d];
    int field_4ad;                     // +0x4ad
    char unknown_4b1[0x4c1 - 0x4b1];
    DirectPlay_4ca5d0* dp;             // +0x4c1
    DirectPlay_4ca5d0* dp2;            // +0x4c5
    int field_4c9;                     // +0x4c9
    int field_4cd;                     // +0x4cd
    int field_4d1;                     // +0x4d1
    int field_4d5;                     // +0x4d5
};
#pragma pack(pop)

extern int DAT_0050a780;
extern GUID DAT_004fcd78;

void __cdecl FUN_004c9740(int);
int FUN_004b6220();
extern "C" HRESULT __stdcall DirectPlayCreate(GUID* sp, void** dp, void* unknown);

// FUNCTION: 0x4ca5d0
int __stdcall FUN_004ca5d0(Net_4ca5d0* net, Guid_4ca5d0* sp, Guid_4ca5d0* application)
{
    FUN_004c9740((int)"HAPINET_initmultiplay\n");
    DAT_0050a780 = 0;
    net->field_4ad = *(int*)(FUN_004b6220() + 0x40);
    net->application = *application;
    net->field_4c9 = 0;
    net->field_4cd = 0;
    net->field_4d1 = 0;
    net->field_4d5 = 0;
    net->dp = 0;
    net->dp2 = 0;
    net->sp = *sp;
    int hr = DirectPlayCreate((GUID*)&net->sp, (void**)&net->dp, 0);
    if (hr != 0)
        return 0;
    hr = net->dp->QueryInterface(&DAT_004fcd78, (void**)&net->dp2);
    return hr == 0 ? 1 : 0;
}
