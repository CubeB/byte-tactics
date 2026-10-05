// Decompiled by deepseek-v4.1-flash. Names are provisional.

struct Rect_004bf620 {
    int left;                          // +0x0
    int top;                           // +0x4
    int right;                         // +0x8
    int bottom;                        // +0xc
};

class Surface {
public:
    char unknown_0[0x1c];
    Rect_004bf620 field_1c;            // +0x1c

    Rect_004bf620* GetClipRect(Rect_004bf620* out);
};

// FUNCTION: 0x4bf620
int __stdcall ClipRectangle(Surface* surface, Rect_004bf620* r)
{
    Rect_004bf620 local;
    surface->GetClipRect(&local);
    if (r->right < local.left)
        return 0;
    if (r->left > local.right)
        return 0;
    if (r->bottom < local.top)
        return 0;
    if (r->top > local.bottom)
        return 0;
    if (r->left < local.left)
        r->left = local.left;
    if (r->top < local.top)
        r->top = local.top;
    if (r->right > local.right)
        r->right = local.right;
    if (r->bottom > local.bottom)
        r->bottom = local.bottom;
    if (r->left > r->right)
        return 0;
    return r->top <= r->bottom;
}
