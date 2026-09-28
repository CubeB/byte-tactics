// Decompiled by deepseek-v4.1-flash. Names are provisional.
// TODO: 96.2%. Everything matches except the q = base + offset computation in
// the middle loop: the original emits
//     mov eax,[ebp]; mov ecx,edi; add ecx,eax; mov [ebp],ecx
// while this source makes MSVC 5 emit
//     mov eax,[ebp]; add eax,edi; mov ecx,eax; mov [ebp],eax
// (same 4 instructions, the result register is reused instead of copied).
void* __stdcall FUN_004bbe50(char* name, int flags);

// FUNCTION: 0x4b8c60
void* __stdcall FUN_004b8c60(char* name)
{
    int* base = (int*)FUN_004bbe50(name, 0);
    if (base == 0)
        return 0;

    for (int i = 0; i < (short)base[1]; i++) {
        unsigned short* p = (unsigned short*)((char*)base + base[3 + i]);
        base[3 + i] = (int)p;
        int j = 0;
        if (*p > 0) {
            int* field = (int*)((char*)p + 0x28);
            do {
                *field = (int)((char*)base + *field);
                int* q = (int*)*field;
                q[4] = q[4] + (int)base;
                if (((unsigned char*)q)[0xa] > 0) {
                    for (int k = 0; k < (int)((unsigned char*)q)[0xa]; k++) {
                        ((int*)q[4])[k] = ((int*)q[4])[k] + (int)base;
                        int* s = (int*)((int*)q[4])[k];
                        s[4] = s[4] + (int)base;
                    }
                }
                j++;
                field += 2;
            } while (j < (int)*p);
        }
    }
    return base;
}
