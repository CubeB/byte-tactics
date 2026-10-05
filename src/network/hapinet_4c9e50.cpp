// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// HAPINET_getgames: enumerates the available DirectPlay sessions. The
// EnumSessions callback (HAPINET_getgamescallback) appends each session to the buffer at
// net+0x4bd and bumps the counter at net+0x4e9. The Windows message queue is
// pumped while the enumeration runs; ESC aborts it. Returns the number of
// sessions collected, or -1 when there is no DirectPlay interface or the
// enumeration failed.
#include <windows.h>
#include <string.h>

struct Guid_004c9e50 {
    unsigned long data[4];
};

// COM interface (IDirectPlay2-like); slot 13 (+0x34) is EnumSessions.
class DirectPlay_004c9e50 {
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
    virtual int __stdcall EnumSessions(void* desc, unsigned long timeout, void* callback, void* context, unsigned long flags);
};

// DPSESSIONDESC2
struct SessionDesc_004c9e50 {
    unsigned long size;              // +0x00
    unsigned long flags;             // +0x04
    Guid_004c9e50 instance;          // +0x08
    Guid_004c9e50 application;       // +0x18
    char unknown_28[0x28];           // +0x28
};

#pragma pack(push, 1)
struct Net_004c9e50 {
    char unknown_0[0x43d];
    Guid_004c9e50 application;       // +0x43d
    char unknown_44d[0x465 - 0x44d];
    Guid_004c9e50 instance;          // +0x465
    char unknown_475[0x4bd - 0x475];
    void* sessions;                  // +0x4bd
    char unknown_4c1[0x4c5 - 0x4c1];
    DirectPlay_004c9e50* dp;         // +0x4c5
    char unknown_4c9[0x4e1 - 0x4c9];
    unsigned long timeout;           // +0x4e1
    char unknown_4e5[0x4e9 - 0x4e5];
    int count;                       // +0x4e9
};
#pragma pack(pop)

extern int g_enumSessionsResult;

void __cdecl HapinetTrace(const char* text);
int __stdcall HAPINET_getgamescallback();
int __cdecl FUN_004c1b00();
int __cdecl FUN_004c1ab0();

// FUNCTION: 0x4c9e50
int __stdcall HAPINET_getgames(Net_004c9e50* net, void* sessions, int unused)
{
    HapinetTrace("HAPINET_getgames\n");
    net->count = 0;
    if (net->dp == 0)
        return -1;
    SessionDesc_004c9e50 desc;
    memset(&desc, 0, sizeof(desc));
    desc.size = 0x50;
    desc.application = net->application;
    desc.instance = net->instance;
    net->sessions = sessions;
    int result;
    do {
        result = net->dp->EnumSessions(&desc, net->timeout, HAPINET_getgamescallback, net, 0x81);
        g_enumSessionsResult = result;
        MSG msg;
        if (PeekMessageA(&msg, 0, 0, 0, 0)) {
            GetMessageA(&msg, 0, 0, 0);
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
        if (FUN_004c1b00()) {
            if (FUN_004c1ab0() == 0x1b)
                break;
        }
    } while (result == 0x8877015e);
    if (result == 0)
        return net->count;
    return -1;
}
