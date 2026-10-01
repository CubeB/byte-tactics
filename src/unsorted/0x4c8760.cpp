// Decompiled by GPT-6, finished by space-bunny-free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by
// claude-sonnet-5-5, finished by Space Bunny Free. Names are provisional.
// PARTIAL (was 87.9%). tools/permute.py found the statement shapes below
// (hoisting the scan counter `i`, `while(i<4)` instead of the for, the nested
// `if` for the last two raster guards, and the reordered inner-loop bodies),
// which took 87.9 to 90.5 at the exact 1094 byte count. What still differs is
// listed at the bottom.

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
                                        x-=y0*dx; u-=y0*du; v-=y0*dv; z-=y0*dz;

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