// Decompiled by Opus. Names are provisional.

struct Guid_4ca250 {
    unsigned long data[4];
};

// COM interface (IDirectPlay3-like); slot 35 (+0x8c) is EnumConnections.
class DirectPlay_4ca250 {
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
    virtual int __stdcall Slot31();
    virtual int __stdcall Slot32();
    virtual int __stdcall Slot33();
    virtual int __stdcall Slot34();
    virtual int __stdcall EnumConnections(Guid_4ca250* application, void* callback, void* context, unsigned long flags);
};

#pragma pack(push, 1)
struct Net_4ca250 {
    char unknown_0[0x431];
    int field_431;                   // +0x431
    int field_435;                   // +0x435
    int field_439;                   // +0x439
    char unknown_43d[0x4c5 - 0x43d];
    DirectPlay_4ca250* dp;           // +0x4c5
    char unknown_4c9[0x4e5 - 0x4c9];
    int field_4e5;                   // +0x4e5
};
#pragma pack(pop)

extern Guid_4ca250 DAT_004fdaf0;

void __cdecl FUN_004c9740(int);
int __stdcall FUN_004ca5d0(Net_4ca250* net, Guid_4ca250* sp, Guid_4ca250* application);
int __stdcall FUN_004ca100(Guid_4ca250* sp, void* connection, unsigned long size, void* name, unsigned long flags, void* context);

// FUNCTION: 0x4ca250
int __stdcall FUN_004ca250(Net_4ca250* net, int a, int b, int c, Guid_4ca250* application)
{
    FUN_004c9740((int)"HAPINET_getconnections\n");
    if (net->dp == 0) {
        Guid_4ca250 sp = DAT_004fdaf0;
        Guid_4ca250 app = *application;
        if (!FUN_004ca5d0(net, &sp, &app)) {
            return 0;
        }
    }
    net->field_431 = a;
    net->field_4e5 = 0;
    net->field_435 = b;
    net->field_439 = c;
    int hr = net->dp->EnumConnections(application, FUN_004ca100, net, 1);
    return hr == 0 ? 1 : 0;
}
