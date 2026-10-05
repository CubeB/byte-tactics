// Decompiled by Opus. Names are provisional.
// Stores a 16-byte rectangle passed by value into the object at +0x1c.

struct Rect_004c6b10 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

class Class_004c6b10 {
public:
    char unknown_0[0x1c];
    Rect_004c6b10 rect;                // +0x1c

    void SetClipRect(Rect_004c6b10 r);
};

// FUNCTION: 0x4c6b10
void Class_004c6b10::SetClipRect(Rect_004c6b10 r)
{
    rect = r;
}
