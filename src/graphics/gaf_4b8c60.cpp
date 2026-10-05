// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
void* __stdcall HAPI_LoadFile(char* name, int flags);

// A block of the root object: a count, a sub-count, then a run of 8-byte
// entries whose first int is an offset that is rebased in place.
struct Entry { int off; int pad; };
struct Blk {
    unsigned short count;
    unsigned char pad1[8];
    unsigned char subcount;
    unsigned char pad2[29];
    Entry e[2];
};

// FUNCTION: 0x4b8c60
void* __stdcall LoadGaf(char* name)
{
    int* base = (int*)HAPI_LoadFile(name, 0);
    if (base == 0)
        return 0;

    for (int i = 0; i < (short)base[1]; i++) {
        Blk* p = (Blk*)((char*)base + base[3 + i]);
        base[3 + i] = (int)p;
        int j = 0;
        if (p->count > 0) {
            do {
                int* q = (int*)((char*)base + p->e[j].off);
                p->e[j].off = (int)q;
                q[4] = q[4] + (int)base;
                if (((unsigned char*)q)[0xa] > 0) {
                    for (int k = 0; k < (int)((unsigned char*)q)[0xa]; k++) {
                        ((int*)q[4])[k] = ((int*)q[4])[k] + (int)base;
                        int* s = (int*)((int*)q[4])[k];
                        s[4] = s[4] + (int)base;
                    }
                }
                j++;
            } while (j < (int)p->count);
        }
    }
    return base;
}
