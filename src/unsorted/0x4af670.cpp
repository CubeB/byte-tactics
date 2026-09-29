// Decompiled by space-bunny-free, finished by muse-spark-1.3-free. Names are provisional.
// Click handler of the file requester (FILEREQ.GUI, opened by 0x4afa30).
// On close (field_60 == -1) it restores the saved drive and directory and
// frees the request data. Otherwise it acts on the entry the user clicked:
// LOAD/SWIN enter a directory, CANC accepts, NAME takes the highlighted file,
// PATH walks one level up and the *DRV entries pick a drive letter.
//
// 99.1% (check.py), and the total size is now exact: 945 bytes, same as the
// original. Only three instructions in the PATH branch still differ, and they
// are all the same defect: MSVC 5 puts the loop counter in the SIB *base* slot
// and `req` in the *index* slot, where the original does the opposite.
//   original  cmp byte [ebp + ecx + 0x23], 0x3a   SIB 0x0d  (base ebp, index ecx)
//   ours      cmp byte [ecx + ebp + 0x23], 0x3a   SIB 0x29  (base ecx, index ebp)
// Same address, same length, opposite operand slots; it happens for all three
// accesses (`cwd[n-1]`, `cwd[n]`, `cwd[n+1]`), and the plain `cwd[n]` in the
// loop head already agrees (`cmp byte [ecx + edx], 0x5c`, base edx, index ecx).
//
// Every source shape tried for the three body accesses keeps the swapped slots
// (all 945 bytes, all 99.1%):
//   * `req->cwd[n-1]` / `req->cwd[n+1]` / `req->cwd[n]`, and the same three
//     spelled as `*(req->cwd + n +- 1)`,
//   * the same three through the offset-0 field with the constant in the index,
//     `req->unknown_0[0x23 + n]`, and left associative `*(req->unknown_0 + 0x23 + n)`,
//   * a block-local `char* cwd = req->cwd` (then the displacement is -1 and the
//     base is the pointer, which does NOT match: the original's base is `req`),
//   * a walked `char* p = req->cwd + n` with `p[-1]` / `p[1]` / `*p`,
//   * a named `int m = n - 1` (MSVC folds it straight back in),
//   * a named index with the offset in it (`int k = 0x23 + n`, changes the block),
//   * `((char*)req)[0x23 + n]`, `*((char*)req + 0x23 + n)` and
//     `*((char*)req + (0x23 + n))`,
//   * a block-local `char* r = (char*)req` with `r[0x23 + n]`, at the top of the
//     function and inside the branch, and the whole function rewritten so that
//     the request pointer itself is a cast-free `char*` local.
// What does flip the slots is the *register* the pointer ends up in, not the
// spelling: `char* f = (char*)req + 0x24; f[0x23 + n]` gives the original's
// order, `[edx + ecx + 0x23]`, because MSVC's lea put `req->cwd` in edx, while
// the same cast-free subscript on a pointer held in ebp, or a `char* g =
// (char*)gadget` held in ebx, always gives `[ecx + ebp + 0x23]` /
// `[ecx + ebx + 0x23]`. So the split
// tracks which register holds the base pointer (a caller-saved one that came
// out of a lea behaves like the original, a callee-saved one does not), and the
// original needs the base to be `req` in ebp. Since ebp is pinned by the rest
// of the function (`lea edx, [ebp+0x24]`, `[ebp+0x23c]`, `push ebp` for the
// callback), this looks like a register-role effect inside MSVC's SIB builder
// that no expression tree of this block reaches.
//
// muse-spark-1.3-free follow-up (all scored free via check.py --sym, 945 bytes,
// 99.1% every time, same 3 SIB diffs): the slot order is not reachable from the
// source at all. Minimal wcl probes show MSVC 5 ALWAYS emits the int count as
// the SIB base and the pointer as the index for a (count, pointer) pair, in
// every register combination tried: [eax+ecx] (p0), [eax+esi] (p1),
// [ecx+esi] (p4), [ecx+eax] (switch version), [ecx+edx] (this function head).
// Rule of thumb: lower-numbered register becomes base. The original's body
// (SIB 0x0d, base ebp over ecx) is the SOLE exception found anywhere, while
// its own loop head ([ecx+edx], SIB base ecx) follows the rule. Verified the
// raw bytes with objdump: orig `80 7c 0d 23 3a`. No TU-state effect either:
// prepending matched sibling 0x4af5b0 above (s2) changes nothing, and neither
// do <vector>/<map> headers, for- vs while-loop, Yoda comparison, switch on
// req->cwd[n-1], or an anchor member at +0x23 with (&req->anchor)[n].
// Per the guide this is the rare "commutative operand order from earlier TU
// state" bucket: say so and move on. Next step would need the real preceding
// function in the original TU (binary neighbour 0x4af5b0 did not flip it).
#include <string.h>

#pragma pack(push, 1)
struct Entry_004af670 {              // 0x15b bytes
    char unknown_0[2];
    char name[0x10];                 // +0x02
    char unknown_12[0xb6 - 0x12];
    char value[0xba - 0xb6];         // +0xb6
    short field_ba;                  // +0xba
    char unknown_bc[0x15b - 0xbc];
};

struct Req_004af670 {
    char unknown_0[0x24];
    char cwd[0x100];                 // +0x24
    char save_drive[0x10];           // +0x124
    char save_cwd[0x100];            // +0x134
    char* names;                     // +0x234
    char* sizes;                     // +0x238
    char* selected;                  // +0x23c
    int field_240;                   // +0x240
    void (__stdcall* callback)(void*);   // +0x244
};

struct Layer_004af670 {
    char unknown_0[4];
    Entry_004af670* entries;         // +0x04
    char unknown_8[4];
    Req_004af670* req;               // +0x0c
};

struct Gadget_004af670 {
    char unknown_0[0x18];
    Layer_004af670* layer;           // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                    // +0x60
};
#pragma pack(pop)

int __stdcall FUN_004a0300(Entry_004af670* entries, int i, char* name);
int __stdcall FUN_0049fdf0(Entry_004af670* entries, char* name, int type);
Entry_004af670* __stdcall FUN_0049ff90(Entry_004af670* entries, char* name);
void __stdcall FUN_0049fa90(Gadget_004af670* gadget);
void __stdcall FUN_004ab0a0(Gadget_004af670* gadget);
void __stdcall FUN_004af5b0(Req_004af670* req);
char* __stdcall FUN_004b6af0(char* text, int n);
int __stdcall FUN_004bc300(char* drive);
void __stdcall FUN_004bc360(const char* path);
void __cdecl FUN_004d85a0(void* data);

// FUNCTION: 0x4af670
void __stdcall FUN_004af670(Gadget_004af670* gadget)
{
    Req_004af670* req = gadget->layer->req;
    if (gadget->field_60 == -1) {
        FUN_004bc300(req->save_drive);
        FUN_004bc360(req->save_cwd);
        FUN_004d85a0(req);
        return;
    }

    Entry_004af670* entries = gadget->layer->entries;
    char drive[2];
    drive[1] = 0;
    int result = 0;

    if (FUN_004a0300(entries, gadget->field_60, "LOAD")
        || FUN_004a0300(entries, gadget->field_60, "SWIN")) {
        char* name = FUN_004b6af0(req->names,
                                  FUN_0049ff90(entries, "SWIN")->field_ba);
        if (name[0] == '\\') {
            strcpy(req->selected, name);
            int i;
            for (i = 0; i < 10; i++) {
                if (req->selected[i] == ' ') {
                    req->selected[i] = 0;
                    break;
                }
            }
            int n = (int)strlen(req->cwd);
            char* tail = req->selected;
            if (n == 3) {
                tail++;
            }
            strcat(req->cwd, tail);
            FUN_004bc360(req->cwd);
        } else {
            result = 1;
            strcpy(req->selected, req->cwd);
            strcat(req->selected, "\\");
            strcat(req->selected, name);
        }
    } else if (FUN_004a0300(entries, gadget->field_60, "CANC")) {
        result = 1;
    } else if (FUN_004a0300(entries, gadget->field_60, "PATH")) {
        int n = (int)strlen(req->cwd);
        if (n > 0) {
            while (n > 0) {
                if (req->cwd[n] == '\\') {
                    if (req->cwd[n - 1] == ':') {
                        req->cwd[n + 1] = 0;
                        FUN_004bc360(req->cwd);
                    } else {
                        req->cwd[n] = 0;
                        FUN_004bc360(req->cwd);
                    }
                    break;
                }
                n--;
            }
        }
    } else if (FUN_004a0300(entries, gadget->field_60, "NAME")) {
        gadget->field_60 = FUN_0049fdf0(entries, "LOAD", 14);
        int n = FUN_0049fdf0(entries, "NAME", 3);
        result = 1;
        strcpy(req->selected, entries[n].value);
    } else if (FUN_004a0300(entries, gadget->field_60, "ADRV")) {
        drive[0] = 'A';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "BDRV")) {
        drive[0] = 'B';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "CDRV")) {
        drive[0] = 'C';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "DDRV")) {
        drive[0] = 'D';
        FUN_004bc300(drive);
    } else if (FUN_004a0300(entries, gadget->field_60, "VDRV")) {
        drive[0] = 'R';
        FUN_004bc300(drive);
    }

    if (result == 1) {
        if (req->callback) {
            req->callback(req);
        }
    } else {
        FUN_004af5b0(req);
        FUN_0049fa90(gadget);
        FUN_004ab0a0(gadget);
    }
}
