// Decompiled by Sonnet and Opus. Names are provisional.

struct Rect_004c6b10 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

class Surface {
public:
    char unknown_0[0x1c];
    Rect_004c6b10 clip;      // +0x1c

    Rect_004c6b10* GetClipRect(Rect_004c6b10* out);
    void SetClipRect(Rect_004c6b10 r);
};

// FUNCTION: 0x4c6ae0
Rect_004c6b10* Surface::GetClipRect(Rect_004c6b10* out)
{
    *out = clip;
    return out;
}

// Stores a 16-byte rectangle passed by value into the object at +0x1c.
// FUNCTION: 0x4c6b10
void Surface::SetClipRect(Rect_004c6b10 r)
{
    clip = r;
}
