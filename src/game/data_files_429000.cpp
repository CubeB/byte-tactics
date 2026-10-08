// Decompiled by Space Bunny Free, deepseek-v4.1-flash, Opus, Haiku, DeepSeek V4.1 Flash and Claude Opus 5.5. Names are provisional.
// Data files: the GPF version check, the data path builder, the loaders for
// bitmaps, fonts, palettes, sounds, anims (GAF), textures and 3DO objects, and
// the code that frees them, all reached through g_game.
#include <stdio.h>
#include <string.h>

#include "../util/tdf.h"

struct AnimEntry {
    char name[0x40];                   // +0x0
    void* gaf;                         // +0x40
};

#pragma pack(push, 1)
struct Game {
    char unknown_0[0x519];
    char messageBox[1];                // +0x519
    char unknown_51a[0x1439b - 0x51a];
    void* field_1439b;                 // +0x1439b
    char unknown_1439f[8];
    char palette[0x400];               // +0x143a7
    int baseHeight;                    // +0x147a7
    int animCount;                     // +0x147ab
    AnimEntry* anims;                  // +0x147af
    char pad_147b3[0x8];
    int cannonshell;                   // +0x147bb
    int plasmasm1;                     // +0x147bf
    int plasmamd;                      // +0x147c3
    int ultrashell;                    // +0x147c7
    int plasmasm2;                     // +0x147cb
    int smoke1;                        // +0x147cf
    int smoke2;                        // +0x147d3
    int fire1;                         // +0x147d7
    int alfboom1;                      // +0x147db
    int radlogo;                       // +0x147df
    int radlogohigh;                   // +0x147e3
    int nuclogo;                       // +0x147e7
    int h2oboom2;                      // +0x147eb
    int lavasplash;                    // +0x147ef
    int flamestream;                   // +0x147f3
    int explosion;                     // +0x147f7
    int explode2;                      // +0x147fb
    int explode3;                      // +0x147ff
    int explode4;                      // +0x14803
    int explode5;                      // +0x14807
    int nuke1;                         // +0x1480b
    int shadow;                        // +0x1480f
    int igvictory;                     // +0x14813
    int igdefeat;                      // +0x14817
    int igpaused;                      // +0x1481b
    int panelSide[5];                  // +0x1481f
    int panelBot2[5];                  // +0x14833
    int panelBot[5];                   // +0x14847
    int vismask;                       // +0x1485b
    int black1;                        // +0x1485f
    int black2;                        // +0x14863
    int black3;                        // +0x14867
    int black4;                        // +0x1486b
    int gray1;                         // +0x1486f
    int gray2;                         // +0x14873
    int gray3;                         // +0x14877
    int gray4;                         // +0x1487b
    char pad_1487f[0x4];
    int cursorAttack;                  // +0x14883
    int cursorAirstrike;               // +0x14887
    int cursorTooFar;                  // +0x1488b
    int cursorCapture;                 // +0x1488f
    int cursorDefend;                  // +0x14893
    int cursorRepair;                  // +0x14897
    int cursorPatrol;                  // +0x1489b
    int cursorPickup;                  // +0x1489f
    int cursorTeleport;                // +0x148a3
    int cursorRevive;                  // +0x148a7
    int cursorReclamate;               // +0x148ab
    int cursorLoad;                    // +0x148af
    int cursorUnload;                  // +0x148b3
    int cursorMove;                    // +0x148b7
    int cursorSelect;                  // +0x148bb
    int cursorFindSite;                // +0x148bf
    int cursorRed;                     // +0x148c3
    int cursorGrn;                     // +0x148c7
    int cursorNormal;                  // +0x148cb
    int cursorHourglass;               // +0x148cf
    int pathIcon;                      // +0x148d3
    void* logos;                       // +0x148d7
    void* logos32;                     // +0x148db
    int blockCount;                    // +0x148df
    void** blocks;                     // +0x148e3
    int animplayCount;                 // +0x148e7
    void** animplayItems;              // +0x148eb
    int field_148ef;                   // +0x148ef
    int fxGaf;                         // +0x148f3
    int igTitles;                      // +0x148f7
    int vismasks;                      // +0x148fb
    int fog;                           // +0x148ff
    int cursors;                       // +0x14903
    int panelTop[8];                   // +0x14907
    char unknown_14927[0x33a0f - 0x14927];
    int soundCount;                    // +0x33a0f
    void* sounds[0x100];               // +0x33a13
    char soundNames[0x100][0x20];      // +0x33e13
    char soundFiles[0x100][0x20];      // +0x35e13
    char unknown_37e13[0x37f39 - 0x37e13];
    int count2;                        // +0x37f39
    char unknown_37f3d[0x38d6f - 0x37f3d];
    char progress;                     // +0x38d6f
    unsigned char loadProgress;        // +0x38d70
    char unknown_38d71[0x391f9 - 0x38d71];
    void* field_391f9;                 // +0x391f9
    void* field_391fd;                 // +0x391fd
};
#pragma pack(pop)

extern Game* g_game;
extern char DAT_005119b8[];

void* __cdecl FUN_004d83b0(const char* name, int size);
void* __cdecl FUN_004d84a0(void* param_1, const char* name, unsigned int param_3);
void __cdecl FUN_004d85a0(void* param_1);
char* __stdcall StripExtension(char* name);
int GetPreferredLanguage(void);
void* __stdcall HAPI_OpenFileRead(char* path);
int __stdcall HAPI_CloseFile(void* file);
void __stdcall FatalError(char* path);
char* __stdcall Translate(char* text);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
char* __stdcall BuildDataPath(char* buf, const char* dir, const char* name, const char* ext);

// FUNCTION: 0x429000
void CheckGpfVersion()
{
    TdfFile parser;
    char buf[64];
    char path[256];
    int found = 0;

    BuildDataPath(path, "gamedata", "version", "tdf");
    if (((TdfFile*)&parser)->LoadFile(path)) {
        if (((TdfFile*)&parser)->SelectRecord("Version")) {
            if (parser.current->GetFieldString(buf, "GPFVersion", 0x40, DAT_005119b8)) {
                found = 1;
                if (_strcmpi("v3.0", buf) != 0) {
                    OpenMessageBox(g_game->messageBox,
                                 Translate("Warning!  Your copy of Revision.GPF is the wrong version for this executable.  You may experience some problems if you continue playing.  Please download the latest version of the TA patch from www.cavedog.com and reinstall the patch."),
                                 0x1e0, 1, 1);
                }
            }
        }
        if (found == 0) {
            OpenMessageBox(g_game->messageBox,
                         Translate("Warning!  Your copy of Revision.GPF is the wrong version for this executable.  You may experience some problems if you continue playing.  Please download the latest version of the TA patch from www.cavedog.com and reinstall the patch."),
                         0x1e0, 1, 1);
        }
    }
}

// Builds "<dir>-<side>\<name>" into the caller's buffer when a side prefix is
// available and the file opens, otherwise "<dir>\<name>". An optional
// extension is appended after cutting any existing one at the last '.'.
// FUNCTION: 0x4290f0
char* __stdcall BuildDataPath(char* buf, const char* dir, const char* name, const char* ext)
{
    char* side = (char*)GetPreferredLanguage();
    if (side) {
        sprintf(buf, "%s-%s\\%s", dir, side, name);
        if (ext != 0 && strlen(ext) != 0) {
            StripExtension(buf);
            strcat(buf, ".");
            strcat(buf, ext);
        }
        void* file = HAPI_OpenFileRead(buf);
        if (file) {
            HAPI_CloseFile(file);
            return buf;
        }
    }
    strcpy(buf, dir);
    strcat(buf, "\\");
    strcat(buf, name);
    if (ext != 0 && strlen(ext) != 0) {
        StripExtension(buf);
        strcat(buf, ".");
        strcat(buf, ext);
    }
    return buf;
}

void* __stdcall LoadPcx(char* path, int param_2);

// Loads bitmaps\<name>.PCX; reports the path when loading fails.
// FUNCTION: 0x429290
void* __stdcall LoadBitmapByName(const char* name, int param_2)
{
    char path[256];
    BuildDataPath(path, "bitmaps", name, "PCX");
    void* bitmap = LoadPcx(path, param_2);
    if (bitmap == 0) {
        FatalError(path);
    }
    return bitmap;
}

void* __stdcall HAPI_LoadFile(char* path, int* outSize);

// FUNCTION: 0x4292e0
void* __stdcall LoadFontByName(const char* name)
{
    char path[256];
    BuildDataPath(path, "fonts", name, "FNT");
    void* font = HAPI_LoadFile(path, 0);
    if (font == 0) {
        FatalError(path);
    }
    return font;
}

int __stdcall HAPI_FileLengthByName(char* path);
int __stdcall LoadPcxPalette(char* path, void* data);
int __stdcall WriteBufferToFile(char* filename, void* data, int size);
void __stdcall RemoveFile(char* path);

// Loads palettes\<name>.PAL; when that is missing, builds it from the
// palettes\PALETTE.PCX file (0x400 bytes) and tries to drop the cached
// ALP/LHT/SHD derivatives so they get rebuilt.
// FUNCTION: 0x429330
void* __stdcall LoadPaletteByName(char* name)
{
    char path[256];
    char namepath[256];
    void* result;

    BuildDataPath(namepath, "palettes", name, "PAL");
    if (HAPI_FileLengthByName(namepath) != 0) {
        result = HAPI_LoadFile(namepath, 0);
        int bad = (result == 0);
        if (bad) {
            FatalError(namepath);
            return result;
        }
    }
    else {
        result = FUN_004d83b0("PALETTE", 0x400);
        BuildDataPath(path, "palettes", name, "PCX");
        if (LoadPcxPalette(path, result) == 0) {
            FatalError(path);
        }
        WriteBufferToFile(namepath, result, 0x400);
        BuildDataPath(path, "palettes", "PALETTE", "ALP");
        RemoveFile(path);
        BuildDataPath(path, "palettes", "PALETTE", "LHT");
        RemoveFile(path);
        BuildDataPath(path, "palettes", "PALETTE", "SHD");
        RemoveFile(path);
    }
    return result;
}

void* __stdcall LoadSoundFile(const char* name);

// Finds or adds a sound in the game's 32-byte sound tables: looks it up by
// logical name (in the +0x33e13 table) when one is given, otherwise by file
// name (in the +0x35e13 table), and loads the file through LoadSoundFile when
// the sound is new. Returns the index, or 0 when the table is full.
// FUNCTION: 0x429470
int __stdcall LoadSoundByName(char* name, const char* file)
{
    int i;
    for (i = 0; i < g_game->soundCount; i++) {
        if (name != 0) {
            if (g_game->soundNames[i][0] != 0 &&
                _strnicmp(g_game->soundNames[i], name, 0x20) == 0)
                return i;
        } else {
            if (_strnicmp(g_game->soundFiles[i], file, 0x20) == 0)
                return i;
        }
    }
    if (g_game->soundCount + 1 >= 0x100)
        return 0;
    g_game->sounds[g_game->soundCount] = LoadSoundFile(file);
    strncpy(g_game->soundFiles[g_game->soundCount], file, 0x20);
    if (name != 0)
        strncpy(g_game->soundNames[g_game->soundCount], name, 0x20);
    else
        strcpy(g_game->soundNames[g_game->soundCount], DAT_005119b8);
    g_game->soundCount++;
    return i;
}

struct Bitmap_004b8da0 {
    short width;              // +0x0
    short height;             // +0x2
    short field_4;            // +0x4
    short field_6;            // +0x6
    unsigned char field_8;    // +0x8
    unsigned char field_9;    // +0x9
    unsigned char field_a;    // +0xa
    unsigned char field_b;    // +0xb
    int field_c;              // +0xc
    unsigned char* data;      // +0x10
    int field_14;             // +0x14
    unsigned char pixels[1];  // +0x18
};

struct Pic_004295b0 {
    int w;               // +0x0
    int h;               // +0x4
    // Never read, but makes the local frame 0x4c bytes.
    int unknown;         // +0x8
    char header[0x40];   // +0xc
};

int __stdcall HAPI_FileLength(void* file);
int __stdcall HAPI_readfromfile(void* file, void* buf, int size);
int __stdcall HAPI_SeekFile(void* file, int pos);
Bitmap_004b8da0* __stdcall AllocFrame(char* name, int width, int height);

// Loads a picture from `path`. It reads a 0x40-byte header; if bit 0 of the
// byte at header+0x2c is set it seeks to the offset stored at header+0x28,
// reads width and height (two dwords), allocates a RADARPIC bitmap and reads
// width*height pixels into it. Otherwise it returns null. The two dwords at
// header+4 and header+8 are reported through the output pointers.
// FUNCTION: 0x4295b0
Bitmap_004b8da0* __stdcall LoadRadarPic(char* path, int* outX, int* outY)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0) {
        return 0;
    }
    HAPI_FileLength(file);
    Pic_004295b0 pic;
    HAPI_readfromfile(file, pic.header, 0x40);
    Bitmap_004b8da0* bmp;
    if (pic.header[0x2c] & 1) {
        HAPI_SeekFile(file, *(int*)(pic.header + 0x28));
        HAPI_readfromfile(file, &pic.w, 8);
        bmp = AllocFrame("RADARPIC", pic.w, pic.h);
        HAPI_readfromfile(file, bmp->data, pic.w * pic.h);
    } else {
        bmp = 0;
    }
    if (outX != 0 && outY != 0) {
        *outX = *(int*)(pic.header + 4);
        *outY = *(int*)(pic.header + 8);
    }
    HAPI_CloseFile(file);
    return bmp;
}

// Reads a whole file into a new buffer in ten chunks, updating the loading
// progress (9..90) after each chunk.
// FUNCTION: 0x429660
char* __stdcall LoadFileWithProgress(char* path)
{
    void* file = HAPI_OpenFileRead(path);
    if (file == 0) {
        FatalError(path);
    }
    int size = HAPI_FileLength(file);
    int chunk = size / 10;
    char* buf = (char*)FUN_004d83b0(path, size);
    int done = 0;
    for (int progress = 9; progress <= 90; progress += 9) {
        done += HAPI_readfromfile(file, buf + done, chunk);
        g_game->loadProgress = progress;
    }
    if (done < size) {
        HAPI_readfromfile(file, buf + done, size - done);
    }
    HAPI_CloseFile(file);
    return buf;
}

void* __stdcall LoadGaf(char* path);

// FUNCTION: 0x429700
void* __stdcall LoadAnimGaf(char* name)
{
    char path[256];
    int i;
    for (i = 0; i < g_game->animCount; i++) {
        if (_strcmpi(g_game->anims[i].name, name) == 0) {
            return g_game->anims[i].gaf;
        }
    }
    BuildDataPath(path, "anims", name, "GAF");
    void* gaf = LoadGaf(path);
    if (gaf == 0) {
        FatalError(path);
    }
    g_game->anims = (AnimEntry*)FUN_004d84a0(g_game->anims, "Animation Files",
                                             (g_game->animCount + 1) * 0x44);
    strcpy(g_game->anims[g_game->animCount].name, name);
    g_game->anims[g_game->animCount].gaf = gaf;
    g_game->animCount++;
    return gaf;
}

void* __stdcall FindGafEntry(void* gaf, const char* name);

// FUNCTION: 0x429850
void __stdcall FindGafSequence(int param_1, char* param_2)
{
    FindGafEntry((void*)param_1, param_2);
}

struct Frame_00429870 {
    char unknown_0[2];
    char flag;                          // +0x2
};

#define FLAG(p) (((Frame_00429870*)(p))->flag = 0)

// Loads the game's GAF files into the g_game resource fields (fx, igtitles,
// vismasks, fog, cursors, one per side) and then, from gamedata/sidedata.TDF,
// the base height and the per-side panel graphics.
// FUNCTION: 0x429870
void LoadGameResources()
{
    char* gaf;

    g_game->fxGaf = (int)LoadAnimGaf("fx");
    // Re-read the field into gaf after each group load, not the call's result.
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

    // Declared here, not at the top: keeps the TdfFile constructor after the head.
    TdfFile parser;
    char buf[256];

    BuildDataPath(buf, "gamedata", "sidedata", "TDF");
    ((TdfFile*)&parser)->LoadFile(buf);
    sprintf(buf, "GENERAL");
    ((TdfFile*)&parser)->ResetCurrentRecord();
    if (((TdfFile*)&parser)->SelectRecord(buf) == 1) {
        g_game->baseHeight = parser.current->GetFieldInt("baseheight", 0x1e0);
    } else {
        g_game->baseHeight = 0x1e0;
    }

    int i = 0;
    // while (1) with a mid-body break: a for (;;) gets rotated.
    while (1) {
        sprintf(buf, "SIDE%d", i);
        ((TdfFile*)&parser)->ResetCurrentRecord();
        if (!((TdfFile*)&parser)->SelectRecord(buf))
            break;
        // Kept in an int local before the test: gives cmp instead of test.
        int intgaf = parser.current->GetFieldString(buf, "intgaf", 0x1e, DAT_005119b8);
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

// Frees the game's entry table at +0x147af (each entry's pointer at +0x40),
// then the table itself, clears the count and the pointer, zeroes a second
// count-based array at +0x14907 and six scalars below it.
// FUNCTION: 0x42a010
void FreeAnimFiles()
{
    int i;
    for (i = 0; i < g_game->animCount; i++) {
        FUN_004d85a0(g_game->anims[i].gaf);
    }
    FUN_004d85a0(g_game->anims);
    g_game->animCount = 0;
    g_game->anims = 0;
    for (i = 0; i < g_game->count2; i++) {
        g_game->panelTop[i] = 0;
    }
    g_game->field_148ef = g_game->fxGaf = g_game->igTitles =
        g_game->vismasks = g_game->fog = g_game->cursors = 0;
}

// Appends an item to the game's growable "Animplay Pointers" array (the
// array that 0x415b30 walks).
// FUNCTION: 0x42a0e0
void __stdcall AddAnimplayPointer(void* item)
{
    g_game->animplayItems = (void**)FUN_004d84a0(g_game->animplayItems, "Animplay Pointers", (g_game->animplayCount + 1) * 4);
    g_game->animplayItems[g_game->animplayCount] = item;
    g_game->animplayCount++;
}

struct Ref_0042a140 {
    unsigned short index;           // +0x0
    unsigned short value;           // +0x2
    unsigned char kind;             // +0x4
    char unknown_5[3];
    void* src;                      // +0x8
};

struct Elem_0042a140 {
    int type;                       // +0x0
    char unknown_4[0xc];
    void* name;                     // +0x10
    char unknown_14[8];
    int flags;                      // +0x1c
};

struct Model_0042a140 {
    char unknown_0[8];
    int count;                      // +0x8
    char unknown_c[0x1c];
    Elem_0042a140* entries;         // +0x28
    Model_0042a140* child1;         // +0x2c
    Model_0042a140* child2;         // +0x30
};

void __stdcall InitGafSequence(Ref_0042a140* ref, void* src, int index);

// Walks a loaded 3DO model: recurses into its two child models, then for each
// object resolves its texture name against the loaded GAF files and either
// points it at a plain texture or starts an animation sequence.
// FUNCTION: 0x42a140
void __stdcall BindModelTextures(Model_0042a140* model, const char* name)
{
    if (model->child1)
        BindModelTextures(model->child1, name);
    if (model->child2)
        BindModelTextures(model->child2, name);

    Elem_0042a140* elem = model->entries;
    for (int i = 0; i < model->count; i++, elem++) {
        if (elem->name) {
            // Left uninitialised: initialising it to 0 changes the code.
            unsigned short* entry;
            elem->flags &= ~4;
            for (int j = 0; j < g_game->blockCount; j++) {
                entry = (unsigned short*)FindGafEntry(g_game->blocks[j], (const char*)elem->name);
                if (entry)
                    break;
            }
            if (entry == 0) {
                entry = (unsigned short*)FindGafEntry(g_game->logos, (const char*)elem->name);
                if (entry != 0 && *entry == 10)
                    elem->flags |= 4;
                if (entry == 0) {
                    // Flag 1 before the colour store, which stays last: keeps the blocks from tail-merging.
                    elem->flags |= 1;
                    elem->type = 0xd1;
                    continue;
                }
            }
            if (*entry > 1) {
                InitGafSequence((Ref_0042a140*)&elem->name, entry, 0);
                elem->flags |= 2;
                if (!(elem->flags & 4)) {
                    g_game->animplayItems = (void**)FUN_004d84a0(g_game->animplayItems, "Animplay Pointers", (g_game->animplayCount + 1) * 4);
                    g_game->animplayItems[g_game->animplayCount] = &elem->name;
                    g_game->animplayCount++;
                }
                continue;
            }
            // Texture stored before the flag 2 clear.
            elem->name = *(void**)((char*)entry + 0x28);
            elem->flags &= ~2;
        }
    }
}

void* __stdcall Load3do(char* param);
void __stdcall MirrorObject(void* data);

// Loads objects3d\<name>.3DO: aborts with the path if the file is missing,
// then prepares the loaded model.
// FUNCTION: 0x42a2c0
void* __stdcall LoadObject3d(const char* name)
{
    char path[256];
    BuildDataPath(path, "objects3d", name, "3DO");
    void* data = Load3do(path);
    if (data == 0) {
        FatalError(path);
    }
    MirrorObject(data);
    BindModelTextures((Model_0042a140*)data, name);
    return data;
}

// Loads the two game fonts (COMIX and smlfont) into g_game; the loader is
// 0x4292e0, inlined twice.
// FUNCTION: 0x42a320
void LoadGameFonts()
{
    g_game->field_391f9 = LoadFontByName("COMIX");
    g_game->field_391fd = LoadFontByName("smlfont");
}

// FUNCTION: 0x42a3b0
void FreeGameFonts()
{
    FUN_004d85a0(g_game->field_391fd);
    g_game->field_391fd = 0;
    FUN_004d85a0(g_game->field_391f9);
    g_game->field_391f9 = 0;
}

// FUNCTION: 0x42a400
int LoadDefaultPalette()
{
    void* palette = LoadPaletteByName("PALETTE");
    memcpy(g_game->palette, palette, 0x400);
    FUN_004d85a0(palette);
    return 1;
}

struct FindData_0042a440 {
    char unknown_0[0x14];
    char name[260];                    // +0x14
};

int __stdcall CountDirectoryEntries(const char* path, int flag);
int __stdcall HAPI_FindFirst(const char* path, FindData_0042a440* fd, int a, int b);
int __stdcall HAPI_FindNext(int handle, FindData_0042a440* fd);
void __stdcall HAPI_FindClose(int handle);

// FUNCTION: 0x42a440
void LoadTextureGafs()
{
    char path[256];
    FindData_0042a440 fd;

    BuildDataPath(path, "textures", "*", "GAF");
    int count = CountDirectoryEntries(path, 0);
    g_game->blockCount = count - 1;
    void** texturePtrs = (void**)FUN_004d83b0("TEXTURE PTRS", count * 4);
    g_game->blocks = texturePtrs;
    int handle = HAPI_FindFirst(path, &fd, -1, 1);
    if (handle != -1) {
        int progress = 0;
        int r;
        do {
            if (_strcmpi(fd.name, "logos.GAF") == 0) {
                texturePtrs--;
            } else {
                BuildDataPath(path, "textures", fd.name, "GAF");
                *texturePtrs = LoadGaf(path);
                g_game->progress = (char)(progress / (count - 1));
            }
            r = HAPI_FindNext(handle, &fd);
            texturePtrs++;
            progress += 100;
        } while (r == 0);
        g_game->progress = 100;
        HAPI_FindClose(handle);
    }
    g_game->animplayCount = 0;
    g_game->animplayItems = 0;
}

// Frees the game's block table at +0x148e3 (each of its count entries, then
// the table itself) and the buffer at +0x148eb, clearing the pointers and
// the size at +0x148e7.
// FUNCTION: 0x42a570
void FreeTextureGafs()
{
    for (int i = 0; i < g_game->blockCount; i++) {
        FUN_004d85a0(g_game->blocks[i]);
        g_game->blocks[i] = 0;
    }
    FUN_004d85a0(g_game->blocks);
    g_game->blocks = 0;
    if (g_game->animplayItems) {
        FUN_004d85a0(g_game->animplayItems);
        g_game->animplayItems = 0;
        g_game->animplayCount = 0;
    }
}

#include <vector>

class Class_004c9390 {
public:
    char* data;

    void ReleaseRef();
};

class Class_004c91a0 {
public:
    char* p;

    ~Class_004c91a0() { ((Class_004c9390*)this)->ReleaseRef(); }
};

#pragma pack(push, 1)
struct Def_0042a610 {
    char unknown_0[0x20];
    char name[0x122];                  // +0x20
    unsigned int field_142;            // +0x142
    unsigned int field_146;            // +0x146
};
#pragma pack(pop)

int __stdcall ComputeChecksum(unsigned char* data, int len);
void __stdcall ListDirectory(const char* pattern, int flags, std::vector<Class_004c91a0>* out);
void* __stdcall HAPI_LoadOpenFile(char* name, void* f, unsigned int* outSize);
void __cdecl ProtectBlockReadWrite(void* p);
void __cdecl ProtectBlockReadOnly(void* p);

// Loads a unit type's script (scripts\NAME.cob), every GUI file matching
// guis\NAME*.gui and its download file (download\NAME.tdf), XORs the 4-byte
// checksum of each file into field_142, and locks the unit type table while
// reading. Does nothing once field_142 is nonzero.
// FUNCTION: 0x42a610
void __stdcall ComputeUnitScriptChecksum(Def_0042a610* def)
{
    if (def->field_142)
        return;

    ProtectBlockReadWrite(g_game->field_1439b);

    char path[256];
    int size;
    BuildDataPath(path, "scripts", def->name, "cob");
    void* data = HAPI_LoadFile(path, &size);
    if (data) {
        def->field_142 ^= ComputeChecksum((unsigned char*)data, size);
        FUN_004d85a0(data);
    }

    std::vector<Class_004c91a0> files;
    char name[64];
    strcpy(name, def->name);
    strcat(name, "*");

    BuildDataPath(path, "guis", name, "gui");
    ListDirectory(path, 0, &files);

    for (unsigned int i = 0; i < files.size(); i++) {
        BuildDataPath(path, "guis", files[i].p, "gui");
        void* data2 = HAPI_LoadFile(path, &size);
        if (data2) {
            def->field_142 ^= ComputeChecksum((unsigned char*)data2, size);
            FUN_004d85a0(data2);
        }
    }

    BuildDataPath(path, "download", def->name, "tdf");
    void* f = HAPI_OpenFileRead(path);
    if (f) {
        size = HAPI_FileLength(f);
        if (size > 0) {
            void* data3 = HAPI_LoadOpenFile(path, f, 0);
            if (data3) {
                def->field_142 ^= ComputeChecksum((unsigned char*)data3, size);
                FUN_004d85a0(data3);
            }
        }
        HAPI_CloseFile(f);
    }

    def->field_142 ^= def->field_146;
    ProtectBlockReadOnly(g_game->field_1439b);
}

// The compiler-generated vector deleting destructor of the 12-byte class whose
// constructor is 0x4c2ea0 and destructor 0x4c2eb0 (a global of this class,
// DAT_0051f310, is built by 0x49e610 and destroyed by its atexit handler
// 0x49e630).
//
// The global below exists only to make the compiler emit the COMDAT here.
// FUNCTION: 0x42a870 ??_ETdfFile@@QAEPAXI@Z
static TdfFile* s_array = new TdfFile[1];
