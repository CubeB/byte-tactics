// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-sonnet-5-5, finished by Space Bunny Free, finished by fledge-alpha-free, finished by claude-opus-5-5. Names are provisional.
// PARTIAL 94.2% (1279 of 1279 bytes). Rebuilt on the structure of the matched
// sibling 0x4c8760 (same function with a three-int vertex and no l channel):
// - Frame (0x7d60, all sixteen slots as in the original): one `nextVertex`,
//   `out`, `y1` and `x` declared once in the enclosing block and shared by both
//   edge walks, dx/du/dv/dz/dl at function scope, `y0` at function scope
//   before the slopes, and a separate `next` declared inside each walk.
// - `vertices` out of the callee-saved registers (the original loads it into
//   edx at the null check, walks the scan with that edx, and reloads it from
//   its argument slot twice per walk head): the current vertex is read as
//   vertices[index*4+k] (a CSE temp in ebp, `mov ecx,edi; shl ecx,4;
//   lea ebp,[ecx+esi]`), and y0 is read before nextVertex is computed. Reading
//   y0 after nextVertex raised the C2 priority of vertices' last piece from 13
//   to 83 (tools/c2prio.py) and kept vertices in a register through the scan,
//   which moved lowY, highX, lowX and the zero (65.7% against 83.5%).
// - The right walk ends with `index=(index+1)&3`; `index=next` makes `next` a
//   real candidate whose head piece takes edx and puts vertices back in esi.
// Still differs: in the right walk only, the temporaries from `du` to the
// y0<0 clamp have eax and edx swapped (ours hoists the reload of next into edx
// and loads coords into eax; the original hoists coords into edx, like the
// left walk). The left walk's `next` is a register candidate (its reload goes
// to its own piece, eax); the right walk's named `next` is a copy of the
// `(index+1)&3` CSE and C2 forwards it into memory (c2prio: "dropped before the
// sort"). Any extra use of next that makes C2 rebuild `vertices+next*16` as a
// CSE (y1=vertices[next*4+1], or dx/dz/dl through vertices[next*4+k] in the
// right walk) fixes every register in the function, but then the right walk's
// nextVertex web is forwarded too, nextVertex no longer pairs with dl in the
// frame and the slots reshuffle (88.6%, permuter score 225 against 940 here).
// Neutral here: helpers or operand swaps for coords[next*2], unsigned or %
// spellings of the wrap, code-free temporaries before du, per-walk or
// function-scope next/out/nextVertex/y1/x, the clamp and slope operand orders.
// The permuter (15 min) only reordered the right walk's clamp to compensate.
// Lead: the original's right-walk head (`lea esi,[edi+1]; and esi,3;
// mov eax,esi`) mirrors the left walk's (`lea edx,[edi-1]; mov eax,edx`), so it
// probably holds the wrap CSE in esi and a separate `next` candidate in eax.
// `index=next` (or `index=next&3`) gets that candidate and the right walk's du
// to dl right, but its head piece (C2 priority 280-350) is coloured before
// y0's head piece (200), takes edx, and the prologue falls back to vertices in
// esi (42%). The missing piece is whatever lowers that head piece's priority.
// GPT-6 retry (#5175): moving the right-walk `nextVertex` assignment before
// the y0 load scored 66.8%; splitting `next` declaration from its assignment
// after the y0 load scored 84.3%. The 94.2% source remains best.
// GPT-6 retry (#5217): rechecked the 94.2% source and retried `index=next`;
// that still gives the right-walk candidate priority too early and drops to
// 42.0%, with wide register and stack changes. Restored the 94.2% version.

struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall FUN_004c8020(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall FUN_004c8bb0(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
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
                        {
                            out=&spans[0][0];
                            int index = lowIndex;
                            do {
                                int next=index-1;
                                if (next<0) next=3;
                                y0=vertices[index*4+1];
                                nextVertex=vertices+next*4;
                                y1=nextVertex[1];
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
                                int next=(index+1)&3;
                                y0=vertices[index*4+1];
                                nextVertex=vertices+next*4;
                                y1=nextVertex[1];
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
                                FUN_004c8020(row,span,target,texture);
                            span+=10;
                        }
                    }
                }
            }
        }
}
