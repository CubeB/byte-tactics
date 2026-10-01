// Decompiled by GPT-6, finished by space-bunny-free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by
// claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// PARTIAL, 90.5% (1094 original bytes, 1094 ours; was 87.9% at the same size).
// What got it here (tools/permute.py): hoisting the scan counter to a function
// scope `i` and spelling the scan `while(i<4)`, splitting the raster guard so
// the last two tests are a nested `if` instead of a fourth `&&`, and moving
// `x+=dx` and `out+=10` to their own statements in the two inner loops. Those
// shapes changed the frame-slot layout for the better. Nothing about the
// instruction stream changed: same instructions, same order, same byte count.
// What still differs (every hunk is slot assignment or the fix-up multiply):
//  1. The original's 14 frame slots are grouped
//       0x10 {next (loop 1), dv}  0x14 {lowX, n (loop 1), next (loop 2)}
//       0x38 {the vertices+next*3 temp}
//     while ours has
//       0x10 {lowX, n (loop 1), the temp}  0x14 {next, both loops}  0x38 {dv}
//     So the original really does have two distinct neighbour-index variables
//     (loop 1's `previous`, loop 2's `next`), each living in a different slot,
//     with loop 1's sharing its slot with `dv` afterwards.
//  2. The y0<0 fix-up multiplies: the original loads dx/du/dv into ecx/edx and
//     then does `imul reg, ebp` (so the register holds the slope and ebp holds
//     y0), ours folds the memory operand (`mov ecx,ebp; imul ecx,[dx]`).
//     Operand order in the source is NOT the lever: writing `y0*dx` instead of
//     `dx*y0` compiles identically (measured, 90.5 either way).
// Measured and NOT the lever (all <= 90.5 or worse, via free --sym scoring):
//  - naming the neighbour vertex (`int* nextVertex=vertices+next*3`) in both
//    loops: 64.3%, and it demotes `vertices` out of esi in the scan loop;
//  - splitting `next` into `previous` (loop 1) and `next` (loop 2), both at
//    function scope, or `previous` at function scope and `next` block scoped:
//    69.7% and 68.5%, a 13 slot frame (0x7d54) because `y1` then shares with
//    `n` and loop 1's `out` with `lowX`, which the original does not do;
//  - reordering the function-scope declaration list (`dv, dx, du, next`):
//    90.5%, no slot moves;
//  - `index=next;` at the bottom of loop 1 instead of the `index--` pair:
//    72.1% at 1081 bytes (the original keeps the raw index-1 spill at 0x44);
//  - three more tools/permute.py runs (15 min each) from 87.9% and from 90.5%:
//    90.5% at best, the same hunks. The second run started at 90.5 and found
//    nothing in 1498 candidates, so the statement-order space around this shape
//    is exhausted.
// Earlier passes on the 87.9% version recorded as measured-and-exhausted:
// every single and pair change of next/dv/dx/du/x/y1/out/y0/dz scope (function /
// outer block / per-loop block), do-while versus for(yy) inner loop, named
// currentVertex versus inline, min/max locals inside the if, `bottom` as a
// variable or an expression (about 200 files); 500 random per-variable-per-loop
// scope assignments (best 86.1); loop 2 with no `next` variable at all
// (`((index+1)&3)` written out): 68.5; each edge loop as a static inline helper
// (both, or one of them): 68.8.
// The remaining difference is the split of the neighbour index into two
// variables, which needs a 14 slot frame; every source spelling of it tried
// collapses the frame to 13 slots, so one more live value is always needed.
// Re-indenting the body (no token changes) leaves the score at 90.5, so the
// score does not depend on the indentation the permuter left behind.

struct Surface_4c8760 { unsigned short width, height; };
void __stdcall FUN_004c7a20(int, int*, Surface_4c8760*, Surface_4c8760*);

// FUNCTION: 0x4c8760
void __stdcall FUN_004c8760(Surface_4c8760* target, Surface_4c8760* texture, int* vertices, int* coords)
{
    int i, defaults[8];
    int spans[800][10];
    int next, dv, dx, du;
    int lowY, highY, highX, lowX;
    int lowIndex, highIndex;
    if (target && texture && vertices) {
        if (!coords) {
            coords=defaults;
            defaults[0]=0; defaults[1]=0;
            defaults[2]=texture->width-1; defaults[3]=0;
            defaults[4]=texture->width-1; defaults[5]=texture->height-1;
            defaults[6]=0; defaults[7]=texture->height-1;
        }
        lowY=999999; highX=-999999; highY=-999999; lowX=999999;
        i = 0;
        while (i<4) {
            int y=vertices[i*3+1];
            if(y<lowY) { lowY=y; lowIndex=i; }
            if(y>highY) { highY=y; highIndex=i; }
            int x=vertices[i*3];
            if(x>highX) highX=x;
            if(x<lowX) lowX=x;
            i = i + 1;
        }
        if (highX>=0 && lowX<=target->width-1 && highY>=0) {
            if (lowY<=target->height-1) {
                int bottom=target->height-1;
                if(lowY<0) lowY=0;
                if(highY>bottom) highY=bottom;
                if(highY!=lowY) {
                    int y1; int x;
                    {
                        int* out=&spans[0][0], index=lowIndex;
                        do {
                            next=index-1;
                            if (next<0) next=3;
                            int* currentVertex=vertices+index*3;
                            int y0=currentVertex[1];
                            y1=vertices[next*3+1];
                            if (y0<y1) {
                                int dy = y1-y0;
                                dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                                x=currentVertex[0]*0x10000+0xffff;
                                int z=currentVertex[2]*0x10000;
                                int u=coords[index*2]*0x10000;
                                int v=coords[index*2+1]*0x10000;

                                du=(coords[next*2]*0x10000-u)/dy;
                                dv=(coords[next*2+1]*0x10000-v)/dy;
                                int dz = (vertices[next*3+2]*0x10000-z)/dy;

                                if(y0<0) {
                                    x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;

                                    y0=0;
                                }
                                if(y1>bottom) y1=bottom;
                                if(y0<y1) {
                                    int n=y1-y0;
                                    do {
                                        out[0]=x>>16; out[2]=u;
                                        x+=dx;

                                        out[3]=v;

                                        out[6]=z;

                                        u+=du;
                                        v+=dv; out+=10;
                                        z+=dz;

                                    } while(--n);
                                }
                            }
                            index--;
                            if(index<0) index=3;
                        } while(index!=highIndex);
                    }
                    {
                        int* out=&spans[0][0];
                        int index = lowIndex;
                        do {
                            next=(index+1)&3;
                            int* currentVertex=vertices+index*3;
                            int y0=currentVertex[1];
                            y1=vertices[next*3+1];
                            if (y0<y1) {
                                int dy = y1-y0;
                                dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                                x=currentVertex[0]*0x10000+0xffff;
                                int z=currentVertex[2]*0x10000;
                                int u=coords[index*2]*0x10000;
                                int v=coords[index*2+1]*0x10000;

                                du=(coords[next*2]*0x10000-u)/dy;
                                dv=(coords[next*2+1]*0x10000-v)/dy;
                                int dz = (vertices[next*3+2]*0x10000-z)/dy;

                                if (y0 < 0) {
                                    x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;

                                    y0=0;
                                }
                                if(y1>bottom) y1=bottom;
                                if(y0<y1) {
                                    int n=y1-y0;
                                    do {
                                        out[1]=x>>16; x+=dx;
                                        out[4]=u;

                                        out[5]=v;

                                        out[7]=z;

                                        u+=du;
                                        v+=dv; z+=dz;
                                        out+=10;

                                    } while(--n);
                                }
                            }
                            index=next;
                        } while (index != highIndex);
                    }
                    int* span=&spans[0][0];
                    for(int row=lowY;row<highY;row++) {
                        if(span[1]-span[0]>0)
                            FUN_004c7a20(row,span,target,texture);
                        span+=10;
                    }
                }
            }
        }
    }
}