// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Sub_004679a0_a {
    char unknown_0[1];
    int field_1;                     // +0x01
    char unknown_5[0xd - 0x5];
    int field_d;                     // +0x0d
    char unknown_11[0x19 - 0x11];
    int field_19;                    // +0x19
    int field_1d;                    // +0x1d
};

struct Sub_004679a0_b {
    char unknown_0[0x30];
    int field_30;                    // +0x30
    unsigned short* lightbar;        // +0x34
};

struct Game {
    char unknown_0[0x51d];
    void* gaf;                       // +0x51d
    char unknown_521[0x37e3f - 0x521];
    Sub_004679a0_a a;                // +0x37e3f
    Sub_004679a0_b b;                // +0x37e60
};
#pragma pack(pop)

extern Game* g_game;

unsigned short* __stdcall FindGafEntry(void* gaf, const char* name);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);

// FUNCTION: 0x4679a0
void LoadLightBar()
{
    Sub_004679a0_a* a = &g_game->a;
    a->field_1 = a->field_d = a->field_19 = a->field_1d = 0;
    Sub_004679a0_b* b = &g_game->b;
    b->field_30 = 0;
    b->lightbar = 0;
    unsigned short* frames = FindGafEntry(g_game->gaf, "LIGHTBAR");
    g_game->b.lightbar = (unsigned short*)GetGafFrame(frames, 1);
    g_game->b.lightbar[3] = 0;
    g_game->b.lightbar[2] = 0;
}
