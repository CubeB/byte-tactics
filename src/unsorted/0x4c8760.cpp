// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by
// space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 87.3% (1094 original bytes, 1094 ours). Frame now matches the
// original at 0x7d58 (14 scalar slots 0x10..0x44). The winning change was
// hoisting the shared edge-walk temporaries `next`, `dv`, `dx`, `du` to
// function scope ahead of the min/max locals, so lowX stopped folding onto
// next's slot and the scalar region grew the missing dword. Up from 72.5.
// Fixed earlier: the second span loop writes its u/v through offsets 4 and 5
// (out = &spans[0][0], out[1]=x, out[4]=u, out[5]=v, out[7]=z), and the
// rasterise guard is `span[1]-span[0] > 0`.
// What still differs (one allocator swap of the low temp region):
//   original: 0x10 next,dv | 0x14 lowX,n1 | 0x38 nextVertex
//   ours:     0x10 lowX,n1,nextVertex | 0x14 next | 0x38 dv
// (n1 = first loop counter). Ours gives lowX 0x10 and pushes next to 0x14;
// the original gives next/dv 0x10 and leaves 0x14 for lowX/n1. The
// `test eax,eax` vs `test edx,edx` at the first loop head and the second
// loop exit using ecx instead of eax for highIndex follow from that swap.
// Tried and flat at 87.3: 24 declaration-order permutations of the min/max
// locals, moving those locals inside the outer if, splitting lowX out,
// reordering next/dv/dx/du, hoisting z, hoisting x/y1, sharing n between the
// loops. Hoisting dz dropped to 76.2, an explicit nextVertex pointer to 67.4.
// Next: force lowX's live range to extend past the edge loops (or next's to
// start before the scan) so the allocator cannot pick 0x10 for lowX.
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
