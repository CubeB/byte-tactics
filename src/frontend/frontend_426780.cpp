// Decompiled by deepseek-v4.1-flash. Names are provisional.

void StopAllSounds();
void __stdcall FUN_0041d7b0(char* dest, const char* a, const char* b, const char* c);
int __stdcall FUN_004bbc40(char* path);
void __stdcall SetOffscreenSurface(int param);
void __stdcall FillSurface(int a, int b);
void FlipScreen();
void __stdcall FUN_004c22d0(int param);
int PopKey(void);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

class Class_0047bf20 {
public:
    void Close();
};

class Class_0047c6c0 {
public:
    void Play();
};

class MoviePlayer {
public:
    char pad[0x5b8];
    MoviePlayer(char* path, int a, int b, int c, int d, int e);
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x37e1b];
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x38d7b - 0x37e1f];
    void* field_38d7b;                 // +0x38d7b
    char unknown_38d7f[0x39241 - 0x38d7f];
    int field_39241;                   // +0x39241
};
#pragma pack(pop)

extern Game* g_game;

// FUNCTION: 0x426780
void __stdcall PlayMovie(char* param_1)
{
    char path[256];

    StopAllSounds();
    FUN_0041d7b0(path, "Data", param_1, "zrb");
    if (FUN_004bbc40(path) != 0) {
        SetOffscreenSurface(g_game->field_37e1b);
        FillSurface(0, 0);
        FlipScreen();
        FUN_004c22d0(0);
        do {
            g_game->field_38d7b = new MoviePlayer(path, 0, 600000, 1, 2000000, 1);
            ((Class_0047c6c0*)g_game->field_38d7b)->Play();
            Class_0047bf20* p = (Class_0047bf20*)g_game->field_38d7b;
            if (p != 0) {
                p->Close();
                operator delete(p);
            }
        } while (g_game->field_39241 != 0);
        g_game->field_38d7b = 0;
        while (PopKey() != 0) {
        }
        SetOffscreenSurface(g_game->field_37e1b);
        FillSurface(0, 0);
        FlipScreen();
    }
}
