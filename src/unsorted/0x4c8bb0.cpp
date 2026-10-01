// Decompiled by GPT-6, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-sonnet-5-5. Names are provisional.
// PARTIAL, 57.1% (1279 original bytes, 1279 ours; was 54.1% at 1275 bytes).
// What changed (claude-sonnet-5-5): the instruction count and byte count now equal
// the original. Two things did it: the min/max scan walks the vertices with a
// pointer (`p+=4`, which puts the null-check zero in ebp like the original), and
// the edge loops read `currentVertex[k]` / `nextVertex[k]` pointers with the
// `next=index-1; ... index--; if(index<0) index=3;` pair (the compiler still
// folds the raw index-1 into one of the 15 slots; the original keeps it as the
// 16th slot [esp+0x4c], so the frame is 0x7d5c versus 0x7d60). The rest is a
// handful of small statement-order changes found with tools/permute.py (guard
// order texture/target/vertices, `p` declared first, highY/lowY/lowX/highX init
// order, nextVertex before currentVertex in loop 1, `bottom` after the lowY clamp).
// What still differs (all of it is register/stack-slot allocation):
//  1. `vertices` is pinned in esi from the null check on, so the min/max scan
//     copies it (`mov edx,esi`) and highX is demoted to a stack slot (the
//     original keeps highX in edi and reloads vertices from [esp+0x7d7c] at each
//     loop head: `mov esi,[v]; ... lea ebp,[ecx+esi]; mov ecx,[v]; add eax,ecx`).
//     Experiment (not a legal fix): reading the heads through a global pointer
//     instead of `vertices` makes the prologue/min-max match and gives 64%
//     with the hoisted-slope shape below. Nothing legal tried makes `vertices`
//     memory-resident: register keyword, &vertices, early return, guard order,
//     pointer walker, function-scope currentVertex/nextVertex, helper functions.
//  2. Frame 0x7d5c versus 0x7d60. Hoisting dx/du/dv to function scope (as in
//     0x4c8760) gives the original frame 0x7d60, defaults 0x50, spans 0x70, and the
//     tail slots dv 0x38, du 0x3c, dx 0x40, highIndex 0x48 land where the original
//     has them, but the head slots (original: 0x10 {nv,dl}, 0x14 {next1,dz},
//     0x18 {lowX,n1,next2}, 0x1c y1, 0x20 y0, 0x24 x) do not, and the score drops
//     to about 40-45 because of that. The same next-split / grouping problem as
//     0x4c8760 (see its notes) decides it.
//  3. In the original u,v,z,lg sit in esi,edi,ebx,ebp with coords cached in ebx
//     (restored after the inner loop); previous stays in edx across the
//     skipped-edge path.

struct Surface_4c8bb0 { unsigned short width, height; };
void __stdcall FUN_004c8020(int, int*, Surface_4c8bb0*, Surface_4c8bb0*);

// FUNCTION: 0x4c8bb0
void __stdcall FUN_004c8bb0(Surface_4c8bb0* target, Surface_4c8bb0* texture, int* vertices, int* coords)
{
    int y0;
    int defaults[8];
    int spans[800][10];
    int next;
    int lowIndex;
    int highIndex;
    int dz;
    int dl;
    int lowY;
    int highY;
    int highX;
    int lowX;
    if (texture && target && vertices) {
        int* p=vertices;
        if (!coords) {
            coords=defaults;
            defaults[0]=0; defaults[1]=0;
            defaults[2]=texture->width-1; defaults[3]=0;
            defaults[4]=texture->width-1; defaults[5]=texture->height-1;
            defaults[6]=0; defaults[7]=texture->height-1;
        } highY=-999999; lowY=999999; lowX=999999;
        highX=-999999;
        for(int i=0;i<4;i++) {
            int y=p[1];
            if(y<lowY) { lowY=y; lowIndex=i; }
            int x=p[0];
            if(y>highY) { highY=y; highIndex=i; }
            if(x>highX) highX=x;
            if(x<lowX) lowX=x;
            p+=4;
        }
        if (highX>=0 && lowX<=target->width-1 && highY>=0 && lowY<=target->height-1) {
                    if(lowY<0) lowY=0;
                    int bottom=target->height-1;
                    if(highY>bottom) highY=bottom;
                    if(lowY!=highY) {
                        int y1; int x;
        {
        int* out;
        int index=lowIndex;
        out=&spans[0][0];
        do {
        next=index-1;
        if (next<0) next=3;
        int* nextVertex=vertices+next*4;
        int* currentVertex=vertices+index*4;
        y0=currentVertex[1];
        y1=nextVertex[1];
        if (y1>0 && y0<y1) {
            int dy=y1-y0;
            int dx = ((nextVertex[0]-currentVertex[0])*0x10000)/dy;
            x=currentVertex[0]*0x10000+0xffff;
            int u=coords[index*2]*0x10000;
            int v=coords[index*2+1]*0x10000;
            int z;
            z = currentVertex[2]*0x10000;
            int lg=currentVertex[3]*0x10000;
            int du=(coords[next*2]*0x10000-u)/dy;
            int dv=(coords[next*2+1]*0x10000-v)/dy;
            dz=(nextVertex[2]*0x10000-z)/dy;
            dl=(0x10000*nextVertex[3]-lg)/dy;
            if(y0<0) { x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; lg-=y0*dl; y0=0; }
            if(y1>bottom) y1=bottom;
            if(y0<y1) {
            int n = y1-y0;
            do {
            out[0]=x>>16; x+=dx; out[2]=u; u+=du; out[3]=v; v+=dv; out[6]=z; out[8]=lg; z+=dz; out+=10; lg+=dl;
            } while(--n);
            }
            }
        index--; if(index<0) index=3;
        } while(index!=highIndex);
        }
        {
        int* out;
        int index=lowIndex;
        out=&spans[0][0];
        do {
        next=(index+1)&3;
        int* currentVertex=vertices+index*4;
        y0=currentVertex[1];
        int* nextVertex=vertices+next*4;
        y1=nextVertex[1];
        if (y1>0 && y0<y1) {
            int dy=y1-y0;
            int dx=((nextVertex[0]-currentVertex[0])*0x10000)/dy;
            x=currentVertex[0]*0x10000+0xffff;
            int u=coords[index*2]*0x10000;
            int v=coords[index*2+1]*0x10000;
            int z;
            z = currentVertex[2]*0x10000;
            int lg=currentVertex[3]*0x10000;
            int du=(coords[next*2]*0x10000-u)/dy;
            int dv=(coords[next*2+1]*0x10000-v)/dy;
            dz=(nextVertex[2]*0x10000-z)/dy;
            dl=(nextVertex[3]*0x10000-lg)/dy;
            if(y0<0) { x-=dx*y0; u-=du*y0; v-=dv*y0; z-=dz*y0; lg-=dl*y0; y0=0; }
            if(y1>bottom) y1=bottom;
            if(y0<y1) {
            int n = y1-y0;
            do {
            out[1]=x>>16; x+=dx; out[4]=u; u+=du; out[5]=v; out[7]=z; v+=dv; out[9]=lg; z+=dz; out+=10; lg+=dl;
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