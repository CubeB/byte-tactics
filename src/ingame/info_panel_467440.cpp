// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, GPT-6.1-sol, finished by mimo-v2.6-pro, finished by DeepSeek V4.1 Flash, checked by GPT-6. Names are provisional.
// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, GPT-6.1-sol. Names are provisional.
// Kept its own file: in info_panel.cpp the file's total symbol count moves the
// last loop's player pointer from eax to edx.
// Five loops over the unit array (stride 0x118).

#pragma pack(push, 1)

struct Vec3_00467440 {
    int x;
    int y;
    int z;
};

union UnitPos_00467440 {
    Vec3_00467440 vec;
    struct {
        short f6a;
        short f6c;
        short f6e;
        short f70;
        short f72;
        short f74;
    } half;
};

struct UnitDef_00467440 {
    char unknown_0[0x204];
    short radardistance;               // +0x204
    short sonardistance;               // +0x206
    short mincloakdistance;            // +0x208
    short radardistancejam;            // +0x20a
    short sonardistancejam;            // +0x20c
    char unknown_20e[0x245 - 0x20e];
    unsigned int flags2;               // +0x245
};

struct PlayerData_00467440 {
    char unknown_0[0x97];
    unsigned char field_97;            // +0x97
    char unknown_98[0x9b - 0x98];
    unsigned char field_9b;            // +0x9b
};

struct Owner_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerData_00467440* data;         // +0x27
    char unknown_2b[0x73 - 0x2b];
    char field_73;                     // +0x73
    char unknown_74[0x108 - 0x74];
    unsigned char field_108[1];        // +0x108
};

struct Unit {
    char unknown_0[0x6a];
    UnitPos_00467440 pos;              // +0x6a
    char unknown_76[0x92 - 0x76];
    UnitDef_00467440* def;             // +0x92
    Owner_00467440* player;          // +0x96
    char unknown_9a[0xb0 - 0x9a];
    int workTime;                      // +0xb0
    char unknown_b4[0xff - 0xb4];
    unsigned char playerIndex;            // +0xff
    char unknown_100[0x10e - 0x100];
    unsigned char activateFlags;           // +0x10e
    char unknown_10f;
    unsigned int flags;                // +0x110
    char unknown_114[0x118 - 0x114];
};

struct MapSize_00467440 {
    unsigned int width;                // +0x0
    unsigned int height;               // +0x4

    int Contains(unsigned int tx, unsigned int ty)
    {
        return tx < width && ty < height;
    }
};

struct ByteMap_00467440 {
    unsigned char* data;               // +0x0
    MapSize_00467440 size;             // +0x4

    unsigned char Get(int x, int y) { return data[size.width * y + x]; }
};

struct PlayerInfo_00467440 {
    void* field_0;                     // +0x0
    char unknown_4[0x27 - 0x4];
    PlayerData_00467440* data;         // +0x27
    char unknown_2b[0x67 - 0x2b];
    Unit* field_67;                    // +0x67
    Unit* field_6b;                    // +0x6b
    char unknown_6f[0x7c - 0x6f];
    ByteMap_00467440 explored;         // +0x7c
    char unknown_88[0x146 - 0x88];
    unsigned char field_146;           // +0x146
    char unknown_147[0x14b - 0x147];
};

struct Game {
    char unknown_0[0x1b63];
    PlayerInfo_00467440 players[10];   // +0x1b63, stride 0x14b
    char unknown_2851[0x2a3c - 0x2851];
    unsigned short numPlayers;         // +0x2a3c
    char unknown_2a3e[0x2a43 - 0x2a3e];
    unsigned char playerIndex;         // +0x2a43
    char unknown_2a44[0x14273 - 0x2a44];
    unsigned short* field_14273;       // +0x14273
    char unknown_14277[0x14281 - 0x14277];
    unsigned char field_14281;         // +0x14281
    char unknown_14282[0x14357 - 0x14282];
    Unit* units;                       // +0x14357
    Unit* units_end;                   // +0x1435b
    char unknown_1435f[0x38a47 - 0x1435f];
    int ticks;                         // +0x38a47

    PlayerInfo_00467440* Current() { return &players[playerIndex]; }
};
#pragma pack(pop)

class DetectionVisitor {
public:
    virtual void MarkUnitsInRadarOrSonarRadius(Unit* unit);
    int field_4;                       // +0x4
    int field_8;                       // +0x8
    Vec3_00467440 pos;                 // +0xc
};

class RadarJamVisitor {
public:
    virtual void ApplyRadarJamFlag(Unit* unit);
};

class SonarJamVisitor {
public:
    virtual void ApplySonarJamFlag(Unit* unit);
};

extern Game* g_game;

void __stdcall VisitObjectsInRange(Vec3_00467440* pos, int range, void* visitor);
bool __stdcall HasReadyUnitInRange(int player, Vec3_00467440* p, int range);

static inline int IsExplored_00467440(PlayerInfo_00467440* p, UnitPos_00467440* pos)
{
    int tx = pos->half.f6c >> 5;
    int ty = (pos->half.f74 - (pos->half.f70 >> 1)) >> 5;
    if (p->explored.size.Contains(tx, ty) && p->explored.Get(tx, ty) != 0)
        return 1;
    return 0;
}

static inline int IsSeen_00467440(PlayerInfo_00467440* p, UnitPos_00467440* pos)
{
    int tx = pos->half.f6c >> 5;
    int ty = (pos->half.f74 - (pos->half.f70 >> 1)) >> 5;
    if (!p->explored.size.Contains(tx, ty))
        return 0;
    return (g_game->field_14273[p->explored.size.width * ty + tx] &
            (1 << g_game->playerIndex)) != 0;
}

// FUNCTION: 0x467440
void UpdateSensorRadarAndCloak(void)
{
    if (g_game->numPlayers < 2) {
        return;
    }
    unsigned char player = g_game->playerIndex;
    Unit* first = g_game->units + 1;
    Unit* last = g_game->units_end;
    PlayerInfo_00467440* pl = (PlayerInfo_00467440*)((char*)g_game + 0x1b63
        + (unsigned int)g_game->playerIndex * 0x14b);
    Unit* u;

    Unit* a;
    for (a = first; a <= last; a++) {
        if (a->flags & 0x10000000) {
            a->flags &= ~0x1000;
            if (a->playerIndex == player
                || (a->player->field_108[pl->field_146] != 0
                    && (a->player->data->field_97 & 0x40) != 0)
                || (*(int*)pl != 0 && (pl->data->field_9b & 0x40) != 0)) {
                a->flags |= 0x300;
            } else {
                a->flags &= ~0x700;
            }
        }
    }

    for (u = pl->field_67; u <= pl->field_6b; u++) {
        if ((u->flags & 0x10000000) && !(u->flags & 0x4000) && (u->activateFlags & 1)) {
            if (u->def->radardistance != 0 || u->def->sonardistance != 0) {
                // t is computed before b is loaded, then squared.
                short a = u->def->radardistance;
                int t = a + u->pos.half.f70 * 2;
                short b = u->def->sonardistance;
                t = t * t;
                int s = (int)b * (int)b;
                if (a <= b) {
                    a = b;
                }
                Vec3_00467440* pp = &u->pos.vec;
                DetectionVisitor v;
                v.field_4 = t;
                v.field_8 = s;
                v.pos = u->pos.vec;
                VisitObjectsInRange(pp, (int)a << 16, &v);
            }
        }
    }

    for (u = first; u <= last; u++) {
        // The ff local and the `, 1` term keep the playerIndex load a separate term.
        unsigned char ff;
        if ((u->flags & 0x10000000) && (ff = u->playerIndex, 1) && ff != pl->field_146 && (u->activateFlags & 1)) {
            if (u->def->radardistancejam != 0) {
                int r = (int)u->def->radardistancejam << 16;
                Vec3_00467440* pp = &u->pos.vec;
                RadarJamVisitor v;
                VisitObjectsInRange(pp, r, &v);
            }
            if (u->def->sonardistancejam != 0) {
                int r2 = (int)u->def->sonardistancejam << 16;
                Vec3_00467440* pp2 = &u->pos.vec;
                SonarJamVisitor v;
                VisitObjectsInRange(pp2, r2, &v);
            }
        }
    }

    for (u = first; u <= last; u++) {
        if ((u->flags & 0x10000000) && u->player->field_0 != 0) {
            char c = u->player->field_73;
            if (c == 1 || c == 2) {
                if (u->def->flags2 & 0x2000) {
                    if (HasReadyUnitInRange(u->playerIndex, &u->pos.vec, u->def->mincloakdistance)) {
                        // Written through an int& so the flags load stays after the store.
                        int& b0 = u->workTime;
                        b0 = g_game->ticks + 0x5a;
                        u->flags |= 0x1000;
                    }
                }
            }
        }
    }

    for (u = first; u <= last; u++) {
        // Shared temp: using u->flags directly changes the code.
        unsigned int f = u->flags;
        if ((f & 0x10000000) && !(f & 0x100) && !(u->activateFlags & 4)) {
            // int, not unsigned char: the latter spills playerIndex.
            int pi = g_game->playerIndex;
            PlayerInfo_00467440* p2 = g_game->Current();
            // unsigned int: an int merges the two zero blocks.
            unsigned int vis;
            if ((g_game->field_14281 & 2) == 2) {
                vis = IsExplored_00467440(p2, &u->pos);
            } else {
                vis = IsSeen_00467440(p2, &u->pos);
            }
            if ((int)vis) {
                u->flags = f | 0x100;
            }
        }
    }
}
