// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
#include <string.h>

class DirectPlay_004c9920 {
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

struct GUID_004c9920 {
    int data[4];
};

struct SessionDesc_004c9920 {
    unsigned long dwSize;             // +0x0
    unsigned long dwFlags;            // +0x4
    GUID_004c9920 guidInstance;       // +0x8
    GUID_004c9920 guidApplication;    // +0x18
    unsigned long dwMaxPlayers;       // +0x28
    unsigned long dwCurrentPlayers;   // +0x2c
    char* lpszSessionName;            // +0x30
    char* lpszPassword;               // +0x34
    unsigned long dwReserved1;        // +0x38
    unsigned long dwReserved2;        // +0x3c
    unsigned long dwUser1;            // +0x40
    unsigned long dwUser2;            // +0x44
    unsigned long dwUser3;            // +0x48
    unsigned long dwUser4;            // +0x4c
};

#pragma pack(push, 1)
struct Net_004c9920 {
    char name[0x10];                  // +0x0
    char unknown_10[0x43d - 0x10];
    GUID_004c9920 application;        // +0x43d
    char unknown_44d[0x45d - 0x44d];
    SessionDesc_004c9920 session;     // +0x45d
    char unknown_4ad[0x4c5 - 0x4ad];
    DirectPlay_004c9920* dp;          // +0x4c5
    char unknown_4c9[0x4dd - 0x4c9];
    int maxPlayers;                   // +0x4dd
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);

// FUNCTION: 0x4c9920
int __stdcall FUN_004c9920(Net_004c9920* net, char* name, int a3, int a4, int a5, int a6, int a7)
{
    FUN_004c9740((int)"HAPINET_createnewgame\n");
    if (net->dp != 0) {
        strncpy((char*)net, name, 0x10);
        memset(&net->session, 0, 0x50);
        net->session.dwMaxPlayers = net->maxPlayers;
        net->session.guidApplication = net->application;
        net->session.dwSize = 0x50;
        net->session.lpszSessionName = name;
        net->session.dwUser4 = a7;
        net->session.dwUser3 = a6;
        net->session.dwUser2 = a5;
        net->session.dwUser1 = a4;
        net->session.lpszPassword = (char*)a3;
        int r = net->dp->Slot24(&net->session, 2);
        unsigned long size = 0x50;
        int hr = net->dp->GetSessionDesc(0, &size);
        if (hr == (int)0x8877001e) {
            void* buf = FUN_004d83b0("DP SESSION DATA2", size);
            if (buf != 0) {
                if (net->dp->GetSessionDesc(buf, &size) == 0)
                    net->session.guidInstance = *(GUID_004c9920*)((char*)buf + 8);
                FUN_004d85a0(buf);
            }
        }
        if (r == 0)
            return 1;
    }
    return 0;
}
