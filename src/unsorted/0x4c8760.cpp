// Decompiled by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, 65.3% (1094 original bytes, 1093 ours). Timebox hit.
// The two span-edge loops and the final dispatch loop now match except for
// the stack frame. Remaining difference: the alloca constant is 0x7d54, the
// original is 0x7d58, so every esp offset below the clip rect and the two arg
// offsets are 4 too small (defaults base 0x44 vs 0x48, spans base 0x64 vs
// 0x68). The cause is slot allocation, not just size: our lowIndex lands at
// [esp+0x30] where the original keeps it at [esp+0x28], while minY is at the
// same [esp+0x24] in both, so one 4-byte local is missing from our small
// locals region and another variable is packed differently.
// Two fixes that gained ground: writing the dispatch test as
// `span[1]-span[0] > 0` (sub+test, not cmp) and, before that, simplifying it
// to a single comparison. The next thing to try: match the declaration order
// of lowIndex/highIndex against the du/dv temporaries so the small locals
// region grows by the missing dword.
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
                {
                    int* out=&spans[0][0];
                    int index=lowIndex;
                    do {
                        int previous=index-1;
                        int next=previous;
                        if (next<0) next=3;
                        int* currentVertex=vertices+index*3;
                        int y0=currentVertex[1];
                        int y1=vertices[next*3+1];
                        if (y0<y1) {
                            int dy=y1-y0;
                            int dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                            int x=currentVertex[0]*0x10000+0xffff;
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
                        int y1=vertices[next*3+1];
                        if (y0<y1) {
                            int dy=y1-y0;
                            int dx=((vertices[next*3]-currentVertex[0])*0x10000)/dy;
                            int x=currentVertex[0]*0x10000+0xffff;
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
