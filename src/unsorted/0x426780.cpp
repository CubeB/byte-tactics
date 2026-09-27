// Decompiled by deepseek-v4.1-flash. Names are provisional.

void FUN_0047f750();
void __stdcall FUN_0041d7b0(char* dest, const char* a, const char* b, const char* c);
int __stdcall FUN_004bbc40(char* path);
void __stdcall FUN_004c69a0(int param);
void __stdcall FUN_004c6890(int a, int b);
void FUN_004c63a0();
void __stdcall FUN_004c22d0(int param);
int FUN_004c1ab0(void);
void* operator new(unsigned int size);
void operator delete(void* p);

class Class_0047bf20 {
public:
    void FUN_0047bf20();
};

class Class_0047c6c0 {
public:
    void FUN_0047c6c0();
};

class Class_0047bdf0 {
public:
    char pad[0x5b8];
    Class_0047bdf0(char* path, int a, int b, int c, int d, int e);
};

#pragma pack(push, 1)
struct Game_00426780 {
    char unknown_0[0x37e1b];
    int field_37e1b;                   // +0x37e1b
    char unknown_37e1f[0x38d7b - 0x37e1f];
    void* field_38d7b;                 // +0x38d7b
    char unknown_38d7f[0x39241 - 0x38d7f];
    int field_39241;                   // +0x39241
};
#pragma pack(pop)

extern Game_00426780* g_game;

// FUNCTION: 0x426780
void __stdcall FUN_00426780(char* param_1)
{
    char path[256];

    FUN_0047f750();
    FUN_0041d7b0(path, "Data", param_1, "zrb");
    if (FUN_004bbc40(path) != 0) {
        FUN_004c69a0(g_game->field_37e1b);
        FUN_004c6890(0, 0);
        FUN_004c63a0();
        FUN_004c22d0(0);
        do {
            g_game->field_38d7b = new Class_0047bdf0(path, 0, 600000, 1, 2000000, 1);
            ((Class_0047c6c0*)g_game->field_38d7b)->FUN_0047c6c0();
            Class_0047bf20* p = (Class_0047bf20*)g_game->field_38d7b;
            if (p != 0) {
                p->FUN_0047bf20();
                operator delete(p);
            }
        } while (g_game->field_39241 != 0);
        g_game->field_38d7b = 0;
        while (FUN_004c1ab0() != 0) {
        }
        FUN_004c69a0(g_game->field_37e1b);
        FUN_004c6890(0, 0);
        FUN_004c63a0();
    }
}
