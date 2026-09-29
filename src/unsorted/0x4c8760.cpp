// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by
// space-bunny-free. Names are provisional.
// PARTIAL, 67.9% (1094 original bytes, 1093 ours).
// The three raster blocks (min/max scan, both span-edge loops, the dispatch
// loop) are instruction-for-instruction identical to the original; every
// remaining diff is the stack frame layout. Measured frame rule for MSVC 5
// here: alloca = (0x10 gap + scalar locals + defaults) + array size - 0x10, so
// the original's 0x7d58 needs 0x58 of non-array bytes where we emit 0x54, i.e.
// exactly ONE extra 4-byte scalar slot (the original has 14 scalar dwords at
// [esp+0x10]..[esp+0x44]; we emit 13 at [esp+0x10]..[esp+0x40]). The extra one
// is visible as `x` having its own slot: the original keeps span x at 0x18 and
// the min/max lowX at 0x14, while we fold x onto lowX's slot. Everything else
// (defaults at 0x48, spans at 0x68, the two arg slots and the saved-register
// gap) is then 4 too low, which is why every esp offset in the diff is -4.
// Gained here: declaring the span-loop `x` and `y1` once in the
// `if (highY != lowY)` block instead of once per loop (65.3 -> 67.9).
// Next thing to try: find the construct that stops the allocator folding x onto
// lowX (a named local live across the min/max loop looks most likely), then
// reorder the tail so previous sits at 0x44 and nextVertex at 0x38.
struct Surface_4c8760 { unsigned short width, height; };
void __stdcall FUN_004c7a20(int, int*, Surface_4c8760*, Surface_4c8760*);

// FUNCTION: 0x4c8760
void __stdcall FUN_004c8760(Surface_4c8760* target, Surface_4c8760* texture, int* vertices, int* coords)
{
    int defaults[8];
    int spans[800][10];
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
                        int next=previous;
                        if (next<0) next=3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        y1=vertices[next*3+1];
                        if (y0<y1) {
                            int dy=y1-y0;
                            int dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                            x=currentVertex[0]*0x10000+0xffff;
                            int z=currentVertex[2]*0x10000;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;

                            int du=(coords[next*2]*0x10000-u)/dy;
                            int dv=(coords[next*2+1]*0x10000-v)/dy;
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
                    int* out=&spans[0][1];
                    int index=lowIndex;
                    do {
                        int next=(index+1)&3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        y1=vertices[next*3+1];
                        if (y0<y1) {
                            int dy=y1-y0;
                            int dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                            x=currentVertex[0]*0x10000+0xffff;
                            int z=currentVertex[2]*0x10000;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;

                            int du=(coords[next*2]*0x10000-u)/dy;
                            int dv=(coords[next*2+1]*0x10000-v)/dy;
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
