// The console commands: what the player can type after Enter, each with the
// function that runs it. 0x419560 hands the three tables to 0x4b7760, which
// files every name in a sorted table with its handler and its mask; a cheat
// (mask 2) works only once cheats are on, a debug command (mask 4) only in a
// debug build. Each table ends with a record whose name is null.

struct CommandArgs;     // the parsed command line (CommandArgs)

struct ConsoleCommand {
    const char* name;                             // +0x0
    void (__stdcall* run)(CommandArgs* args);     // +0x4
    int mask;                                     // +0x8
};

void __stdcall CmdMemDump(CommandArgs* args);
void __stdcall CmdMem(CommandArgs* args);
void __stdcall CmdAI(CommandArgs* args);
void __stdcall CmdAssign(CommandArgs* args);
void __stdcall CmdAssert(CommandArgs* args);
void __stdcall CmdDPrint(CommandArgs* args);
void __stdcall CmdBurnAll(CommandArgs* args);
void __stdcall CmdBurnOne(CommandArgs* args);
void __stdcall CmdFeature(CommandArgs* args);
void __stdcall CmdShading(CommandArgs* args);
void __stdcall CmdSelectable(CommandArgs* args);
void __stdcall CmdKill(CommandArgs* args);
void __stdcall CmdZBuffer(CommandArgs* args);
void __stdcall CmdAntiAlias(CommandArgs* args);
void __stdcall CmdShadow(CommandArgs* args);
void __stdcall CmdDither(CommandArgs* args);
void __stdcall CmdSwitchAlt(CommandArgs* args);
void __stdcall CmdTShadow(CommandArgs* args);
void __stdcall CmdFShadow(CommandArgs* args);
void __stdcall CmdLOSType(CommandArgs* args);
void __stdcall CmdLight(CommandArgs* args);
void __stdcall CmdRCache(CommandArgs* args);
void __stdcall CmdEdge(CommandArgs* args);
void __stdcall CmdSearch(CommandArgs* args);
void __stdcall CmdCDPlay(CommandArgs* args);
void __stdcall CmdCDStop(CommandArgs* args);
void __stdcall CmdSound3D(CommandArgs* args);
void __stdcall CmdMove(CommandArgs* args);
void __stdcall CmdLogo(CommandArgs* args);
void __stdcall CmdIWin(CommandArgs* args);
void __stdcall CmdILose(CommandArgs* args);
void __stdcall CmdSeaLevel(CommandArgs* args);
void __stdcall CmdControl(CommandArgs* args);
void __stdcall CmdView(CommandArgs* args);
void __stdcall CmdGive(CommandArgs* args);
void __stdcall CmdScrollSpeed(CommandArgs* args);
void __stdcall CmdIFace(CommandArgs* args);
void __stdcall CmdLOS(CommandArgs* args);
void __stdcall CmdMapping(CommandArgs* args);
void __stdcall CmdContour(CommandArgs* args);
void __stdcall CmdSelBoxes(CommandArgs* args);
void __stdcall CmdTreeDeath(CommandArgs* args);
void __stdcall CmdNoShake(CommandArgs* args);
void __stdcall CmdNow(CommandArgs* args);
void __stdcall CmdDoubleShot(CommandArgs* args);
void __stdcall CmdHalfShot(CommandArgs* args);
void __stdcall CmdRadar(CommandArgs* args);
void __stdcall CmdATM(CommandArgs* args);
void __stdcall CmdScreenChat(CommandArgs* args);
void __stdcall CmdNoMetal(CommandArgs* args);
void __stdcall CmdNoEnergy(CommandArgs* args);
void __stdcall CmdGamma(CommandArgs* args);
void __stdcall CmdSing(CommandArgs* args);
void __stdcall CmdClock(CommandArgs* args);
void __stdcall CmdFilm(CommandArgs* args);
void __stdcall CmdFilmSpeed(CommandArgs* args);
void __stdcall CmdSave(CommandArgs* args);
void __stdcall CmdReload(CommandArgs* args);
void __stdcall CmdReloadAIProfiles(CommandArgs* args);
void __stdcall CmdBigBrother(CommandArgs* args);
void __stdcall CmdProfile(CommandArgs* args);
void __stdcall CmdNowISee(CommandArgs* args);
void __stdcall CmdNetStats(CommandArgs* args);
void __stdcall CmdMusicMode(CommandArgs* args);
void __stdcall CmdMakePoster(CommandArgs* args);
void __stdcall CmdMeteor(CommandArgs* args);
void __stdcall CmdDrop(CommandArgs* args);
void __stdcall CmdInclude(CommandArgs* args);
void __stdcall CmdDebugBreak(CommandArgs* args);
void __stdcall CmdPrintWeights(CommandArgs* args);
void __stdcall CmdSenderror(CommandArgs* args);
void __stdcall CmdShootAll(CommandArgs* args);
void __stdcall CmdShareMetal(CommandArgs* args);
void __stdcall CmdShareEnergy(CommandArgs* args);
void __stdcall CmdShareMapping(CommandArgs* args);
void __stdcall CmdShareRadar(CommandArgs* args);
void __stdcall CmdShareAll(CommandArgs* args);
void __stdcall CmdSetShareMetal(CommandArgs* args);
void __stdcall CmdSetShareEnergy(CommandArgs* args);
void __stdcall CmdShowRanges(CommandArgs* args);
void __stdcall CmdCompression(CommandArgs* args);
void __stdcall CmdBPS(CommandArgs* args);
void __stdcall CmdSFX(CommandArgs* args);

// The commands anyone may type (mask 1).
// GLOBAL: 0x501d38
ConsoleCommand g_consoleCommands[44] = {
    {"NoShake", CmdNoShake, 1},
    {"Contour", CmdContour, 1},
    {"ScrollSpeed", CmdScrollSpeed, 1},
    {"IFace", CmdIFace, 1},
    {"Give", CmdGive, 1},
    {"CDPlay", CmdCDPlay, 1},
    {"CDStop", CmdCDStop, 1},
    {"Sound3D", CmdSound3D, 1},
    {"Shading", CmdShading, 1},
    {"AntiAlias", CmdAntiAlias, 1},
    {"Shadow", CmdShadow, 1},
    {"Dither", CmdDither, 1},
    {"SwitchAlt", CmdSwitchAlt, 1},
    {"TShadow", CmdTShadow, 1},
    {"FShadow", CmdFShadow, 1},
    {"LOSType", CmdLOSType, 1},
    {"Light", CmdLight, 1},
    {"RCache", CmdRCache, 1},
    {"Selectable", CmdSelectable, 1},
    {"MusicMode", CmdMusicMode, 1},
    {"Logo", CmdLogo, 1},
    {"ScreenChat", CmdScreenChat, 1},
    {"Gamma", CmdGamma, 1},
    {"Clock", CmdClock, 1},
    {"NetStats", CmdNetStats, 1},
    {"Sing", CmdSing, 1},
    {"NoMetal", CmdNoMetal, 1},
    {"NoEnergy", CmdNoEnergy, 1},
    {"BigBrother", CmdBigBrother, 1},
    {"Now", CmdNow, 1},
    {"Drop", CmdDrop, 1},
    {"ShootAll", CmdShootAll, 1},
    {"ShareMetal", CmdShareMetal, 1},
    {"ShareEnergy", CmdShareEnergy, 1},
    {"ShareMapping", CmdShareMapping, 1},
    {"ShareRadar", CmdShareRadar, 1},
    {"ShareAll", CmdShareAll, 1},
    {"ShowRanges", CmdShowRanges, 1},
    {"SetShareMetal", CmdSetShareMetal, 1},
    {"SetShareEnergy", CmdSetShareEnergy, 1},
    {"Compression", CmdCompression, 1},
    {"BPS", CmdBPS, 1},
    {"SFX", CmdSFX, 1},
    {0, 0, 0},
};

// The cheats (mask 2).
// GLOBAL: 0x501f48
ConsoleCommand g_cheatCommands[11] = {
    {"Radar", CmdRadar, 2},
    {"ATM", CmdATM, 2},
    {"View", CmdView, 2},
    {"LOS", CmdLOS, 2},
    {"Mapping", CmdMapping, 2},
    {"DoubleShot", CmdDoubleShot, 2},
    {"HalfShot", CmdHalfShot, 2},
    {"NowISee", CmdNowISee, 2},
    {"Meteor", CmdMeteor, 2},
    {"MakePoster", CmdMakePoster, 2},
    {0, 0, 0},
};

// GLOBAL: 0x501fcc
int DAT_00501fcc = 0;

// The debug commands (mask 4).
// GLOBAL: 0x501fd0
ConsoleCommand g_debugCommands[31] = {
    {"AI", CmdAI, 4},
    {"Control", CmdControl, 4},
    {"Kill", CmdKill, 4},
    {"IWin", CmdIWin, 4},
    {"ILose", CmdILose, 4},
    {"Film", CmdFilm, 4},
    {"FilmSpeed", CmdFilmSpeed, 4},
    {"Assert", CmdAssert, 4},
    {"Assign", CmdAssign, 4},
    {"BurnAll", CmdBurnAll, 4},
    {"BurnOne", CmdBurnOne, 4},
    {"DebugBreak", CmdDebugBreak, 4},
    {"DPrint", CmdDPrint, 4},
    {"Edge", CmdEdge, 4},
    {"Include", CmdInclude, 4},
    {"Mem", CmdMem, 4},
    {"MemDump", CmdMemDump, 4},
    {"Move", CmdMove, 4},
    {"PrintWeights", CmdPrintWeights, 4},
    {"Profile", CmdProfile, 4},
    {"Reload", CmdReload, 4},
    {"ReloadAIProfiles", CmdReloadAIProfiles, 4},
    {"Save", CmdSave, 4},
    {"SeaLevel", CmdSeaLevel, 4},
    {"Search", CmdSearch, 4},
    {"SelBoxes", CmdSelBoxes, 4},
    {"Senderror", CmdSenderror, 4},
    {"TreeDeath", CmdTreeDeath, 4},
    {"Feature", CmdFeature, 4},
    {"ZBuffer", CmdZBuffer, 4},
    {0, 0, 0},
};
