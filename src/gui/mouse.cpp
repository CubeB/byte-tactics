// Decompiled by Opus, deepseek-v4.1-flash, Sonnet 5.5, space-bunny-free and Haiku. Names are provisional.
// The software mouse cursor: the hide/show refcount blits, the mouse-event
// ring ("MOUSE EVENTS"), the saved-background surfaces ("SAVEMOUSE 1" to 3),
// the cursor blit thread and the two spin locks it takes.
#include <windows.h>
#include <string.h>

#pragma pack(push, 2)

// The cursor bitmap at +0x1b2: its size and the hotspot its origin is
// offset by.
struct GafFrame {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};

// A saved-background surface descriptor at +0x1be.
struct Desc_004c25e0 {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
};

// A mouse event, 0x18 bytes: the cursor position (the rectangle at +0x196)
// and the event's own fields.
struct Event_4c2d60 {
    int x;                             // +0x0
    int y;                             // +0x4
    int data[4];                       // +0x8
};

// The local the lock and blit calls fill with a locked surface.
struct Surface_004c23e0 {
    int data[12];
};

// The GUI object GetDisplay returns: the fields the mouse code uses.
struct Obj_004c2380 {
    char unknown_0[0xf0];
    unsigned short bit0_9 : 10;        // +0xf0
    unsigned short active : 1;         // +0xf0, bit 10
    unsigned short bit11_15 : 5;
    char unknown_f2[0x186 - 0xf2];
    int capacity;                      // +0x186
    Event_4c2d60* entries;             // +0x18a
    int head;                          // +0x18e
    int tail;                          // +0x192
    Event_4c2d60 rect;                 // +0x196, the last mouse event
    int count;                         // +0x1ae
    GafFrame* sprite;                  // +0x1b2
    int x;                             // +0x1b6
    int y;                             // +0x1ba
    Desc_004c25e0* saved;              // +0x1be
    Desc_004c25e0* under;              // +0x1c2
    Desc_004c25e0* work;               // +0x1c6
    int running;                       // +0x1ca, the thread's handle
    int mode;                          // +0x1ce
    int visible;                       // +0x1d2
    int pending;                       // +0x1d6
};

#pragma pack(pop)

Obj_004c2380* GetDisplay(void);

extern LONG g_gfxBlitLockHeld;
extern LONG g_gfxBlitLockOwner;
extern HANDLE g_gfxBlitLockEvent;

void __cdecl BlitSurface(void* dst, void* src, int x, int y);
void __stdcall DrawFrame(void* dst, GafFrame* bmp, int x, int y);
void __stdcall DrawSurface(Desc_004c25e0* dst, Desc_004c25e0* bmp, int x, int y);
int __stdcall LockPrimary(void* out);
int __stdcall UnlockPrimary(void* unused, RECT* r1, RECT* r2);
void __stdcall ResetClipRect(void* d);
void* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* obj);
int __stdcall StartThread(void* param_1, unsigned int param_2, void* param_3);
void __stdcall SleepMilliseconds(unsigned int param_1);
void* __cdecl GameAllocIgnoreTag(char* name, unsigned int size);
void __cdecl GameFreeThunk(void* p);

void __stdcall RedrawMouseCursor(Obj_004c2380* app);
void __cdecl MouseThreadProc(int param);

// The copy must stay memcpy: a struct assignment computes the source with
// mov/add, not lea.
static inline void GetRect(Event_4c2d60* out)
{
    Obj_004c2380* p = GetDisplay();
    memcpy(out, &p->rect, sizeof(Event_4c2d60));
}

// The cursor thread's lock, held on the "MOUS" tag.
static inline LONG LockMouse()
{
    while (1) {
        LONG r = InterlockedExchange(&g_gfxBlitLockHeld, 0x4d4f5553);
        if (r == 0) {
            g_gfxBlitLockOwner = 0x4d4f5553;
            return 0;
        }
        if (g_gfxBlitLockOwner == 0x4d4f5553)
            return r;
        WaitForSingleObject(g_gfxBlitLockEvent, INFINITE);
    }
}

static inline void UnlockMouse(LONG held)
{
    if (held == 0) {
        g_gfxBlitLockOwner = 0;
        InterlockedExchange(&g_gfxBlitLockHeld, 0);
        SetEvent(g_gfxBlitLockEvent);
    }
}

// The blit lock the frame setter takes, held on the "MAIN" tag.
static inline LONG LockMain()
{
    while (1) {
        LONG r = InterlockedExchange(&g_gfxBlitLockHeld, 0x4d41494e);
        if (r == 0) {
            g_gfxBlitLockOwner = 0x4d41494e;
            return 0;
        }
        if (g_gfxBlitLockOwner == 0x4d41494e)
            return r;
        WaitForSingleObject(g_gfxBlitLockEvent, INFINITE);
    }
}

static inline void UnlockMain(LONG held)
{
    if (held == 0) {
        g_gfxBlitLockOwner = 0;
        InterlockedExchange(&g_gfxBlitLockHeld, 0);
        SetEvent(g_gfxBlitLockEvent);
    }
}

// Blits the cursor's current frame at its position when the cursor is
// visible (count <= 0) and the mouse thread does not own the blit (mode != 1).
// FUNCTION: 0x4c2380
void DrawSoftwareCursor()
{
    Obj_004c2380* obj = GetDisplay();
    if (obj->mode != 1 && obj->count <= 0) {
        Event_4c2d60 r;
        GetRect(&r);
        DrawFrame(0, obj->sprite, r.x, r.y);
    }
}

// Draws the saved bitmap at the object's position: locks the screen surface
// (LockPrimary), blits with the hand-written routine BlitSurface, then
// unlocks with the rectangle that was drawn.
// FUNCTION: 0x4c23e0
void __stdcall RestoreCursorBackground(Obj_004c2380* obj)
{
    if (obj->visible) {
        Desc_004c25e0* bmp = obj->saved;
        Surface_004c23e0 s;
        if (LockPrimary(&s)) {
            BlitSurface(&s, bmp, obj->x, obj->y);
            RECT r;
            r.left = obj->x;
            r.top = obj->y;
            r.right = r.left + bmp->width;
            r.bottom = r.top + bmp->height;
            UnlockPrimary(&s, &r, 0);
        }
    }
}

// Increments the hide counter and restores the saved background from the
// screen when the counter was zero.
// FUNCTION: 0x4c2470
void HideSoftwareCursor()
{
    Obj_004c2380* o = GetDisplay();
    if (o->mode != 1 && o->count++ == 0)
        DrawSurface(0, o->saved, o->x, o->y);
}

// Captures the background under the cursor position and blits the cursor's
// bitmap there.
// FUNCTION: 0x4c24b0
void __stdcall CaptureBackgroundAndDrawCursor(Obj_004c2380* obj)
{
    if (obj->visible) {
        Surface_004c23e0 s;
        if (LockPrimary(&s)) {
            RECT r;
            POINT pt;
            GetCursorPos(&pt);
            obj->rect.x = pt.x;
            obj->rect.y = pt.y;
            obj->x = pt.x - obj->sprite->dx;
            obj->y = pt.y - obj->sprite->dy;
            obj->saved->width = obj->sprite->width;
            obj->saved->height = obj->sprite->height;
            obj->saved->pitch = obj->sprite->width;
            BlitSurface(obj->saved, &s, -obj->x, -obj->y);
            DrawFrame(&s, obj->sprite, obj->rect.x, obj->rect.y);
            r.left = obj->x;
            r.top = obj->y;
            r.right = r.left + obj->sprite->width;
            r.bottom = r.top + obj->sprite->height;
            UnlockPrimary(&s, &r, 0);
        }
    }
}

// Redraws the software mouse cursor: reads the cursor position, computes the
// cursor's top left (position minus the sprite's origin), rebuilds the saved
// background and the composed image in the three off-screen surface
// descriptors at +0x1be, +0x1c2 and +0x1c6 (each resized to the sprite),
// restores what the old cursor covered, draws the sprite, and hands the union
// of the old and new cursor rectangles to UnlockPrimary to be shown.
// FUNCTION: 0x4c25e0
void __stdcall RedrawMouseCursor(Obj_004c2380* app)
{
    // info doubles as the first rect for BlitSurface: no separate rect local.
    Surface_004c23e0 info;
    RECT r1;
    RECT r2;
    POINT pt;
    if (app->visible == 0)
        return;
    if (LockPrimary(&info) == 0)
        return;
    GetCursorPos(&pt);
    int x = pt.x;
    int y = pt.y;
    app->rect.x = x;
    app->rect.y = y;
    x -= app->sprite->dx;
    y -= app->sprite->dy;
    app->under->width = app->sprite->width;
    app->under->height = app->sprite->height;
    app->under->pitch = app->sprite->width;
    app->work->width = app->sprite->width;
    app->work->height = app->sprite->height;
    app->work->pitch = app->sprite->width;
    BlitSurface(app->under, &info, -x, -y);
    BlitSurface(app->under, app->saved, app->x - x, app->y - y);
    BlitSurface(app->work, app->under, 0, 0);
    ResetClipRect(app->work);
    DrawFrame(app->work, app->sprite, app->sprite->dx, app->sprite->dy);
    BlitSurface(app->saved, app->work, x - app->x, y - app->y);
    r1.left = app->x;
    r1.top = app->y;
    // Right and bottom computed from the just-stored left and top fields.
    r1.right = r1.left + app->saved->width;
    r1.bottom = app->saved->height + r1.top;
    r2.left = x;
    r2.top = y;
    r2.right = x + app->sprite->width;
    r2.bottom = y + app->sprite->height;
    BlitSurface(&info, app->saved, app->x, app->y);
    BlitSurface(&info, app->work, x, y);
    app->saved->width = app->sprite->width;
    app->saved->height = app->sprite->height;
    app->saved->pitch = app->sprite->width;
    BlitSurface(app->saved, app->under, 0, 0);
    app->x = x;
    app->y = y;
    UnlockPrimary(&info, &r1, &r2);
}

// Moves the cursor towards the position: decrements the hide counter, reads
// the cursor, stores it as the object's rectangle, offsets the object by the
// bitmap's half size, blits it from the screen with DrawSurface, then, if the
// counter is still not positive, copies the rectangle and draws the bitmap
// with DrawFrame.
// Must be __fastcall: the original file was built with /Gr.
// FUNCTION: 0x4c2870
void __fastcall ShowSoftwareCursor(void)
{
    Obj_004c2380* o = GetDisplay();
    if (o->mode != 1) {
        if (--o->count <= 0) {
            POINT pt;
            GetCursorPos(&pt);
            o->rect.x = pt.x;
            o->rect.y = pt.y;
            if (o->sprite != 0) {
                o->x = pt.x - o->sprite->dx;
                o->y = pt.y - o->sprite->dy;
                o->saved->width = o->sprite->width;
                o->saved->height = o->sprite->height;
                o->saved->pitch = o->sprite->width;
            }
            DrawSurface(o->saved, 0, -o->x, -o->y);
            Obj_004c2380* o2 = GetDisplay();
            if (o2->mode != 1 && o2->count <= 0) {
                Event_4c2d60 r;
                GetRect(&r);
                DrawFrame(0, o2->sprite, r.x, r.y);
            }
        }
    }
}

// The worker thread: redraws the cursor every 0x21 ticks until the stop flag
// at +0x1d6 is set, then clears it.
// FUNCTION: 0x4c2990
void __cdecl MouseThreadProc(int param)
{
    SetThreadPriority(GetCurrentThread(), 2);
    while (*(int*)(param + 0x1d6) == 0) {
        int start = GetTickCount() + 0x21;
        LONG held = LockMouse();
        if (*(int*)(param + 0x1b2) != 0)
            RedrawMouseCursor((Obj_004c2380*)param);
        UnlockMouse(held);
        Sleep(max(1, start - (int)GetTickCount()));
    }
    *(int*)(param + 0x1d6) = 0;
}

// Starts the worker thread (0x4c2990) that StopMouseThread stops.
// FUNCTION: 0x4c2a80
int __stdcall StartMouseThread(Obj_004c2380* s)
{
    s->pending = 0;
    s->running = StartThread((void*)MouseThreadProc, 0x8000, s);
    if (s->running) {
        s->mode = 1;
        return 1;
    }
    return 0;
}

// Asks the worker to stop and waits up to 20 x 100 ms for it to acknowledge.
// FUNCTION: 0x4c2ac0
int __stdcall StopMouseThread(Obj_004c2380* s)
{
    if (s->running == 0)
        return 1;
    s->pending = 1;
    int tries = 0;
    while (s->pending != 0) {
        if (++tries > 20)
            return 0;
        SleepMilliseconds(100);
    }
    s->running = 0;
    return 1;
}

// Stores `value` at +0x1b2 of the object GetDisplay returns while holding
// the "MAIN" spin lock; a lock already held by "MAIN" is not released here.
// FUNCTION: 0x4c2b20
void __stdcall SetCursorSprite(int value)
{
    LONG held = LockMain();
    GetDisplay()->sprite = (GafFrame*)value;
    UnlockMain(held);
}

// Returns the shared cursor bitmap pointer.
// FUNCTION: 0x4c2ba0
int GetCursorSprite()
{
    return (int)GetDisplay()->sprite;
}

// Resets the event ring buffer's head and tail.
// FUNCTION: 0x4c2bb0
void ClearMouseEventQueue()
{
    Obj_004c2380* p = GetDisplay();
    p->head = 0;
    p->tail = 0;
}

// Initialises the mouse-event input object: allocates the event queue,
// resets it, creates the three save-mouse surfaces, and optionally starts
// the worker thread at 0x4c2990. The second GetDisplay call is ClearMouseEventQueue
// inlined.
// FUNCTION: 0x4c2bd0
void __stdcall InitMouse(int count, int start)
{
    Obj_004c2380* p = (Obj_004c2380*)GetDisplay();
    p->capacity = count;
    p->entries = (Event_4c2d60*)GameAllocIgnoreTag("MOUSE EVENTS", count * 0x18);
    Obj_004c2380* q = (Obj_004c2380*)GetDisplay();
    q->head = 0;
    q->tail = 0;
    p->sprite = 0;
    p->saved = (Desc_004c25e0*)AllocSurface("SAVEMOUSE 1", 0x640, 1);
    p->under = (Desc_004c25e0*)AllocSurface("SAVEMOUSE 2", 0x640, 1);
    p->work = (Desc_004c25e0*)AllocSurface("SAVEMOUSE 3", 0x640, 1);
    p->count = 1;
    p->visible = 0;
    p->mode = 0;
    p->pending = 0;
    if (start == 1) {
        p->pending = 0;
        p->running = StartThread((void*)MouseThreadProc, 0x8000, p);
        if (p->running != 0)
            p->mode = 1;
    } else {
        p->running = 0;
    }
}

// Shuts down the object GetDisplay returns: when flag bit 10 is set and
// the worker at +0x1ca is running, asks it to stop (+0x1d6) and waits up to
// 20 x 100 ms for it to acknowledge; then releases the three objects at
// +0x1be..+0x1c6 and frees the event buffer at +0x18a.
// FUNCTION: 0x4c2cc0
void ShutdownMouse()
{
    Obj_004c2380* o = GetDisplay();
    if (o->entries) {
        // Separate nested ifs, not `&&`: it changes how the bit is tested.
        if (o->active) {
            if (o->running) {
                o->pending = 1;
                int i = 0;
                // while (1) keeps the loop test at the top.
                while (1) {
                    if (++i > 20)
                        break;
                    SleepMilliseconds(100);
                    if (!o->pending) {
                        o->running = 0;
                        break;
                    }
                }
            }
        }
        FreeSurface(o->work);
        FreeSurface(o->under);
        FreeSurface(o->saved);
        GameFreeThunk(o->entries);
        o->entries = 0;
    }
}

// FUNCTION: 0x4c2d60
int __stdcall PopMouseEvent(Event_4c2d60* out)
{
    Obj_004c2380* q = GetDisplay();
    if (q->head == q->tail) {
        *out = q->rect;
        return 0;
    }
    *out = q->entries[q->tail];
    if (++q->tail == q->capacity) {
        q->tail = 0;
    }
    return 1;
}

// Peeks at the next event in the queue without removing it (compare the
// pop at 0x4c2d60).
// FUNCTION: 0x4c2de0
int __stdcall PeekMouseEvent(Event_4c2d60* out)
{
    Obj_004c2380* q = GetDisplay();
    if (q->head == q->tail) {
        *out = q->rect;
        return 0;
    }
    *out = q->entries[q->tail];
    return 1;
}

// Pushes an event onto the queue unless it is full (compare the pop at
// 0x4c2d60).
// FUNCTION: 0x4c2e30
void __stdcall PushMouseEvent(Event_4c2d60* ev)
{
    Obj_004c2380* q = GetDisplay();
    if ((q->head + 1) % q->capacity != q->tail) {
        q->entries[q->head] = *ev;
        if (++q->head == q->capacity) {
            q->head = 0;
        }
    }
}
