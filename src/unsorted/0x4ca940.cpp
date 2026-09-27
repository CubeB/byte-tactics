// Decompiled by Opus. Names are provisional.

// COM interface; slot 2 (+0x8) is IUnknown::Release.
class Iface_004ca940 {
public:
    virtual long __stdcall QueryInterface(void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};

struct Net_004ca940 {
    Iface_004ca940* lobby;             // +0x0
    Iface_004ca940* dp;                // +0x4
};

void FUN_004c9740(int);

// Releases the DirectPlay interface, then the lobby interface.
// FUNCTION: 0x4ca940
int __stdcall FUN_004ca940(Net_004ca940* net)
{
    FUN_004c9740((int)"HAPINET_releasedplayinterface\n");
    int result = 0;
    if (net->dp != 0) {
        result = net->dp->Release();
        net->dp = 0;
    }
    if (net->lobby != 0) {
        net->lobby->Release();
        net->lobby = 0;
    }
    return result;
}
