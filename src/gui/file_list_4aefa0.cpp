// Decompiled by space-bunny-free, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Space Bunny Free, finished by DeepSeek V4.1 Flash, finished by opus. Names are provisional.
// MATCH. Earlier passes chased a register/frame-slot tie in the first loop for
// seven rounds, but the real cause was the control flow after it: the
// `je 0x4af11e` at 0x4af05a (n == -1) jumps to the start/end setup of the
// SECOND bubble pass, not past it. So only the first pass (over 0..n) sits
// inside `if (n != -1)`; start = n + 1, end = count - 1 and the second pass
// always run, which sorts the whole list when no line starts with a
// backslash. With that fixed, the first loop is a plain `for` and every slot
// and register falls into place.
// The second pass compares keys the other way round, keys[i+1] - keys[i]
// (0x4af174: `mov eax,[esi+ebx]` is keys[i+1], `mov edx,[edi]` is keys[i]),
// so with keys the part after the split sorts descending while the part
// before it sorts ascending; both _strcmpi calls compare ptr1[i] with
// ptr1[i+1]. n records the LAST line starting with a backslash (0x4af03f
// stores i on every hit).
#include <string.h>

void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
char* __stdcall SkipTextLines(char* list, int index);
int __cdecl _strcmpi(const char* a, const char* b);

// FUNCTION: 0x4aefa0
void __stdcall SortFileList(char* list1, char* list2, int* keys, int count)
{
    char** ptr2;
    char** ptr1;
    int swapped;
    int n;
    char* buf2;
    char* buf1;
    int end;
    int start;
    int i;

    buf1 = (char*)FUN_004d83b0("SORTED LIST1", 0x17700);
    buf2 = (char*)FUN_004d83b0("SORTED LIST2", 0x17700);
    ptr1 = (char**)FUN_004d83b0("PTR LIST1", 0x2ee0);
    ptr2 = (char**)FUN_004d83b0("PTR LIST2", 0x2ee0);
    n = -1;
    for (i = 0; i < count; i++) {
        ptr1[i] = SkipTextLines(list1, i);
        if (list2)
            ptr2[i] = SkipTextLines(list2, i);
        if (ptr1[i][0] == '\\')
            n = i;
    }
    if (n != -1) {
        do {
            swapped = 0;
            for (i = 0; i < n; i++) {
                int diff;
                if (keys)
                    diff = keys[i] - keys[i+1];
                else
                    diff = _strcmpi(ptr1[i], ptr1[i+1]);
                if (diff > 0) {
                    char* t = ptr1[i];
                    ptr1[i] = ptr1[i+1];
                    ptr1[i+1] = t;
                    if (list2) {
                        t = ptr2[i];
                        ptr2[i] = ptr2[i+1];
                        ptr2[i+1] = t;
                    }
                    if (keys) {
                        int u = keys[i];
                        keys[i] = keys[i+1];
                        keys[i+1] = u;
                    }
                    swapped = 1;
                }
            }
        } while (swapped);
    }
    start = n + 1;
    end = count - 1;
    do {
        swapped = 0;
        for (i = start; i < end; i++) {
            int diff;
            if (keys)
                diff = keys[i+1] - keys[i];
            else
                diff = _strcmpi(ptr1[i], ptr1[i+1]);
            if (diff > 0) {
                char* t = ptr1[i];
                ptr1[i] = ptr1[i+1];
                ptr1[i+1] = t;
                if (list2) {
                    t = ptr2[i];
                    ptr2[i] = ptr2[i+1];
                    ptr2[i+1] = t;
                }
                if (keys) {
                    int u = keys[i];
                    keys[i] = keys[i+1];
                    keys[i+1] = u;
                }
                swapped = 1;
            }
        }
    } while (swapped);
    {
        char* p = buf1;
        char* q = buf2;
        for (i = 0; i < count; i++) {
            strcpy(p, ptr1[i]);
            p += strlen(ptr1[i]) + 1;
            if (list2) {
                strcpy(q, ptr2[i]);
                q += strlen(ptr2[i]) + 1;
            }
        }
        memcpy(list1, buf1, p - buf1);
        if (list2)
            memcpy(list2, buf2, q - buf2);
    }
    FUN_004d85a0(ptr2);
    FUN_004d85a0(ptr1);
    FUN_004d85a0(buf2);
    FUN_004d85a0(buf1);
}
