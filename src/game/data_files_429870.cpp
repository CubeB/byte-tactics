// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Loads the game's GAF files into the g_game resource fields (fx, igtitles,
// vismasks, fog, cursors, one per side) and then, from gamedata/sidedata.TDF,
// the base height and the per-side panel graphics.
//
// The long straight-line head re-reads g_game-><group> into a local (`gaf`)
// after the LoadAnimGaf group load, which is what keeps the pointer in esi
// across the FindGafEntry calls; storing straight into the field and reusing
// the call's eax instead loses the reload and the register.
//
// Two things in the tail are load-bearing: the parser local (and buf) must be
// declared at its point of use, not at the top, or MSVC hoists the
// TdfFile constructor above the whole head block; and the loop must be
// `while (1)` with a mid-body break (a `for (;;)` gets rotated). The
// GetFieldString result goes into an `int` local before the test, which is what
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

void* __stdcall LoadAnimGaf(char* name);
void* __stdcall FindGafEntry(void* gaf, const char* name);
void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);

class TdfFile {
public:
    int field_0;
    void* current;                      // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    void ResetCurrentRecord();
    int SelectRecord(char* name);
    void Unload();
};

class TdfRecord {
public:
    int GetFieldString(char* dst, char* key, int size, char* def);
    int GetFieldInt(const char* name, int def);
};

struct Frame_00429870 {
    char unknown_0[2];
    char flag;                          // +0x2
};

#define FLAG(p) (((Frame_00429870*)(p))->flag = 0)

// FUNCTION: 0x429870
void LoadGameResources()
{
    char* gaf;

    g_game->fxGaf = (int)LoadAnimGaf("fx");
    gaf = (char*)g_game->fxGaf;
    g_game->smoke1 = (int)FindGafEntry(gaf, "smoke 1");
    g_game->smoke2 = (int)FindGafEntry(gaf, "smoke 2");
    g_game->fire1 = (int)FindGafEntry(gaf, "fire1");
    g_game->alfboom1 = (int)FindGafEntry(gaf, "alfboom1");
    FLAG(g_game->alfboom1);
    g_game->radlogo = (int)FindGafEntry(gaf, "radlogo");
    g_game->radlogohigh = (int)FindGafEntry(gaf, "radlogohigh");
    g_game->nuclogo = (int)FindGafEntry(gaf, "nuclogo");
    g_game->h2oboom2 = (int)FindGafEntry(gaf, "h2oboom2");
    FLAG(g_game->h2oboom2);
    g_game->lavasplash = (int)FindGafEntry(gaf, "lavasplash");
    FLAG(g_game->lavasplash);
    g_game->cannonshell = (int)FindGafEntry(gaf, "cannonshell");
    g_game->plasmasm1 = (int)FindGafEntry(gaf, "plasmasm");
    g_game->plasmamd = (int)FindGafEntry(gaf, "plasmamd");
    g_game->ultrashell = (int)FindGafEntry(gaf, "ultrashell");
    g_game->plasmasm2 = (int)FindGafEntry(gaf, "plasmasm");
    g_game->flamestream = (int)FindGafEntry(gaf, "flamestream");
    g_game->explosion = (int)FindGafEntry(gaf, "explosion");
    FLAG(g_game->explosion);
    g_game->explode2 = (int)FindGafEntry(gaf, "explode2");
    FLAG(g_game->explode2);
    g_game->explode3 = (int)FindGafEntry(gaf, "explode3");
    FLAG(g_game->explode3);
    g_game->explode4 = (int)FindGafEntry(gaf, "explode4");
    FLAG(g_game->explode4);
    g_game->explode5 = (int)FindGafEntry(gaf, "explode5");
    FLAG(g_game->explode5);
    g_game->nuke1 = (int)FindGafEntry(gaf, "nuke1");
    FLAG(g_game->nuke1);
    g_game->shadow = (int)FindGafEntry(gaf, "shadow");

    g_game->igTitles = (int)LoadAnimGaf("igtitles");
    gaf = (char*)g_game->igTitles;
    g_game->igvictory = (int)FindGafEntry(gaf, "igvictory");
    g_game->igdefeat = (int)FindGafEntry(gaf, "igdefeat");
    g_game->igpaused = (int)FindGafEntry(gaf, "igpaused");

    g_game->vismasks = (int)LoadAnimGaf("vismasks");
    gaf = (char*)g_game->vismasks;
    g_game->vismask = (int)FindGafEntry(gaf, "vismask");

    g_game->fog = (int)LoadAnimGaf("fog");
    gaf = (char*)g_game->fog;
    g_game->black1 = (int)FindGafEntry(gaf, "Black1");
    g_game->black2 = (int)FindGafEntry(gaf, "Black2");
    g_game->black3 = (int)FindGafEntry(gaf, "Black3");
    g_game->black4 = (int)FindGafEntry(gaf, "Black4");
    g_game->gray1 = (int)FindGafEntry(gaf, "Gray1");
    g_game->gray2 = (int)FindGafEntry(gaf, "Gray2");
    g_game->gray3 = (int)FindGafEntry(gaf, "Gray3");
    g_game->gray4 = (int)FindGafEntry(gaf, "Gray4");

    g_game->cursors = (int)LoadAnimGaf("cursors");
    gaf = (char*)g_game->cursors;
    g_game->cursorAttack = (int)FindGafEntry(gaf, "cursorattack");
    g_game->cursorAirstrike = (int)FindGafEntry(gaf, "cursorairstrike");
    g_game->cursorTooFar = (int)FindGafEntry(gaf, "cursortoofar");
    g_game->cursorCapture = (int)FindGafEntry(gaf, "cursorcapture");
    g_game->cursorDefend = (int)FindGafEntry(gaf, "cursordefend");
    g_game->cursorRepair = (int)FindGafEntry(gaf, "cursorrepair");
    g_game->cursorPatrol = (int)FindGafEntry(gaf, "cursorpatrol");
    g_game->cursorPickup = (int)FindGafEntry(gaf, "cursorpickup");
    g_game->cursorTeleport = (int)FindGafEntry(gaf, "cursorteleport");
    g_game->cursorReclamate = (int)FindGafEntry(gaf, "cursorreclamate");
    g_game->cursorLoad = (int)FindGafEntry(gaf, "cursorload");
    g_game->cursorUnload = (int)FindGafEntry(gaf, "cursorunload");
    g_game->cursorMove = (int)FindGafEntry(gaf, "cursormove");
    g_game->cursorSelect = (int)FindGafEntry(gaf, "cursorselect");
    g_game->cursorFindSite = (int)FindGafEntry(gaf, "cursorfindsite");
    g_game->cursorRed = (int)FindGafEntry(gaf, "cursorred");
    g_game->cursorGrn = (int)FindGafEntry(gaf, "cursorgrn");
    g_game->cursorNormal = (int)FindGafEntry(gaf, "cursornormal");
    g_game->cursorHourglass = (int)FindGafEntry(gaf, "cursorhourglass");
    g_game->pathIcon = (int)FindGafEntry(gaf, "pathicon");
    g_game->cursorRevive = (int)FindGafEntry(gaf, "cursorrevive");

    TdfFile parser;
    char buf[256];

    BuildDataPath(buf, "gamedata", "sidedata", "TDF");
    ((TdfFile*)&parser)->LoadFile(buf);
    sprintf(buf, "GENERAL");
    ((TdfFile*)&parser)->ResetCurrentRecord();
    if (((TdfFile*)&parser)->SelectRecord(buf) == 1) {
        g_game->baseHeight = ((TdfRecord*)parser.current)->GetFieldInt("baseheight", 0x1e0);
    } else {
        g_game->baseHeight = 0x1e0;
    }

    int i = 0;
    while (1) {
        sprintf(buf, "SIDE%d", i);
        ((TdfFile*)&parser)->ResetCurrentRecord();
        if (!((TdfFile*)&parser)->SelectRecord(buf))
            break;
        int intgaf = ((TdfRecord*)parser.current)->GetFieldString(buf, "intgaf", 0x1e, DAT_005119b8);
        if (intgaf != 0) {
            char* side = (char*)LoadAnimGaf(buf);
            if (side) {
                g_game->panelTop[i] = (int)side;
                g_game->panelSide[i] = (int)FindGafEntry(side, "PANELTOP");
                g_game->panelBot[i] = (int)FindGafEntry(side, "PANELSIDE");
                g_game->panelBot2[i] = (int)FindGafEntry(side, "PANELBOT");
            }
        }
        i++;
    }
    ((TdfFile*)&parser)->Unload();
}
