// Decompiled by Opus, Sonnet, Haiku, space-bunny-free, GPT-6.1-sol, DeepSeek V4.1 Flash. Names are provisional.
// The GUI library's key ring buffer, the virtual-key translation and the
// cursor helpers.
#include <windows.h>
#include <string.h>

#pragma pack(push, 2)
struct Queue_004c1ab0 {
    char unknown_0[0xf2];
    int size;                          // +0xf2
    int entries[30];                   // +0xf6
    int head;                          // +0x16e
    int tail;                          // +0x172
};
#pragma pack(pop)

#pragma pack(push, 1)
struct DisplayContext
{
    char unknown_0[0x1d2];
    int field_at_0x1d2;
};
#pragma pack(pop)

Queue_004c1ab0* GetDisplay(void);

extern int g_gfxBlitLockHeld;
extern HANDLE g_gfxBlitLockEvent;

// Empties the ring buffer (sibling of 0x4c1a60, which also resets head/tail).
// FUNCTION: 0x4c1a40
void ClearKeyQueue(void)
{
    Queue_004c1ab0* q = GetDisplay();
    q->head = 0;
    q->tail = 0;
}

// Sibling of 0x4c1ab0/0x4c1b00/0x4c1b20: (re)initialises the ring buffer,
// clamping the requested size to the buffer's capacity (30 entries).
// FUNCTION: 0x4c1a60
void __stdcall InitKeyQueue(int size)
{
    Queue_004c1ab0* q = GetDisplay();
    if (size <= 0x1e)
        q->size = size;
    else
        q->size = 0x1e;

    q = GetDisplay();
    q->head = 0;
    q->tail = 0;
}

// FUNCTION: 0x4c1aa0
void DestroyPreCleanup(void)
{
}

// Pops the next entry from a small ring buffer (0 when empty); 0x4c1b00
// peeks at it and 0x4c1b20 pushes.
// FUNCTION: 0x4c1ab0
int PopKey(void)
{
    Queue_004c1ab0* q = GetDisplay();
    int v;
    if (q->head == q->tail) {
        v = 0;
    } else {
        // Next index in a local, entry read as entries[next - 1]: sets the load order.
        int next = q->tail + 1;
        v = q->entries[next - 1];
        q->tail++;
        if (q->tail == q->size)
            q->tail = 0;
    }
    return v;
}

// FUNCTION: 0x4c1b00
int __cdecl PeekKey()
{
    Queue_004c1ab0* q = GetDisplay();
    int field_16e = q->head;
    int field_172 = q->tail;
    if (field_16e == field_172) {
        return 0;
    }
    return q->entries[field_172];
}

// Pushes an entry onto the small ring buffer that 0x4c1ab0 pops and 0x4c1b00
// peeks at; the entry is dropped when the buffer is full.
// 0x4c1d50 calls this out of line at the variable-key sites; only its own
// static inline clone (PushKey) is expanded, for the constant-key cases.
// Must not be auto-inlined: 0x4c1d50 calls it out of line.
#pragma auto_inline(off)
// FUNCTION: 0x4c1b20
void __stdcall PushKeyCode(int v)
{
    Queue_004c1ab0* q = GetDisplay();
    // Store indexes with `next`, but the full test recomputes q->head + 1 itself.
    int next = q->head + 1;
    if ((q->head + 1) % q->size != q->tail) {
        q->entries[next - 1] = v;
        q->head++;
        if (q->head == q->size)
            q->head = 0;
    }
}
#pragma auto_inline(on)

// IsKeyDown: maps a key code to a Windows virtual key code and reports
// whether it is held down. GetAsyncKeyState is a short, so the helper keeps a
// short result: that is what makes the compiler test the low word with
// `neg ax` and only mask the low byte. The cases are listed in the order the
// original emits their blocks; the jump table sorts the case values itself.
static short key_state(int vk)
{
    return GetAsyncKeyState(vk) & 0xfffe;
}

// FUNCTION: 0x4c1b80
int __stdcall IsKeyDown(int key)
{
    switch (key)
    {
    case 0xfb:  return key_state(0x12) != 0;
    case 0xf9:  return key_state(0x10) != 0;
    case 0xfa:  return key_state(0x11) != 0;
    case 0xf5:  return key_state(0x26) != 0;
    case 0xf7:  return key_state(0x28) != 0;
    case 0xf4:  return key_state(0x25) != 0;
    case 0xf6:  return key_state(0x27) != 0;
    case 0x20:  return key_state(0x20) != 0;
    }
    return 0;
}

// Key handler: translates a virtual key to the game's internal key code and
// pushes it onto the small ring buffer (0x4c1ab0/0x4c1b00/0x4c1b20). Cases
// 0x21..0x2e push a constant, so MSVC inlines the push helper there and tail
// merges the identical head-adjust blocks; the remaining sites call it out of
// line.

// PushKeyCode, inlined at the constant-key cases.
static inline void PushKey(int v)
{
    Queue_004c1ab0* q = GetDisplay();
    int next = q->head + 1;
    if ((q->head + 1) % q->size == q->tail)
        return;
    q->entries[next - 1] = v;
    q->head++;
    if (q->head == q->size)
        q->head = 0;
}

// FUNCTION: 0x4c1d50
void __stdcall HandleVirtualKey(int key, int flag)
{
    int ctrl = key_state(VK_CONTROL) != 0;

    switch (key) {
    case VK_UP:     PushKey(0xf5); return;
    case VK_DOWN:   PushKey(0xf7); return;
    case VK_LEFT:   PushKey(0xf4); return;
    case VK_RIGHT:  PushKey(0xf6); return;
    case VK_INSERT: PushKey(0xee); return;
    case VK_DELETE: PushKey(0xef); return;
    case VK_HOME:   PushKey(0xf0); return;
    case VK_END:    PushKey(0xf1); return;
    case VK_PRIOR:  PushKey(0xf2); return;
    case VK_NEXT:   PushKey(0xf3); return;
    case VK_PAUSE:  PushKeyCode(0xf8); return;
    case VK_F1:     PushKeyCode(ctrl ? 0xce : 0xe2); return;
    case VK_F2:     PushKeyCode(ctrl ? 0xcf : 0xe3); return;
    case VK_F3:     PushKeyCode(ctrl ? 0xd0 : 0xe4); return;
    case VK_F4:     PushKeyCode(ctrl ? 0xd1 : 0xe5); return;
    case VK_F5:     PushKeyCode(ctrl ? 0xd2 : 0xe6); return;
    case VK_F6:     PushKeyCode(ctrl ? 0xd3 : 0xe7); return;
    case VK_F7:     PushKeyCode(ctrl ? 0xd4 : 0xe8); return;
    case VK_F8:     PushKeyCode(ctrl ? 0xd5 : 0xe9); return;
    case VK_F9:     PushKeyCode(ctrl ? 0xd6 : 0xea); return;
    case VK_F10:    PushKeyCode(ctrl ? 0xd7 : 0xeb); return;
    case VK_F11:    PushKeyCode(ctrl ? 0xd8 : 0xec); return;
    case VK_F12:    PushKeyCode(ctrl ? 0xd9 : 0xed); return;
    }

    if (flag == 0 && ctrl == 0)
        return;

    if (key >= '0' && key <= '9') {
        if (ctrl)
            PushKeyCode(key + 0x94);
        else
            PushKeyCode(key);
        return;
    }
    if (key >= 'A' && key <= 'Z') {
        if (ctrl)
            PushKeyCode(key + 0x69);
        else
            PushKeyCode(key + 0x20);
        return;
    }
    if (key >= 0xba && key <= 0xc0) {
        char tbl[7] = {0x3b, 0x3d, 0x2c, 0x2d, 0x2e, 0x2f, 0x60};
        PushKeyCode(tbl[key - 0xba]);
        return;
    }
    if (key >= 0xdb && key <= 0xde) {
        char tbl[4] = {0x5b, 0x5c, 0x5d, 0x27};
        PushKeyCode(tbl[key - 0xdb]);
        return;
    }
}

// FUNCTION: 0x4c22a0
void InitBlitLock()
{
    g_gfxBlitLockHeld = 0;
    if (g_gfxBlitLockEvent) {
        ResetEvent(g_gfxBlitLockEvent);
        return;
    }
    g_gfxBlitLockEvent = CreateEventA(NULL, FALSE, FALSE, NULL);
}

// FUNCTION: 0x4c22d0
void __stdcall SetCursorOverlayEnabled(int param)
{
    DisplayContext* obj = (DisplayContext*)GetDisplay();
    obj->field_at_0x1d2 = param;
}

// FUNCTION: 0x4c22f0
void __stdcall SetCursorPosition(int x, int y)
{
    SetCursorPos(x, y);
}

// FUNCTION: 0x4c2310
void __stdcall GetCursorPosition(int* x, int* y)
{
    POINT pt;
    GetCursorPos(&pt);
    *x = pt.x;
    *y = pt.y;
}

// FUNCTION: 0x4c2340
void __stdcall GetCurrentMouseEvent(int* param_1)
{
    int ptr = (int)GetDisplay();
    memcpy(param_1, (void*)(ptr + 0x196), 6 * 4);
}

// FUNCTION: 0x4c2360
void __stdcall SetCurrentMouseEvent(int* param_1)
{
    int eax = (int)GetDisplay();
    int edi = eax + 0x196;
    memcpy((void*)edi, param_1, 24);
}
