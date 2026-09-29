// Decompiled by GPT-6, finished by deepseek-v4.1-flash. Names are provisional.
// Partial 51.5%. Still differs:
//  - frame is 0x7d5c vs original 0x7d60: the original has one extra live int in
//    the low temp region (0x10..0x4f vs our 0x10..0x4b), so every param/default
//    offset is 4 lower. The original keeps highX in edi; we spill it to [esp+0x10].
//    The original also uses [esp+0x4c] for `index-1` in the first edge loop.
//  - the null checks use edx as the zero register; original uses ebp (xor ebp,ebp
//    right after push ebp), which shifts the whole prologue.
//  - min/max loop and both edge loops still have register/instruction-order diffs.
//  - highX register/order and the min/max init store order differ.
// Fixes that did land: second edge loop writes ints 1,4,5,7,9 of the span row
// (the row is [xL,xR,uL,vL,uR,vR,zL,zR,lL,lR]), the rasterise guard is
// span[1]-span[0]>0 (not !=0 && >=0), and min/max locals are declared at point
// of use after the default-coords block.
struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall FUN_004c8020(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall FUN_004c8bb0(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
{
    int defaults[8];
    int spans[800][10];
    if (target && texture && vertices) {
        if (!coords) {
            coords=defaults;
            defaults[0]=0; defaults[1]=0;
            defaults[2]=texture->width-1; defaults[3]=0;
            defaults[4]=texture->width-1; defaults[5]=texture->height-1;
            defaults[6]=0; defaults[7]=texture->height-1;
        }
        int lowY=999999, highY=-999999, highX=-999999, lowX=999999;
        int lowIndex, highIndex;
        for(int i=0;i<4;i++) {
            int y=vertices[i*4+1];
            if(y<lowY) { lowY=y; lowIndex=i; }
            if(y>highY) { highY=y; highIndex=i; }
            int x=vertices[i*4];
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
                        int next=(index-1 < 0 ? 3 : index-1);
                        int* nextVertex=vertices+next*4;
                        int y0=vertices[index*4+1];
                        int y1=nextVertex[1];
                        if (y1>0 && y0<y1) {
                            int dy=y1-y0;
                            int dx=((nextVertex[0]-vertices[index*4])*0x10000)/dy;
                            int x=vertices[index*4]*0x10000+0xffff;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;
                            int z=vertices[index*4+2]*0x10000;
                            int light=vertices[index*4+3]*0x10000;
                            int du=(coords[next*2]*0x10000-u)/dy;
                            int dv=(coords[next*2+1]*0x10000-v)/dy;
                            int dz=(nextVertex[2]*0x10000-z)/dy;
                            int dl=(nextVertex[3]*0x10000-light)/dy;
                            if(y0<0) {
                                x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;
                                light-=dl*y0;
                                y0=0;
                            }
                            if(y1>bottom) y1=bottom;
                            if(y0<y1) {
                                int n=y1-y0;
                                do {
                                    out[0]=x>>16; x+=dx;
                                    out[2]=u;
                                    u+=du;
                                    out[3]=v;
                                    v+=dv;
                                    out[6]=z;
                                    out[8]=light;
                                    out+=10;

                                    z+=dz;
                                    light+=dl;
                                } while(--n);
                            }
                        }
                        index=next;
                    } while(index!=highIndex);
                }
                {
                    int* out=&spans[0][0];
                    int index=lowIndex;
                    do {
                        int next=(index+1)&3;
                        int* nextVertex=vertices+next*4;
                        int y0=vertices[index*4+1];
                        int y1=nextVertex[1];
                        if (y1>0 && y0<y1) {
                            int dy=y1-y0;
                            int dx=((nextVertex[0]-vertices[index*4])*0x10000)/dy;
                            int x=vertices[index*4]*0x10000+0xffff;
                            int u=coords[index*2]*0x10000;
                            int v=coords[index*2+1]*0x10000;
                            int z=vertices[index*4+2]*0x10000;
                            int light=vertices[index*4+3]*0x10000;
                            int du=(coords[next*2]*0x10000-u)/dy;
                            int dv=(coords[next*2+1]*0x10000-v)/dy;
                            int dz=(nextVertex[2]*0x10000-z)/dy;
                            int dl=(nextVertex[3]*0x10000-light)/dy;
                            if(y0<0) {
                                x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0;
                                light-=dl*y0;
                                y0=0;
                            }
                            if(y1>bottom) y1=bottom;
                            if(y0<y1) {
                                int n=y1-y0;
                                do {
                                    out[1]=x>>16; x+=dx;
                                    out[4]=u;
                                    u+=du;
                                    out[5]=v;
                                    v+=dv;
                                    out[7]=z;
                                    out[9]=light;
                                    out+=10;

                                    z+=dz;
                                    light+=dl;
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
