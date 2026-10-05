// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct ArchiveDirectory {
    int count;   // +0
    int base;    // +4
};

struct ArchiveEntry {
    int a;                 // +0
    int b;                 // +4
    unsigned char flags;   // +8
};

// FUNCTION: 0x4be010
void __stdcall HAPI_RelocateDirectory(ArchiveDirectory* h, int delta)
{
    h->base += delta;
    for (int i = h->count - 1; i >= 0; i--) {
        ArchiveEntry* p = (ArchiveEntry*)(h->base + i * 9);
        p->a += delta;
        p->b += delta;
        if (p->flags & 1)
            HAPI_RelocateDirectory((ArchiveDirectory*)p->b, delta);
    }
}
