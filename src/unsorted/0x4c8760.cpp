// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by
// space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1. Names are provisional.
// PARTIAL, 85.3% (1094 original bytes, 1094 ours).
// The min/max scan and both span-edge loops emit the same instruction set as
// the original; every remaining diff is slot coloring and instruction
// scheduling.
// What got it here: the second span loop writes its u/v through offsets 4 and
// 5 (out[1]=x, out[4]=u, out[5]=v, out[7]=z), not 2/3 (67.9 -> 72.5), and
// promoting the loop-body `int dv` to function scope supplies the missing
// 14th scalar slot, which fixes the frame (0x7d54 -> 0x7d58, so every
// parameter and spans offset now lines up) (72.8 -> 85.3).
// Measured final layout, ours vs original:
//   x=0x18, y1=0x1c, highY=0x20, lowIndex=0x28, out=0x30, dx=0x34,
//   bottom=0x3c, highIndex=0x40, previous=0x44, loop2 n=0x28 all match.
// What still differs:
// 1. lowX and next are swapped: ours lowX=0x10, next=0x14 (original next=0x10,
//    lowX=0x14 shared with loop1 n and loop2 index). Ours then shares 0x10
//    with nextVertex (orig 0x38) and loop1 n (orig 0x14).
// 2. dv/lowY/du form a rotation: ours dv=0x24, lowY=0x2c, du=0x38; the
//    original has lowY=0x24, du=0x2c, nextVertex=0x38 and lets dv share
//    next's 0x10.
// 3. Pixel-loop increment order: the original emits u+=du before v+=dv in
//    source order; ours emits v+=dv first (dv is loaded early into ecx).
// Tried and rejected: reordering the lowIndex/du/out/dx declarations produced
// byte-identical output, and moving the dv declaration to two different
// positions also changed nothing, so this function's coloring is not
// declaration-order driven. Promoting dz to function scope as well overshoots
// (74.2%, 1090 bytes).
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
