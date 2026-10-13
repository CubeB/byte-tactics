// MouseEvent (Thaldren's MouseEvent): the 0x18-byte record of one mouse
// event, the one GetCurrentMouseEvent (0x4c2340) copies out of the display and
// SetCurrentMouseEvent (0x4c2360) copies back, the point, the key flags, the
// tick, the message and the double-click count. Nothing is included.
#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

struct MouseEvent {
    int x;                             // +0x0
    int y;                             // +0x4
    unsigned int keyFlags;             // +0x8
    int scaledTick;                    // +0xc
    int msg;                           // +0x10
    int dblClick;                      // +0x14
};

#endif
