// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by fledge-alpha-free. Names are provisional.
// PARTIAL, 57.1% (1279 original bytes, 1279 ours; 9 diff hunks, 181 removed /
// 183 added lines). This is the best of everything tried here and below; the
// per-pass history in the rest of this header still stands.
// fledge-alpha-free pass: tools/permute.py (15 min, 2687 evaluated) lowered its
// register/stack cost score 3284->3009 but did not move check.py off 57.1%, and
// its best.cpp only differs by inlined helpers/tmp renames, so it was discarded.
// Also neutral: hoisted-unused `int pad;` (no size change), `bottom` computed
// before the lowY clamp, and the sibling's nested last-two raster guards.
// WHAT THIS PASS ADDED (Space Bunny Free). Frame arithmetic, which is easy to
// get wrong and is worth writing down. After `mov eax,SIZE; call _alloca_probe`
// esp is entry_esp-SIZE (the call's own return address has already been popped),
// so before the four pushes the arguments sit at [esp+SIZE+4], [esp+SIZE+8],
// [esp+SIZE+0xc], [esp+SIZE+0x10] and after them at [esp+SIZE+0x14] and up.
// That makes the original (SIZE 0x7d60): target 0x7d74, texture 0x7d78, vertices
// 0x7d7c, coords 0x7d80, return address 0x7d70, local area 0x00-0x7d6f. The
// local area is exactly scalars 0x00-0x4f + defaults[8] 0x50-0x6f + spans[800][10]
// 0x70-0x7d6f, so SIZE = 0x7d70-0x10 = 0x7d60 with no slack. Ours has scalars
// 0x00-0x4b, defaults 0x4c-0x6b, spans 0x6c-0x7d6b, SIZE 0x7d5c. Both waste
// 0x00-0x0f, so the whole frame difference is exactly ONE dword of scalars: the
// original has 16 scalar slots, we have 15. The defaults store order and every
// value stored are already identical (both emit [0],[1],[2],[4],[3],[6],[5],[7]),
// so fixing the frame fixes all ten of those lines at once.
// SLOT INVENTORY OF THE ORIGINAL (0x10..0x4c, roles / static [esp+K] refs):
//   0x10 12 {nextVertex, dl}      0x24  8 {x}
//   0x14 11 {next(loop1), dz}     0x28  6 {highY}
//   0x18 10 {lowX, n(loop1), next(loop2)}   0x2c 6 {out, written by both loops}
//   0x1c  6 {y1}                  0x30  6 {lowIndex, n(loop2)}
//   0x20  8 {y0}                  0x34  5 {lowY}
//   0x38  6 {dv}   0x3c 6 {du}   0x40 6 {dx}
//   0x44  3 {bottom}   0x48 3 {highIndex}   0x4c 2 {raw index-1, loop 1 only}
// The order is roughly descending by reference count (which is how MSVC 5 ranks
// slots, and it is what makes the sibling 0x4c8760's frame reproducible) but it
// is not a pure sort: 0x1c's 6 sits ahead of the two 8s. Ours, same measurement:
// 13, 12, 9, 10, 8, 9, 8, 6, 6, 6, 5, 6, 5, 3, 3.
// The missing 16th slot is `y1`: the original gives y1 a slot of its own, we
// merge y1 with the inner-loop counter `n` (both at [esp+0x18]). Nothing tried
// stops that merge; `previous = index-1; next = previous;` as a real second
// variable does not add a slot either, because MSVC folds the copy and the
// frame stays 0x7d5c.
// THE ONE BLOCKER. MSVC pins `vertices` in esi, so the min/max scan needs a
// second register for its walk pointer (edi) and one extra callee-saved, which
// pushes lowX AND highX out to slots. In the original the walk pointer IS the
// parameter, in edx (`mov edx,[esp+0x7d7c]` at the third null test, `add edx,0x10`
// per iteration, then reloaded from [esp+0x7d7c] at each loop head), the walk
// frees esi, and the four callee-saved registers go to coords (ebx), lowY (esi),
// highX (edi) and lowX (ebp). The cleanest fingerprint: the original has NO
// highX stack slot at all, it ends the scan with `test edi,edi`, while we end it
// with `mov eax,[esp+0x10]; test eax,eax`. Nothing legal makes `vertices`
// memory-resident (see the list in item 1 below plus the measured list here).
// MEASURED THIS PASS, ALL WORSE THAN 57.1% (free --sym, one change at a time
// unless noted):
//  - guard order `target && texture && vertices` on its own: 53.0% (1281). The
//    original does load target first, but the reorder costs more than it wins.
//  - min/max scan reading `int x=p[0]` AFTER the highY test, so x and y can
//    share eax and edx stays free for the walk pointer: 53.4% (1281). This does
//    achieve two of the original's fingerprints at once (null-test zero in ebp
//    and the walk pointer in edx) but scatters the scalar slots.
//  - guard order + that scan order: 53.6% (1283); plus lowY/highY/lowX/highX init
//    order: 53.6% (1283).
//  - `previous = index-1; next = previous; if(next<0) next=3;` with
//    `index = previous` at the bottom: 50.3% (1271), frame still 0x7d5c.
//  - indexed scan `vertices[i*4+1]` / `vertices[i*4]`: 52.4%; with the x-after-
//    highY order 52.9%. MSVC still makes its own copy for the induction variable.
//  - `int* p = vertices;` moved after the `if (!coords)` block: 52.4%.
//  - one `out` shared by both loops (the original writes [esp+0x2c] from both
//    loop heads, ours writes 0x34 then 0x40): 56.3%.
//  - dx/du/dv hoisted to function scope: 39.1% by score, and note this one is
//    NOT simply worse: it has only 163 removed / 165 added lines against 181 /
//    183, it does give the original frame 0x7d60 with defaults 0x50 and spans
//    0x70, and dv/du land at 0x38/0x3c where the original has them. The score
//    metric is therefore not a plain edit distance; a misaligned head costs more
//    than a few extra changed lines save.
//  - tools/permute.py (17 min, 1562 candidates, 24 did not compile): 57.1% ->
//    57.1%, no improvement.
// NEUTRAL (kept, they read better): `y1` and `x` at function scope instead of
// inside `if(lowY!=highY)`; `currentVertex` declared before `nextVertex` in both
// edge loops (the original computes currentVertex first and loads its [1] first:
// `mov esi,[ebp+4]` then `mov ecx,[eax+4]`).
// What changed (claude-sonnet-5-5): the instruction count and byte count now equal
// the original. Two things did it: the min/max scan walks the vertices with a
// pointer (`p+=4`, which puts the null-check zero in ebp like the original), and
// the edge loops read `currentVertex[k]` / `nextVertex[k]` pointers with the
// `next=index-1; ... index--; if(index<0) index=3;` pair (the compiler still
// folds the raw index-1 into one of the 15 slots; the original keeps it as the
// 16th slot [esp+0x4c], so the frame is 0x7d5c versus 0x7d60). The rest is a
// handful of small statement-order changes found with tools/permute.py (guard
// order texture/target/vertices, `p` declared first, highY/lowY/lowX/highX init
// order, `bottom` after the lowY clamp; the loop-1 nextVertex/currentVertex order
// it lists was later swapped back to currentVertex first, which measures neutral).
// What still differs (all of it is register/stack-slot allocation):
//  1. `vertices` is pinned in esi from the null check on, so the min/max scan
//     copies it (`mov edx,esi`) and highX is demoted to a stack slot (the
//     original keeps highX in edi and reloads vertices from [esp+0x7d7c] at each
//     loop head: `mov esi,[v]; ... lea ebp,[ecx+esi]; mov ecx,[v]; add eax,ecx`).
//     Experiment (not a legal fix): reading the heads through a global pointer
//     instead of `vertices` makes the prologue/min-max match and gives 64%
//     with the hoisted-slope shape below. Nothing legal tried makes `vertices`
//     memory-resident: register keyword, &vertices, early return, guard order,
//     pointer walker, function-scope currentVertex/nextVertex, helper functions.
//  2. Frame 0x7d5c versus 0x7d60. Hoisting dx/du/dv to function scope (as in
//     0x4c8760) gives the original frame 0x7d60, defaults 0x50, spans 0x70, and the
//     tail slots dv 0x38, du 0x3c, dx 0x40, highIndex 0x48 land where the original
//     has them, but the head slots (original: 0x10 {nv,dl}, 0x14 {next1,dz},
//     0x18 {lowX,n1,next2}, 0x1c y1, 0x20 y0, 0x24 x) do not, and the score drops
//     to about 40-45 because of that. The same next-split / grouping problem as
//     0x4c8760 (see its notes) decides it.
//  3. In the original u,v,z,lg sit in esi,edi,ebx,ebp with coords cached in ebx
//     (restored after the inner loop); previous stays in edx across the
//     skipped-edge path.

struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall FUN_004c8020(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall FUN_004c8bb0(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
{
    int y0;
    int y1;
    int x;
    int defaults[8];
    int spans[800][10];
    int next;
    int lowIndex;
    int highIndex;
    int dz;
    int dl;
    int lowY;
    int highY;
    int highX;
    int lowX;
    if (texture && target && vertices) {
        int* p=vertices;
        if (!coords) {
            coords=defaults;
            defaults[0]=0; defaults[1]=0;
            defaults[2]=texture->width-1; defaults[3]=0;
            defaults[4]=texture->width-1; defaults[5]=texture->height-1;
            defaults[6]=0; defaults[7]=texture->height-1;
        } highY=-999999; lowY=999999; lowX=999999;
        highX=-999999;
        for(int i=0;i<4;i++) {
            int y=p[1];
            if(y<lowY) { lowY=y; lowIndex=i; }
            int x=p[0];
            if(y>highY) { highY=y; highIndex=i; }
            if(x>highX) highX=x;
            if(x<lowX) lowX=x;
            p+=4;
        }
        if (highX>=0 && lowX<=target->width-1 && highY>=0 && lowY<=target->height-1) {
                    if(lowY<0) lowY=0;
                    int bottom=target->height-1;
                    if(highY>bottom) highY=bottom;
                    if(lowY!=highY) {
        {
        int* out;
        int index=lowIndex;
        out=&spans[0][0];
        do {
        next=index-1;
        if (next<0) next=3;
        int* currentVertex=vertices+index*4;
        y0=currentVertex[1];
        int* nextVertex=vertices+next*4;
        y1=nextVertex[1];
        if (y1>0 && y0<y1) {
            int dy=y1-y0;
            int dx = ((nextVertex[0]-currentVertex[0])*0x10000)/dy;
            x=currentVertex[0]*0x10000+0xffff;
            int u=coords[index*2]*0x10000;
            int v=coords[index*2+1]*0x10000;
            int z;
            z = currentVertex[2]*0x10000;
            int lg=currentVertex[3]*0x10000;
            int du=(coords[next*2]*0x10000-u)/dy;
            int dv=(coords[next*2+1]*0x10000-v)/dy;
            dz=(nextVertex[2]*0x10000-z)/dy;
            dl=(0x10000*nextVertex[3]-lg)/dy;
            if(y0<0) { x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; lg-=y0*dl; y0=0; }
            if(y1>bottom) y1=bottom;
            if(y0<y1) {
            int n = y1-y0;
            do {
            out[0]=x>>16; x+=dx; out[2]=u; u+=du; out[3]=v; v+=dv; out[6]=z; out[8]=lg; z+=dz; out+=10; lg+=dl;
            } while(--n);
            }
            }
        index--; if(index<0) index=3;
        } while(index!=highIndex);
        }
        {
        int* out;
        int index=lowIndex;
        out=&spans[0][0];
        do {
        next=(index+1)&3;
        int* currentVertex=vertices+index*4;
        y0=currentVertex[1];
        int* nextVertex=vertices+next*4;
        y1=nextVertex[1];
        if (y1>0 && y0<y1) {
            int dy=y1-y0;
            int dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
            x=currentVertex[0]*0x10000+0xffff;
            int u=coords[index*2]*0x10000;
            int v=coords[index*2+1]*0x10000;
            int z;
            z = currentVertex[2]*0x10000;
            int lg=currentVertex[3]*0x10000;
            int du=(coords[next*2]*0x10000-u)/dy;
            int dv=(coords[next*2+1]*0x10000-v)/dy;
            dz=(nextVertex[2]*0x10000-z)/dy;
            dl=(nextVertex[3]*0x10000-lg)/dy;
            if(y0<0) { x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; lg-=dl*y0; y0=0; }
            if(y1>bottom) y1=bottom;
            if(y0<y1) {
            int n = y1-y0;
            do {
            out[1]=x>>16; x+=dx; out[4]=u; u+=du; out[5]=v; out[7]=z; v+=dv; out[9]=lg; z+=dz; out+=10; lg+=dl;
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