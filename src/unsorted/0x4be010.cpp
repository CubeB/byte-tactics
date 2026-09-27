// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Class_004be010 {
    int count;   // +0
    int base;    // +4
};

struct Elem_004be010 {
    int a;                 // +0
    int b;                 // +4
    unsigned char flags;   // +8
};

// FUNCTION: 0x4be010
void __stdcall FUN_004be010(Class_004be010* h, int delta)
{
    h->base += delta;
    for (int i = h->count - 1; i >= 0; i--) {
        Elem_004be010* p = (Elem_004be010*)(h->base + i * 9);
        p->a += delta;
        p->b += delta;
        if (p->flags & 1)
            FUN_004be010((Class_004be010*)p->b, delta);
    }
}
