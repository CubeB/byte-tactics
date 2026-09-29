// Decompiled by space-bunny-free, finished by space-bunny-free. Names are provisional.
// 92.9 percent, 642 of the original's 642 bytes, so every instruction is present
// and the four remaining hunks are pure register / schedule placement.
//
// WHAT THE ORIGINAL DOES, all of it read off the bytes:
//  - six dword arguments, all six read: (char* path, char* list, char* sizes,
//    int mode, int flag, int what). Argument 1 lands in ebp and is the
//    destination buffer: strncpy writes "\\" through it, the per entry name is
//    appended after it, the copy is saved at frame +0x1c for the collector, and
//    the function ends by storing 0 through it. Argument 2 is the path handed
//    to the second search only, argument 3 the sizes list, argument 4 the
//    extension-stripping flag, argument 5 the mode, argument 6 the "collect".
//  - the frame is 0x22c: the count at +0x10, ONE search handle at +0x14 (both
//    searches store that same slot, so the source has a single function scope
//    handle), the times base at +0x18, a saved copy of list at +0x1c, the
//    running times pointer at +0x20, a 0x118 byte _finddata_t at +0x24 (attrib
//    at +0, name at +0x14, mtime at +0xc, so the time fields are plain longs
//    here, not __time32_t) and a char[256] buffer at +0x13c that _itoa writes
//    the file size into;
//  - the mode test and the handle test are ONE condition: both false tests jump
//    to the same one-instruction block at 0x4af438, the load of the sizes
//    pointer, and the completed directory walk jumps over it to 0x4af43f. So the
//    file walk runs in every case, and with mode 1 the sizes list is continued
//    from wherever the directory walk left it;
//  - the collector 0x4aefa0 is called with (list, 0, times, count), except when
//    what is 2, when the times pointer is 0 as well. The two arms duplicate the
//    four pushes and tail merge, which is why the call is written twice.
//  - the return value is the count: the epilogue's
//    mov eax, [esp + 0x10] just after the FUN_004d85a0 call is the last use of
//    the count, and eax survives the whole epilogue untouched. Declaring the
//    function void loses exactly those 4 bytes, which was the previous
//    638-of-642 92.7 percent.
//
// WHAT IS STILL WRONG HERE, one allocator state, four sites:
//  1. the first search loads the sizes pointer before the handle test, the
//     original after it (the second search's load is right in both);
//  2. the count read-modify-write at the end of each loop body goes through edi
//     here and through eax in the original, and the original's store sits before
//     the two argument pushes where this file's sits between them, so MSVC 5
//     rematerialises it as [esp + 0x18];
//  3. in both loops the reload of the times base into edi sits before
//     push esi / call FUN_004bc8d0 here and after it in the original;
//  4. sites 2 and 3 are the same register (edi) being handed to two things, so
//     this is very likely one decision rather than four.
//
// THE ONE SOURCE DETAIL WORTH THE LAST POINTS: the search handle is ONE
// function scope variable shared by both searches, not a pair of block scoped
// ones. Both searches then store the same slot, and the saved copy of list and
// the times base land in the slots the original uses. That is the guide's item 2:
// the two block scoped handles were two live graph nodes, and merging them
// demoted a variable one step.
//
// Tried with no effect this round: `num = num + 1` for `num++`, `long num`,
// `unsigned int num`, moving the count's declaration after the times base (92.5),
// dropping the tp local (compile fail), removing the braces around the second
// search (92.9, no change), a function scope copy of the sizes parameter used by
// both loops (89.1) and one used by the first loop only (79.8). Moving
// FUN_004bc8d0 inside the `find != -1` guard, which is what the original's
// control flow actually does, costs 4.7 points (88.2), so the source really does
// call it unconditionally.
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
int __stdcall FUN_004af320(char* path, char* list, char* sizes, int mode, int flag, int what)
{
    char* first = list;
    int num = 0;
    char* times = (char*)FUN_004d83b0("FILETIMES", 0x2ee0);
    long* tp = (long*)times;
    struct _finddata_t fd;
    char text[256];
    int find;

    if (mode == 1) {
        find = FUN_004bc4b0("*.", &fd, -1, 1);
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
        find = FUN_004bc4b0(path, &fd, -1, 1);
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
    return num;
}
