// Decompiled by Opus. Names are provisional.
// HAPINET_uninitmultiplay: closes and releases the two DirectPlay interfaces,
// releases the two lobby interfaces and frees the connection buffer.

// COM interface (IDirectPlay-like); slot 4 (+0x10) is Close.
class DirectPlay_4c9b70 {
public:
    virtual int __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual int __stdcall AddPlayerToGroup(unsigned long group, unsigned long player);
    virtual int __stdcall Close();
};

// COM interface of which only Release (+0x8) is used.
class Unknown_4c9b70 {
public:
    virtual int __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};

#pragma pack(push, 1)
struct Net_4c9b70 {
    char unknown_0[0x4c1];
    DirectPlay_4c9b70* dp1;          // +0x4c1
    DirectPlay_4c9b70* dp;           // +0x4c5
    Unknown_4c9b70* lobby2;          // +0x4c9
    Unknown_4c9b70* lobby1;          // +0x4cd
    int* connection;                 // +0x4d1
    int connection_size;             // +0x4d5
};
#pragma pack(pop)

void __cdecl FUN_004c9740(int);
void __cdecl FUN_004d85a0(int* param_1);

// FUNCTION: 0x4c9b70
void __stdcall FUN_004c9b70(Net_4c9b70* net)
{
    FUN_004c9740((int)"HAPINET_uninitmultiplay\n");
    if (net->dp != 0) {
        net->dp->Close();
        net->dp->Release();
        net->dp = 0;
    }
    if (net->dp1 != 0) {
        net->dp1->Close();
        net->dp1->Release();
        net->dp1 = 0;
    }
    if (net->lobby1 != 0) {
        net->lobby1->Release();
        net->lobby1 = 0;
    }
    if (net->lobby2 != 0) {
        net->lobby2->Release();
        net->lobby2 = 0;
    }
    if (net->connection != 0) {
        FUN_004d85a0(net->connection);
        net->connection = 0;
        net->connection_size = 0;
    }
}
