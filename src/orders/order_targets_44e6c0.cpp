// Decompiled by DeepSeek V4.1 Flash. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x1427f];
    unsigned char seaLevel;            // +0x1427f
};

struct Pos_0044e6c0 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

#pragma pack(push, 2)
class Class_0044e6c0 {
public:
    char unknown_0[8];
    unsigned short field_8;            // +0x8
    short field_a;                     // +0xa
    short field_c;                     // +0xc
    char unknown_e[0x26 - 0xe];
    Pos_0044e6c0 pos;                  // +0x26

    void FUN_0044e6c0(int param_1);
};
#pragma pack(pop)

extern Game* g_game;

int __stdcall FUN_00485070(Pos_0044e6c0* pos);

#define max(a, b) (((a) > (b)) ? (a) : (b))

// FUNCTION: 0x44e6c0
void Class_0044e6c0::FUN_0044e6c0(int param_1)
{
    field_8 |= 8;
    field_c = param_1;
    if ((unsigned char)field_8 & 0x20) {
        pos.y = (max(FUN_00485070(&pos), g_game->seaLevel) + param_1) << 16;
        if (pos.y > 0x1ff0000)
            pos.y = 0x1ff0000;
    }
}
