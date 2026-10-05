// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads the game's GAF files into the g_game resource fields (fx, igtitles,
// vismasks, fog, cursors, one per side) and then, from gamedata/sidedata.TDF,
// the base height and the per-side panel graphics.
//
// The long straight-line head re-reads g_game-><group> into a local (`gaf`)
// after the FUN_00429700 group load, which is what keeps the pointer in esi
// across the FUN_004b8d40 calls; storing straight into the field and reusing
// the call's eax instead loses the reload and the register.
//
// Two things in the tail are load-bearing: the parser local (and buf) must be
// declared at its point of use, not at the top, or MSVC hoists the
// Class_004c2ea0 constructor above the whole head block; and the loop must be
// `while (1)` with a mid-body break (a `for (;;)` gets rotated). The
// FUN_004c48c0 result goes into an `int` local before the test, which is what
// makes MSVC emit `cmp eax, ebx` instead of `test eax, eax`.
#include <stdio.h>

#pragma pack(push, 1)
struct Game {
    char pad_0[0x147a7];
    int baseHeight;  // +0x147a7
    char pad_147ab[0x10];
    int cannonshell;  // +0x147bb
    int plasmasm1;  // +0x147bf
    int plasmamd;  // +0x147c3
    int ultrashell;  // +0x147c7
    int plasmasm2;  // +0x147cb
    int smoke1;  // +0x147cf
    int smoke2;  // +0x147d3
    int fire1;  // +0x147d7
    int alfboom1;  // +0x147db
    int radlogo;  // +0x147df
    int radlogohigh;  // +0x147e3
    int nuclogo;  // +0x147e7
    int h2oboom2;  // +0x147eb
    int lavasplash;  // +0x147ef
    int flamestream;  // +0x147f3
    int explosion;  // +0x147f7
    int explode2;  // +0x147fb
    int explode3;  // +0x147ff
    int explode4;  // +0x14803
    int explode5;  // +0x14807
    int nuke1;  // +0x1480b
    int shadow;  // +0x1480f
    int igvictory;  // +0x14813
    int igdefeat;  // +0x14817
    int igpaused;  // +0x1481b
    int panelSide[5];  // +0x1481f
    int panelBot2[5];  // +0x14833
    int panelBot[5];  // +0x14847
    int vismask;  // +0x1485b
    int black1;  // +0x1485f
    int black2;  // +0x14863
    int black3;  // +0x14867
    int black4;  // +0x1486b
    int gray1;  // +0x1486f
    int gray2;  // +0x14873
    int gray3;  // +0x14877
    int gray4;  // +0x1487b
    char pad_1487f[0x4];
    int cursorAttack;  // +0x14883
    int cursorAirstrike;  // +0x14887
    int cursorTooFar;  // +0x1488b
    int cursorCapture;  // +0x1488f
    int cursorDefend;  // +0x14893
    int cursorRepair;  // +0x14897
    int cursorPatrol;  // +0x1489b
    int cursorPickup;  // +0x1489f
    int cursorTeleport;  // +0x148a3
    int cursorRevive;  // +0x148a7
    int cursorReclamate;  // +0x148ab
    int cursorLoad;  // +0x148af
    int cursorUnload;  // +0x148b3
    int cursorMove;  // +0x148b7
    int cursorSelect;  // +0x148bb
    int cursorFindSite;  // +0x148bf
    int cursorRed;  // +0x148c3
    int cursorGrn;  // +0x148c7
    int cursorNormal;  // +0x148cb
    int cursorHourglass;  // +0x148cf
    int pathIcon;  // +0x148d3
    char pad_148d7[0x1c];
    int fxGaf;  // +0x148f3
    int igTitles;  // +0x148f7
    int vismasks;  // +0x148fb
    int fog;  // +0x148ff
    int cursors;  // +0x14903
    int panelTop[8];  // +0x14907
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void* __stdcall FUN_00429700(char* name);
void* __stdcall FUN_004b8d40(void* gaf, const char* name);
void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);

class Class_004c2ea0 {
public:
    int field_0;
    void* current;                      // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3e10 {
public:
    void FUN_004c3e10();
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

class Class_004c46c0 {
public:
    int FUN_004c46c0(const char* name, int def);
};

class Class_004c48c0 {
public:
    int FUN_004c48c0(char* dst, char* key, int size, char* def);
};

class Class_004c3240 {
public:
    void FUN_004c3240();
};

struct Frame_00429870 {
    char unknown_0[2];
    char flag;                          // +0x2
};

#define FLAG(p) (((Frame_00429870*)(p))->flag = 0)

// FUNCTION: 0x429870
void FUN_00429870()
{
    char* gaf;

    g_game->fxGaf = (int)FUN_00429700("fx");
    gaf = (char*)g_game->fxGaf;
    g_game->smoke1 = (int)FUN_004b8d40(gaf, "smoke 1");
    g_game->smoke2 = (int)FUN_004b8d40(gaf, "smoke 2");
    g_game->fire1 = (int)FUN_004b8d40(gaf, "fire1");
    g_game->alfboom1 = (int)FUN_004b8d40(gaf, "alfboom1");
    FLAG(g_game->alfboom1);
    g_game->radlogo = (int)FUN_004b8d40(gaf, "radlogo");
    g_game->radlogohigh = (int)FUN_004b8d40(gaf, "radlogohigh");
    g_game->nuclogo = (int)FUN_004b8d40(gaf, "nuclogo");
    g_game->h2oboom2 = (int)FUN_004b8d40(gaf, "h2oboom2");
    FLAG(g_game->h2oboom2);
    g_game->lavasplash = (int)FUN_004b8d40(gaf, "lavasplash");
    FLAG(g_game->lavasplash);
    g_game->cannonshell = (int)FUN_004b8d40(gaf, "cannonshell");
    g_game->plasmasm1 = (int)FUN_004b8d40(gaf, "plasmasm");
    g_game->plasmamd = (int)FUN_004b8d40(gaf, "plasmamd");
    g_game->ultrashell = (int)FUN_004b8d40(gaf, "ultrashell");
    g_game->plasmasm2 = (int)FUN_004b8d40(gaf, "plasmasm");
    g_game->flamestream = (int)FUN_004b8d40(gaf, "flamestream");
    g_game->explosion = (int)FUN_004b8d40(gaf, "explosion");
    FLAG(g_game->explosion);
    g_game->explode2 = (int)FUN_004b8d40(gaf, "explode2");
    FLAG(g_game->explode2);
    g_game->explode3 = (int)FUN_004b8d40(gaf, "explode3");
    FLAG(g_game->explode3);
    g_game->explode4 = (int)FUN_004b8d40(gaf, "explode4");
    FLAG(g_game->explode4);
    g_game->explode5 = (int)FUN_004b8d40(gaf, "explode5");
    FLAG(g_game->explode5);
    g_game->nuke1 = (int)FUN_004b8d40(gaf, "nuke1");
    FLAG(g_game->nuke1);
    g_game->shadow = (int)FUN_004b8d40(gaf, "shadow");

    g_game->igTitles = (int)FUN_00429700("igtitles");
    gaf = (char*)g_game->igTitles;
    g_game->igvictory = (int)FUN_004b8d40(gaf, "igvictory");
    g_game->igdefeat = (int)FUN_004b8d40(gaf, "igdefeat");
    g_game->igpaused = (int)FUN_004b8d40(gaf, "igpaused");

    g_game->vismasks = (int)FUN_00429700("vismasks");
    gaf = (char*)g_game->vismasks;
    g_game->vismask = (int)FUN_004b8d40(gaf, "vismask");

    g_game->fog = (int)FUN_00429700("fog");
    gaf = (char*)g_game->fog;
    g_game->black1 = (int)FUN_004b8d40(gaf, "Black1");
    g_game->black2 = (int)FUN_004b8d40(gaf, "Black2");
    g_game->black3 = (int)FUN_004b8d40(gaf, "Black3");
    g_game->black4 = (int)FUN_004b8d40(gaf, "Black4");
    g_game->gray1 = (int)FUN_004b8d40(gaf, "Gray1");
    g_game->gray2 = (int)FUN_004b8d40(gaf, "Gray2");
    g_game->gray3 = (int)FUN_004b8d40(gaf, "Gray3");
    g_game->gray4 = (int)FUN_004b8d40(gaf, "Gray4");

    g_game->cursors = (int)FUN_00429700("cursors");
    gaf = (char*)g_game->cursors;
    g_game->cursorAttack = (int)FUN_004b8d40(gaf, "cursorattack");
    g_game->cursorAirstrike = (int)FUN_004b8d40(gaf, "cursorairstrike");
    g_game->cursorTooFar = (int)FUN_004b8d40(gaf, "cursortoofar");
    g_game->cursorCapture = (int)FUN_004b8d40(gaf, "cursorcapture");
    g_game->cursorDefend = (int)FUN_004b8d40(gaf, "cursordefend");
    g_game->cursorRepair = (int)FUN_004b8d40(gaf, "cursorrepair");
    g_game->cursorPatrol = (int)FUN_004b8d40(gaf, "cursorpatrol");
    g_game->cursorPickup = (int)FUN_004b8d40(gaf, "cursorpickup");
    g_game->cursorTeleport = (int)FUN_004b8d40(gaf, "cursorteleport");
    g_game->cursorReclamate = (int)FUN_004b8d40(gaf, "cursorreclamate");
    g_game->cursorLoad = (int)FUN_004b8d40(gaf, "cursorload");
    g_game->cursorUnload = (int)FUN_004b8d40(gaf, "cursorunload");
    g_game->cursorMove = (int)FUN_004b8d40(gaf, "cursormove");
    g_game->cursorSelect = (int)FUN_004b8d40(gaf, "cursorselect");
    g_game->cursorFindSite = (int)FUN_004b8d40(gaf, "cursorfindsite");
    g_game->cursorRed = (int)FUN_004b8d40(gaf, "cursorred");
    g_game->cursorGrn = (int)FUN_004b8d40(gaf, "cursorgrn");
    g_game->cursorNormal = (int)FUN_004b8d40(gaf, "cursornormal");
    g_game->cursorHourglass = (int)FUN_004b8d40(gaf, "cursorhourglass");
    g_game->pathIcon = (int)FUN_004b8d40(gaf, "pathicon");
    g_game->cursorRevive = (int)FUN_004b8d40(gaf, "cursorrevive");

    Class_004c2ea0 parser;
    char buf[256];

    FUN_004290f0(buf, "gamedata", "sidedata", "TDF");
    ((Class_004c2f60*)&parser)->FUN_004c2f60(buf);
    sprintf(buf, "GENERAL");
    ((Class_004c3e10*)&parser)->FUN_004c3e10();
    if (((Class_004c3410*)&parser)->FUN_004c3410(buf) == 1) {
        g_game->baseHeight = ((Class_004c46c0*)parser.current)->FUN_004c46c0("baseheight", 0x1e0);
    } else {
        g_game->baseHeight = 0x1e0;
    }

    int i = 0;
    while (1) {
        sprintf(buf, "SIDE%d", i);
        ((Class_004c3e10*)&parser)->FUN_004c3e10();
        if (!((Class_004c3410*)&parser)->FUN_004c3410(buf))
            break;
        int intgaf = ((Class_004c48c0*)parser.current)->FUN_004c48c0(buf, "intgaf", 0x1e, DAT_005119b8);
        if (intgaf != 0) {
            char* side = (char*)FUN_00429700(buf);
            if (side) {
                g_game->panelTop[i] = (int)side;
                g_game->panelSide[i] = (int)FUN_004b8d40(side, "PANELTOP");
                g_game->panelBot[i] = (int)FUN_004b8d40(side, "PANELSIDE");
                g_game->panelBot2[i] = (int)FUN_004b8d40(side, "PANELBOT");
            }
        }
        i++;
    }
    ((Class_004c3240*)&parser)->FUN_004c3240();
}
