// Decompiled by GPT-6 Astra. Names are provisional.
// Partial: 36.8%. Tile traversal, stack layout and drawing-call scheduling differ.
// Stopped early to publish within the remaining usage budget.
#include <stdlib.h>
struct Point { int x,y; };
struct Rect { int left,top,right,bottom; };
#pragma pack(push,1)
struct Movement { char pad0[0x10]; int width; char pad14[4]; unsigned int* states; };
struct UnitDef { char pad0[0x1b6]; Movement* movement; };
struct Unit { char pad0[0x92]; UnitDef* def; };
struct Tile { unsigned short unit,feature; unsigned char height; char pad5[2]; unsigned char metal; unsigned short object; char pada[2]; unsigned char flags; };
struct PathCell { unsigned char flags; signed char direction; char pad2[2]; };
struct PathMap { char pad0[0x1c]; PathCell* cells; int width; };
struct Player { char pad0[0x7c]; unsigned char* fog; int fogWidth; char pad84[0x14b-0x84]; };
struct Game {
    char pad0[0xdcb]; unsigned char colors[16]; char padddb[0x1b63-0xddb];
    Player players[10]; char pad2851[0x2a43-(0x1b63+10*0x14b)]; unsigned char playerIndex;
    char pad2a44[0x14207-0x2a44]; PathMap* paths;
    char pad1420b[0x14233-0x1420b]; int width,height,viewWidth;
    char pad1423f[0x1427f-0x1423f]; unsigned char seaLevel,mode;
    char pad14281[6]; Tile* tiles;
    char pad1428b[0x1431f-0x1428b]; int scrollX,scrollY;
    char pad14327[0x37e23-0x14327]; int bottom;
    char pad37e27[0x391fd-0x37e27]; int font;
};
#pragma pack(pop)
extern Game* g_game;
extern int DAT_00511dd0;
extern unsigned char DAT_004fcc68[];
extern signed char DAT_004fd670[],DAT_004fd678[];
Unit* __stdcall FUN_0048c190(int,int);
void __stdcall FUN_004be950(void*,int,int,int,int,int);
void __stdcall FUN_004c1420(int);
int __stdcall FUN_004c13f0();
void __stdcall FUN_004c13a0(int,int);
void __stdcall FUN_004c14f0(void*,const char*,int,int,int);
void __stdcall FUN_004c0310(void*,Point*,int,int);
void __stdcall FUN_004bf6f0(void*,Rect*,int);
void __stdcall FUN_004181d0(void*,Point*,unsigned char*);
static inline Point Screen(int x,int y,unsigned char h)
{
    Point p; p.x=(x+8)*16-g_game->scrollX; p.y=(y+2)*16-(h>>1)-g_game->scrollY; return p;
}
// FUNCTION: 0x418310
void __stdcall FUN_00418310(void* surface)
{
    if (!g_game->mode && !DAT_00511dd0) return;
    Movement* movement=0;
    Player* player=&g_game->players[g_game->playerIndex];
    if (g_game->mode==1) {
        Unit* unit=FUN_0048c190(0,0);
        if (unit) movement=unit->def->movement;
    }
    int firstY=g_game->scrollY/16;
    int firstX=g_game->scrollX/16;
    int lastX=g_game->viewWidth+firstX+1;
    if (lastX>=g_game->width-1) lastX=g_game->width-1;
    int lastY=g_game->height-1;
    unsigned char* colors=g_game->colors;
    unsigned char arrowColor;
    for (int y=firstY;y<lastY;++y) {
        int offscreen=1;
        for (int x=firstX;x<lastX;++x) {
            Point p[4];
            unsigned char heights[4];
            Tile* tile=&g_game->tiles[x+y*g_game->width];
            heights[0]=tile->height;
            p[0]=Screen(x,y,heights[0]);
            ++tile; ++x;
            heights[1]=tile->height;
            p[1]=Screen(x,y,heights[1]);
            tile+=g_game->width; ++y;
            heights[2]=tile->height;
            p[2]=Screen(x,y,heights[2]);
            --tile; --x;
            heights[3]=tile->height;
            p[3]=Screen(x,y,heights[3]);
            tile-=g_game->width; --y;
            if (p[0].y<g_game->bottom) offscreen=0;
            if (g_game->mode==1) {
                if (movement) {
                    unsigned int state=(movement->states[x+(y>>4)*movement->width]>>((y&15)*2))&3;
                    if (state<3) {
                        unsigned char color=colors[DAT_004fcc68[state]];
                        FUN_004be950(surface,p[0].x,p[0].y,p[2].x,p[2].y,color);
                        FUN_004be950(surface,p[1].x,p[1].y,p[3].x,p[3].y,color);
                    }
                }
                PathCell* cell=&g_game->paths->cells[x+y*g_game->paths->width];
                if (cell->flags&4) {
                    FUN_004c1420(g_game->font);
                    FUN_004c13a0(rand()&255,FUN_004c13f0());
                    FUN_004c14f0(surface,"G",p[0].x,p[0].y,-1);
                }
                if ((cell->flags&~4)!=0 && (cell->flags&~4)!=3) {
                    int cx=p[0].x+8,cy=p[0].y+8;
                    // Flags 5 and 6 pass this test without initializing the original color.
                    switch(cell->flags) {
                    case 1: arrowColor=colors[15]; break;
                    case 2: arrowColor=colors[4]; break;
                    }
                    FUN_004be950(surface,cx-DAT_004fd670[cell->direction]*14,cy-DAT_004fd678[cell->direction]*14,cx,cy,arrowColor);
                    int direction=(cell->direction+1)&7;
                    FUN_004be950(surface,cx-DAT_004fd670[direction]*4,cy-DAT_004fd678[direction]*4,cx,cy,arrowColor);
                    direction=(cell->direction-1)&7;
                    FUN_004be950(surface,cx-DAT_004fd670[direction]*4,cy-DAT_004fd678[direction]*4,cx,cy,arrowColor);
                }
            } else if (g_game->mode==2) {
                if (tile->height>g_game->seaLevel) {
                    FUN_004be950(surface,p[0].x,p[0].y,p[1].x,p[1].y,colors[15]);
                    FUN_004be950(surface,p[0].x,p[0].y,p[3].x,p[3].y,colors[15]);
                } else {
                    FUN_004be950(surface,p[0].x,p[0].y,p[1].x,p[1].y,colors[13]);
                    FUN_004be950(surface,p[0].x,p[0].y,p[3].x,p[3].y,colors[13]);
                }
                if (tile->unit) FUN_004c0310(surface,p,4,(unsigned char)tile->unit);
                else if (tile->object!=0xffff) FUN_004c0310(surface,p,4,(unsigned char)(tile->object-56));
                if (tile->feature) {
                    FUN_004be950(surface,p[0].x,p[0].y,p[2].x,p[2].y,(unsigned char)tile->feature);
                    FUN_004be950(surface,p[1].x,p[1].y,p[3].x,p[3].y,(unsigned char)tile->feature);
                }
                if (tile->flags&2) {
                    FUN_004be950(surface,(p[0].x+p[1].x)/2,(p[0].y+p[1].y)/2+2,(p[1].x+p[2].x)/2-2,(p[1].y+p[2].y)/2,colors[15]);
                    FUN_004be950(surface,(p[1].x+p[2].x)/2-2,(p[1].y+p[2].y)/2,(p[2].x+p[3].x)/2,(p[2].y+p[3].y)/2-2,colors[15]);
                    FUN_004be950(surface,(p[2].x+p[3].x)/2,(p[2].y+p[3].y)/2-2,(p[3].x+p[0].x)/2+2,(p[3].y+p[0].y)/2,colors[15]);
                    FUN_004be950(surface,(p[3].x+p[0].x)/2+2,(p[3].y+p[0].y)/2,(p[0].x+p[1].x)/2,(p[0].y+p[1].y)/2+2,colors[15]);
                }
            } else if (g_game->mode==3) {
                if (tile->height>g_game->seaLevel) {
                    FUN_004be950(surface,p[0].x,p[0].y,p[1].x,p[1].y,colors[15]);
                    FUN_004be950(surface,p[0].x,p[0].y,p[3].x,p[3].y,colors[15]);
                } else {
                    FUN_004be950(surface,p[0].x,p[0].y,p[1].x,p[1].y,colors[13]);
                    FUN_004be950(surface,p[0].x,p[0].y,p[3].x,p[3].y,colors[13]);
                }
                FUN_004c1420(g_game->font);
                FUN_004c13a0(colors[15],FUN_004c13f0());
                char buffer[20];
                FUN_004c14f0(surface,_itoa(tile->metal,buffer,10),p[0].x+2,p[0].y+2,-1);
            } else if (g_game->mode==4) {
                FUN_004be950(surface,p[0].x,p[0].y,p[1].x,p[1].y,colors[0]);
                FUN_004be950(surface,p[0].x,p[0].y,p[3].x,p[3].y,colors[0]);
                if (player->fog[(y/2)*player->fogWidth+x/2]) {
                    Rect r;
                    r.left=p[0].x-5; r.right=p[0].x+5; r.top=p[0].y-5; r.bottom=p[0].y+5;
                    FUN_004bf6f0(surface,&r,colors[15]);
                }
            }
            if (DAT_00511dd0) FUN_004181d0(surface,p,heights);
        }
        if (offscreen) break;
    }
}
