// Decompiled by Opus. Names are provisional.
#include <windows.h>

// DPLAYX.dll ordinal 2 (DirectPlayEnumerateA from the DirectX 5 dplay.h).
typedef BOOL (__stdcall* EnumSpCallback_4ca400)(GUID* sp, char* name, DWORD major, DWORD minor, void* context);
extern "C" HRESULT __stdcall DirectPlayEnumerateA(EnumSpCallback_4ca400 callback, void* context);

#pragma pack(push, 1)
struct Net_4ca400 {
    char unknown_0[0x431];
    int field_431;                   // +0x431
    int field_435;                   // +0x435
    int field_439;                   // +0x439
    char unknown_43d[0x4e5 - 0x43d];
    int field_4e5;                   // +0x4e5
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);
BOOL __stdcall FUN_004ca330(GUID* sp, char* name, DWORD major, DWORD minor, void* context);

// FUNCTION: 0x4ca400
int __stdcall FUN_004ca400(Net_4ca400* net, int a, int c)
{
    FUN_004c9740((int)"HAPINET_getproviders\n");
    net->field_4e5 = 0;
    net->field_431 = a;
    net->field_439 = c;
    DirectPlayEnumerateA(FUN_004ca330, net);
    return 1;
}
