// Decompiled by Sonnet. Names are provisional.

struct Rect_004b6720 {
    int left;
    int top;
    int right;
    int bottom;
};

// FUNCTION: 0x4b6720
int __stdcall PointInRect(Rect_004b6720* r, int x, int y)
{
    if (x < r->left || x > r->right)
        return 0;
    if (y < r->top || y > r->bottom)
        return 0;
    return 1;
}
