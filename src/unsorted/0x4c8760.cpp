// Decompiled by GPT-6, finished by space-bunny-free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 87.3% (1094 original bytes, 1094 ours).
// Fixed this pass: hoisting all four shared edge-walk temps (next, dv, dx, du)
// to function scope, ahead of the min/max locals, restored the frame from
// 0x7d54 to the original 0x7d58 (one extra scalar slot) and took 72.5 to 87.3.
// The min/max scan and both span-edge loops now emit the same instruction
// set; every remaining diff is stack slot assignment and instruction order.
// What still differs (12 diff hunks, all downstream of the slot crossing):
// 1. The allocator crosses two slot pairs. Original: {next,dv} at 0x10,
//    {lowX,n1} at 0x14, {currentVertex} at 0x38. Ours: {lowX,n1} and
//    {currentVertex} at 0x10, {next} at 0x14, {dv} at 0x38. The frame size and
//    slot count now match, only which variable gets which offset differs.
//    Moving the hoisted temp block to the top, after lowIndex, or splitting
//    next/dv from dx/du (v2, v3, v4) all stayed 87.3, so the position of the
//    temp block is inert once hoisted.
// 2. First span loop body order: the original emits `x+=dx` right after
//    `out[0]=x>>16`, before `out[2]=u`; ours emits it after `out+=10`.
// 3. Second span loop exit: original does `mov edi,eax; mov eax,[highIndex];
//    cmp edi,eax`; ours uses ecx for highIndex and `cmp eax,ecx`.
// Tried a rowEnd temporary, reversing lowIndex/highIndex declaration order,
// rewriting the final for loop as while, and three temp-block orderings; none
// changed the score. The 128-set header sweep also found no improvement. The
// remaining approach is to find the declaration/scope construct that makes
// the allocator give {next,dv} slot 0x10 and {lowX,n1} slot 0x14.
// Second pass (deepseek-v4.1-flash), all scored from the /Fa listing:
// every declaration-order permutation of the min/max group and of the
// temp group is inert (identical slot table); swapping the y and x
// compare order moves lowY/highY but not lowX/next; hoisting out, n,
// previous or currentVertex to function scope reshuffles many slots but
// never to the original set; switching the first loop to
// next=index-1; previous=next; is inert. So the slot table follows the
// allocator's live-range order, not declaration order, and the crossing
// needs a change to a lifetime, not to a declaration.
// Third pass (deepseek-v4.1-flash retry), all measured with free --sym runs:
// The single primary divergence is lowX: original puts it at 0x14, we put it
// at 0x10; every other diff follows from that (our unnamed `vertices+next*3`
// temp then reuses 0x10, forcing next to 0x14 and dv to 0x38, where the
// original has next/dv at 0x10 and the next-vertex pointer at 0x38).
//  - All 24 declaration-order permutations of lowY/highY/highX/lowX: inert.
//  - All 24 permutations of their four assignment statements: max 87.3, never
//    better (13 land at 86.7 or 87.0 because the store order changes).
//  - Block-scoping `next` inside both edge loops (two locals) DOES reproduce
//    the original's sharing pattern exactly (dv shares loop1 next's slot;
//    loop2 next shares lowX's slot) but the frame drops to 0x7d54 and the
//    whole scalar arena shifts: 67.9%. Any shadowed/block `next` variant hits
//    the same 67.9% attractor.
//  - An explicit `int* nextVertex` local (variant D) still lands in 0x10 and
//    scores 81.9%.
//  - Hoisting x/y1 to function scope, and moving dv or dv+dx into the
//    if(highY!=lowY) block, are all inert (dv stays at 0x38).
// So the 0x10/0x14 crossover is a live-range decision, not a declaration one,
// exactly as the earlier passes concluded. Nothing left to try by hand.
// Fourth pass (deepseek-v4.1-flash, timeboxed, no new variants scored): the
// original's per-loop slot split proves the two edge loops use SEPARATE
// variables in the source: loop1 next at 0x10 (shared with dv1/dv2) but loop2
// next at 0x14 (shared with lowX/n1), and n1 at 0x14 while n2 is at 0x28
// (with lowIndex). So next and n are block-scoped per loop, while dv, du, dx,
// x, y1, out merge across the loops. The known block-scoped-next experiment
// reproduced that sharing but shrank the frame to 0x7d54; the extra merged
// slot is most likely n1+n2 merging with each other once they become two
// block locals. Next step: block-scope next AND keep n1/n2 from merging
// (a lifetime that spans both loops), then re-check. Also still open: the
// y0 fixup multiplies emit `mov ecx,[mem]; imul ecx,ebp` (delta left) in the
// original but `mov ecx,ebp; imul ecx,[mem]` (y0 left) in ours for the same
// `dx*y0` source text, and the loop body schedule puts `x+=dx` right after
// `out[0]=x>>16` in the original but after `out+=10` in ours.
struct Surface_4c8760 { unsigned short width, height; };
void __stdcall FUN_004c7a20(int, int*, Surface_4c8760*, Surface_4c8760*);

// FUNCTION: 0x4c8760
void __stdcall FUN_004c8760(Surface_4c8760* target, Surface_4c8760* texture, int* vertices, int* coords)
{
    int defaults[8];
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
        lowY=999999; highY=-999999; highX=-999999; lowX=999999;
        for(int i=0;i<4;i++) {
            int y=vertices[i*3+1];
            if(y<lowY) { lowY=y; lowIndex=i; }
            if(y>highY) { highY=y; highIndex=i; }
            int x=vertices[i*3];
            if(x>highX) highX=x;
            if(x<lowX) lowX=x;
        }
        if(highX>=0 && lowX<=target->width-1 && highY>=0 && lowY<=target->height-1) {
            int bottom=target->height-1;
            if(lowY<0) lowY=0;
            if(highY>bottom) highY=bottom;
            if(highY!=lowY) {
                int x; int y1;
                {
                    int* out=&spans[0][0];
                    int index=lowIndex;
                    do {
                        int previous=index-1;
                        next=previous;
                        if (next<0) next=3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        y1=vertices[next*3+1];
                        if (y0<y1) {
                            int dy=y1-y0;
                            dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                            x=currentVertex[0]*0x10000+0xffff;
                            int z=currentVertex[2]*0x10000;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;

                            du=(coords[next*2]*0x10000-u)/dy;
                            dv=(coords[next*2+1]*0x10000-v)/dy;
                            int dz=(vertices[next*3+2]*0x10000-z)/dy;

                            if(y0<0) {
                                x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;

                                y0=0;
                            }
                            if(y1>bottom) y1=bottom;
                            if(y0<y1) {
                                int n=y1-y0;
                                do {
                                    out[0]=x>>16; x+=dx;
                                    out[2]=u;

                                    out[3]=v;

                                    out[6]=z;

                                    out+=10;
                                    u+=du; v+=dv;
                                    z+=dz;

                                } while(--n);
                            }
                        }
                        index=previous;
                        if(index<0) index=3;
                    } while(index!=highIndex);
                }
                {
                    int* out=&spans[0][0];
                    int index=lowIndex;
                    do {
                        next=(index+1)&3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        y1=vertices[next*3+1];
                        if (y0<y1) {
                            int dy=y1-y0;
                            dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                            x=currentVertex[0]*0x10000+0xffff;
                            int z=currentVertex[2]*0x10000;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;

                            du=(coords[next*2]*0x10000-u)/dy;
                            dv=(coords[next*2+1]*0x10000-v)/dy;
                            int dz=(vertices[next*3+2]*0x10000-z)/dy;

                            if(y0<0) {
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

                                    out+=10;
                                    u+=du; v+=dv;
                                    z+=dz;

                                } while(--n);
                            }
                        }
                        index=next;
                    } while(index!=highIndex);
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
