// Decompiled by deepseek-v4.1-flash. Names are provisional.
// PARTIAL, best 58.2%. Clipped 8-bit sprite blit: copies the part of `src`
// that overlaps `dst`, skipping source pixels equal to the source colour key
// and writing the destination colour key instead.
//
// What still differs from the original:
//  1. The x-clip negation. The original computes ecx = x + src->x - dst->x
//     and then `neg ecx` before storing dstCol = -ecx (the value tested by
//     `jns`). Writing `sx = -sx` after two statements lets MSVC 5 fold the
//     negation into the branch and compute dst->x - src->x - x directly, so
//     the `neg` is missing and the two stores to [esp+0x14]/[esp+0x18] move
//     into the branches. Writing the negation separately (so it survives)
//     instead shifts the register allocation: the compiler then puts the
//     scratch difference in ecx and x in ebp (original: difference in edx,
//     x in ecx) and dstRow in ebx (original: edx), which costs more of the
//     function than the folded negation does. This is the version with the
//     higher score; a structurally correct attempt with the neg is 55.4%.
//  2. The row pointers. The original loads src->width into eax, copies it to
//     ebx, moves srcRow into eax and does `imul eax, ebx`, then adds
//     src->data straight from memory; ours hoists src->data into ebx and
//     multiplies width by srcRow (`imul eax, ebp`). Same for the destination.
//  3. The inner copy loop guard: the original tests `n != 0` and counts down
//     (`test ebx, ebx; je`), ours tests `n > 0` and counts up (`jle`).
//
// Suspected original bug: the horizontal copy count `n` is clamped to
// dst->width but dstCol is not subtracted from it, so when dstCol > 0 the
// loop writes up to dstCol bytes past the end of the destination row
// (the vertical clip does account for dstRow). Kept as the original does.

struct Image_004b9d70 {
    unsigned short width;   // +0x0
    unsigned short height;  // +0x2
    short x;                // +0x4
    short y;                // +0x6
    unsigned char colorKey; // +0x8
    char unknown_9[7];      // +0x9
    unsigned char* data;    // +0x10
};

// FUNCTION: 0x4b9d70
void __stdcall FUN_004b9d70(Image_004b9d70* src, Image_004b9d70* dst, int x, int y)
{
    int srcCol, dstCol;
    int sx = x + src->x - dst->x;
    sx = -sx;
    if (sx < 0) {
        srcCol = -sx;
        dstCol = 0;
    } else {
        srcCol = 0;
        dstCol = sx;
    }

    int srcRow, dstRow;
    int sy = dst->y - src->y - y;
    if (sy < 0) {
        srcRow = -sy;
        dstRow = 0;
    } else {
        srcRow = 0;
        dstRow = sy;
    }

    int n = dst->width;
    if (src->width - srcCol <= n)
        n = src->width - srcCol;

    for (; srcRow < src->height; srcRow++, dstRow++) {
        if (dstRow >= dst->height)
            break;
        unsigned char* s = src->data + srcRow * src->width + srcCol;
        unsigned char* d = dst->data + dstRow * dst->width + dstCol;
        for (int i = 0; i < n; i++) {
            if (*s != src->colorKey)
                *d = dst->colorKey;
            s++;
            d++;
        }
    }
}
