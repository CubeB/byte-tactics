// Decompiled by deepseek-v4.1-flash. Names are provisional.
// One tick of a UI element: advances the reference at +0x30 when the active
// flag is set and forwards the entry it lands on, then peeks the next event,
// tests its point against the element's rectangle (from the entry at +0x18)
// and, on a hit or a plain code, pops the event into the element at +0x3c.

struct Ref_004ab5d0;
struct Src_004ab5d0;
struct Sub_004ab5d0;
struct Event_004ab5d0;
struct Rect_004ab5d0;

extern void __stdcall AdvanceGafSequence(Ref_004ab5d0* ref, int step);
extern int __stdcall GetGafSequenceFrame(Ref_004ab5d0* ref);
extern void __stdcall FUN_004c2b20(int handle);
extern int __stdcall PeekMouseEvent(Event_004ab5d0* out);
extern void __stdcall FUN_004c2340(Event_004ab5d0* out);
extern void __stdcall PopMouseEvent(Event_004ab5d0* out);
extern void __stdcall FUN_004a1680(char* table, int index, Rect_004ab5d0* out);
extern int __stdcall FUN_004a1920(Rect_004ab5d0* r, int px, int py);

struct Ref_004ab5d0 {
    unsigned short index;              // +0x0
    short value;                       // +0x2
    unsigned char kind;                // +0x4
    char unknown_5[3];
    Src_004ab5d0* src;                 // +0x8
};

struct Sub_004ab5d0 {
    char unknown_0[4];
    char* table;                       // +0x4
};

struct Event_004ab5d0 {
    int data[6];
};

struct Rect_004ab5d0 {
    int x0;                            // +0x0
    int y0;                            // +0x4
    int x1;                            // +0x8
    int y1;                            // +0xc
};

struct Flags_004ab5d0 {
    unsigned int active : 1;
    unsigned int rest : 31;
};

#pragma pack(push, 2)
struct Dialog {
    char unknown_0[0x18];
    Sub_004ab5d0* sub;                 // +0x18
    char unknown_1c[0x30 - 0x1c];
    Ref_004ab5d0 ref;                  // +0x30
    Event_004ab5d0 event;              // +0x3c
    int field_54;                      // +0x54
    char unknown_58[0x5c - 0x58];
    Flags_004ab5d0 flags;              // +0x5c
    char unknown_60[0x9a - 0x60];
    int field_9a;                      // +0x9a
};
#pragma pack(pop)

// FUNCTION: 0x4ab5d0
void __stdcall FUN_004ab5d0(Dialog* p)
{
    if (p->flags.active) {
        int old = p->ref.index;
        AdvanceGafSequence(&p->ref, p->field_9a);
        if (p->ref.index != old)
            FUN_004c2b20(GetGafSequenceFrame(&p->ref));
    }

    Event_004ab5d0 e;
    if (PeekMouseEvent(&e) != 0) {
        if (p->sub != 0) {
            Rect_004ab5d0 r;
            FUN_004a1680(p->sub->table, 0, &r);
            if (FUN_004a1920(&r, e.data[0], e.data[1]) != 0 || e.data[2] == 0) {
                PopMouseEvent(&e);
                p->field_54 = e.data[2];
                p->event = e;
            }
        }
    } else {
        FUN_004c2340(&p->event);
    }
}
