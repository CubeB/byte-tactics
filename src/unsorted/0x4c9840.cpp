// Decompiled by Opus. Names are provisional.
// HAPINET_receivepacket: IDirectPlay2::Receive (+0x64) with DPRECEIVE_ALL into
// the from/to player ids kept in the network object; returns
// 0x887700aa when there is no DirectPlay interface.

// COM interface (IDirectPlay2-like); slot 25 (+0x64) is Receive.
class DirectPlay_4c9840 {
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
    virtual int __stdcall Receive(unsigned long* from, unsigned long* to, unsigned long flags, void* data, unsigned long* size);
};

#pragma pack(push, 1)
struct Net_4c9840 {
    char unknown_0[0x4b5];
    unsigned long from;              // +0x4b5
    unsigned long to;                // +0x4b9
    char unknown_4bd[8];
    DirectPlay_4c9840* dp;           // +0x4c5
};
#pragma pack(pop)

void FUN_004c9740(int);

// FUNCTION: 0x4c9840
int __stdcall FUN_004c9840(Net_4c9840* net, void* data, unsigned long* size)
{
    FUN_004c9740((int)"HAPINET_receivepacket\n");
    int result = 0x887700aa;
    if (net->dp != 0) {
        result = net->dp->Receive(&net->from, &net->to, 1, data, size);
    }
    return result;
}
