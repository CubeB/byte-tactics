// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Fit `text` into `limit` pixels: if it is too wide, chop characters off the
// end until it (plus the "..." marker, when flag is set) fits, then append the
// marker. Charset -1 means the default font (FUN_004a5030), anything else
// selects the holder entry (FUN_004a1810) and measures with GetTextWidth.
//
// The two locals `dots` and `width` must be declared in this order even though
// `dots` is only assigned after `width` is computed. With `int width;` first
// MSVC keeps width in edi at the truncation test and emits `lea edx,[ebp+edi]`
// (254 bytes, 89.6%); with `int dots;` declared first the allocator knows width
// is dead there and emits `add edi,ebp` as the original does.
#include <string.h>

struct Entry_004a1810;

struct Holder_004ac610 {
    int unknown_0;                     // +0x00
    void* entries;                     // +0x04
};

struct Class_004ac610 {
    char unknown_0[0x18];
    Holder_004ac610* holder;           // +0x18
};

void __stdcall FUN_004a1810(Entry_004a1810* entries, int index);
int __stdcall FUN_004a5030(unsigned char* text);
int GetFont();
int __stdcall GetTextWidth(int font, unsigned char* text);

static inline int Measure_004ac610(unsigned char* text, int charset)
{
    if (charset == -1)
        return FUN_004a5030(text);
    return GetTextWidth(GetFont(), text);
}

// FUNCTION: 0x4ac610
void __stdcall FUN_004ac610(Class_004ac610* obj, unsigned char* text, int limit,
                            int charset, int flag)
{
    void* entries = obj->holder->entries;
    if (charset != -1)
        FUN_004a1810((Entry_004a1810*)entries, charset);

    int dots;
    int width;
    if (charset == -1) {
        width = FUN_004a5030(text);
        if (width < limit)
            return;
    } else {
        width = GetTextWidth(GetFont(), text);
        if (width < limit)
            return;
    }

    dots = flag ? Measure_004ac610((unsigned char*)"...", charset) : 0;

    unsigned char* end = text;
    while (*end)
        end++;

    while (width + dots > limit) {
        if (end == text)
            break;
        end--;
        *end = 0;
        width = Measure_004ac610(text, charset);
    }

    if (flag)
        strcpy((char*)end, "...");
}
