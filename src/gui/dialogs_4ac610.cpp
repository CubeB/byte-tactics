// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Fit `text` into `limit` pixels: if it is too wide, chop characters off the
// end until it (plus the "..." marker, when flag is set) fits, then append the
// marker. Charset -1 means the default font (GetTextPixelWidth), anything else
// selects the holder entry (SelectFontForEntry) and measures with GetTextWidth.
#include <string.h>

struct Entry_004a1810;

struct Holder_004ac610 {
    int unknown_0;                     // +0x00
    void* entries;                     // +0x04
};

struct Dialog {
    char unknown_0[0x18];
    Holder_004ac610* holder;           // +0x18
};

void __stdcall SelectFontForEntry(Entry_004a1810* entries, int index);
int __stdcall GetTextPixelWidth(unsigned char* text);
int GetFont();
int __stdcall GetTextWidth(int font, unsigned char* text);

static inline int Measure_004ac610(unsigned char* text, int charset)
{
    if (charset == -1)
        return GetTextPixelWidth(text);
    return GetTextWidth(GetFont(), text);
}

// FUNCTION: 0x4ac610
void __stdcall TruncateTextWithEllipsis(Dialog* obj, unsigned char* text, int limit,
                            int charset, int flag)
{
    void* entries = obj->holder->entries;
    if (charset != -1)
        SelectFontForEntry((Entry_004a1810*)entries, charset);

    // Declared before width: the allocator then knows width is dead at the truncation test.
    int dots;
    int width;
    if (charset == -1) {
        width = GetTextPixelWidth(text);
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
