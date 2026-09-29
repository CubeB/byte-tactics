// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Walks a directory tree (the search 0x4bc4b0 allocates, the same one 0x4bcb50
// uses) and, for every plain file, marks the matching entry of every open
// HAPI archive: the entry named "path + file name" is looked up with
// FUN_004bb4e0 in each archive's directory (from the search's current archive
// index on) and gets bit 2 set unless bit 1 is already set. Sub directories
// other than "." and ".." are entered with the path extended by "name\\".
//
// NOT MATCHED: 99.2%, size exact. Exactly one instruction differs, the target
// of the file loop's entry test at 0x4be5d4. When the loop has nothing to do
// (`i >= d->count` on entry) the original jumps PAST the reload of the search
// handle into esi at 0x4be66d and lands on the FUN_004bc640 argument setup at
// 0x4be672; here the guard jumps to the reload. The join block 0x4be66d has
// four predecessors (the two strcmp matches at 0x4be4dd and 0x4be51a, the
// recursion at 0x4be5ba and the loop latch falling through from 0x4be667), and
// the original simply lets the fifth edge, the guard, skip it, which is safe
// because on that edge esi still holds the handle.
//
// A second session (this one) spent about thirty shapes on that one edge and
// every one of them still emits `jge` to the reload, so all of these are ruled
// out, not merely untried:
//   - the inner loop: `for (; i < n; i++)`, `for (i = (i < 0 ? 0 : i + 1); ...)`,
//     `while (i < n) { ...; i++; }`, `if (i < n) do { ... } while (++i < n);`,
//     `for (;;) { if (i >= n) break; ...; i++; }`, `while (1) { if (...) break;
//     ...; i++; }` (that last one is the only one that changes anything: it
//     stops the rotation and drops to 68.0), `for (i = i; ...)`;
//   - the clamp: the `if (i < 0) i = 0; else i++;` three-statement form, the
//     `i = (i < 0) ? 0 : i + 1;` assignment and the clamp as the `for` init;
//   - `i` as int, unsigned (94.1: it also breaks the two strcmp joins) and long;
//   - the handle as a `Find*` throughout with `(Find*)-1` compared, and the
//     local count and the local name pointer pulled out of the loop (61.8 and
//     76.8: both change the whole frame);
//   - the outer loop: `do { } while (cond)`, `while (1) { ...; if (cond ==
//     -1) break; }`, the `if (h == -1) return;` guard versus the whole body and
//     the tail nested inside an `if (h != -1)`, and the whole if/else wrapped in
//     an extra nested block;
//   - the if/else: the two branches swapped, `else if (!(attrib & 0x10))`,
//     `if ((attrib & 0x10) != 0)`, and the file branch ending in `continue`
//     instead of an `if` guard (which is what the original's branchy strcmp
//     wants but does not move the edge either).
// So the edge is not a loop-shape, type, branch-order or local-order effect. The
// only shape that even touched it is one that adds a live local to the loop
// (`Entry* e = 0;` before the body, 92.3), and that goes the wrong way. Worth
// trying next: something that gives the compiler a second definition of the
// handle inside the outer loop, so the join block at 0x4be66d exists for the
// directory branch only and the file branch's latch reaches 0x4be672 directly.
#include <io.h>
#include <string.h>

#pragma pack(push, 1)
struct Find_004be400 {
    char dir[0x100];         // +0x000
    char pattern[0x100];     // +0x100
    int state;               // +0x200
    char recursive;          // +0x204
    long handle;             // +0x205
    int index;               // +0x209
};

struct Entry_004be400 {                // 9 bytes
    int field_0;
    int field_4;
    unsigned char flags;               // +0x8
};

struct Table_004be400 {
    int count;
    Entry_004be400* entries;
};

struct Header_004be400 {
    char unknown_0[0x10];
    Table_004be400* table;             // +0x10
};

struct File_004be400 {
    char unknown_0[8];
    Header_004be400* header;           // +0x8
};

struct Display_004be400 {
    char unknown_0[0x618];
    File_004be400** files;             // +0x618
    int count;                         // +0x61c
};
#pragma pack(pop)

Display_004be400* FUN_004b6220();
Entry_004be400* __stdcall FUN_004bb4e0(Table_004be400* table, char* name);
int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(int handle, struct _finddata_t* fd);
void FUN_004d85a0(void* p);

// FUNCTION: 0x4be400
void __stdcall FUN_004be400(char* path, int state, int recursive)
{
    Display_004be400* d = FUN_004b6220();
    char buf[0x100];
    struct _finddata_t fd;

    strcpy(buf, path);
    strcat(buf, "*");
    int h = FUN_004bc4b0(buf, &fd, state, recursive);
    if (h == -1)
        return;
    do {
        if (fd.attrib & 0x10) {
            if (strcmp(fd.name, ".") != 0 && strcmp(fd.name, "..") != 0) {
                strcpy(buf, path);
                strcat(buf, fd.name);
                strcat(buf, "\\");
                FUN_004be400(buf, ((Find_004be400*)h)->state, 0);
            }
        } else {
            int i = ((Find_004be400*)h)->state;
            if (i < 0)
                i = 0;
            else
                i++;
            for (; i < d->count; i++) {
                strcpy(buf, path);
                strcat(buf, fd.name);
                Entry_004be400* e = FUN_004bb4e0(d->files[i]->header->table, buf);
                if (e && !(e->flags & 1))
                    e->flags |= 2;
            }
        }
    } while (FUN_004bc640(h, &fd) != -1);
    Find_004be400* f = (Find_004be400*)h;
    if (f) {
        if (f->state < 0)
            _findclose(f->handle);
        FUN_004d85a0(f);
    }
}
