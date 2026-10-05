// Decompiled by Opus. Names are provisional.
// HAPINET_sendpacketguaranteed: IDirectPlay2::Send (+0x68) with
// DPSEND_GUARANTEED; returns 0x887700aa when there is no DirectPlay
// interface.

// COM interface (IDirectPlay2-like); slot 26 (+0x68) is Send.
class DirectPlay_4c9800 {
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
    virtual int __stdcall Send(unsigned long from, unsigned long to, unsigned long flags, void* data, unsigned long size);
};

#pragma pack(push, 1)
struct Net_4c9800 {
    char unknown_0[0x4c5];
    DirectPlay_4c9800* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl HapinetTrace(int);

// FUNCTION: 0x4c9800
int __stdcall HAPINET_sendpacketguaranteed(Net_4c9800* net, unsigned long from, unsigned long to, void* data, unsigned long size)
{
    HapinetTrace((int)"HAPINET_sendpacketguaranteed\n");
    int result = 0x887700aa;
    if (net->dp != 0) {
        result = net->dp->Send(from, to, 1, data, size);
    }
    return result;
}
