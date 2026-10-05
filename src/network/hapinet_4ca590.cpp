// Decompiled by Sonnet. Names are provisional.

void __cdecl FUN_004c9740(int param_1);

class Iface_4ca590 {
public:
    virtual int __stdcall Slot00(); virtual int __stdcall Slot01(); virtual int __stdcall Slot02();
    virtual int __stdcall Slot03(); virtual int __stdcall Slot04(); virtual int __stdcall Slot05();
    virtual int __stdcall Slot06(); virtual int __stdcall Slot07(); virtual int __stdcall Slot08();
    virtual int __stdcall Slot09(); virtual int __stdcall Slot10(); virtual int __stdcall Slot11();
    virtual int __stdcall Slot12(); virtual int __stdcall Slot13(); virtual int __stdcall Slot14();
    virtual int __stdcall Slot15(); virtual int __stdcall Slot16(); virtual int __stdcall Slot17();
    virtual int __stdcall Slot18(); virtual int __stdcall Slot19(); virtual int __stdcall Slot20();
    virtual int __stdcall Slot21(); virtual int __stdcall Slot22(); virtual int __stdcall Slot23();
    virtual int __stdcall Slot24(); virtual int __stdcall Slot25(); virtual int __stdcall Slot26();
    virtual int __stdcall Slot27(); virtual int __stdcall Slot28(); virtual int __stdcall Slot29();
    virtual int __stdcall Slot30(); virtual int __stdcall Slot31(); virtual int __stdcall Slot32();
    virtual int __stdcall Slot33(); virtual int __stdcall Slot34(); virtual int __stdcall Slot35();
    virtual int __stdcall Slot36(); virtual int __stdcall Slot37();
    virtual int __stdcall Slot38(int val, int flag);   // +0x98
};

#pragma pack(push, 1)
struct Obj_4ca590 {
    char unknown_0[0x4c5];
    Iface_4ca590* iface;   // +0x4c5
};
#pragma pack(pop)

// FUNCTION: 0x4ca590
int __stdcall FUN_004ca590(Obj_4ca590* param_1, int* param_2)
{
    FUN_004c9740((int)"HAPINET_initconnection\n");
    return param_1->iface->Slot38(*param_2, 0) >= 0;
}
