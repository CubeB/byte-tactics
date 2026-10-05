// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// HAPINET_joingame: fills the session description at +0x45d from the stored
// application GUID (+0x43d) and the requested instance GUID, opens it, then
// re-reads the negotiated description and copies the session name into the
// object's name buffer.
// The body is nested in `if (net->dp != 0)` with a single trailing `return 0`
// and `return 1` inside: writing the two failures as early `return 0`s makes
// MSVC give the `size` local its own slot (`push ecx`) instead of reusing the
// first parameter's home.
#include <windows.h>
#include <string.h>

// COM interface (IDirectPlay-like). The methods are __stdcall so `this` is
// pushed on the stack, as in the original.
class DirectPlay_4c9fd0 {
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
    virtual int __stdcall GetSessionDesc(void* data, unsigned long* size);
    virtual int __stdcall Slot23();
    virtual int __stdcall Slot24(void* desc, unsigned long flags);
};

struct Guid_4c9fd0 {
    unsigned long d1;
    unsigned long d2;
    unsigned long d3;
    unsigned long d4;
};

struct SessionDesc_4c9fd0 {
    unsigned long dwSize;                 // +0x0
    unsigned long dwFlags;                // +0x4
    Guid_4c9fd0 guidInstance;             // +0x8
    Guid_4c9fd0 guidApplication;          // +0x18
    unsigned long dwMaxPlayers;           // +0x28
    unsigned long dwCurrentPlayers;       // +0x2c
    char* lpszSessionName;                // +0x30
    char unknown_34[0x50 - 0x34];
};

#pragma pack(push, 1)
struct Net_4c9fd0 {
    char name[0x43d];                     // +0x0
    Guid_4c9fd0 appGuid;                  // +0x43d
    char unknown_44d[0x10];               // +0x44d
    SessionDesc_4c9fd0 desc;              // +0x45d
    char unknown_4ad[0x18];               // +0x4ad
    DirectPlay_4c9fd0* dp;                // +0x4c5
};
#pragma pack(pop)

void __cdecl HapinetTrace(const char*);
void* __cdecl FUN_004d83b0(char* name, unsigned long size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4c9fd0
int __stdcall HAPINET_joingame(Net_4c9fd0* net, Guid_4c9fd0 guid)
{
    HapinetTrace("HAPINET_joingame\n");
    if (net->dp != 0) {
        memset(&net->desc, 0, 0x50);
        net->desc.dwSize = 0x50;
        net->desc.guidApplication = net->appGuid;
        net->desc.guidInstance = guid;
        if (net->dp->Slot24(&net->desc, 1) == 0) {
            unsigned long size = 0x50;
            if (net->dp->GetSessionDesc(0, &size) == (int)0x8877001e) {
                SessionDesc_4c9fd0* p = (SessionDesc_4c9fd0*)FUN_004d83b0("DP SESSION DATA2", size);
                if (p != 0) {
                    if (net->dp->GetSessionDesc(p, &size) == 0) {
                        net->desc.guidInstance = p->guidInstance;
                        if (p->lpszSessionName != 0)
                            lstrcpynA(net->name, p->lpszSessionName, 0x11);
                    }
                    FUN_004d85a0(p);
                }
            }
            return 1;
        }
    }
    return 0;
}
