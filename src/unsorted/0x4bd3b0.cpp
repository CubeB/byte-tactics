// Decompiled by DeepSeek V4.1 Flash. Names are provisional.
// Best 86.7 percent (1154 vs 1148 bytes). What still differs, all register
// allocation:
//  - the top block loads out[0] into esi then copies to eax; the original
//    loads into eax and copies to esi (byte-neutral), and the original spills
//    root to [esp+0x18] just before the realloc call, ours earlier.
//  - the base+root address is emitted as [ebx+esi]; the original as [esi+ebx]
//    (initial header store and the first-pass count increment).
//  - at the second allocator call ours reloads out[1] into eax instead of
//    keeping base in ebx for the count read and the root+4 store.
//  - the name allocator call computes strlen+1+nameOff as not/dec/lea+1, the
//    original as not/add (reordering the expression did not change it here).
//  - the entry pointer for the name/flags stores is recomputed instead of the
//    original's single eax, and the file/dir data store address likewise.
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
    out[0] = entries + *(unsigned int*)((char*)out[1] + root) * 9;
    out[1] = (unsigned int)FUN_004d84a0((void*)out[1], DAT_0050a56c, out[0]);
    *(unsigned int*)((char*)out[1] + root + 4) = entries;

    h = FUN_004bc4b0(buf, &fd, -1, 1);
    if (h != -1) {
        ent = 0;
        do {
            if (strcmp(fd.name, DAT_00502910) != 0 && strcmp(fd.name, DAT_0050a548) != 0) {
                unsigned int nameOff = out[0];
                out[0] = nameOff + strlen(fd.name) + 1;
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
