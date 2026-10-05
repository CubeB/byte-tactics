// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Key handler: translates a virtual key to the game's internal key code and
// pushes it onto the small ring buffer (0x4c1ab0/0x4c1b00/0x4c1b20). Cases
// 0x21..0x2e push a constant, so MSVC inlines the push helper there and tail
// merges the identical head-adjust blocks; the remaining sites call it out of
// line.
#include <windows.h>

#pragma pack(push, 2)
struct Queue_004c1ab0 {
    char unknown_0[0xf2];
    int size;                          // +0xf2
    int entries[30];                   // +0xf6
    int head;                          // +0x16e
    int tail;                          // +0x172
};
#pragma pack(pop)

Queue_004c1ab0* GetDisplay(void);
void __stdcall PushKeyCode(int v);

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

static short key_state(int vk)
{
    return GetAsyncKeyState(vk) & 0xfffe;
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
