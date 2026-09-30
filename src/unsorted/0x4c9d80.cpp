// Decompiled by Opus. Names are provisional.

// COM interface (IDirectPlay3-like); slot 20 (+0x50) is GetPlayerData.
class DirectPlay_4c9d80 {
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
    virtual int __stdcall GetPlayerData(unsigned long player, void* data, unsigned long* size, unsigned long flags);
};

#pragma pack(push, 1)
struct Net_4c9d80 {
    char unknown_0[0x4c5];
    DirectPlay_4c9d80* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);

// FUNCTION: 0x4c9d80
int __stdcall FUN_004c9d80(Net_4c9d80* net, unsigned long player, void* data, unsigned long* size)
{
    FUN_004c9740((int)"HAPINET_getplayerdatabuffer\n");
    if (net->dp != 0 && net->dp->GetPlayerData(player, data, size, 0) == 0) {
        return 1;
    }
    return 0;
}
