// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash. Names are provisional.
// deepseek-v4.1-flash pass (timeboxed 2026-10-01), body unchanged at 54.1%.
// Fifteenth pass (deepseek-v4.1-flash, 2026-10-01): the winning hoist fix of
// sibling 0x4c8760 does NOT transfer here. Hoisting ALL block-scoped edge
// temps (next, dv, dx, du, dz, dl, y0, y1, n, nextVertex) to function scope
// scores 45.1% (1271 bytes) and hoisting only next/dv/dx/du scores 46.5%
// (1275 bytes), both regressions from 54.1 at 1275 bytes, so the temps stay
// block-scoped and the 0x7d5c frame plus edx null-guard deltas remain.
// Second deepseek-v4.1-flash pass (timeboxed 2026-10-01): re-checked 54.1%
// (1275 of 1279 bytes) and confirmed the two biggest structural deltas remain
// the frame (ours 0x7d5c, original 0x7d60, one 4-byte scalar short) and the
// guard null register (original keeps 0 in ebp, ours in edx because ebp holds
// the third pointer argument). Both are upstream of the whole body, so no
// local hunk edit repays them.
// Measured the vK candidate the previous pass left unscored (free --sym):
// currentVertex+nextVertex pointer locals in both edge loops plus a live
// previous tail (index=previous; if(index<0) index=3): 49.1%, frame still
// 0x7d5c, so vK is dead. Also scored live-previous tail shapes on the
// current body: vTail (if/fixup) 50.1%, ternary tail 50.1%, comma
// assignment next=(previous=index-1) 50.2%, comma+ternary 50.1%. All fold
// previous away and keep the 15-slot arena. New theory for the next pass:
// the original's tail fixup survives because next's slot [esp+0x14] is
// clobbered by dz (0x4c8dce) before the tail, so the head fixup value is
// NOT available there and the CSE cannot fire; find a source shape where
// next's value is genuinely dead before the tail.
// deepseek-v4.1-flash pass (timeboxed), body unchanged at 54.1%. The untried
// "unpinned vertices" lever from the brief was tested and does NOT grow the
// frame: the missing 16th slot [esp+0x4c] (raw index-1 of loop1) stays absent
// and the frame stays 0x7d5c in every form. Measured (free --sym):
//  - vA: sibling 0x4c8760 loop shape (currentVertex pointer + inline
//    vertices[next*4], live previous tail index=previous;if(<0)=3): 26.5%.
//  - vB: ALL vertex reads recomputed inline (no cv/nv pointer at all),
//    previous live to a re-fixed tail: 50.8%, 1286B. Prologue still pins
//    vertices in esi and zeroes edx (NOT the original's zero-ebp / reload-
//    vertices set), proving "recompute at each use" alone does not unpin it.
//  - vK: BOTH currentVertex and nextVertex pointers so vertices is touched
//    only at the loop head (the true original access pattern), + live previous
//    tail. Written but not scored before the timebox; candidates for the next
//    pass to measure first.
// Root cause confirmed again: MSVC merges `previous` (raw index-1) into next's
// slot because at body peak next dies early (last read coords[next*2+1]) and
// previous is only read at the tail, so their live ranges are disjoint and the
// allocator shares one slot, keeping the frame at 15. The original does NOT
// merge them (previous 0x4c, next/dz 0x14): its four accumulators u,v,z,lg sit
// in esi/edi/ebx/ebp so no callee-saved reg is free for the long-lived
// previous, forcing it to spill to 0x4c. Ours frees a callee-saved reg (by
// pinning vertices to one and/or spilling an accumulator), so previous stays
// in a register and never gets a slot. Next lever to try: force the four
// accumulators u,v,z,lg to occupy esi/edi/ebx/ebp simultaneously (write them
// as live locals read/written across the whole body) so previous must spill.
// deepseek-v4.1-flash pass (timeboxed, no score change): deleting the unused
// function-scope `int* out;` (both loop blocks declare their own) is
// byte-identical at 54.1 / 1275 bytes, so that outer dummy does not hold the
// missing 16th slot; the 0x7d5c vs 0x7d60 frame gap stands.
// deepseek-v4.1-flash pass (timeboxed 2026-10-01): the "force the four
// accumulators to stay live across both loops" lever was tested by hoisting
// u,v,z,lg to function scope; it is score-neutral at 54.1 / 1275 bytes, so
// that does not make previous spill and the frame gap remains the blocker.
// Partial 54.1% (best this file has reached; check.py prints 54.1).
// space-bunny-free pass: no improvement, all variants below scored below the
// 54.1% baseline, so this file is unchanged apart from these notes. What I
// re-confirmed: the frame is 0x7d5c against the original's 0x7d60, a single
// missing dword, and because the whole low scalar region is 4 bytes low
// EVERY stack reference in both edge loops differs mechanically (that alone
// is most of the 447 diff lines). The remaining structural delta is the
// min/max scan: the original walks the vertex pointer as an induction
// variable in edx (`add edx,0x10` in the latch, no reload) and keeps highX
// in edi and lowX reloaded into ebp, so it needs no callee-saved register
// for `vertices`; ours pins `vertices` in ebp, reloads edx from it each
// iteration, and demotes highX and lowX to memory slots 0x10 and 0x14. Per
// brief item 2 that single extra pinned value is what demotes highX and lowX
// past edi and ebp, so the whole prologue difference is one allocator state,
// not two problems.
// Tried this pass, all with free --sym runs, all WORSE than 54.1%:
//  - min/max loop as an explicit pointer walk (`int* pv=vertices;` then
//    `i++, pv+=16`, reading pv[1]/pv[0]): 53.9%. MSVC still emits
//    `mov edx, ebp` at the loop head, i.e. it did not promote pv to an
//    induction variable and left `vertices` pinned in ebp, so the cause
//    survives. (Writing the pointer in the for-init, `for(int i=0,pv=...)`,
//    does not even compile under the checker's MSVC 5 mode, C2440.)
//  - hoisting the ten shared edge-walk temps (nextVertex, y0, y1, x, dx, du,
//    dv, dz, dl, out) to function scope, as the sibling 0x4c8760's winning
//    pass did with next/dv/dx/du: 42.6% and 1284 bytes. Frame grows, score
//    collapses.
//  - hoisting only the five slopes dx,du,dv,dz,dl to function scope: 49.7%
//    (1271 bytes). And the same hoist plus the `index=previous; if(index<0)
//    index=3;` tail: 39.0%.
//  - moving the head to `next=index-1; previous=next;` with the latch
//    `index=previous;` and no second fixup: 41.5%.
// Conclusion for the next attempt: the hoisting trick that won on 0x4c8760
// loses here, so the missing 16th slot has to come from `previous` alone,
// and the register set has to be freed by not pinning `vertices` at all. The
// untested lever for that is the min/max loop's `vertices` uses written so no
// local spans the body, or the two `next`/`n` pairs made genuinely disjoint so
// the arena grows instead of merging.
// Final retry (deepseek-v4.1-flash, timeboxed), what still differs unchanged:
// the 16th frame slot [esp+0x4c] (raw index-1 of the first edge loop, one
// store at 0x4c8cfc, one reload at 0x4c8e9c) and the prologue register set
// (original: zero in ebp, vertices reloaded from its arg slot, texture pinned
// in ecx, highX in edi and lowX in ebp with no slots; ours: zero in edx,
// vertices pinned in ebp, highX/lowX spilled at 0x10/0x14). Analysis this
// pass: the whole register set falls out of vertices NOT being a pinned
// register in the original (its min/max loop walks in edx and the edge loops
// reload [esp+0x7d7c] twice within three instructions), so the zero constant
// takes ebp; ours pins vertices in ebp so zero falls to edx and one body
// register stays free, which is also why the compiler can fold the raw
// index-1 into the tail fixup instead of spilling it. v1 (first loop as
// while(1) with a tail break, raw kept as `previous=index-1; next=previous;
// if(next<0) next=3;` and tail `index=previous; if(index<0) index=3;
// if(index==highIndex) break;`) still compiled to the 15-slot 0x7d5c frame
// (score line not captured before the timebox). Untried idea: stop pinning
// vertices by making every access reload it (no local that spans the body),
// e.g. recompute vertices+N*4 expressions at each use, to flip the global
// register allocation to the original's {zero=ebp, walk=edx} set.
// deepseek-v4.1-flash pass: confirmed by disassembly that the original's
// missing slot [esp+0x4c] is the raw `index-1` of the first edge loop, stored
// at loop head (0x4c8cfc) and reloaded at the loop tail (0x4c8e9c) where the
// code does index=previous; if(index<0) index=3 (fixup re-applied at the
// tail). Tried on top of the current body: (a) sibling 0x4c8760 loop shape
// with currentVertex/nextVertex pointer locals plus live previous (41.9%,
// frame still 0x7d5c), (b) minimal live-previous change
// (previous=index-1; next=previous; if(next<0) next=3; ... tail
// index=previous; if(index<0) index=3) (50.2%, frame still 0x7d5c; the
// compiler folds previous into the tail fixup instead of spilling it, same
// as the earlier passes reported below). The 16th slot remains unreachable.
//
// This is a textured/gouraud triangle rasteriser into 10-int
// span rows, then one FUN_004c8020 call per scanline. The sibling 0x4c8760 is
// the same algorithm without the light channel and is matched to 67.9%; its
// first edge loop uses the same `previous`/`next` idiom adopted here.
//
// Still differs, and it is one systematic cause plus the prologue:
//  deepseek-v4.1 experiments (both neutral, 51.6% and 1275 bytes unchanged):
//  rewriting the defaults block to the original's store order
//  (d0,d1,d2,d4,d3,d6,d5,d7 with CSE'd width-1/height-1) and writing the first
//  edge loop's `next=index-1` as its own expression. The compiler CSEs both
//  back to the shapes already here, so the frame stays one slot short. The
//  original's spans base is 0x70 while this file's is 0x6c (the frame passed
//  to _alloca_probe is array_base + 0x7d00 - 0x10, i.e. the four saved
//  registers count inside that size), so the missing slot is a live scalar in
//  0x10..0x4c; the original keeps `index-1` at [esp+0x4c] and reloads it while
//  this version coalesces it with `next`.
//  - frame is 0x7d5c vs original 0x7d60. The original keeps one more live int
//    in the low temp region: its `defaults`/`spans` start at [esp+0x50] where
//    ours start at [esp+0x4c], so every default, spans and argument offset is
//    4 lower. The original's extra slot is [esp+0x4c], the raw `index-1` in
//    the first edge loop (used twice: for `next` and for the loop update).
//    Giving the first loop that duplicate (as done here) did NOT grow the
//    region: MSVC folded it into an existing slot.
//  - the min/max loop: original keeps lowY in esi, highX in edi, lowX in ebp
//    and reloads `vertices` from its argument slot; ours keeps `vertices` in
//    esi and spills highX/lowX/both. That is an allocator decision, not a
//    source-shape one that reordering fixed.
//  - the null checks use edx as the zero register; the original zeroes ebp
//    (`xor ebp,ebp` between push ebp and push esi). Consequence of the above.
//
// Ruled out (measured with free --sym scratch runs): moving `bottom` before
// the guard, early-return null checks, function-scope min/max declarations,
// a currentVertex pointer, and prepending the real 0x4c8760 (compiler state).
// deepseek-v4.1 also tried, twice (block-scope `next` then function-scope
// `next`), giving the first loop a live `previous` exactly like the original
// disassembly (previous=index-1; next=previous; if(next<0) next=3; ... body ...
// index=previous; if(index<0) index=3;). Both compiled to the same 15-slot
// arena (0x10..0x48, spans 0x6c) at 48.0% and 48.4%: the allocator parked
// `previous` in the top slot and moved highIndex/bottom down one instead of
// growing the frame, so the missing 0x4c slot is not reachable by making
// `previous` live. The 51.6% baseline (`index=next`, no live previous) stays.
// tools/headers.py finds no header set that changes the bytes, and 4 to 64
// unused `extern int` declarations leave the score at exactly 51.5%.
// Also tried here: growing defaults to 9 or 10 ints and biasing its pointer
// by one did not align both arrays; those scored 39.9%, 37.7%, and 47.2%.
// Reordering declarations and taking the address of `previous` were neutral.
// Hoisting the block-scope `x`/`y1` (the sibling 0x4c8760's
// winning change) and hoisting the span `x`/`y1` plus porting its whole loop
// structure; those scored 51.3 and 36.1 and did not grow the frame either.
//
// This pass (deepseek-v4.1-flash): confirmed the missing [esp+0x4c] slot is
// the first loop's raw index-1. Keeping `previous` live across the body (with
// `index=previous; if(index<0) index=3;`) still compiles to a 15-slot frame
// (0x7d5c, spans 0x6c): MSVC proves index==next and drops previous. Mirroring
// 0x4c8760's winning loop verbatim did not transfer here (44.4%, frame shrank
// to 1239 bytes). So the 0x4c slot is not reachable by making `previous` live.
//
// Fixes that did land: second edge loop writes ints 1,4,5,7,9 of the span row
// (the row is [xL,xR,uL,vL,uR,vR,zL,zR,lL,lR]), the rasterise guard is
// span[1]-span[0]>0 (not !=0 && >=0), and the loop temps are function-scope.
// deepseek-v4.1-flash pass (timeboxed 2026-10-01): hoisting `int bottom;` to
// function scope (assigned at its old line from target->height-1) is
// byte-identical at 54.1% / 1275 bytes, so the original 0x7d60 frame does
// not come from a function-scope bottom slot.

// deepseek-v4.1-flash pass (timeboxed 2026-10-01): measured the exact original
// head/tail shape from the disassembly (previous=index-1; next=previous;
// if(next<0) next=3; ... tail index=previous; if(index<0) index=3;) as a
// scratch variant scored free with --sym: 50.2% and 1284 bytes, frame STILL
// 0x7d5c, so the live raw previous alone does not grow the arena. The frame
// word can only come from dz sharing next's 0x14 slot the way it does at
// 0x4c8dce in the original; without that co-location the tail keeps the head
// fixup and the raw dies.
// deepseek-v4.1-flash pass (timeboxed 2026-10-01, one measurement): writing the
// loop-1 y0 fixup products y0-first (`x-=y0*dx; u-=y0*du; v-=y0*dv; z-=y0*dz;
// lg-=y0*dl;`) is byte-identical at 54.1% / 1275 bytes, so the 15-slot frame
// and its 447 stack-reference lines are not steered by the imul operand order
// either. The missing 16th slot at [esp+0x4c] (loop-1 raw index-1) stands as
// the primary divergence, with the vertex induction register (original walks
// vertices in edx, ours pins it in ebp) as the root of the prologue hunk.


struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall FUN_004c8020(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall FUN_004c8bb0(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
{
    int defaults[8];
    int spans[800][10];
    int index;
    int previous;
    int* out;
    int x;
    if (target && texture && vertices) {
        if (!coords) {
            coords=defaults;
            defaults[0]=0; defaults[1]=0;
            defaults[2]=texture->width-1; defaults[4]=texture->width-1;
            defaults[3]=0; defaults[6]=0;
            defaults[5]=texture->height-1; defaults[7]=texture->height-1;
        }
        int lowY=999999, highY=-999999, highX=-999999, lowX=999999;
        int lowIndex, highIndex;
        for(int i=0;i<4;i++) {
            int vx=vertices[i*4+1];
            if(vx<lowY) { lowY=vx; lowIndex=i; }
            if(vx>highY) { highY=vx; highIndex=i; }
            int xx=vertices[i*4];
            if(xx>highX) highX=xx;
            if(xx<lowX) lowX=xx;
        }
        if(highX>=0 && lowX<=target->width-1 && highY>=0 && lowY<=target->height-1) {
            int bottom=target->height-1;
            if(lowY<0) lowY=0;
            if(highY>bottom) highY=bottom;
            if(highY!=lowY) {
                {
                    int* out;
                    out=&spans[0][0];
                    index=lowIndex;
                    do {
                        previous=index-1;
int next;
                        next=index-1;
                        if(next<0) next=3;
int* nextVertex;
                        nextVertex=vertices+next*4;
int y0;
                        y0=vertices[index*4+1];
int y1;
                        y1=nextVertex[1];
                        if (y1>0 && y0<y1) {
                            int dy=y1-y0;
int dx;
                            dx=((nextVertex[0]-vertices[index*4])*0x10000)/dy;

                            x=vertices[index*4]*0x10000+0xffff;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;
                            int z=vertices[index*4+2]*0x10000;
                            int lg=vertices[index*4+3]*0x10000;
int du;
                            du=(coords[next*2]*0x10000-u)/dy;
int dv;
                            dv=(coords[next*2+1]*0x10000-v)/dy;
int dz;
                            dz=(nextVertex[2]*0x10000-z)/dy;
int dl;
                            dl=(nextVertex[3]*0x10000-lg)/dy;
                            if(y0<0) {
                                x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;
                                lg-=dl*y0;
                                y0=0;
                            }
                            if(y1>bottom) y1=bottom;
                            if(y0<y1) {
int n;
                                n=y1-y0;
                                do {
                                    out[0]=x>>16; x+=dx;
                                    out[2]=u;
                                    u+=du;
                                    out[3]=v;
                                    v+=dv;
                                    out[6]=z;
                                    out[8]=lg;
                                    out+=10;

                                    z+=dz;
                                    lg+=dl;
                                } while(--n);
                            }
                        }
index=next;
                    } while(index!=highIndex);
                }
                {
                    int* out;
                    out=&spans[0][0];
                    int index=lowIndex;
                    do {
int next;
                        next=(index+1)&3;
int* nextVertex;
                        nextVertex=vertices+next*4;
int y0;
                        y0=vertices[index*4+1];
int y1;
                        y1=nextVertex[1];
                        if (y1>0 && y0<y1) {
                            int dy=y1-y0;
int dx;
                            dx=((nextVertex[0]-vertices[index*4])*0x10000)/dy;

                            x=vertices[index*4]*0x10000+0xffff;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;
                            int z=vertices[index*4+2]*0x10000;
                            int lg=vertices[index*4+3]*0x10000;
int du;
                            du=(coords[next*2]*0x10000-u)/dy;
int dv;
                            dv=(coords[next*2+1]*0x10000-v)/dy;
int dz;
                            dz=(nextVertex[2]*0x10000-z)/dy;
int dl;
                            dl=(nextVertex[3]*0x10000-lg)/dy;
                            if(y0<0) {
                                x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;
                                lg-=dl*y0;
                                y0=0;
                            }
                            if(y1>bottom) y1=bottom;
                            if(y0<y1) {
int n;
                                n=y1-y0;
                                do {
                                    out[1]=x>>16; x+=dx;
                                    out[4]=u;
                                    u+=du;
                                    out[5]=v;
                                    v+=dv;
                                    out[7]=z;
                                    out[9]=lg;
                                    out+=10;

                                    z+=dz;
                                    lg+=dl;
                                } while(--n);
                            }
                        }
                        index=next;
                    } while(index!=highIndex);
                }
                int* span=&spans[0][0];
                for(int row=lowY;row<highY;row++) {
                    if(span[1]-span[0]>0)
                        FUN_004c8020(row,span,target,texture);
                    span+=10;
                }
            }
        }
    }
}
