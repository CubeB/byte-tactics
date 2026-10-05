// The console commands: what the player can type after Enter, each with the
// function that runs it. 0x419560 hands the three tables to 0x4b7760, which
// files every name in a sorted table with its handler and its mask; a cheat
// (mask 2) works only once cheats are on, a debug command (mask 4) only in a
// debug build. Each table ends with a record whose name is null.

struct CommandArgs;     // the parsed command line (Class_004b73e0)

struct ConsoleCommand {
    const char* name;                             // +0x0
    void (__stdcall* run)(CommandArgs* args);     // +0x4
    int mask;                                     // +0x8
};

void __stdcall FUN_00416240(CommandArgs* args);
void __stdcall FUN_00416270(CommandArgs* args);
void __stdcall FUN_00416280(CommandArgs* args);
void __stdcall FUN_00416310(CommandArgs* args);
void __stdcall FUN_00416370(CommandArgs* args);
void __stdcall FUN_00416380(CommandArgs* args);
void __stdcall FUN_00416390(CommandArgs* args);
void __stdcall FUN_004163a0(CommandArgs* args);
void __stdcall FUN_004163d0(CommandArgs* args);
void __stdcall FUN_00416420(CommandArgs* args);
void __stdcall FUN_00416460(CommandArgs* args);
void __stdcall FUN_004164b0(CommandArgs* args);
void __stdcall FUN_00416500(CommandArgs* args);
void __stdcall FUN_00416510(CommandArgs* args);
void __stdcall FUN_00416550(CommandArgs* args);
void __stdcall FUN_00416590(CommandArgs* args);
void __stdcall FUN_004165c0(CommandArgs* args);
void __stdcall FUN_00416630(CommandArgs* args);
void __stdcall FUN_00416660(CommandArgs* args);
void __stdcall FUN_00416690(CommandArgs* args);
void __stdcall FUN_004166c0(CommandArgs* args);
void __stdcall FUN_00416710(CommandArgs* args);
void __stdcall FUN_00416730(CommandArgs* args);
void __stdcall FUN_00416780(CommandArgs* args);
void __stdcall FUN_004167f0(CommandArgs* args);
void __stdcall FUN_00416810(CommandArgs* args);
void __stdcall FUN_00416820(CommandArgs* args);
void __stdcall FUN_00416860(CommandArgs* args);
void __stdcall FUN_004168d0(CommandArgs* args);
void __stdcall FUN_004169d0(CommandArgs* args);
void __stdcall FUN_00416a30(CommandArgs* args);
void __stdcall FUN_00416a90(CommandArgs* args);
void __stdcall FUN_00416ab0(CommandArgs* args);
void __stdcall FUN_00416b50(CommandArgs* args);
void __stdcall FUN_00416bd0(CommandArgs* args);
void __stdcall FUN_00416cf0(CommandArgs* args);
void __stdcall FUN_00416d20(CommandArgs* args);
void __stdcall FUN_00416d50(CommandArgs* args);
void __stdcall FUN_00416d80(CommandArgs* args);
void __stdcall FUN_00416db0(CommandArgs* args);
void __stdcall FUN_00416e00(CommandArgs* args);
void __stdcall FUN_00416e30(CommandArgs* args);
void __stdcall FUN_00416e60(CommandArgs* args);
void __stdcall FUN_00416e90(CommandArgs* args);
void __stdcall FUN_00417030(CommandArgs* args);
void __stdcall FUN_00417060(CommandArgs* args);
void __stdcall FUN_00417090(CommandArgs* args);
void __stdcall FUN_004170c0(CommandArgs* args);
void __stdcall FUN_00417130(CommandArgs* args);
void __stdcall FUN_00417150(CommandArgs* args);
void __stdcall FUN_004171f0(CommandArgs* args);
void __stdcall FUN_00417290(CommandArgs* args);
void __stdcall FUN_004172e0(CommandArgs* args);
void __stdcall FUN_00417300(CommandArgs* args);
void __stdcall FUN_00417330(CommandArgs* args);
void __stdcall FUN_004173e0(CommandArgs* args);
void __stdcall FUN_00417430(CommandArgs* args);
void __stdcall FUN_00417490(CommandArgs* args);
void __stdcall FUN_004174d0(CommandArgs* args);
void __stdcall FUN_004174e0(CommandArgs* args);
void __stdcall FUN_00417520(CommandArgs* args);
void __stdcall FUN_00417540(CommandArgs* args);
void __stdcall FUN_00417570(CommandArgs* args);
void __stdcall FUN_004175e0(CommandArgs* args);
void __stdcall FUN_00417600(CommandArgs* args);
void __stdcall FUN_00417760(CommandArgs* args);
void __stdcall FUN_004177a0(CommandArgs* args);
void __stdcall FUN_004177e0(CommandArgs* args);
void __stdcall FUN_00417a60(CommandArgs* args);
void __stdcall FUN_00418bb0(CommandArgs* args);
void __stdcall FUN_00418c70(CommandArgs* args);
void __stdcall FUN_00418ca0(CommandArgs* args);
void __stdcall FUN_00418cd0(CommandArgs* args);
void __stdcall FUN_00418d90(CommandArgs* args);
void __stdcall FUN_00418e50(CommandArgs* args);
void __stdcall FUN_00418fd0(CommandArgs* args);
void __stdcall FUN_00419090(CommandArgs* args);
void __stdcall FUN_00419340(CommandArgs* args);
void __stdcall FUN_00419400(CommandArgs* args);
void __stdcall FUN_004194c0(CommandArgs* args);
void __stdcall FUN_004194d0(CommandArgs* args);
void __stdcall FUN_00419540(CommandArgs* args);
void __stdcall FUN_00419550(CommandArgs* args);

// The commands anyone may type (mask 1).
// GLOBAL: 0x501d38
ConsoleCommand g_consoleCommands[44] = {
    {"NoShake", FUN_00416e60, 1},
    {"Contour", FUN_00416db0, 1},
    {"ScrollSpeed", FUN_00416cf0, 1},
    {"IFace", FUN_00416d20, 1},
    {"Give", FUN_00416bd0, 1},
    {"CDPlay", FUN_004167f0, 1},
    {"CDStop", FUN_00416810, 1},
    {"Sound3D", FUN_00416820, 1},
    {"Shading", FUN_00416420, 1},
    {"AntiAlias", FUN_00416510, 1},
    {"Shadow", FUN_00416550, 1},
    {"Dither", FUN_00416590, 1},
    {"SwitchAlt", FUN_004165c0, 1},
    {"TShadow", FUN_00416630, 1},
    {"FShadow", FUN_00416660, 1},
    {"LOSType", FUN_00416690, 1},
    {"Light", FUN_004166c0, 1},
    {"RCache", FUN_00416710, 1},
    {"Selectable", FUN_00416460, 1},
    {"MusicMode", FUN_004175e0, 1},
    {"Logo", FUN_004168d0, 1},
    {"ScreenChat", FUN_00417130, 1},
    {"Gamma", FUN_00417290, 1},
    {"Clock", FUN_00417300, 1},
    {"NetStats", FUN_00417570, 1},
    {"Sing", FUN_004172e0, 1},
    {"NoMetal", FUN_00417150, 1},
    {"NoEnergy", FUN_004171f0, 1},
    {"BigBrother", FUN_004174e0, 1},
    {"Now", FUN_00416e90, 1},
    {"Drop", FUN_004177a0, 1},
    {"ShootAll", FUN_00418ca0, 1},
    {"ShareMetal", FUN_00418cd0, 1},
    {"ShareEnergy", FUN_00418d90, 1},
    {"ShareMapping", FUN_00418e50, 1},
    {"ShareRadar", FUN_00418fd0, 1},
    {"ShareAll", FUN_00419090, 1},
    {"ShowRanges", FUN_004194c0, 1},
    {"SetShareMetal", FUN_00419340, 1},
    {"SetShareEnergy", FUN_00419400, 1},
    {"Compression", FUN_004194d0, 1},
    {"BPS", FUN_00419540, 1},
    {"SFX", FUN_00419550, 1},
    {0, 0, 0},
};

// The cheats (mask 2).
// GLOBAL: 0x501f48
ConsoleCommand g_cheatCommands[11] = {
    {"Radar", FUN_00417090, 2},
    {"ATM", FUN_004170c0, 2},
    {"View", FUN_00416b50, 2},
    {"LOS", FUN_00416d50, 2},
    {"Mapping", FUN_00416d80, 2},
    {"DoubleShot", FUN_00417030, 2},
    {"HalfShot", FUN_00417060, 2},
    {"NowISee", FUN_00417540, 2},
    {"Meteor", FUN_00417760, 2},
    {"MakePoster", FUN_00417600, 2},
    {0, 0, 0},
};

// GLOBAL: 0x501fcc
int DAT_00501fcc = 0;

// The debug commands (mask 4).
// GLOBAL: 0x501fd0
ConsoleCommand g_debugCommands[31] = {
    {"AI", FUN_00416280, 4},
    {"Control", FUN_00416ab0, 4},
    {"Kill", FUN_004164b0, 4},
    {"IWin", FUN_004169d0, 4},
    {"ILose", FUN_00416a30, 4},
    {"Film", FUN_00417330, 4},
    {"FilmSpeed", FUN_004173e0, 4},
    {"Assert", FUN_00416370, 4},
    {"Assign", FUN_00416310, 4},
    {"BurnAll", FUN_00416390, 4},
    {"BurnOne", FUN_004163a0, 4},
    {"DebugBreak", FUN_00417a60, 4},
    {"DPrint", FUN_00416380, 4},
    {"Edge", FUN_00416730, 4},
    {"Include", FUN_004177e0, 4},
    {"Mem", FUN_00416270, 4},
    {"MemDump", FUN_00416240, 4},
    {"Move", FUN_00416860, 4},
    {"PrintWeights", FUN_00418bb0, 4},
    {"Profile", FUN_00417520, 4},
    {"Reload", FUN_00417490, 4},
    {"ReloadAIProfiles", FUN_004174d0, 4},
    {"Save", FUN_00417430, 4},
    {"SeaLevel", FUN_00416a90, 4},
    {"Search", FUN_00416780, 4},
    {"SelBoxes", FUN_00416e00, 4},
    {"Senderror", FUN_00418c70, 4},
    {"TreeDeath", FUN_00416e30, 4},
    {"Feature", FUN_004163d0, 4},
    {"ZBuffer", FUN_00416500, 4},
    {0, 0, 0},
};
