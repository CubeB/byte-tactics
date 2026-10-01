// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Best 88.3 percent (1152 vs 1148 bytes). What still differs, all register
// allocation (semantics are believed correct):
//  - the top block loads out[0] into esi then copies to eax; the original
//    loads into eax and copies to esi (byte-neutral), and the original spills
//    root to [esp+0x18] just before the realloc call, ours earlier.
//  - the base+root address is emitted as [ebx+esi]; the original as [esi+ebx]
//    (initial header store and the first-pass count increment).
//  - the second allocator block: we do `mov esi,[ebp]` where the original does
//    `mov eax,[ebp]; mov esi,eax`, and the count read is [ebx+edi] vs the
//    original's [edi+ebx] (keeping base in ebx).
//  - the name allocator call now reads `not ecx; lea eax,[ecx+ebx]` (nlen temp),
//    the original `not ecx; add ecx,ebx; push ecx` (byte-neutral, different reg).
//    Tried nameOff+nlen, nlen+=nameOff, nameOff+strlen+1: all the same lea.
//  - the entry name/flags store pointer is computed as (ent+entries)+base and
//    the leaf data address folds the new allocation into the store
//    ([ecx+eax+4] vs add ecx,eax then [ecx+4]); the `*total += size` store is
//    a load/add/store instead of the original `add [eax],ecx`.
// Tried and rejected (all scored below 88.3): moving the entries declaration to
// its point of use (86.9), out[0]=root+8 with/without an nsize temp (83.5),
// (unsigned)base in the header address (88.2), #include <windows.h> (no change),
// a size temp for the leaf branch (87.0-87.2).
//
// Session 2 (deepseek-v4.1-flash) findings:
//  - The 4-byte size excess (1152 vs 1148) is exactly the DOUBLE read of
//    fd.size in the leaf: base emits two `mov ...,[esp+0x34]` (one for node.size,
//    one reloading for *total += size). Reading fd.size once via a local `fsz`
//    makes the size exactly 1148 but the score drops to 87.9%, because fsz then
//    lands in edi and `*total += fsz` stays a load/add/store (the known stuck
//    `add [eax],ecx`) while node.size is stored from edi. So the right size and
//    the best score are currently mutually exclusive here.
//  - Tried and rejected: `nsize = root + 8` single-read (83.6), `out[0] += 8`
//    capture (88.3, prologue still `mov esi,[ebp]; mov eax,esi`), `out[0] =
//    (root = out[0]) + 8` (81.8, gives `lea eax,[esi+8]`), param_2 as a
//    HapiBuf{size,buf} struct with out->size/out->buf (87.9, prologue unchanged),
//    `root + base` instead of `base + root` for the header/inc SIB (flat 88.3),
//    #include <windows.h> (flat 88.3).
//  - The prologue `mov eax,[mem]; mov esi,eax` vs our `mov esi,[mem]; mov eax,esi`
//    would not flip with struct access, += capture, or an assignment-expression;
//    it recurs at the second allocator block too. The name-allocator `add
//    ecx,ebx; push ecx` vs `lea eax,[ecx+ebx]; push eax` and the `add [eax],ecx`
//    for *total += size are also stuck (see board). Semantics are believed
//    correct throughout; what differs is register allocation and scheduling.
//
// Session 3 (deepseek-v4.1-flash) findings:
//  - Reproduced the fsz-capture variant (87.9, exact 1148 bytes) again; kept
//    this 88.3 version as the file since it scores higher.
//  - Probed MSVC 5 directly (build/scratch/0x4bd3b0/probe): plain `*total += x`
//    DOES emit the RMW `add [eax],ecx` with a single read of x in isolation
//    (int, unsigned, struct fd.size, stores through out[1]-style pointers). A
//    faithful leaf probe (c.cpp g8: fd stack local, out/total params, a call
//    mid-block, entry store, node stores) gives the single fd.size read into
//    ecx (the CSE survives the char store) but still a load/add/store on
//    *total while total stays register-resident. In the real function total is
//    spilled and reloaded into eax, where the original's `mov ecx,eax` copy
//    then `add [eax],ecx` appears. The RMW form looks tied to total being
//    spilled and the addend living in a second register, not to the spelling
//    of `+=`.
#include <io.h>
#include <string.h>

#pragma pack(push, 1)
struct Find_004bd3b0 {
    char dir[0x100];     // +0x000
    char name[0x100];    // +0x100
    int state;           // +0x200
    char recursive;      // +0x204
    long handle;         // +0x205
    int index;           // +0x209
};

struct Entry_004bd3b0 {
    unsigned int name;   // +0
    unsigned int data;   // +4
    unsigned char flags; // +8
};
#pragma pack(pop)

extern char DAT_0050a56c[];  // "Package Data"
extern char DAT_0050a57c[];  // "\\*"
extern char DAT_0050372c[];  // "*"
extern char DAT_0050a548[];  // ".."
extern char DAT_00502910[];  // "."
extern char DAT_00503374[];  // "\\"

void* __cdecl FUN_004d84a0(void* p, const char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(Find_004bd3b0* f, struct _finddata_t* fd);

// FUNCTION: 0x4bd3b0
unsigned int __stdcall FUN_004bd3b0(char* path, unsigned int* out, int* total)
{
    char buf[0x104];
    struct _finddata_t fd;
    int h;
    int trailing;
    unsigned int root;
    unsigned int entries;
    int ent;

    root = out[0];
    unsigned int nsize = out[0] + 8;
    out[0] = nsize;
    char* base = (char*)FUN_004d84a0((void*)out[1], DAT_0050a56c, nsize);
    out[1] = (unsigned int)base;
    *(unsigned int*)(base + root) = 0;

    strcpy(buf, path);
    if (buf[strlen(buf) - 1] != '\\') {
        strcat(buf, DAT_0050a57c);
        trailing = 0;
    } else {
        strcat(buf, DAT_0050372c);
        trailing = 1;
    }

    h = FUN_004bc4b0(buf, &fd, -1, 1);
    if (h != -1) {
        do {
            if (strcmp(fd.name, DAT_00502910) != 0 && strcmp(fd.name, DAT_0050a548) != 0)
                (*(unsigned int*)(base + root))++;
        } while (FUN_004bc640((Find_004bd3b0*)h, &fd) == 0);
        if (h != 0) {
            if (((Find_004bd3b0*)h)->state < 0)
                _findclose(((Find_004bd3b0*)h)->handle);
            FUN_004d85a0((void*)h);
        }
    }

    entries = out[0];
    out[0] = entries + *(unsigned int*)(base + root) * 9;
    char* nb = (char*)FUN_004d84a0((void*)out[1], DAT_0050a56c, out[0]);
    out[1] = (unsigned int)nb;
    *(unsigned int*)(nb + root + 4) = entries;

    h = FUN_004bc4b0(buf, &fd, -1, 1);
    if (h != -1) {
        ent = 0;
        do {
            if (strcmp(fd.name, DAT_00502910) != 0 && strcmp(fd.name, DAT_0050a548) != 0) {
                unsigned int nameOff = out[0];
                unsigned int nlen = strlen(fd.name) + 1;
                out[0] = nlen + nameOff;
                out[1] = (unsigned int)FUN_004d84a0((void*)out[1], DAT_0050a56c, out[0]);
                strcpy((char*)out[1] + nameOff, fd.name);
                Entry_004bd3b0* e = (Entry_004bd3b0*)((char*)out[1] + entries + ent);
                e->name = nameOff;
                e->flags &= 1;
                if ((fd.attrib & 0x10) != 0) {
                    e->flags |= 1;
                    strcpy(buf, path);
                    if (!trailing)
                        strcat(buf, DAT_00503374);
                    strcat(buf, fd.name);
                    ((Entry_004bd3b0*)((char*)out[1] + entries + ent))->data =
                        FUN_004bd3b0(buf, out, total);
                } else {
                    e->flags &= ~1;
                    unsigned int nodeOff = out[0];
                    out[0] = nodeOff + 9;
                    out[1] = (unsigned int)FUN_004d84a0((void*)out[1], DAT_0050a56c, out[0]);
                    ((Entry_004bd3b0*)((char*)out[1] + entries + ent))->data = nodeOff;
                    unsigned char* node = (unsigned char*)((char*)out[1] + nodeOff);
                    *(unsigned int*)(node + 4) = fd.size;
                    *(unsigned int*)node = 0;
                    node[8] = 0;
                    *total += fd.size;
                }
                ent += 9;
            }
        } while (FUN_004bc640((Find_004bd3b0*)h, &fd) == 0);
        if (h != 0) {
            if (((Find_004bd3b0*)h)->state < 0)
                _findclose(((Find_004bd3b0*)h)->handle);
            FUN_004d85a0((void*)h);
        }
    }
    return root;
}
