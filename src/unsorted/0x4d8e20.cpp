// Decompiled by Sonnet. Names are provisional.
// fs:[0x2c] is the thread-local storage array pointer; the value read back
// after the FUN_004d8d70(0, 0) call is field +8 of a __declspec(thread)
// struct that FUN_004d8d70 itself initialises.

extern "C" int __cdecl FUN_004d8d70(int, int);

struct TlsBlock {
    char pad[8];
    void* limit;
};

__declspec(thread) TlsBlock g_tls;

// FUNCTION: 0x4d8e20
void* FUN_004d8e20()
{
    FUN_004d8d70(0, 0);
    return g_tls.limit;
}
