// Decompiled by GPT-5.6 Astra, finished by deepseek-v4.1-flash; verified by GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// GPT-6 retry: scalar/point screen counters, projection and step helpers,
// function-scope quad homes and guarded do/while loops did not improve 55.5%.
// Most screen-counter variants add a hoisted row invariant and a four-byte frame
// increase; keep this best version rather than replacing it with those variants.
// Region-split skeleton, per docs/splitting-huge-functions.md (PR #1287).
// Checked by space-bunny-free; verified by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry: the saved 55.5% source (2203 bytes) remains best after
// three variants. Explicit screen-base induction variables scored 51.5% at
// 2187 bytes; moving the lastX clamp ahead of firstX/lastY scored 52.7% at
// 2207 bytes; spelling the width clamp as a ternary stayed at 55.5%. Restored
// the original source shape because equal score did not justify replacing it.
//
// RETRY NOTE (deepseek-v4.1-flash, 55.5%, no variant beat this file). The frame
// size and byte count are right; the score is stuck because the ORIGINAL walks
// the quad with two screen-base INDUCTION variables, esp+0x38 and esp+0x3c,
// bumped by 0x10 at each corner, and derives each vertex as
//     p[k].x = baseX - scrollX;
//     p[k].y = baseY - (heights[k]>>1) - scrollY;
// with x++/y++/x--/y-- and tile += 13, += 13*width, -= 13, -= 13*width. The
// skeleton recomputes (x+8)*16 and (y+2)*16 per vertex, which is the first
// divergence (see 0x418401-0x41858e) and cascades into the whole register
// allocation. The preheader also diverges before that: the original computes
// lastX = viewWidth + firstX + 1 before lastY = height-1 and the two clamps,
// while this file loads width first at 0x41838e. Both are statement-shape /
// declaration-order levers, not register-allocation noise: the original has
// the loop cursors at esp+0x10 (y) and esp+0x34 (x) with the quad at
// esp+0x14..0x33, so its declaration order is y, p[4], x, baseX, baseY, tile,
// heights[4], lastX, offscreen, movement, lastY, player, firstX.
//
// STATUS: the gate NOW PASSES, and it was the whole ball game. check.py reads
// 55.5% at 2203 of 2203 bytes, the original's exact size, from 36.8% at 2251
// bytes when the skeleton was written.
//
// WHAT THE GATE WAS. The skeleton's frame was `sub esp,0xa4` against the
// original's `sub esp,0x84`: eight dwords too many, while the declared locals
// also totalled 33 dwords. That said the extra eight were not extra
// declarations, and two earlier attempts to remove them by hand both failed:
// moving `offscreen` to function scope (build/scratch/0x418310/f1.cpp) got the
// frame to 0x9c and the score to 20.6%.
//
// THE ACTUAL CAUSE, FOUND BY THE r2 REGION AGENT: the `Screen()` helper.
// `static inline Point Screen(int,int,unsigned char)` is STRUCT-RETURNING, and
// called four times in the inner loop body, and that costs the eight dwords:
// MSVC materialises the returned `Point` and cannot enregister it. Writing the
// four corners out by hand
//     p[0].x=(x+8)*16-g_game->scrollX;
//     p[0].y=(y+2)*16-(heights[0]>>1)-g_game->scrollY;
// takes the frame to `sub esp,0x84`, the original's, and the length to 2203
// bytes, the original's exactly. `Screen()` is still in the SHARED block,
// unused. **A struct-returning helper used inside a loop is a frame cost, not
// a convenience.**
//
// The original's frame, for the record, from the slot map
// (build/scratch/ctx-418310.txt):
//   esp+0x10        the outer loop's y cursor
//   esp+0x14..0x33  the p[0..3] quad (lea eax,[esp+0x14] at 0x418b4b)
//   esp+0x34        the inner loop's x cursor
//   esp+0x38/0x3c   the two screen bases, induction variables, bumped by 0x10
//   esp+0x40/0x44   the tile pointer and a scratch
//   esp+0x48..0x4b  heights[4]
//   esp+0x50..0x6c  EIGHT dwords, each written once in the preheader
//                   (0x4183cc to 0x4183d2) and read once in the epilogue: the
//                   hoisted loop invariants, including `offscreen` (esp+0x54,
//                   set to 1 at 0x4183ef, cleared at 0x4185ae)
//   esp+0x70        the fog Rect of the mode 4 branch (lea ecx,[esp+0x70])
//   ebp is NOT a frame pointer: `lea ebp,[esi+0xdcb]` makes it
//   &g_game->colors, so [ebp+0xd] and [ebp+0xf] are colors[13] and colors[15]
//   and the sea-level test reads them through ebp.
//
// The frame-count arithmetic that pointed the way: the frame is 33 dwords and
// the eight extra dwords are not extra declarations: MSVC is failing to
// enregister eight dwords that the original keeps in registers.
//
// A region agent's slot read sharpens the layout. `offscreen` is one of the
// eight hoisted invariants (its home is read at 0x418b85 from esp+0x54, inside
// the preheader range), so in the original it is a function-scope home and not
// a per-iteration variable. Moving it there by hand failed (see above): it was
// the right diagnosis and the wrong lever, because the eight dwords were the
// helper's, not `offscreen`'s.
//
// FIRST ATTEMPT AT THE FRAME, AND IT FAILED IN AN INFORMATIVE WAY. Moving the
// `offscreen` declaration out of the outer loop and assigning `offscreen=1` at
// the top of it (build/scratch/0x418310/f1.cpp) DOES move the frame the right
// way, to `sub esp,0x9c`, two dwords smaller. And the score collapses: 20.6%
// at 2043 bytes, from 36.8% at 2251. So 208 bytes of code were deleted while
// the frame improved, and here the two are in tension rather than aligned.
// Neither number decides it alone; score them together, which is the doc's
// "score the merge, not just the branch" applied to the skeleton itself.
//
// deepseek-v4.1 session (still 55.5%, 2203 bytes, three probes, none moved a
// byte). The slot map is the whole remaining difference and it is one slot
// wide: ours has p[4] at esp+0x10, x at esp+0x30, y at esp+0x34, baseY at
// esp+0x38, baseX at esp+0x3c, heights at esp+0x40; the original has y at
// esp+0x10, p[4] at esp+0x14, x at esp+0x34, baseX at esp+0x38, baseY at
// esp+0x3c, tile at esp+0x40 and heights at esp+0x48. Everything downstream of
// y follows from that one slot: give y the low slot and p/x/bases/heights land
// where the original has them. What does NOT move y: declaring `int y;` at
// function scope and assigning it in the for-init, and defining `int y=firstY;`
// above the loop with an empty for-init (`for(;y<lastY;++y)`) both recompile to
// the identical 2203 bytes with y still at esp+0x34. Swapping the two
// inner-block declarations (heights[4] before Point p[4]) likewise changes
// nothing at all. So MSVC5's nested-block slot walk, not declaration order,
// decides these homes, and the original's y-below-p layout needs a source shape
// that changes that walk (the array being pinned at the frame bottom while a
// scalar sits below it is the shape to hunt for). The preheader also stores
// lastX twice (before and after the width clamp) where the original stores it
// once at 0x4183cc; the ternary spelling of the clamp was already tried.
// deepseek-v4.1 session 2 (55.5%, 2203 bytes, 18 check.py runs, no variant beat
// this file). Every declaration-order probe recompiled BYTE-IDENTICALLY, so the
// frame here is not declaration-driven: y, x, p[4] and heights[4] moved between
// function scope, loop-body scope and block scope; for-init declarations vs
// pre-declared plus an empty for-init; `Point p[4]` before and after both
// for-inits; a `Tile* tile` local added at function scope and at loop scope.
// Code-shape probes that DID change output and scored worse: `unsigned char
// m=cell->flags&0xfb; if (m!=0 && m!=3)` (54.3%, 2206 bytes) and the explicit
// baseX/baseY induction form with `baseX+=16` per corner (46.5%, 2239 bytes).
// The gap stays the allocator's slot walk: the original keeps y at esp+0x10,
// p[4] at esp+0x14, x at esp+0x34, baseX at esp+0x38, baseY at esp+0x3c, tile at
// esp+0x40 and heights at esp+0x48, ours keeps p[4] at esp+0x10, x at esp+0x30,
// y at esp+0x34, baseY at esp+0x38, baseX at esp+0x3c and heights at esp+0x40,
// with everything from esp+0x50 up identical. The first instruction divergence
// is the preheader (ours hoists the width load into edi because firstY was
// coloured ecx instead of the original's edi), i.e. it is downstream of the same
// whole-function allocation problem, not a statement-order fix.
//
// deepseek-v4.1-flash retry (3 check runs, 55.5%, 2203 bytes, no improvement).
// Two new shape probes, both worse, both recorded so they are not repeated:
//   - Four separate `Point p0..p3` (contiguous, passed as &p0) instead of
//     `Point p[4]`: 30.3%, 2233 bytes. The array form is correct.
//   - Faithful baseX/baseY/tile induction exactly as the disassembly spells it
//     (baseX/baseY initialised from firstX/firstY before the inner loop, the
//     body doing ++x/++tile/baseX+=16 then tile+=width/++y/baseY+=16 then the
//     mirrored decrements, and x++/baseX+=16 in the for-increment): 37.7%,
//     2264 bytes. The induction spelling is NOT what the source used, or its
//     declaration scope is wrong; the recompute form with compiler-generated
//     induction temps is closer.
// Still the same one-slot divergence: y must sit at esp+0x10 (below p[4] at
// esp+0x14) and baseX must precede baseY at esp+0x38/0x3c; ours keeps p at
// esp+0x10 and y at esp+0x34. Everything from esp+0x50 up already matches.

#include <stdlib.h>
// SHARED begin
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
// SHARED end

// FUNCTION: 0x418310
void __stdcall FUN_00418310(void* surface)
{
// REGION r1 begin   0x418310-0x418417
//   prologue, early-out, mode 1 lookup, loop setup, the eight hoisted invariants
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
// REGION r2 begin   0x418417-0x41862c
//   the tile quad walk, the offscreen test and the mode dispatch
        int offscreen=1;
        Point p[4];
        unsigned char heights[4];
        for (int x=firstX;x<lastX;++x) {
            Tile* tile=&g_game->tiles[x+y*g_game->width];
            heights[0]=tile->height;
            p[0].x=(x+8)*16-g_game->scrollX;
            p[0].y=(y+2)*16-(heights[0]>>1)-g_game->scrollY;
            ++tile; ++x;
            heights[1]=tile->height;
            p[1].x=(x+8)*16-g_game->scrollX;
            p[1].y=(y+2)*16-(heights[1]>>1)-g_game->scrollY;
            tile+=g_game->width; ++y;
            heights[2]=tile->height;
            p[2].x=(x+8)*16-g_game->scrollX;
            p[2].y=(y+2)*16-(heights[2]>>1)-g_game->scrollY;
            --tile; --x;
            heights[3]=tile->height;
            p[3].x=(x+8)*16-g_game->scrollX;
            p[3].y=(y+2)*16-(heights[3]>>1)-g_game->scrollY;
            tile-=g_game->width; --y;
            if (p[0].y<g_game->bottom) offscreen=0;
            if (g_game->mode==1) {
// REGION r3 begin   0x41862c-0x41882a  the mode 1 body
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
// REGION r3 end
// REGION r4 begin   0x41882a-0x418a20  the mode 2 body
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
// REGION r4 end
// REGION r5 begin   0x418a20-0x418bab
//   the mode 3 and mode 4 bodies, the DAT_00511dd0 tail call, the offscreen
//   break and the epilogue
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
// REGION r5 end
// REGION r2 end
// REGION r1 end
}
