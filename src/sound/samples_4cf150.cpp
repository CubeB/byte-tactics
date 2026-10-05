// Decompiled by Sonnet. Names are provisional.

typedef void (__stdcall *ReleaseFn_004cf150)(void*);

struct Class_004cf150 {
    char unknown_0[0x30];
    int count;          // +0x30
    char unknown_1[4];   // +0x34
    void* items[0x20];   // +0x38

    void FUN_004cf150();
};

// FUNCTION: 0x4cf150
void Class_004cf150::FUN_004cf150()
{
    void** slot = items;
    for (int i = 0x20; i != 0; i--) {
        void* obj = *slot;
        if (obj != 0) {
            ReleaseFn_004cf150 fn = (ReleaseFn_004cf150)(*(int*)(*(int*)obj + 0x48));
            fn(obj);
            *slot = 0;
            count--;
        }
        slot++;
    }
}
