// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Initialises the mouse-event input object: allocates the event queue, resets
// it, creates the three save-mouse surfaces, and optionally starts the worker
// thread at 0x4c2990. The second GetDisplay call is FUN_004c2bb0 inlined.

#pragma pack(push, 2)
struct Obj_004c2bd0 {
    char unknown_0[0x186];
    int capacity;                      // +0x186
    int entries;                       // +0x18a
    int head;                          // +0x18e
    int tail;                          // +0x192
    char unknown_196[0x1ae - 0x196];
    int active;                        // +0x1ae
    int unknown_1b2;                   // +0x1b2
    char unknown_1b6[0x1be - 0x1b6];
    int obj_1be;                       // +0x1be
    int obj_1c2;                       // +0x1c2
    int obj_1c6;                       // +0x1c6
    int running;                       // +0x1ca
    int unknown_1ce;                   // +0x1ce
    int unknown_1d2;                   // +0x1d2
    int pending;                       // +0x1d6
};
#pragma pack(pop)

void __cdecl MouseThreadProc(void* param_1);
int GetDisplay(void);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
void* __stdcall AllocSurface(char* name, int width, int height);
int __stdcall StartThread(void* param_1, unsigned int param_2, void* param_3);

// FUNCTION: 0x4c2bd0
void __stdcall InitMouse(int count, int start)
{
    Obj_004c2bd0* p = (Obj_004c2bd0*)GetDisplay();
    p->capacity = count;
    p->entries = (int)FUN_004d83b0("MOUSE EVENTS", count * 0x18);
    Obj_004c2bd0* q = (Obj_004c2bd0*)GetDisplay();
    q->head = 0;
    q->tail = 0;
    p->unknown_1b2 = 0;
    p->obj_1be = (int)AllocSurface("SAVEMOUSE 1", 0x640, 1);
    p->obj_1c2 = (int)AllocSurface("SAVEMOUSE 2", 0x640, 1);
    p->obj_1c6 = (int)AllocSurface("SAVEMOUSE 3", 0x640, 1);
    p->active = 1;
    p->unknown_1d2 = 0;
    p->unknown_1ce = 0;
    p->pending = 0;
    if (start == 1) {
        p->pending = 0;
        p->running = StartThread((void*)MouseThreadProc, 0x8000, p);
        if (p->running != 0)
            p->unknown_1ce = 1;
    } else {
        p->running = 0;
    }
}
