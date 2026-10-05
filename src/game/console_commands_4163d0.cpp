// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x2c8e];
    short x;                         // +0x2c8e
    short y;                         // +0x2c90
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

class Class_004b73c0 {
public:
    char* GetArg(int index, char* fallback);
};

short __stdcall FindFeatureType(char* name);
void* __stdcall GetMapCell(int x, int y);
void* __stdcall PlaceFeature(void* target, unsigned short id, void* pos, void* field_64, unsigned char owner);

// FUNCTION: 0x4163d0
void __stdcall CmdFeature(Class_004b73c0* args)
{
    unsigned short id = FindFeatureType(args->GetArg(1, DAT_005119b8));
    if (id != 0xffff) {
        void* target = GetMapCell(g_game->x, g_game->y);
        PlaceFeature(target, id, 0, 0, 10);
    }
}
