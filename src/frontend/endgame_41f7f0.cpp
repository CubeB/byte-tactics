// Decompiled by GPT-6. Names are provisional.
#include <windows.h>
#include <string.h>
class Class_00435100 { public: int FUN_00435100(); };
class Class_00435980 { public: int MissionExists(int); };
class Class_004cfb40 { public: void StopStream(); };
#pragma pack(push,1)
struct Amount { int current,required; };
struct Entry {
    unsigned char kind; char pad1[0x29-1]; unsigned char enabled;
    char pad2a[0xb6-0x2a]; short count; char padb8[2]; Amount amount;
    char padc2[0x15b-0xc2];
};
struct Layer { int unknown; Entry* entries; void (__stdcall* handler)(void*); };
struct Menu { char pad[0x18]; Layer* layer; };
struct Owner { char pad[0x95]; unsigned char flag; };
struct Player { char pad[0x22]; unsigned char message; char pad23[4]; Owner* owner; char pad2b[0x14b-0x2b]; };
struct Slot { unsigned char active; char pad[0x39]; };
struct Engine { char pad[0xd4]; int width,height; char paddc[0xf0-0xdc]; unsigned short low:1; unsigned short network:1; unsigned short high:14; };
struct Game {
    char pad[0x10]; Class_004cfb40* input;
    char pad14[0x519-0x14]; Menu menu;
    char pad535[0xdcb-0x535]; unsigned char textColor;
    char paddcc[0xdda-0xdcc]; unsigned char shadowColor;
    char padddb[0x1b63-0xddb]; Player players[10];
    char pad2851[0x2a42-0x2851]; unsigned char localPlayer;
    char pad2a43[0x2c7e-0x2a43]; int advance;
    char pad2c82[0x37e1b-0x2c82]; void* surface; int width,height;
    char pad37e27[0x38dd9-0x37e27]; Slot slots[10];
    char pad3901d[0x39057-0x3901d]; int state; unsigned deadline,tick; int complete,fade,bar;
    int unknown3906f,skip; void* lastFrame; void* image; void* palette;
    char pad39083[0x391ab-0x39083]; int mission;
    char pad391af[0x391e9-0x391af]; Class_00435100* campaign;
    char pad391ed[0x3923b-0x391ed]; unsigned char flags;
};
#pragma pack(pop)
extern Game* g_game;
extern int DAT_00511dec;
void __stdcall SetOffscreenSurface(void*);
void HandleNetPackets();
Engine* GetDisplay();
void* __stdcall AllocSurface(const char*,int,int);
void __stdcall DrawSurface(void*,void*,int,int);
void __stdcall ReportGameEvent(int);
const char* __stdcall GetRejectReasonText(unsigned);
const char* __stdcall Translate(const char*);
void __stdcall OpenMessageBox(Menu*,const char*,int,int,int);
void __stdcall FUN_0049fa90(Menu*);
void __stdcall FUN_0049fad0(Menu*);
void FlipScreen();
int __stdcall IsScreenNamed(Menu*,const char*);
void __stdcall DrawMessages(void*);
void __stdcall UpdateMenu(Menu*);
void __stdcall FUN_004ab170(Menu*,void*,void*);
void FUN_004c2870();
unsigned GetTicks();
int GetTickRate();
void __stdcall FUN_004c22d0(int);
void FUN_00491a70();
void __stdcall FadeRectangle(void*,int*,int);
char __stdcall FindGameCdDrive(int);
Layer* __stdcall LoadGuiLayer(Menu*,const char*,int);
void __stdcall HandleCdCheckClick(void*);
void __stdcall FUN_0049fb10(Menu*,int);
void __stdcall RenderLayer(Menu*,int);
void SetUpEndMissionScreen();
void __stdcall SetFrontendState(int,int,const char*);
void __stdcall SetGameMode(int);
void __stdcall StartPaletteFade(void*,void*,int);
void OpenEndMissionScreen();
void FillEndGameStatistics();
void EnableEndMissionButtons();
void StepPaletteFade();
void FUN_00476ca0();
void __stdcall FUN_004c2340(int*);
int PopKey();
void __stdcall DrawOutlinedString(void*,const char*,int,int,int);
void __stdcall FUN_0049fa50(Menu*);
void __stdcall FUN_00491c80(int);
void __stdcall FUN_004a0570(Menu*,const char*,int);
void __stdcall PlaySoundByName(const char*,int);
void __stdcall SendPlayerEconomy(Player*,int,int);
void FUN_004c2470();
#define ENABLE_BARS(name) \
    for(int i=0;i<10;++i) { \
        if(g_game->slots[i].active) { \
            wsprintfA(text,"%s%d",name,i); \
            FUN_004a0570(&g_game->menu,text,1); \
        } \
    }
static inline int StatsComplete()
{
    Entry* entries=g_game->menu.layer->entries;
    int count=entries->count;
    for(int i=0;i<count;++i) {
        if(entries[i].kind==13) {
            if(!entries[i].enabled || entries[i].amount.current<entries[i].amount.required) return 0;
        }
    }
    return 1;
}
// FUNCTION: 0x41f7f0
void __stdcall RunEndGameState()
{
    char text[64];
    int event[6];
    unsigned palette[256];
    SetOffscreenSurface(g_game->surface);
    HandleNetPackets();
    switch(g_game->state) {
    case 0:
        if(g_game->campaign->FUN_00435100()==3) {
            Engine* e=GetDisplay();
            g_game->lastFrame=AllocSurface("Copy of last game frame",e->width,e->height);
            DrawSurface(g_game->lastFrame,g_game->surface,e->width,e->height);
            ReportGameEvent(7);
            g_game->state=1;
            Player* player=&g_game->players[g_game->localPlayer];
            if(player->message && player->message!=2) {
                const char* name=GetRejectReasonText(player->message);
                OpenMessageBox(&g_game->menu,Translate(name),320,1,1);
                FUN_0049fa90(&g_game->menu);
                FUN_0049fad0(&g_game->menu);
                player->message=0;
            }
        } else { g_game->lastFrame=0; g_game->state=2; }
        break;
    case 1:
        if(IsScreenNamed(&g_game->menu,"MSGBOX.GUI")) {
            if(g_game->state==1) {
                Engine* e=GetDisplay();
                DrawSurface(g_game->surface,g_game->lastFrame,e->width,e->height);
                DrawMessages(g_game->surface);
                UpdateMenu(&g_game->menu);
                FUN_004ab170(&g_game->menu,0,0);
                FUN_004c2870();
                FlipScreen();
            }
        } else g_game->state=2;
        break;
    case 2:
        g_game->fade=10;
        g_game->tick=GetTicks()+1;
        g_game->complete=0;
        FUN_004c22d0(0);
        g_game->state=3;
        break;
    case 3:
        if(!g_game->complete) {
            event[1]=0; event[0]=0;
            event[2]=g_game->width; event[3]=g_game->height;
            if(g_game->tick<GetTicks()) {
                FadeRectangle(0,event,g_game->fade-29);
                g_game->tick=GetTicks()+1;
                --g_game->fade;
                if(!g_game->fade) g_game->complete=1;
            }
        } else {
            FUN_00491a70(); g_game->state=4;
            SetOffscreenSurface(g_game->surface);
        }
        break;
    case 4:
        if(g_game->campaign->FUN_00435100()==1 && !FindGameCdDrive(0)) {
            Layer* l=LoadGuiLayer(&g_game->menu,"CDCHECK.GUI",0x101);
            l->handler=HandleCdCheckClick;
            FUN_004c22d0(1);
            FUN_0049fb10(&g_game->menu,1);
            RenderLayer(&g_game->menu,0x40);
            g_game->state=8;
        } else g_game->state=5;
        break;
    case 5: {
        SetUpEndMissionScreen();
        int next=((Class_00435980*)g_game->campaign)->MissionExists(g_game->mission+1);
        if(g_game->campaign->FUN_00435100()==1 && (g_game->flags&0x10) && !next && !g_game->skip) {
            if((unsigned char)GetDisplay()->network) {
                if(!g_game->players[0].owner->flag) SetFrontendState(4,0x4ce,"c:\\cavedog\\wargame\\endgame.cpp");
                else SetFrontendState(5,0x4d3,"c:\\cavedog\\wargame\\endgame.cpp");
            } else SetFrontendState(2,0x4d9,"c:\\cavedog\\wargame\\endgame.cpp");
            SetGameMode(2);
        } else if(g_game->campaign->FUN_00435100()==1 && (g_game->flags&0x10) && g_game->image) {
            memset(palette,0,sizeof(palette));
            StartPaletteFade(g_game->palette,palette,5);
            g_game->tick=GetTicks()+1;
            g_game->state=6;
            DrawSurface(g_game->surface,g_game->image,0,0);
        } else {
            OpenEndMissionScreen(); FillEndGameStatistics(); EnableEndMissionButtons();
            FUN_0049fad0(&g_game->menu); FUN_0049fa90(&g_game->menu);
            g_game->state=7;
        }
        break;
    }
    case 6:
        if(!g_game->complete) {
            StepPaletteFade();
            unsigned now=GetTicks();
            now+=GetTickRate();
            g_game->deadline=now;
            DAT_00511dec=0;
        } else {
            if(!DAT_00511dec) { FUN_00476ca0(); DAT_00511dec=1; }
            if(g_game->deadline<GetTicks()) {
                FUN_004c2340(event);
                if(PopKey() || g_game->advance) {
                    g_game->input->StopStream();
                    OpenEndMissionScreen(); EnableEndMissionButtons(); FillEndGameStatistics();
                    FUN_0049fad0(&g_game->menu); FUN_0049fa90(&g_game->menu);
                    g_game->state=7;
                }
                unsigned deadline=GetTickRate()*5+g_game->deadline;
                if(deadline<GetTicks())
                    DrawOutlinedString(g_game->surface,Translate("Click to continue."),g_game->textColor,g_game->shadowColor,g_game->height-20);
            }
        }
        break;
    case 7: {
        if(StatsComplete()) {
            FUN_0049fa50(&g_game->menu);
            g_game->state=8; FUN_00491c80(19); FUN_004c22d0(1);
            break;
        }
        UpdateMenu(&g_game->menu); FUN_004ab170(&g_game->menu,0,0);
        int skip=0;
        int clicked=PopKey();
        if(clicked && g_game->campaign->FUN_00435100()!=3) skip=1;
        if(g_game->deadline<GetTicks() || skip) {
            if(clicked) {
                { ENABLE_BARS("Kills") }
                { ENABLE_BARS("Losses") }
                { ENABLE_BARS("EProduced") }
                { ENABLE_BARS("MProduced") }
                { ENABLE_BARS("EWasted") }
                { ENABLE_BARS("MWasted") }
                { ENABLE_BARS("Score") }
                PlaySoundByName("ActivateAllStatBars",0);
            }
            switch(g_game->bar) {
            case 0: { ENABLE_BARS("Kills") } PlaySoundByName("EndGameStatBar",0); break;
            case 1: { ENABLE_BARS("Losses") } PlaySoundByName("EndGameStatBar",0); break;
            case 2: { ENABLE_BARS("EProduced") } PlaySoundByName("EndGameStatBar",0); break;
            case 3: { ENABLE_BARS("MProduced") } PlaySoundByName("EndGameStatBar",0); break;
            case 4: { ENABLE_BARS("EWasted") } PlaySoundByName("EndGameStatBar",0); break;
            case 5: { ENABLE_BARS("MWasted") } PlaySoundByName("EndGameStatBar",0); break;
            case 6: { ENABLE_BARS("Score") } PlaySoundByName("EndGameScore",0); break;
            }
            if(g_game->campaign->FUN_00435100()==3) {
                Player* player=&g_game->players[g_game->localPlayer];
                for(int j=0;j<2;++j) SendPlayerEconomy(player,0,0);
            }
            g_game->deadline=GetTicks()+10;
            ++g_game->bar;
        }
        break;
    }
    case 8:
        FUN_004c2470(); UpdateMenu(&g_game->menu); FUN_004c2870(); FlipScreen();
        FUN_004c2470(); FUN_004ab170(&g_game->menu,0,0); FUN_004c2870();
        break;
    }
    FlipScreen();
}
