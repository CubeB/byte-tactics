// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by fledge-alpha-free, finished by claude-opus-5-5, finished by GPT-6, matched by claude-opus-5-5. Names are provisional.
// MATCH (#5391), from 94.2%. Two changes, both needed (each alone scores
// 94.2% or 88.6%):
//  1. Both walks read y1 as `vertices[next*4+1]` instead of `nextVertex[1]`.
//     That second use of `vertices + next*16` makes it a CSE, which each walk
//     head builds in two expression temporaries (eax, ecx). In the right walk
//     those two temporaries put the eax/ecx/edx rotation on the original's
//     phase, so du through the y0<0 clamp get edx/eax as in the original.
//     (The left walk's registers come out the same either way.)
//  2. `next` is one function-scope variable shared by both walks, not a
//     separate `int next` in each. With per-walk `next`, (1) forwards the right
//     walk's nextVertex into a CSE home of its own (3 references), nextVertex
//     drops from 6 references to 3 and six frame slots move (88.6%). With one
//     `next`, the address CSE is the same expression in both walks and keeps
//     the 6-reference home that pairs with dl in the frame, as in the
//     original. This is the same trick as the shared index in the matched
//     sibling 0x4c1000.
// Earlier leads, kept for reference: a `currentVertex` local in the right walk
// gets the rotation but changes the head (94.18%); `index=next` makes `next`
// a register candidate and moves vertices into esi (42%).
struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall DrawLitTexturedSpan(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall DrawLitTexturedPolygon(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
{
    int i, defaults[8];
    int spans[800][10];
    int y0;
    int dv, dx, du, dz, dl;
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
                int y=vertices[i*4+1];
                if(y<lowY) { lowY=y; lowIndex=i; }
                if(y>highY) { highY=y; highIndex=i; }
                int x=vertices[i*4];
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
                        int next;
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                next=index-1;
                                if (next<0) next=3;
                                y0=vertices[index*4+1];
                                nextVertex=vertices+next*4;
                                y1=vertices[next*4+1];
                                if (y1>0 && y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-vertices[index*4+0])*0x10000)/dy;
                                    x=vertices[index*4+0]*0x10000+0xffff;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    int z=vertices[index*4+2]*0x10000;
                                    int l=vertices[index*4+3]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    dz=(nextVertex[2]*0x10000-z)/dy;
                                    dl=(nextVertex[3]*0x10000-l)/dy;
                                    if(y0<0) {
                                        x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; l-=dl*y0;
                                        y0=0;
                                    }
                                    if(y1>bottom) y1=bottom;
                                    if(y0<y1) {
                                        int n=y1-y0;
                                        do {
                                            out[0]=x>>16; x+=dx; out[2]=u; u+=du; out[3]=v; v+=dv; out[6]=z; out[8]=l; z+=dz; out+=10; l+=dl;
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
                                next=(index+1)&3;
                                y0=vertices[index*4+1];
                                nextVertex=vertices+next*4;
                                y1=vertices[next*4+1];
                                if (y1>0 && y0<y1) {
                                    int dy = y1-y0;
                                    dx=((nextVertex[0]-vertices[index*4+0])*0x10000)/dy;
                                    x=vertices[index*4+0]*0x10000+0xffff;
                                    int u=coords[index*2]*0x10000;
                                    int v=coords[index*2+1]*0x10000;
                                    int z=vertices[index*4+2]*0x10000;
                                    int l=vertices[index*4+3]*0x10000;
                                    du=(coords[next*2]*0x10000-u)/dy;
                                    dv=(coords[next*2+1]*0x10000-v)/dy;
                                    dz=(nextVertex[2]*0x10000-z)/dy;
                                    dl=(nextVertex[3]*0x10000-l)/dy;
                                    if(y0<0) {
                                        x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; l-=dl*y0;
                                        y0=0;
                                    }
                                    if(y1>bottom) y1=bottom;
                                    if(y0<y1) {
                                        int n=y1-y0;
                                        do {
                                            out[1]=x>>16; x+=dx; out[4]=u; u+=du; out[5]=v; out[7]=z; v+=dv; out[9]=l; z+=dz; out+=10; l+=dl;
                                        } while(--n);
                                    }
                                }
                                index=(index+1)&3;
                            } while(index!=highIndex);
                        }
                        int* span=&spans[0][0];
                        for(int row=lowY;row<highY;row++) {
                            if(span[1]-span[0]>0)
                                DrawLitTexturedSpan(row,span,target,texture);
                            span+=10;
                        }
                    }
                }
            }
        }
}
