// Decompiled by Opus. Names are provisional.

// COM interface (IDirectPlay3-like); slot 29 (+0x74) is SetPlayerData.
class DirectPlay_4c9d30 {
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
    virtual int __stdcall SetPlayerData(unsigned long player, void* data, unsigned long size, unsigned long flags);
};

#pragma pack(push, 1)
struct Net_4c9d30 {
    char unknown_0[0x4c5];
    DirectPlay_4c9d30* dp;           // +0x4c5
};
#pragma pack(pop)

void __cdecl HapinetTrace(int);

// FUNCTION: 0x4c9d30
int __stdcall HAPINET_setplayerdatabuffer(Net_4c9d30* net, unsigned long player, void* data, unsigned long size)
{
    HapinetTrace((int)"HAPINET_setplayerdatabuffer\n");
    if (net->dp != 0 && net->dp->SetPlayerData(player, data, size, 2) == 0) {
        return 1;
    }
    return 0;
}
