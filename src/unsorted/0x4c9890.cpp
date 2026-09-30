// Decompiled by Opus. Names are provisional.
// HAPINET_updategameinfo: copies the game name into the network object, fills
// the session description (DPSESSIONDESC2 at +0x45d: name and the four user
// dwords) and applies it with IDirectPlay2::SetSessionDesc (+0x7c). Returns 1
// on success.
#include <string.h>

// COM interface (IDirectPlay2-like); slot 31 (+0x7c) is SetSessionDesc.
class DirectPlay_4c9890 {
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
    virtual int __stdcall Slot30();
    virtual int __stdcall SetSessionDesc(void* desc, unsigned long flags);
};

// DPSESSIONDESC2
struct SessionDesc_4c9890 {
    unsigned long size;              // +0x0
    char unknown_4[0x30 - 0x4];
    char* name;                      // +0x30
    char unknown_34[0x40 - 0x34];
    unsigned long user1;             // +0x40
    unsigned long user2;             // +0x44
    unsigned long user3;             // +0x48
    unsigned long user4;             // +0x4c
};

#pragma pack(push, 1)
struct Net_4c9890 {
    char name[0x10];                 // +0x0
    char unknown_10[0x45d - 0x10];
    SessionDesc_4c9890 desc;         // +0x45d
    char unknown_4ad[0x4c5 - 0x4ad];
    DirectPlay_4c9890* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl FUN_004c9740(const char* fmt, ...);

// FUNCTION: 0x4c9890
int __stdcall FUN_004c9890(Net_4c9890* net, char* name, char* data, int d, int c, int b, int a)
{
    FUN_004c9740("HAPINET_updategameinfo\n");
    if (net->dp != 0) {
        strncpy(net->name, name, 0x10);
        net->desc.size = 0x50;
        net->desc.user4 = a;
        net->desc.user3 = b;
        net->desc.user2 = c;
        net->desc.user1 = d;
        net->desc.name = name;
        if (net->dp->SetSessionDesc(&net->desc, 0) == 0)
            return 1;
    }
    return 0;
}
