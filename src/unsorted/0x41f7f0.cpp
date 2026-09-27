// Decompiled by GPT-6. Names are provisional.
#include <windows.h>
#include <string.h>
class Class_00435100 { public: int FUN_00435100(); };
class Class_00435980 { public: int FUN_00435980(int); };
class Class_004cfb40 { public: void FUN_004cfb40(); };
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
void __stdcall FUN_004c69a0(void*);
void FUN_00453d40();
Engine* FUN_004b6220();
void* __stdcall FUN_004c69f0(const char*,int,int);
void __stdcall FUN_004c6b70(void*,void*,int,int);
void __stdcall FUN_0046c620(int);
const char* __stdcall FUN_00452c40(unsigned);
const char* __stdcall FUN_004c5740(const char*);
void __stdcall FUN_004abd90(Menu*,const char*,int,int,int);
void __stdcall FUN_0049fa90(Menu*);
void __stdcall FUN_0049fad0(Menu*);
void FUN_004c63a0();
int __stdcall FUN_004ab060(Menu*,const char*);
void __stdcall FUN_00464060(void*);
void __stdcall FUN_004a9fd0(Menu*);
void __stdcall FUN_004ab170(Menu*,void*,void*);
void FUN_004c2870();
unsigned FUN_004b6340();
int FUN_004b6330();
void __stdcall FUN_004c22d0(int);
void FUN_00491a70();
void __stdcall FUN_004bf4d0(void*,int*,int);
char __stdcall FUN_0041d6a0(int);
Layer* __stdcall FUN_004aa8f0(Menu*,const char*,int);
void __stdcall FUN_0041f680(void*);
void __stdcall FUN_0049fb10(Menu*,int);
void __stdcall FUN_004a81e0(Menu*,int);
void FUN_0041da60();
void __stdcall FUN_00425860(int,int,const char*);
void __stdcall FUN_00490b30(int);
void __stdcall FUN_0041dfc0(void*,void*,int);
void FUN_0041f0a0();
void FUN_0041e420();
void FUN_0041f400();
void FUN_0041e270();
void FUN_00476ca0();
void __stdcall FUN_004c2340(int*);
int FUN_004c1ab0();
void __stdcall FUN_004c1830(void*,const char*,int,int,int);
void __stdcall FUN_0049fa50(Menu*);
void __stdcall FUN_00491c80(int);
void __stdcall FUN_004a0570(Menu*,const char*,int);
void __stdcall FUN_0047f1a0(const char*,int);
void __stdcall FUN_004573d0(Player*,int,int);
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
void __stdcall FUN_0041f7f0()
{
    char text[64];
    int event[6];
    unsigned palette[256];
    FUN_004c69a0(g_game->surface);
    FUN_00453d40();
    switch(g_game->state) {
    case 0:
        if(g_game->campaign->FUN_00435100()==3) {
            Engine* e=FUN_004b6220();
            g_game->lastFrame=FUN_004c69f0("Copy of last game frame",e->width,e->height);
            FUN_004c6b70(g_game->lastFrame,g_game->surface,e->width,e->height);
            FUN_0046c620(7);
            g_game->state=1;
            Player* player=&g_game->players[g_game->localPlayer];
            if(player->message && player->message!=2) {
                const char* name=FUN_00452c40(player->message);
                FUN_004abd90(&g_game->menu,FUN_004c5740(name),320,1,1);
                FUN_0049fa90(&g_game->menu);
                FUN_0049fad0(&g_game->menu);
                player->message=0;
            }
        } else { g_game->lastFrame=0; g_game->state=2; }
        break;
    case 1:
        if(FUN_004ab060(&g_game->menu,"MSGBOX.GUI")) {
            if(g_game->state==1) {
                Engine* e=FUN_004b6220();
                FUN_004c6b70(g_game->surface,g_game->lastFrame,e->width,e->height);
                FUN_00464060(g_game->surface);
                FUN_004a9fd0(&g_game->menu);
                FUN_004ab170(&g_game->menu,0,0);
                FUN_004c2870();
                FUN_004c63a0();
            }
        } else g_game->state=2;
        break;
    case 2:
        g_game->fade=10;
        g_game->tick=FUN_004b6340()+1;
        g_game->complete=0;
        FUN_004c22d0(0);
        g_game->state=3;
        break;
    case 3:
        if(!g_game->complete) {
            event[1]=0; event[0]=0;
            event[2]=g_game->width; event[3]=g_game->height;
            if(g_game->tick<FUN_004b6340()) {
                FUN_004bf4d0(0,event,g_game->fade-29);
                g_game->tick=FUN_004b6340()+1;
                --g_game->fade;
                if(!g_game->fade) g_game->complete=1;
            }
        } else {
            FUN_00491a70(); g_game->state=4;
            FUN_004c69a0(g_game->surface);
        }
        break;
    case 4:
        if(g_game->campaign->FUN_00435100()==1 && !FUN_0041d6a0(0)) {
            Layer* l=FUN_004aa8f0(&g_game->menu,"CDCHECK.GUI",0x101);
            l->handler=FUN_0041f680;
            FUN_004c22d0(1);
            FUN_0049fb10(&g_game->menu,1);
            FUN_004a81e0(&g_game->menu,0x40);
            g_game->state=8;
        } else g_game->state=5;
        break;
    case 5: {
        FUN_0041da60();
        int next=((Class_00435980*)g_game->campaign)->FUN_00435980(g_game->mission+1);
        if(g_game->campaign->FUN_00435100()==1 && (g_game->flags&0x10) && !next && !g_game->skip) {
            if((unsigned char)FUN_004b6220()->network) {
                if(!g_game->players[0].owner->flag) FUN_00425860(4,0x4ce,"c:\\cavedog\\wargame\\endgame.cpp");
                else FUN_00425860(5,0x4d3,"c:\\cavedog\\wargame\\endgame.cpp");
            } else FUN_00425860(2,0x4d9,"c:\\cavedog\\wargame\\endgame.cpp");
            FUN_00490b30(2);
        } else if(g_game->campaign->FUN_00435100()==1 && (g_game->flags&0x10) && g_game->image) {
            memset(palette,0,sizeof(palette));
            FUN_0041dfc0(g_game->palette,palette,5);
            g_game->tick=FUN_004b6340()+1;
            g_game->state=6;
            FUN_004c6b70(g_game->surface,g_game->image,0,0);
        } else {
            FUN_0041f0a0(); FUN_0041e420(); FUN_0041f400();
            FUN_0049fad0(&g_game->menu); FUN_0049fa90(&g_game->menu);
            g_game->state=7;
        }
        break;
    }
    case 6:
        if(!g_game->complete) {
            FUN_0041e270();
            unsigned now=FUN_004b6340();
            now+=FUN_004b6330();
            g_game->deadline=now;
            DAT_00511dec=0;
        } else {
            if(!DAT_00511dec) { FUN_00476ca0(); DAT_00511dec=1; }
            if(g_game->deadline<FUN_004b6340()) {
                FUN_004c2340(event);
                if(FUN_004c1ab0() || g_game->advance) {
                    g_game->input->FUN_004cfb40();
                    FUN_0041f0a0(); FUN_0041f400(); FUN_0041e420();
                    FUN_0049fad0(&g_game->menu); FUN_0049fa90(&g_game->menu);
                    g_game->state=7;
                }
                unsigned deadline=FUN_004b6330()*5+g_game->deadline;
                if(deadline<FUN_004b6340())
                    FUN_004c1830(g_game->surface,FUN_004c5740("Click to continue."),g_game->textColor,g_game->shadowColor,g_game->height-20);
            }
        }
        break;
    case 7: {
        if(StatsComplete()) {
            FUN_0049fa50(&g_game->menu);
            g_game->state=8; FUN_00491c80(19); FUN_004c22d0(1);
            break;
        }
        FUN_004a9fd0(&g_game->menu); FUN_004ab170(&g_game->menu,0,0);
        int skip=0;
        int clicked=FUN_004c1ab0();
        if(clicked && g_game->campaign->FUN_00435100()!=3) skip=1;
        if(g_game->deadline<FUN_004b6340() || skip) {
            if(clicked) {
                { ENABLE_BARS("Kills") }
                { ENABLE_BARS("Losses") }
                { ENABLE_BARS("EProduced") }
                { ENABLE_BARS("MProduced") }
                { ENABLE_BARS("EWasted") }
                { ENABLE_BARS("MWasted") }
                { ENABLE_BARS("Score") }
                FUN_0047f1a0("ActivateAllStatBars",0);
            }
            switch(g_game->bar) {
            case 0: { ENABLE_BARS("Kills") } FUN_0047f1a0("EndGameStatBar",0); break;
            case 1: { ENABLE_BARS("Losses") } FUN_0047f1a0("EndGameStatBar",0); break;
            case 2: { ENABLE_BARS("EProduced") } FUN_0047f1a0("EndGameStatBar",0); break;
            case 3: { ENABLE_BARS("MProduced") } FUN_0047f1a0("EndGameStatBar",0); break;
            case 4: { ENABLE_BARS("EWasted") } FUN_0047f1a0("EndGameStatBar",0); break;
            case 5: { ENABLE_BARS("MWasted") } FUN_0047f1a0("EndGameStatBar",0); break;
            case 6: { ENABLE_BARS("Score") } FUN_0047f1a0("EndGameScore",0); break;
            }
            if(g_game->campaign->FUN_00435100()==3) {
                Player* player=&g_game->players[g_game->localPlayer];
                for(int j=0;j<2;++j) FUN_004573d0(player,0,0);
            }
            g_game->deadline=FUN_004b6340()+10;
            ++g_game->bar;
        }
        break;
    }
    case 8:
        FUN_004c2470(); FUN_004a9fd0(&g_game->menu); FUN_004c2870(); FUN_004c63a0();
        FUN_004c2470(); FUN_004ab170(&g_game->menu,0,0); FUN_004c2870();
        break;
    }
    FUN_004c63a0();
}
