// Decompiled by Opus. Names are provisional.

void __cdecl FUN_00461170(const char* fmt, ...);

class Class_0044f940 {
public:
    void FUN_0044f940(void* data, int size);
};

class Class_0044fc10 {
public:
    int FUN_0044fc10(void* session, int from);
};

struct Game_00461180 {
    char unknown_0[0x14];
    char session[4];                 // +0x14
};

extern Game_00461180* g_game;
extern Class_0044f940 DAT_005129d0;  // net condenser
extern int DAT_005129f1;

// Storing the send result in a local first keeps `sete` (a direct
// `== 0 ? 1 : 0` on the call folds to neg/sbb/inc).
// FUNCTION: 0x461180
int __stdcall FUN_00461180(int from, int to, void* data, int size)
{
    FUN_00461170("bytes to send to (DPID)(%ld): %ld\n", to, size);
    void* session = g_game->session;
    DAT_005129f1 = to;
    DAT_005129d0.FUN_0044f940(data, size);
    int result = ((Class_0044fc10*)&DAT_005129d0)->FUN_0044fc10(session, from);
    return result == 0 ? 1 : 0;
}
