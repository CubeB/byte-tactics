// Decompiled by longcat-2.5-preview-free, edited and finished by deepseek-v4.1-flash, finished by DeepSeek V4.1 Flash, finished by Claude Opus 5.5. Names are provisional.
//
// The front-end state machine of frontend.cpp (line numbers in the
// CheckFrontendStateChange calls are the original __LINE__ values).
// Unused: without it the case 16:21 player loop addresses [esi+ecx].
#include <string>
#include <string.h>
#include <stdio.h>

// GLOBAL: 0x511de8
extern char* g_game;

// GLOBAL: 0x511fb8
extern char DAT_00511fb8[];

// GLOBAL: 0x512c80
extern int DAT_00512c80;

// GLOBAL: 0x503004
extern char DAT_00503004[];

// GLOBAL: 0x50329c
extern char DAT_0050329c[];

// GLOBAL: 0x503294
extern char DAT_00503294[];

// GLOBAL: 0x50328c
extern char DAT_0050328c[];

// GLOBAL: 0x503284
extern char DAT_00503284[];

// GLOBAL: 0x50327c
extern char DAT_0050327c[];

// GLOBAL: 0x50324c
extern char DAT_0050324c[];

// GLOBAL: 0x502f9c
extern char DAT_00502f9c[];

// GLOBAL: 0x4fcdc8
extern char DAT_004fcdc8[];

// GLOBAL: 0x4fcdb8
extern char DAT_004fcdb8[];

// GLOBAL: 0x4fcda8
extern char DAT_004fcda8[];

// GLOBAL: 0x4fdaf0
struct V4i { int a; int b; int c; int d; };
extern V4i DAT_004fdaf0;

struct Bits_00426e80 {
    unsigned short b0 : 1;
    unsigned short b1 : 1;
    unsigned short b2 : 1;
    unsigned short b3 : 1;
    unsigned short b4 : 1;
    unsigned short : 11;
};

struct Obj_00426e80 {
    char unknown_0[0xf0];
    unsigned short bit0 : 1;
    unsigned short flag : 1;           // +0xf0, mask 2
};

#pragma pack(push, 1)
struct Pd_00426e80 {
    char unknown_0[0x97];
    unsigned short ready : 1;          // +0x97
    unsigned short rest_97 : 15;       // +0x97
    char unknown_99[2];                // +0x99
    unsigned short : 6;                // +0x9b
    unsigned short b6 : 1;             // +0x9b, mask 0x40
    unsigned short : 9;
};
#pragma pack(pop)

int CodeChecksumFailed(void);
void __stdcall OpenMessageBox(char* dest, char* text, int param_3, int param_4, int param_5);
void __stdcall SetCursorOverlayEnabled(int param);
void __stdcall FUN_00434ab0(int param);
void __stdcall PlayMovie(char* param);
void __stdcall OpenNewGameMenu(int param);
void __stdcall SetGameMode(int param);
void __stdcall FUN_0041d9f0(int param);
int __stdcall SelectConnection(int param);
int __stdcall CreateLocalPlayer(unsigned char playerIndex, int param2);
int __stdcall JoinNetGame(V4i v, int idx);
int __stdcall GetTextPixelWidth(char* param);
void __stdcall AddNetPlayer(int param);
void __stdcall SetOffscreenSurface(int param);
void __stdcall FillSurface(int param1, int param2);
void __stdcall FlipScreen(void);
void __stdcall QuitApp(int param);
void __stdcall HAPINET_guaranteepackets(int param);
void __stdcall CloseTopScreen(int param);
void __stdcall FUN_004ab0a0(int param);
void __stdcall HAPINET_quitgame(int param);
void __stdcall InitPacketManager(int param1, int param2);
void __stdcall PopKey(void);
Obj_00426e80* __stdcall GetDisplay(void);
void __stdcall OpenMainMenu(void);
void __stdcall SaveSettings(void);
int __stdcall InitLobbiedConnection(void);
void __stdcall ResetPlayerSlots(void);
void __stdcall OpenSingleMenu(void);
void __stdcall LoadSettings(void);
void __stdcall OpenMissionBriefing(void);
void __stdcall OpenSkirmishMenu(void);
void __stdcall FillProviderList(void);
void __stdcall OpenModemDialog(void);
void __stdcall OpenSerialDialog(void);
void __stdcall OpenTcpDialog(void);
void __stdcall OpenSelectGameDialog(void);
void __stdcall CloseNetSession(void);
void __stdcall CreateNetGame(void);
int __stdcall InitNetConnection(void);
void __stdcall LeaveNetGame(void);
void __stdcall BroadcastPlayerInfo(void);
void __stdcall OpenOptionsPanel(void);
void __stdcall FinishUnitSync(void);
void __stdcall OpenBattleRoom(void);
void __stdcall UpdateBattleRoom(void);
int __stdcall GetServiceProviderIndex(void);
void __stdcall ReportGameEvent(int param);
void __stdcall DeleteUnitSync(void);
void __stdcall ShutdownScoreTables(void);
void __stdcall FUN_00491a70(void);
int __stdcall InitScoreReporting(void);
void __stdcall ShowEndMissionScreen(void);
void __stdcall FUN_004c2470(void);
void __stdcall FUN_004c2870(void);
struct Player { void SetType(int param); };
struct Mission { void LoadMissionByName(int param); };

void __stdcall CheckFrontendStateChange(int line, char* file);
void __stdcall SetFrontendSubState(char state, int line, char* file);
void __stdcall SetFrontendState(char state, int line, char* file);

// Inlined copies of the front-end state helpers, one per inlining outcome
// (see the top of the file). LogStateChange is CheckFrontendStateChange.
static void LogStateChange(int line, char* file)
{
    char buf[256];
    if (CodeChecksumFailed()) {
        sprintf(buf, DAT_00502f9c, line, file);
        OpenMessageBox(g_game + 0x519, buf, 500, 1, 1);
    }
}

// SetFrontendSubState with its log call left out of line.
static void SetSubState(char state, int line, char* file)
{
    CheckFrontendStateChange(line, file);
    g_game[0x2bbf] = state;
    g_game[0x2bc0] = state;
}

static void SetSubStateLogged(char state, int line, char* file)
{
    LogStateChange(line, file);
    g_game[0x2bbf] = state;
    g_game[0x2bc0] = state;
}

// SetFrontendState with the sub-state change inlined too.
static void SetState(char state, int line, char* file)
{
    CheckFrontendStateChange(line, file);
    g_game[0x2bbe] = state;
    SetSubState(0, 0x9b, DAT_00503004);
}

// SetFrontendState with the sub-state change left out of line.
static void SetStateSubCall(char state, int line, char* file)
{
    CheckFrontendStateChange(line, file);
    g_game[0x2bbe] = state;
    SetFrontendSubState(0, 0x9b, DAT_00503004);
}

static void SetStateLogged(char state, int line, char* file)
{
    LogStateChange(line, file);
    g_game[0x2bbe] = state;
    SetFrontendSubState(0, 0x9b, DAT_00503004);
}

// ApplyPendingSubState.
static void UpdateSubState()
{
    char next = g_game[0x2bc0];
    if (next != g_game[0x2bbf])
        SetSubState(next, 0xa3, DAT_00503004);
}

// Real frontend.cpp helpers that are always inlined here; they have their own
// files, so they are defined without annotations.
void __stdcall SetFrontendErrorText(char* text)
{
    strncpy(DAT_00511fb8, text, 0xf9);
}

void ShowFrontendErrorText()
{
    if (strlen(DAT_00511fb8) != 0) {
        OpenMessageBox(g_game + 0x519, DAT_00511fb8, GetTextPixelWidth(DAT_00511fb8) + 0x14, 1, 1);
        DAT_00511fb8[0] = 0;
    }
}

void BlankScreen()
{
    SetOffscreenSurface(*(int*)(g_game + 0x37e1b));
    FillSurface(0, 0);
    FlipScreen();
}

void FUN_00425b60()
{
    SetOffscreenSurface(*(int*)(g_game + 0x37e1b));
    FUN_004c2470();
    FUN_004c2870();
    FlipScreen();
}

// ConnectToService, as inlined in case 15 and in case 20.
static int UseService(void)
{
    if (InitNetConnection()) {
        ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
        return 1;
    }
    SetFrontendErrorText(DAT_0050324c);
    SetStateSubCall(0xf, 0x3bc, DAT_00503004);
    SetFrontendSubState(0, 0x3bd, DAT_00503004);
    return 0;
}

static int UseServiceCalls(void)
{
    if (InitNetConnection()) {
        ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
        return 1;
    }
    SetFrontendErrorText(DAT_0050324c);
    SetFrontendState(0xf, 0x3bc, DAT_00503004);
    SetFrontendSubState(0, 0x3bd, DAT_00503004);
    return 0;
}

// FUNCTION: 0x426e80
void RunFrontendStateMachine(void)
{
    UpdateSubState();

    switch ((unsigned char)g_game[0x2bbe]) {
    case 0: {
        Obj_00426e80* p = GetDisplay();
        SetCursorOverlayEnabled(0);
        if (p->flag) {
            if (*(int*)(g_game + 0x3923d)) {
                PlayMovie(DAT_0050329c);
                SetState(1, 0x3dc, DAT_00503004);
                *(int*)(g_game + 0x3923d) = 0;
                SaveSettings();
                return;
            }
            if (*(int*)(g_game + 0x39245) == 0) {
                PlayMovie(DAT_0050329c);
                SetState(2, 0x3e6, DAT_00503004);
            } else
                SetState(2, 0x3e9, DAT_00503004);
        } else
            SetState(2, 0x3ed, DAT_00503004);
        break;
    }

    case 2:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (InitLobbiedConnection()) {
                ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
                ((Bits_00426e80*)(g_game + 0x2bee))->b4 = 1;
                SetState(0x10, 0x403, DAT_00503004);
                SetSubState(0x12, 0x404, DAT_00503004);
                SetCursorOverlayEnabled(1);
                return;
            }
            FUN_00434ab0(0);
            ((Bits_00426e80*)(g_game + 0x2bee))->b4 = 0;
            OpenMainMenu();
            if (DAT_00512c80 == 0) {
                SetSubState(1, 0x40d, DAT_00503004);
                SetCursorOverlayEnabled(1);
                return;
            }
            SetSubState(6, 0x40f, DAT_00503004);
            SetCursorOverlayEnabled(1);
            return;
        case 1:
            FUN_00425b60();
            return;
        case 6:
            FUN_00434ab0(3);
            ((Bits_00426e80*)(g_game + 0x2a44))->b3 = 0;
            SetState(0xf, 0x41c, DAT_00503004);
            return;
        case 5:
            ((Bits_00426e80*)(g_game + 0x2a44))->b3 = 1;
            SetState(7, 0x421, DAT_00503004);
            return;
        case 7:
            SetState(0, 0x425, DAT_00503004);
            return;
        case 9:
            SetState(3, 0x429, DAT_00503004);
            return;
        case 8:
            BlankScreen();
            QuitApp(0);
            return;
        }
        break;

    case 1:
        PlayMovie(DAT_00503294);
        SetState(2, 0x437, DAT_00503004);
        break;

    case 3:
        PlayMovie(DAT_0050328c);
        SetState(2, 0x43c, DAT_00503004);
        break;

    case 4:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            SetSubState(1, 0x443, DAT_00503004);
            return;
        case 1:
            PlayMovie(DAT_00503284);
            PlayMovie(DAT_0050328c);
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 0;
            SetState(2, 0x44a, DAT_00503004);
            SetGameMode(2);
            return;
        }
        break;

    case 5:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            SetSubState(1, 0x454, DAT_00503004);
            return;
        case 1:
            PlayMovie(DAT_0050327c);
            PlayMovie(DAT_0050328c);
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 0;
            SetState(2, 0x45b, DAT_00503004);
            SetGameMode(2);
            return;
        }
        break;

    case 7:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            OpenSingleMenu();
            ResetPlayerSlots();
            SetSubState(1, 0x47c, DAT_00503004);
            return;
        case 1:
            FUN_00425b60();
            return;
        case 10:
            OpenNewGameMenu(1);
            SetState(8, 0x485, DAT_00503004);
            SetSubState(1, 0x486, DAT_00503004);
            return;
        case 11:
            LoadSettings();
            FUN_00434ab0(2);
            SetState(9, 0x493, DAT_00503004);
            return;
        case 13:
            SetState(0xa, 0x497, DAT_00503004);
            OpenOptionsPanel();
            SetSubState(1, 0x499, DAT_00503004);
            return;
        case 3:
            SetState(2, 0x49d, DAT_00503004);
            return;
        case 14:
            OpenNewGameMenu(1);
            SetState(8, 0x4a2, DAT_00503004);
            SetSubState(1, 0x4a3, DAT_00503004);
            return;
        }
        break;

    case 10:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            FUN_00425b60();
            return;
        case 3:
            SetState(7, 0x4b1, DAT_00503004);
            return;
        }
        break;

    case 8:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            FUN_00425b60();
            return;
        case 15:
            FUN_00434ab0(1);
            SetState(0xb, 0x4c2, DAT_00503004);
            return;
        case 16:
            FUN_00434ab0(1);
            SetState(0xc, 0x4c7, DAT_00503004);
            return;
        case 3:
            SetState(7, 0x4cb, DAT_00503004);
            return;
        }
        break;

    case 9:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            OpenSkirmishMenu();
            SetSubState(1, 0x4d9, DAT_00503004);
            return;
        case 1:
            FUN_00425b60();
            return;
        case 2:
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
            return;
        case 3:
            SetState(7, 0x4e5, DAT_00503004);
            return;
        }
        break;

    case 11:
    case 12:
    case 13:
    case 14:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            OpenMissionBriefing();
            SetSubState(1, 0x4f5, DAT_00503004);
            return;
        case 1:
            FUN_00425b60();
            return;
        case 2:
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
            return;
        case 3:
            switch ((unsigned char)g_game[0x2bbe]) {
            case 0xb:
                OpenNewGameMenu(0);
                SetState(8, 0x505, DAT_00503004);
                SetSubState(1, 0x506, DAT_00503004);
                return;
            case 0xc:
                OpenNewGameMenu(1);
                SetState(8, 0x50a, DAT_00503004);
                SetSubState(1, 0x50b, DAT_00503004);
                return;
            case 0xd:
                FUN_004c2470();
                ShowEndMissionScreen();
                SetGameMode(7);
                FUN_0041d9f0(7);
                FUN_004c2870();
                return;
            case 0xe:
                SetGameMode(2);
                SetState(7, 0x516, DAT_00503004);
                return;
            }
            break;
        }
        break;

    case 15:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            ((Bits_00426e80*)(g_game + 0x2aaf))->b1 = 0;
            ((Bits_00426e80*)(g_game + 0x2aaf))->b0 = 0;
            if (InitLobbiedConnection()) {
                ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
                SetState(0x10, 0x52d, DAT_00503004);
                SetSubState(0x12, 0x52e, DAT_00503004);
                return;
            }
            FillProviderList();
            if (SelectConnection(-1))
                SetSubState(2, 0x534, DAT_00503004);
            else
                SetSubState(1, 0x536, DAT_00503004);
            return;
        case 13:
            SetState(0xa, 0x53b, DAT_00503004);
            OpenOptionsPanel();
            SetSubState(1, 0x53d, DAT_00503004);
            return;
        // Stays between cases 13 and 2: its position decides case 20 registers.
        case 1:
            FUN_00425b60();
            return;
        case 2:
            if (memcmp(g_game + 0x39201, DAT_004fcdc8, 0x10) == 0) {
                SetState(0x14, 0x547, DAT_00503004);
                SetSubState(1, 0x548, DAT_00503004);
                OpenModemDialog();
                return;
            }
            if (memcmp(g_game + 0x39201, DAT_004fcdb8, 0x10) == 0) {
                SetState(0x14, 0x54e, DAT_00503004);
                SetSubState(1, 0x54f, DAT_00503004);
                OpenSerialDialog();
                return;
            }
            if (memcmp(g_game + 0x39201, DAT_004fcda8, 0x10) == 0) {
                SetState(0x14, 0x555, DAT_00503004);
                SetSubState(1, 0x556, DAT_00503004);
                OpenTcpDialog();
                return;
            }
            if (UseService()) {
                SetState(0x10, 0x55d, DAT_00503004);
                SetSubState(0, 0x55e, DAT_00503004);
            }
            return;
        case 3:
            SetState(2, 0x564, DAT_00503004);
            // break, not return: lets case 7:11 cross-jump into case 16:17.
            break;
        }
        break;

    case 20:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 1:
            if (((Bits_00426e80*)(g_game + 0x2aaf))->b1) {
                if (UseServiceCalls()) {
                    SetStateSubCall(0x10, 0x571, DAT_00503004);
                    SetSubState(0, 0x572, DAT_00503004);
                } else {
                    ((Bits_00426e80*)(g_game + 0x2aaf))->b0 = 0;
                    ((Bits_00426e80*)(g_game + 0x2aaf))->b1 = 0;
                    SetStateSubCall(0xf, 0x577, DAT_00503004);
                    SetSubState(0, 0x578, DAT_00503004);
                }
            }
            FUN_00425b60();
            return;
        }
        break;

    case 16:
        PopKey();
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            HAPINET_guaranteepackets(1);
            SetSubState(1, 0x587, DAT_00503004);
            ResetPlayerSlots();
            if (memcmp(g_game + 0x39201, DAT_004fcdc8, 0x10) == 0 ||
                memcmp(g_game + 0x39201, DAT_004fcdb8, 0x10) == 0) {
                if (g_game[0x2aaf] & 1) {
                    SetSubState(0x11, 0x58f, DAT_00503004);
                    return;
                }
                *(V4i*)(g_game + 0x2ba2) = DAT_004fdaf0;
            }
            OpenSelectGameDialog();
            ShowFrontendErrorText();
            return;
        case 1:
            FUN_00425b60();
            ShowFrontendErrorText();
            return;
        case 17:
            CreateNetGame();
            if (CreateLocalPlayer(g_game[0x2a42], 1))
                AddNetPlayer(*(int*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b67));
            SetState(0x11, 0x5ab, DAT_00503004);
            return;
        case 19:
            ((Pd_00426e80*)(*(int*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b8a)))->b6 = 1;
        case 18:
            if (JoinNetGame(*(V4i*)(g_game + 0x2ba2), (unsigned char)g_game[0x2a42]) == 0) {
                SetSubState(0, 0x5b3, DAT_00503004);
                return;
            }
            if (g_game[0x2bbf] == 0x12) {
                BlankScreen();
                FUN_00491a70();
                if (InitScoreReporting()) {
                    FUN_004ab0a0((int)(g_game + 0x519));
                    SetSubState(0x14, 0x5c1, DAT_00503004);
                } else
                    SetSubState(0x15, 0x5c4, DAT_00503004);
            } else
                SetSubState(0x15, 0x5c7, DAT_00503004);
            return;
        case 20:
            FUN_00425b60();
            return;
        case 21: {
            int unit = *(int*)(g_game + 0x4e5);
            if (unit)
                ((Pd_00426e80*)(*(int*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b8a)))->ready = *(unsigned int*)(unit + 4) >> 1;
            else
                ((Pd_00426e80*)(*(int*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b8a)))->ready = 0;
            if (g_game[0x2bbf] == 0x13)
                ((Pd_00426e80*)(*(int*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b8a)))->b6 = 1;
            unsigned char* q = (unsigned char*)(g_game + 0x14b * (unsigned char)g_game[0x2a42] + 0x1b84);
            *q = (((Bits_00426e80*)(g_game + 0x2b4c))->b4 << 1) | (*q & 0xfd);
            if (((Bits_00426e80*)(g_game + 0x2b4c))->b4) {
                ((Mission*)*(int*)(g_game + 0x391e9))->LoadMissionByName((int)(g_game + 0x2ab1));
                for (int i = 0; i < 10; i++) {
                    if (*(int*)(g_game + i * 0x14b + 0x1b63)) {
                        char t = g_game[i * 0x14b + 0x1bd6];
                        if (t == 1 || t == 2)
                            AddNetPlayer(*(int*)(g_game + i * 0x14b + 0x1b67));
                    }
                }
                ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
                return;
            }
            SetStateLogged(0x11, 0x5ea, DAT_00503004);
            return;
        }
        case 3:
            CloseNetSession();
            SetStateLogged(0xf, 0x5f0, DAT_00503004);
            return;
        }
        break;

    case 17:
        switch ((unsigned char)g_game[0x2bbf]) {
        case 0:
            if (!((Bits_00426e80*)(g_game + 0x2a44))->b2) {
                OpenBattleRoom();
                ReportGameEvent(1);
                ReportGameEvent(2);
                SetSubState(1, 0x5ff, DAT_00503004);
                BroadcastPlayerInfo();
                return;
            }
            FinishUnitSync();
            SetSubState(0x11, 0x605, DAT_00503004);
            BroadcastPlayerInfo();
            return;
        case 1:
            UpdateBattleRoom();
            FUN_00425b60();
            if (((Bits_00426e80*)(g_game + 0x2a44))->b2) {
                FinishUnitSync();
                CloseTopScreen((int)(g_game + 0x519));
                SetSubStateLogged(0x11, 0x613, DAT_00503004);
            }
            return;
        case 17: {
            char* p = g_game + 0x1b63;
            for (int i = 0; i < 10; i++) {
                if (p[0x73] == 4)
                    ((Player*)p)->SetType(0);
                p += 0x14b;
            }
            FinishUnitSync();
            ((Bits_00426e80*)(g_game + 0x2a44))->b2 = 1;
            return;
        }
        case 3:
            ((Bits_00426e80*)(g_game + 0x2a44))->b0 = 1;
            HAPINET_quitgame((int)(g_game + 0x14));
            InitPacketManager(2, 100);
            DeleteUnitSync();
            ReportGameEvent(8);
            ShutdownScoreTables();
            if (((Bits_00426e80*)(g_game + 0x2bee))->b4) {
                LeaveNetGame();
                return;
            }
            switch (GetServiceProviderIndex()) {
            case 0:
            case 3:
                SetStateLogged(0xf, 0x640, DAT_00503004);
                break;
            default:
                SetStateLogged(0x10, 0x643, DAT_00503004);
                break;
            }
            return;
        }
        break;
    }
}
