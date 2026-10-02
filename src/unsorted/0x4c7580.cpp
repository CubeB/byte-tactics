// Decompiled by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by GPT-6, finished by deepseek-v4.1-flash, finished by Space Bunny Free. Names are provisional.
// Session claude-sonnet-5-5 (issue 4140 retry, no code change, still 66.9%). Findings:
// (1) The big hunk is check 2 (xmin > clip[2]): MSVC post-RA cross-jumps identical return tails, and
// whenever two tails get the same `lea` register (here check 2 and check 5, both edx) it turns the
// earlier one into `jg <later tail>`. A 20-line synthetic with five `if (c) { if (locked) U(&l); return; }`
// reproduces exactly this. The original keeps check 2 inline although byte-identical to check 5; forcing
// it inline with a dummy store scores 68.7 (not acceptable, so not kept). `||` chain 53.4, else-if chain
// and `Done(locked,&local)` helper identical, goto form for checks 1/4 65.4.
// (2) Root of most register diffs: the temps (clip1, ymax, clip3) get (eax,edx,ecx) in ours but
// (edx,ecx,eax) in the original, so ours loses clip1 at the loop entry (`mov eax,[imin]` clobbers it)
// while the original carries it in edx and only restores it after the idiv block. Operand swaps in the
// checks, local copies of clip[1]/clip[3], T2 inline (dummy), and declaration order did not move it.
// (3) y1 .10 / xmin|j .14 slot swap: unaffected by declaration order or per-walk y1 copies (frame
// grows); `for(; y0<y1; y0++)` compiles identically to the do-while; y0/y1 declared inside the walks
// 59.1; a struct {imin,imax,clip[4]} 59.3. tools/permute.py 15 minutes (241 candidates): no gain.
// PARTIAL 64.0%, 1144 of 1183 bytes. Rewritten in the 0x4c1000 style with the edge-walk
// temporaries (y0/y1/x/dxdy/source steps) shared across both walks and the second walk using
// (i+1)&3 (that one change was 51.7 -> 64.0). Frame and call census now match; the 39-byte gap
// is MSVC cross-jumping: the original keeps 3 of the 5 unlock+return tails inline (lea eax/edx)
// and shares only 2, ours merges 4 into one. Remaining diffs are stack-slot order (clip at
// 0x34 vs 0x3c, imin/imax at 0x44/0x48 vs 0x34/0x38), x/out swapped (0x24/0x28), xmin/y1
// swapped (0x10/0x14) and the y1>clip.top reload in the loops. Note: the earlier published
// 21.5% file with a 46-line header was replaced by a later 13.5% GPT-6 pass before this one.
// Next pass (deepseek-v4.1-flash, stopped by timebox before trying variants): the tails of the
// five early-outs and the final unlock are the main gap. In the original, checks 1 (xmax<left)
// and 4 (ymin>bottom) compile to `mov eax,[esp+0x18]; test eax,eax; je 0x4c7a12; jmp 0x4c7a08`
// (test the locked flag, then jump INTO the final unlock body, which is `lea ecx,[esp+0x6c];
// push ecx; call FUN_004c5fa0` at 0x4c7a08), while checks 2, 3 and 5 keep a full inline
// unlock+epilogue (lea edx / lea eax / lea edx). That is the shape of jump threading through
// `if (locked) { unlock_label: FUN_004c5fa0(&local); }` reached by `goto unlock_label` from
// checks 1 and 4 only, with checks 2, 3 and 5 written as plain `if (locked) FUN_004c5fa0(&local);
// return;` (compare 0x4c63a0 notes: its branch 1 also jumps into the single unlock body).
// Also the FUN_004c5e70 check is `cmp eax, ebp` (ebp holds 0) in the original but `test eax,eax`
// for us: storing the result first (`int ok = FUN_004c5e70(&local); if (ok == 0) return;`) is
// what produced `cmp eax,ebx` in the matched near-copy 0x4b7f90, so try that spelling next.
// Remaining diffs besides the tails: slot assignment (original y1=0x10 xmin/j=0x14 out=0x24
// x=0x28 imin=0x34 imax=0x38 clip=0x3c; ours swaps the 0x10/0x14 and 0x24/0x28 pairs and puts
// clip at 0x34, imin/imax at 0x44/0x48) and the y1>clip.top reload in the edge loops.


// Session deepseek-v4.1-flash (4th, timebox): `int imin, imax; imin = imax = 0;` moved clip to 0x3c
// and imin/imax to 0x34/0x38, matching the original frame map: 64.3 -> 66.9 percent / 1152 bytes.
// Remaining 31-byte gap is still the cross-jump tails (ours 1152 vs original 1183): check 1 threads
// into the final unlock, checks 2/3/5 and the y1>clip.top reload paths differ in slot picks
// (xmin 0x10 vs 0x14, and the [esp+0x48] reload register). No source-only lever found for those.
// Session deepseek-v4.1-flash (3rd, timebox): re-ran check.py twice, no source change, still
// 64.3 percent / 1144 bytes. Remaining hunks are unchanged from the notes above: the five
// early-out tails (checks 1 and 4 should jump into the single final unlock body at 0x4c7a08,
// checks 2/3/5 keep an inline unlock), the slot assignment pairs (0x10/0x14 and 0x24/0x28)
// and the y1>clip.top reload in the edge loops. No variant attempted in this session.
// Session deepseek-v4.1-flash (2nd): the suggested `int ok = FUN_004c5e70(&local);` spelling
// (removing the function-scope `int ok;`) is byte-identical here at 64.3 percent / 1144 bytes,
// so storing the result does not produce the original `cmp eax, ebp` at the first check.
// Tried and reverted: reordering the stack locals (y0/y1 after xmax, out after x, clip before
// imin/imax) to fix the 0x10/0x14 and 0x24/0x28 slot swaps was not completed inside the
// timebox; the declaration block is unchanged from the 64.3 percent version.
// Session deepseek-v4.1-flash (3rd): the check-1/check-4 tails ARE reachable via a shared
// block: writing them as `if (!locked) return; goto unlock_call;` with the label placed at the
// FUN_004c5fa0 call inside the final `if (locked) { ... }` reproduces the original pair exactly
// (`test eax,eax / je <epilogue> / jmp <unlock_call>`, targets 0xa apart); it scores 63.1 percent
// / 1161 bytes though, because the original also keeps full inline epilogues for checks 2, 3 and
// 5 (2 more copies than we emit, the 22 remaining bytes), so the 64.3 percent form stays.
// Session deepseek-v4.1-flash (4th, timebox): swapping the `int clip[4];` and `int imin, imax;`
// declarations (clip first) is byte-identical at 64.3 percent / 1144 bytes, so that pair's slot
// order (ours clip 0x34, imin/imax 0x44/0x48 versus the original imin/imax 0x34/0x38, clip 0x3c)
// is not steered by declaration order. The kept file has clip declared first.
// Session Space Bunny Free: 66.9 -> 86.3 percent, 1152 -> 1182 of 1183 bytes. Four changes:
// (1) In each edge walk, write the top clip offset inlined in all three steps
// (`x += dxdy * (clip[1] - y0);`) instead of through an `int dd` temporary. That alone was
// 68.7 -> 81.8 percent: the temporary makes MSVC keep `clip[1]` live in a scratch register
// across the idiv block and reload it, which is the bulk of the walk-loop diff.
// (2) `j = k = i - 1;` in the first walk instead of `j = i - 1; k = j;` (68.7 percent). The
// two assignments give the compiler one register for the wrapped index; the separate form
// costs an extra `mov ecx, edx`.
// (3) Dropping `imin = imax = 0;` (both are written before any read inside the 4-point loop,
// so the zero stores are dead; keeping them costs two extra `mov [esp+0x38/0x34], ebp` and
// also moved clip out of 0x3c). 81.8 -> 83.7 percent, and the frame map now matches exactly.
// (4) Checks 1 and 4 as `goto unlock;` with the label inside the final
// `if (locked) { unlock: FUN_004c5fa0(&local); }`, and check 5 as a plain early-out spelled
// `if (locked == 1)`. The `== 1` is what keeps check 5's tail from being cross-jumped into
// the shared unlock body: the original keeps FOUR FUN_004c5fa0 call sites (checks 2, 3, 5 and
// the final one) and threads only checks 1 and 4, which is exactly this shape.
// Still open, 1182 vs 1183 bytes. Two hunks, both about which unlock tail check 5 keeps:
// (a) Ours emits `cmp dword ptr [esp+0x18], 1 / jne` for check 5 where the original has
// `mov eax,[esp+0x18] / test eax,eax / je`, so our tail is 1 byte shorter. Spelling check 5
// as plain `if (locked)` gives the right `mov/test/je` but MSVC then cross-jumps that whole
// tail into the shared unlock body and drops the fourth call site (86.3 -> 83.7 percent).
// (b) The first walk's entry allocates the loop index in eax where the original keeps ecx.
// Adding a dead `if (i < 0) i = 3;` at the top of that loop body moves the allocation and
// scores 89.2 percent / 1191 bytes, but it costs 8 real instructions the original does not
// have, so it is not kept here; it is the strongest lead for whoever picks this up, and the
// register rotation it papers over is the one the guide notes at 0x402da0 ("the loop has one
// temporary more or fewer earlier on"). A fast filter (compile only, count the
// FUN_004c5fa0 call sites, which the original has 4 of) over the 9^5 spellings of the five
// early-outs is what found (4); 1137 of 59049 keep 4 calls and scoring all of them tops out
// at this file.
#include <windows.h>

struct Point_004c7580 {
    int x;
    int y;
};

struct Quad_004c7580 {
    Point_004c7580 p[4];
};

struct Rec_004c7580 {
    int left;   // +0x00
    int right;  // +0x04
    int tx;     // +0x08
    int ty;     // +0x0c
    int trx;    // +0x10
    int try_;   // +0x14
    int pad[4];
};

struct Frame_004c7580 {
    unsigned short w;    // +0x00
    unsigned short h;    // +0x02
    char unknown_04[0xc];
    void* data;          // +0x10
};

struct Surface_004c5e70 {
    int data[12];
};

class Class_004c6ae0 {
public:
    char unknown_0[0x1c];
    int clip[4];             // +0x1c left, top, right, bottom

    void FUN_004c6ae0(int* out);
};

int __stdcall FUN_004c5e70(Surface_004c5e70* out);
int __stdcall FUN_004c5fa0(Surface_004c5e70* s);
void __stdcall FUN_004c7310(int y, int* rect, void* surf, void* info);

// FUNCTION: 0x4c7580
void __stdcall FUN_004c7580(void* surf, Frame_004c7580* bmp,
                            Quad_004c7580* dst, Quad_004c7580* src)
{
    if (bmp == 0)
        return;
    if (dst == 0)
        return;

    Surface_004c5e70 local;
    int locked;
    if (surf == 0) {
        int ok = FUN_004c5e70(&local);
        if (ok == 0)
            return;
        locked = 1;
        surf = &local;
    } else {
        locked = 0;
    }

    Quad_004c7580 tmp;
    if (src == 0) {
        src = &tmp;
        tmp.p[0].x = 0;
        tmp.p[0].y = 0;
        tmp.p[1].x = bmp->w - 1;
        tmp.p[1].y = 0;
        tmp.p[2].x = bmp->w - 1;
        tmp.p[2].y = bmp->h - 1;
        tmp.p[3].x = 0;
        tmp.p[3].y = bmp->h - 1;
    }

    int y0, y1;
    Rec_004c7580 recs[800];
    int ymin = 999999;
    int ymax = -999999;
    int xmin = 999999;
    int xmax = -999999;
    int clip[4];
    int imin, imax;
    int i;
    Rec_004c7580* out;
    int j, k;
    int dxdy;
    int x, tx, ty, dtx, dty;

    for (i = 0; i < 4; i++) {
        int y = dst->p[i].y;
        if (y < ymin) {
            ymin = y;
            imin = i;
        }
        if (y > ymax) {
            ymax = y;
            imax = i;
        }
        int xx = dst->p[i].x;
        if (xx > xmax)
            xmax = xx;
        if (xx < xmin)
            xmin = xx;
    }

    ((Class_004c6ae0*)surf)->FUN_004c6ae0(clip);
    if (xmax < clip[0]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (xmin > clip[2]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (ymax < clip[1]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (ymin > clip[3]) {
        if (locked) FUN_004c5fa0(&local);
        return;
    }
    if (ymin < clip[1])
        ymin = clip[1];
    if (ymax > clip[3])
        ymax = clip[3];
    if (ymax == ymin) {
        if (locked == 1)
            FUN_004c5fa0(&local);
        return;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = k = i - 1;
        if (k < 0)
            k = 3;
        y0 = dst->p[i].y;
        y1 = dst->p[k].y;
        if (y1 > clip[1] && y0 < y1) {
            int dy = y1 - y0;
            x = dst->p[i].x;
            dxdy = ((dst->p[k].x - x) << 16) / dy;
            x = (x << 16) + 0xffff;
            tx = src->p[i].x << 16;
            ty = src->p[i].y << 16;
            dtx = ((src->p[k].x << 16) - tx) / dy;
            dty = ((src->p[k].y << 16) - ty) / dy;
            if (y0 < clip[1]) {
                x += dxdy * (clip[1] - y0);
                tx += dtx * (clip[1] - y0);
                ty += dty * (clip[1] - y0);
                y0 = clip[1];
            }
            if (y1 > clip[3])
                y1 = clip[3];
            if (y0 < y1) {
                int n = y1 - y0;
                do {
                    out->left = x >> 16;
                    out->tx = tx;
                    out->ty = ty;
                    x += dxdy;
                    tx += dtx;
                    ty += dty;
                    out++;
                } while (--n);
            }
        }
        i = j;
        if (i < 0)
            i = 3;
        if (i == imax)
            break;
    }

    out = recs;
    i = imin;
    for (;;) {
        j = (i + 1) & 3;
        k = j;
        y0 = dst->p[i].y;
        y1 = dst->p[k].y;
        if (y1 > clip[1] && y0 < y1) {
            int dy = y1 - y0;
            x = dst->p[i].x;
            dxdy = ((dst->p[k].x - x) << 16) / dy;
            x = (x << 16) + 0xffff;
            tx = src->p[i].x << 16;
            ty = src->p[i].y << 16;
            dtx = ((src->p[k].x << 16) - tx) / dy;
            dty = ((src->p[k].y << 16) - ty) / dy;
            if (y0 < clip[1]) {
                x += dxdy * (clip[1] - y0);
                tx += dtx * (clip[1] - y0);
                ty += dty * (clip[1] - y0);
                y0 = clip[1];
            }
            if (y1 > clip[3])
                y1 = clip[3];
            if (y0 < y1) {
                int n = y1 - y0;
                do {
                    out->right = x >> 16;
                    out->trx = tx;
                    out->try_ = ty;
                    x += dxdy;
                    tx += dtx;
                    ty += dty;
                    out++;
                } while (--n);
            }
        }
        i = j;
        if (i == imax)
            break;
    }

    out = recs;
    for (i = ymin; i < ymax; i++) {
        if (out->right - out->left > 0)
            FUN_004c7310(i, (int*)out, surf, bmp);
        out++;
    }

    if (locked) {
unlock:
        FUN_004c5fa0(&local);
    }
}