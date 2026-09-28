// Decompiled by space-bunny-free. Names are provisional.
// 90.8 percent: 638 of the original's 642 bytes, four bytes short, and every
// remaining difference is a register or slot choice rather than any logic.
//
// WHAT THE ORIGINAL DOES, all of it read off the bytes:
//  - six dword arguments, all six read: (char* path, char* list, char* sizes,
//    int mode, int flag, int what);
//  - the frame is 0x22c: five dword locals (count at +0x10, the search handle
//    at +0x14, the times base at +0x18, a saved copy of list at +0x1c, the
//    running times pointer at +0x20), an io.h _finddata_t at +0x24 and a
//    char[256] buffer at +0x13c that _itoa writes the file size into;
//  - mode 1 walks the subdirectories of the search 0x4bc4b0 opened on "*."
//    and appends "\\name" plus a "<DIR>" marker per entry; anything else, and
//    mode 1 as well, lists the plain files of path, optionally cutting the
//    extension (FUN_004bb0f0 when flag is 1) and recording the write time and
//    the size (FUN_004bbc40 through _itoa) per entry;
//  - the mode test and the handle test are ONE condition: both false tests jump
//    to the same one-instruction block at +0x438, the load of the sizes
//    pointer, and the completed directory walk jumps over it to +0x43f. So the
//    file walk runs in every case, and with mode 1 the sizes list is continued
//    from wherever the directory walk left it. That is a bug in Cavedog's code
//    (or at least an accident worth writing down): the directory walk is not an
//    alternative to the file walk, it runs in front of it;
//  - the collector 0x4aefa0 is called at the end with (list, 0, times, count),
//    except when what is 2, when the times pointer is 0 as well.
//
// WHAT IS STILL WRONG HERE, four items:
//  1. the saved copy of list and the times base have their stack slots the
//     other way round: this file puts the copy at +0x18 and the base at +0x1c,
//     the original the other way, which moves four instructions;
//  2. the load of the sizes pointer is placed before the handle test here and
//     after it in the original;
//  3. the count is incremented through edi here and through eax in the
//     original, and the store of the count sits on the other side of the
//     argument pushes;
//  4. the original reloads the now dead count slot into eax after its last
//     call, which is exactly the four bytes this file is short.
//
// THE ONE SOURCE DETAIL THAT UNLOCKED THE REGISTERS: each search has its own
// block scoped handle, declared inside the block that uses it, and the two
// share the slot at +0x14. That is what puts the times base in edi and the
// handle in esi (the original's pair). With one function scope handle the times
// base takes esi, the handle never gets a register at all, and the whole rest of
// the function shifts by two instructions (209 against 211).
//
// The cost of that, and the reason the search stopped here: to get the handle
// promoted, FUN_004bc8d0 has to sit OUTSIDE the "if (find != -1)" block, as it
// is in this file, so this file also closes a search that failed. The original
// does not: its find == -1 path jumps to +0x438, past the FUN_004bc8d0 call at
// +0x42c. Moving the call back inside the if block (with the handle still block
// scoped, or with one function scope handle) is the control flow the original
// has and scores 82.2 percent, with the same two register differences. So
// something in the original is neither of these two shapes: it closes the search
// only when the search worked, and still promotes the handle. That is where the
// next attempt should start.
//
// Tried with no effect: the declaration order of the locals (all 24 orders of
// count, times base, running pointer and saved copy, with the handles function
// scoped and with them block scoped as here), declaring the times base and the
// running pointer uninitialised and assigning them after the declarations, the
// handle as int, unsigned, long, void* or a struct pointer, the times base as
// char*, int*, long*, void*, const or unsigned char*, the saved copy as char*,
// const char* or unsigned char*, the saved copy or the count assigned by a
// statement rather than a declaration initialiser, a hand written finddata
// instead of io.h's, the sizes pointer copied into a block scoped variable in
// one or in both searches, and N unused extern declarations in front of the
// function for every N from 1 to 12 (the header lever is dead here, as at
// 0x438ea0 and 0x4bcb50).
#include <stdlib.h>
#include <string.h>
#include <io.h>

void* FUN_004d83b0(char* name, unsigned int size);
void FUN_004d85a0(void* p);
int __stdcall FUN_004aefa0(char* names, char* sizes, void* times, int count);
int __stdcall FUN_004bc4b0(const char* path, struct _finddata_t* fd, int state, char recursive);
int __stdcall FUN_004bc640(int handle, struct _finddata_t* fd);
void __stdcall FUN_004bc8d0(int handle);
char* __stdcall FUN_004bb0f0(char* name);
long __stdcall FUN_004bbc40(char* name);

// FUNCTION: 0x4af320
void __stdcall FUN_004af320(char* path, char* list, char* sizes, int mode, int flag, int what)
{
    char* first = list;
    int num = 0;
    char* times = (char*)FUN_004d83b0("FILETIMES", 0x2ee0);
    long* tp = (long*)times;
    struct _finddata_t fd;
    char text[256];

    if (mode == 1) {
        int find = FUN_004bc4b0("*.", &fd, -1, 1);
        if (find != -1)
            do {
                if (fd.name[0] != '.' && fd.attrib == 0x10) {
                    strncpy(list, "\\", 1);
                    list++;
                    strcpy(list, fd.name);
                    list += strlen(fd.name);
                    *list++ = 0;
                    if (sizes) {
                        strcpy(sizes, "<DIR>");
                        sizes += 6;
                    }
                }
                num++;
            } while (FUN_004bc640(find, &fd) != -1);
        FUN_004bc8d0(find);
    }
    {
        int find = FUN_004bc4b0(path, &fd, -1, 1);
        if (find != -1)
            do {
                if (fd.name[0] != '.' && fd.attrib != 0x10) {
                    strcpy(list, fd.name);
                    if (flag == 1) {
                        FUN_004bb0f0(list);
                    }
                    list += strlen(list) + 1;
                    *tp++ = fd.time_write;
                    if (sizes) {
                        _itoa(FUN_004bbc40(fd.name), text, 10);
                        strcpy(sizes, text);
                        sizes += strlen(text) + 1;
                    }
                }
                num++;
            } while (FUN_004bc640(find, &fd) != -1);
        FUN_004bc8d0(find);
    }
    if (what) {
        if (what == 2) {
            FUN_004aefa0(first, 0, 0, num);
        } else {
            FUN_004aefa0(first, 0, times, num);
        }
    }
    FUN_004d85a0(times);
    *list = 0;
}
