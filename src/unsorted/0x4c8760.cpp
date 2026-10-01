// Decompiled by GPT-6, finished by space-bunny-free, finished by GPT-6.1-sol,
// finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// PARTIAL, 87.9% (1094 original bytes, 1094 ours). Same instruction set as the
// original, every remaining diff is a stack slot assignment (12 hunks).
// What still differs: the original's 14 frame slots are grouped
//   0x10 {next (loop 1), dv}   0x14 {lowX, n (loop 1), next (loop 2)}   0x38 {vertices+next*3 temp}
// while ours has
//   0x10 {lowX, n (loop 1), the temp}   0x14 {next, both loops}   0x38 {dv}
// and the fix-up multiplies then load dx/du/dv first (`mov ecx,[dx]; imul ecx,ebp`)
// where ours folds the memory operand (`mov ecx,ebp; imul ecx,[dx]`).
// Last pass (claude-sonnet-5-5): `index--; if(index<0) index=3;` at the bottom of
// loop 1 with `next=index-1;` at the head (instead of a named `previous`) took
// 87.3 to 87.9 (the raw index-1 is still a spilled CSE temp at 0x44, as in the
// original). Measured and NOT the lever (all <= 87.9, free --sym):
//  - every single and pair change of: next/dv/dx/du/x/y1/out/y0/dz scope
//    (function / outer block / per-loop block), do-while versus for(yy) inner
//    loop, named currentVertex versus inline, min/max locals inside the if,
//    declaration order, `bottom` as a variable or expression (about 200 files);
//    500 random per-variable-per-loop scope assignments (best 86.1);
//  - splitting `next` into two variables (function or block scope) gives the
//    {next1,dv} sharing of the original but collapses to a 13 slot frame (0x7d54)
//    and 67.9 to 73: y1 then shares with n and out of loop 1 shares with lowX,
//    which the original does not do;
//  - loop 2 without any `next` variable (`((index+1)&3)` written out): 68.5;
//  - each edge loop as a static inline helper (both, or one of them): 68.8;
//  - tools/permute.py (two 20 minute runs): 87.9 at best, same hunks.
// In the original the slot groups come out in roughly descending static
// reference count (11,10,8,6,6,5,6,6,6,6,4,3,3,2 stack references from 0x10 up);
// with `index=next` at the bottom of loop 1 (no raw index-1 temp) `next` alone
// takes 0x10 and {lowX,n} 0x14, so the grouping of next1/dv versus
// lowX/n1/next2 is what decides the frame, and no source spelling tried gets it.

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
                        next=index-1;
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
                        index--;
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