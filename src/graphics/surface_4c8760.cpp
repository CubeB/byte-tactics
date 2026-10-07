// Decompiled by GPT-6, finished by space-bunny-free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by
// claude-sonnet-5-5, finished by Space Bunny Free, finished by claude-opus-5-5.
// Names are provisional.

struct Surface_4c8760 { unsigned short width, height; };
void __stdcall DrawTexturedSpan(int, int*, Surface_4c8760*, Surface_4c8760*);

// FUNCTION: 0x4c8760
void __stdcall DrawTexturedPolygon(Surface_4c8760* target, Surface_4c8760* texture, int* vertices, int* coords)
{
    int i, defaults[8];
    int spans[800][10];
    // Declared before the slopes: dx*y0 then loads dx first.
    int y0;
    int dv, dx, du;
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
                        int* out;
                        int* nextVertex;
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                // Declared per walk body, not shared: sets the frame slot packing.
                                int next=index-1;
                                if (next<0) next=3;
                                int* currentVertex=vertices+index*3;
                                y0=currentVertex[1];
                                nextVertex=vertices+next*3;
                                y1=nextVertex[1];
                                if (y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
                                    x=currentVertex[0]*0x10000+0xffff;
                                    int z=currentVertex[2]*0x10000;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    int dz = (nextVertex[2]*0x10000-z)/dy;
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
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                int next=(index+1)&3;
                                int* currentVertex=vertices+index*3;
                                y0=currentVertex[1];
                                nextVertex=vertices+next*3;
                                y1=nextVertex[1];
                                if (y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
                                    x=currentVertex[0]*0x10000+0xffff;
                                    int z=currentVertex[2]*0x10000;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    int dz = (nextVertex[2]*0x10000-z)/dy;
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
                                // Recomputed, not index=next: a copy changes the compares and loads.
                                index=(index+1)&3;
                            } while(index!=highIndex);
                        }
                        int* span=&spans[0][0];
                        for(int row=lowY;row<highY;row++) {
                            if(span[1]-span[0]>0)
                                DrawTexturedSpan(row,span,target,texture);
                            span+=10;
                        }
                    }
                }
            }
        }
}
