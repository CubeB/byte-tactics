// Decompiled by Opus. Names are provisional.
// Walks a chain of stack frames (each frame holds the next frame pointer and
// a return address), recording up to `max` return addresses after skipping
// `skip`, then copies `copyMax` dwords of the raw stack.

char __cdecl FUN_004d8d70(void* p, int size);
char __cdecl FUN_004de180(int param);
void __cdecl FUN_004de700(int* frame, int* stack, int eip, int skip, int* out, int max, int* count);

// FUNCTION: 0x4de600
void __cdecl FUN_004de600(int* frame, int* stack, int eip, int skip, int* out, int max, int* count,
                          int* copy, int copyMax, int* copied)
{
    *count = 0;
    if (skip == 0) {
        out[*count] = eip;
        (*count)++;
    } else {
        skip--;
    }
    if (FUN_004de180(0)) {
        FUN_004de700(frame, stack, eip, skip + 1, out, max, count);
    } else {
        for (int* fp = frame; *count < max; fp = (int*)*fp) {
            if (FUN_004d8d70(fp, 8))
                break;
            if (fp < stack)
                break;
            if (*fp <= (int)fp)
                break;
            if (fp[1] == 0)
                break;
            if (skip == 0) {
                out[*count] = fp[1];
                (*count)++;
            } else {
                skip--;
            }
        }
    }
    int i = 0;
    if (stack != 0) {
        for (i = 0; i < copyMax; i++) {
            if (FUN_004d8d70(&stack[i], 4))
                break;
            copy[i] = stack[i];
        }
    }
    *copied = i;
    for (; i < copyMax; i++)
        copy[i] = 0;
}
