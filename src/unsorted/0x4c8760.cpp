// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by
// space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1,
// finished by space-bunny-free. Names are provisional.
// PARTIAL, 85.5% (1094 original bytes, 1094 ours), confirmed by check.py.
// The min/max scan, both span-edge loops and the final row loop all emit the
// original's instruction set. Every remaining diff is frame-slot coloring plus
// a little scheduling inside it.
// WHAT GOT IT HERE: the second span loop writes its u/v through offsets 4 and
// 5 (out[1]=x, out[4]=u, out[5]=v, out[7]=z), not 2/3 (67.9 -> 72.5);
// promoting the loop-body `int dv` to function scope supplies the missing 14th
// scalar slot, which fixes the frame (0x7d54 -> 0x7d58, so every parameter and
// spans offset now lines up) (72.8 -> 85.3); and moving `out+=10` in BOTH
// pixel loops to the very end, after the u/v/z accumulator adds rather than
// before them, is worth the last 0.2 points and makes loop 1's increment
// schedule match the original (85.3 -> 85.5).
//
// THE WHOLE REMAINING DIFF IS ONE SWAP. Measured scalar slots, ours vs
// original (0x10 is the first scalar slot; 0x00-0x0c is the outgoing-arg area):
//   slot   original        ours
//   0x10   next, dv        lowX, nextVertex, loop1 n
//   0x14   lowX, loop1 n,  next, loop2 next
//          loop2 next
//   0x18   x               x          (match)
//   0x1c   y1              y1         (match)
//   0x20   highY           highY      (match)
//   0x24   lowY            dv
//   0x28   lowIndex, loop2 n  lowIndex (match)
//   0x2c   du              lowY
//   0x30   out             out        (match)
//   0x34   dx              dx         (match)
//   0x38   nextVertex      du
//   0x3c   bottom          bottom     (match)
//   0x40   highIndex       highIndex  (match)
//   0x44   previous        previous   (match)
// lowX and next occupy 0x10 and 0x14 in the OPPOSITE order, and the other five
// mismatches are all knock-on effects of that one swap through MSVC's dead-slot
// reuse: with `next` in 0x10, the reuse when `dv` is born picks 0x10 and
// `nextVertex` is given a fresh 0x38; with `next` in 0x14, `nextVertex` reuses
// lowX's dead 0x10 and `dv` takes lowY's dead 0x24. So the whole problem is
// "which of lowX and next gets the lower slot", nothing else. Slots 0x18, 0x1c,
// 0x20, 0x28, 0x30, 0x34, 0x3c, 0x40 and 0x44 already agree, so the frame
// layout pass itself is fine; only the first two slots are permuted.
//
// WHAT DOES NOT MOVE THE COLORING (all byte-identical, verified by scoring):
//   * any permutation of the function-scope declaration order, including the
//     order implied by the original's own slot map (next, lowX, x, y1, highY,
//     lowY, lowIndex, du, out, dx, nextVertex, bottom, highIndex, previous);
//   * block vs function scope for nextVertex, du, dv, y0, dy, currentVertex,
//     u, v, z, lowX, next;
//   * the clamp written with an explicit `t` temp (`t=dx; t*=y0; x-=t;`), with
//     one temp or four;
//   * flipping the multiply operand order in the clamp (`x-=y0*dx`);
//   * reordering the clamp statements to x, v, u, z.
// So this function's coloring is not declaration-order driven and not
// scope-driven. Only a change in the NUMBER of memory-resident variables moves
// it, and every such change tried also changed the frame size: making `next`
// block-scoped per loop gives 0x7d54 (73.4%), and collapsing loop 2's index
// and next into one variable gives 1056 bytes (43.9%). Promoting dz to function
// scope as well overshoots (74.2%, 1090 bytes).
//
// TWO REMAINING SHAPE DIFFS, both believed to be symptoms of the swap above:
// 1. The y0<0 clamp. The original materialises the memory operand
//    (`mov ecx,[dx] / imul ecx,ebp`); ours folds it and materialises the
//    register operand (`mov ecx,ebp / imul ecx,[dx]`). MSVC 5 folds in every
//    spelling tried, so this is not an independent lever.
// 2. The loop latches. Original loop 1: `mov edx,[previous] / mov edi,edx /
//    test edi,edi / jge` (it tests the COPY); ours tests `edx`, the source.
//    Writing it as a ternary `index = previous<0 ? 3 : previous;` gives
//    84.7%, so that is not it either. Original loop 2: loads next and highIndex,
//    copies next to edi, then `cmp edi,eax`; ours loads highIndex into ecx and
//    compares eax (next) against it.
struct Surface_4c8760 { unsigned short width, height; };
void __stdcall FUN_004c7a20(int, int*, Surface_4c8760*, Surface_4c8760*);

// FUNCTION: 0x4c8760
void __stdcall FUN_004c8760(Surface_4c8760* target, Surface_4c8760* texture, int* vertices, int* coords)
{
    int defaults[8];
    int spans[800][10];
    int next;
    int lowX;
    int x;
    int y1;
    int highY;
    int lowY;
    int dx;
    int* out;
    int du;
    int lowIndex;
    int* nextVertex;
    int bottom;
    int highIndex;
    int previous;
    int highX;
    int dv;
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
            int vy=vertices[i*3+1];
            if(vy<lowY) { lowY=vy; lowIndex=i; }
            if(vy>highY) { highY=vy; highIndex=i; }
            int vx=vertices[i*3];
            if(vx>highX) highX=vx;
            if(vx<lowX) lowX=vx;
        }
        if(highX>=0 && lowX<=target->width-1 && highY>=0 && lowY<=target->height-1) {
            bottom=target->height-1;
            if(lowY<0) lowY=0;
            if(highY>bottom) highY=bottom;
            if(highY!=lowY) {
                {
                    out=&spans[0][0];
                    int index=lowIndex;
                    do {
                        previous=index-1;
                        next=previous;
                        if (next<0) next=3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        y1=vertices[next*3+1];
                        nextVertex=vertices+next*3;
                        if (y0<y1) {
                            int dy=y1-y0;
                            dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
                            x=currentVertex[0]*0x10000+0xffff;
                            int z=currentVertex[2]*0x10000;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;

                            du=(coords[next*2]*0x10000-u)/dy;
                            dv=(coords[next*2+1]*0x10000-v)/dy;
                            int dz=(nextVertex[2]*0x10000-z)/dy;

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

                                    u+=du; v+=dv;
                                    z+=dz;

                                    out+=10;
                                } while(--n);
                            }
                        }
                        index=previous;
                        if(index<0) index=3;
                    } while(index!=highIndex);
                }
                {
                    out=&spans[0][0];
                    int index=lowIndex;
                    do {
                        next=(index+1)&3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        y1=vertices[next*3+1];
                        nextVertex=vertices+next*3;
                        if (y0<y1) {
                            int dy=y1-y0;
                            dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
                            x=currentVertex[0]*0x10000+0xffff;
                            int z=currentVertex[2]*0x10000;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;

                            du=(coords[next*2]*0x10000-u)/dy;
                            dv=(coords[next*2+1]*0x10000-v)/dy;
                            int dz=(nextVertex[2]*0x10000-z)/dy;

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

                                    u+=du; v+=dv;
                                    z+=dz;

                                    out+=10;
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
