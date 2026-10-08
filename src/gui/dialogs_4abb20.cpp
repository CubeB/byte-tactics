// Decompiled by Opus, deepseek-v4.1-flash, Haiku, Sonnet, space-bunny-free, LongCat 2.5 Preview Free, GPT-6.1-sol, mimo-v2.6-pro, claude-opus-5-5, GPT-6, Claude Opus 5.5, DeepSeek V4.1 Flash and Space Bunny Free. Names are provisional.
//
// The dialog creators and handlers (CONFIRM.GUI, YESORNO.GUI, MSGBOX.GUI,
// NOTEXIST.GUI, CHOICE3.GUI, INPUT.GUI), the word-wrap and text-fit helpers,
// and the 16x16 "COLS" colour grid of the palette editor.
//
// <math.h> is kept for its symbol count: the palette helpers' register
// allocation follows the file's declaration count (docs/c2-regalloc.md).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#pragma pack(push, 1)

struct Menu_004abb20;

// The 0x15b-byte GUI control record: entry 0 holds the entry count at +0xb6
// and the layer's own fields from +0xbc on, the other entries hold their NUL
// terminated text there.
struct Entry_004abb20 {
    unsigned char type;            // +0x00
    char unknown_01[0x13 - 0x01];
    short x;                       // +0x13
    short y;                       // +0x15
    short w;                       // +0x17
    short h;                       // +0x19
    int flags;                     // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        struct {                   // entry 0: the count and the dialog's fields
            short count;           // +0xb6
            char unknown_b8[0xbc - 0xb8];
            void* surface;         // +0xbc
            char unknown_c0[0xcc - 0xc0];
            char name_cc[0x10];    // +0xcc
            char name_dc[0x10];    // +0xdc
            char unknown_ec[0x140 - 0xec];
            unsigned short field_140; // +0x140
            char unknown_142[0x15b - 0x142];
        };
        char text[0x15b - 0xb6];   // +0xb6, the other entries' text
    };
};

// The layer a dialog's +0x18 points at: the entry table at +4 and the
// installed handler at +8.
struct Layer_004abb20 {
    Layer_004abb20* next;          // +0x00
    Entry_004abb20* entries;       // +0x04
    void (__stdcall* handler)(Menu_004abb20*); // +0x08
};

// The dialog: the layer at +0x18 and the current entry index at +0x60.
struct Menu_004abb20 {
    char unknown_0[0x18];
    Layer_004abb20* layer;         // +0x18
    char unknown_1c[0x60 - 0x1c];
    int field_60;                  // +0x60
};

// One cell of the 16x16 "COLS" grid gadget.
struct Rect_004abb20 {
    int left;
    int top;
    int right;
    int bottom;
};

// One palette entry.
struct PalEntry_004abb20 {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char flags;
};

// The palette copy and the remap table built from it.
struct Palette_004abb20 {
    char unknown_0[0xb2];
    PalEntry_004abb20 dest[256];   // +0xb2
    char unknown_4b2[0x400];
    unsigned char table[256];      // +0x8b2
};

// One channel slider: its value byte at +0x140.
struct Slider_004abb20 {
    char unknown_0[0x140];
    unsigned char value;           // +0x140
};

// The palette editor dialog: the selected entry at +0x9b2 and the three
// sliders at +0xcb6.
struct PaletteDialog_004abb20 {
    char unknown_0[0x9b2];
    int index;                     // +0x9b2
    char unknown_9b6[0xcb6 - 0x9b6];
    Slider_004abb20* red;          // +0xcb6
    Slider_004abb20* green;        // +0xcba
    Slider_004abb20* blue;         // +0xcbe
};

#pragma pack(pop)

extern char DAT_00502ae8[];        // "OK"

Layer_004abb20* __stdcall LoadGuiLayer(Menu_004abb20* menu, const char* name, int flags);
int __stdcall FindGadgetIndex(Entry_004abb20* entries, const char* name, int type);
int __stdcall IsGadgetNamed(Entry_004abb20* entries, int index, char* name);
char* __stdcall Translate(char* text);
char* __stdcall WordWrapText(Menu_004abb20* menu, char* text, int width, int index);
int __stdcall RenderLayer(Menu_004abb20* menu, unsigned int flags);
void __stdcall AddTextGadget(Layer_004abb20* layer, char* type, char* text, int x, short y,
                            int width, int attr);
int __stdcall GetFontHeight();
int __stdcall GetTextPixelWidth(unsigned char* text);
int GetScreenWidth();
int GetScreenHeight();
void __stdcall FUN_004a0570(Menu_004abb20* menu, const char* name, int flag);
void __stdcall FUN_0049fb10(Menu_004abb20* menu, int flag);
void __stdcall FUN_0049fa90(Menu_004abb20* menu);
void __stdcall UpdateMenu(Menu_004abb20* menu);
void __cdecl FUN_004d85a0(void* p);
void* __cdecl FUN_004d83b0(char* name, unsigned int size);
int __stdcall SelectFontForEntry(Entry_004abb20* entries, int index);
int GetFont();
int __stdcall GetTextWidth(int font, unsigned char* text);
int __stdcall FillRectangle(void* surface, Rect_004abb20* rect, int color);
int __stdcall SetPaletteColors(unsigned char* src, int start, int count);

struct FileHandle;
unsigned int __stdcall HAPI_WriteFile(FileHandle* file, void* data, unsigned int size);

void __stdcall YesNoDialogHandler(Menu_004abb20* menu);
void __stdcall MessageBoxHandler(Menu_004abb20* menu);
void __stdcall NotExistDialogHandler(Menu_004abb20* menu);
void __stdcall Choice3DialogHandler(Menu_004abb20* menu);
void __stdcall InputDialogHandler(Menu_004abb20* menu);

// Opens the confirmation dialog (CONFIRM.GUI) and sets its title text.
// FUNCTION: 0x4abb20
int __stdcall OpenConfirmDialog(Menu_004abb20* sub, char* title)
{
    Layer_004abb20* layer = LoadGuiLayer(sub, "CONFIRM.GUI", 0);
    if (layer) {
        Entry_004abb20* gadgets = layer->entries;
        int i = FindGadgetIndex(gadgets, "TITL", 5);
        strcpy(gadgets[i].text, title);
        gadgets[i].x = -1;
        return 1;
    }
    return 0;
}

// Same shape as 0x4ac300: tests the current GUI entry against "CHC1" and
// "CHC2".
// FUNCTION: 0x4abba0
void __stdcall YesNoDialogHandler(Menu_004abb20* gadget)
{
    Entry_004abb20* entries = gadget->layer->entries;
    int index = gadget->field_60;
    IsGadgetNamed(entries, index, "CHC1");
    IsGadgetNamed(entries, index, "CHC2");
}

// Opens the YESORNO.GUI dialog, fills its CHC1 / CHC2 / TITL text fields and
// installs YesNoDialogHandler as the handler. Same shape as 0x4abb20 / 0x4ac130.
// FUNCTION: 0x4abbd0
int __stdcall OpenYesNoDialog(Menu_004abb20* sub, char* param_2, char* param_3, char* param_4)
{
    Layer_004abb20* layer = LoadGuiLayer(sub, "YESORNO.GUI", 0x800);
    if (layer) {
        Entry_004abb20* entries = sub->layer->entries;
        Entry_004abb20* p1 = &entries[FindGadgetIndex(entries, "CHC1", 1)];
        Entry_004abb20* p2 = &entries[FindGadgetIndex(entries, "CHC2", 1)];
        Entry_004abb20* p3 = &entries[FindGadgetIndex(entries, "TITL", 5)];
        strcpy(p1->text, param_4);
        strcpy(p2->text, param_3);
        strcpy(p3->text, param_2);
        layer->handler = YesNoDialogHandler;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4abd00
void __stdcall MessageBoxHandler(Menu_004abb20* menu)
{
    Layer_004abb20* layer = menu->layer;
    IsGadgetNamed(layer->entries, menu->field_60, DAT_00502ae8);
}

// Returns 1 when the object's entry is named "MSGBOX.GUI" (after a
// two-character prefix); 0 when it has no entry.
// FUNCTION: 0x4abd20
int __stdcall IsMessageBoxScreen(Menu_004abb20* obj)
{
    if (!obj->layer)
        return 0;
    return strcmp((char*)obj->layer->entries + 2, "MSGBOX.GUI") == 0;
}

// Builds the MSGBOX.GUI dialog: word-wraps the (translated) message text, adds
// one TEXT entry per line, sizes the dialog from the longest line, centres it
// on the screen, stretches every text entry to the dialog width, positions the
// OK button and installs MessageBoxHandler as the handler.
//
// Suspected original bug: the max-line-width loop measures the entries at
// entries[count+1] .. entries[count+lines] (0x4abeb8 walks forward from
// count+1, one 0x15b step per line), while the text entries just added live at
// entries[count-lines+1] .. entries[count], so it measures one entry past the
// last line it added.
// Note: the two "OK" strings go to the table base's fields at +0xcc and +0xdc
// (0x4abfba, 0x4abfd8 address them from the table pointer, not from the entry
// that FindGadgetIndex returned, unlike the x/y fields just above them). If those
// are entry 0's own button-name fields that is deliberate, otherwise the OK
// entry is left unnamed.
// FUNCTION: 0x4abd90
int __stdcall OpenMessageBox(Menu_004abb20* gui, char* text, int wrapWidth, int centre, int autoHeight)
{
    Layer_004abb20* layer = LoadGuiLayer(gui, "MSGBOX.GUI", 0x800);
    if (layer) {
        char name[0x100];
        char buf[0x100];
        strcpy(name, Translate(text));
        char* wrapped = WordWrapText(gui, name, wrapWidth, -1);
        RenderLayer(gui, 2);
        strncpy(buf, wrapped, 0xfe);
        Entry_004abb20* entries = gui->layer->entries;
        char* line = strtok(buf, "\n");
        int lines = 0;
        int y = 0x14;
        int next = layer->entries->count + 1;
        if (line) {
            do {
                AddTextGadget(layer, "TEXT", line, 0, y, -1, 2);
                y += GetFontHeight() + 5;
                line = strtok(0, "\n");
                lines++;
            } while (line);
        }
        int width;
        if (autoHeight) {
            width = 0;
            if (lines > 0) {
                char* p = entries[next].text;
                int i = lines;
                do {
                    if (width <= GetTextPixelWidth((unsigned char*)p))
                        width = GetTextPixelWidth((unsigned char*)p);
                    p += 0x15b;
                } while (--i);
            }
            width += 0x14;
        } else {
            width = wrapWidth;
        }
        entries->w = width;
        entries->h = lines * 25 + entries[1].h + 0x28;
        entries->x = (GetScreenWidth() - entries->w) / 2;
        entries->y = (GetScreenHeight() - entries->h) / 2;
        int i;
        for (i = 0; i <= entries->count; i++) {
            if (entries[i].type == 5) {
                entries[i].w = entries->w;
                entries[i].flags = 2;
            }
        }
        if (centre) {
            int k = FindGadgetIndex(entries, "OK", 0xe);
            if (k != -1) {
                entries[k].y = entries->h - entries[k].h - 0xf;
                entries[k].x = entries->w - entries[k].w - 0xf;
                strcpy(entries->name_cc, "OK");
                strcpy(entries->name_dc, "OK");
            }
        } else {
            FUN_004a0570(gui, "OK", 0);
        }
        FUN_0049fb10(gui, 1);
        RenderLayer(gui, 1);
        layer->handler = MessageBoxHandler;
        FUN_0049fa90(gui);
        UpdateMenu(gui);
        FUN_004d85a0(wrapped);
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4ac080
void __stdcall NotExistDialogHandler(Menu_004abb20* obj)
{
    IsGadgetNamed(obj->layer->entries, obj->field_60, DAT_00502ae8);
}

// Opens the "file does not exist" dialog (NOTEXIST.GUI), puts the name in
// its NAME gadget and installs NotExistDialogHandler as its handler; compare 0x4abb20.
// FUNCTION: 0x4ac0a0
int __stdcall OpenNotExistDialog(Menu_004abb20* sub, char* name)
{
    Layer_004abb20* dialog = LoadGuiLayer(sub, "NOTEXIST.GUI", 0);
    if (dialog) {
        Entry_004abb20* gadgets = sub->layer->entries;
        int i = FindGadgetIndex(gadgets, "NAME", 5);
        strcpy(gadgets[i].text, name);
        gadgets[i].x = -1;
        dialog->handler = NotExistDialogHandler;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4ac130
void __stdcall Choice3DialogHandler(Menu_004abb20* param_1)
{
    Entry_004abb20* p = param_1->layer->entries;
    FindGadgetIndex(p, "CHC1", 0xe);
    FindGadgetIndex(p, "CHC2", 0xe);
    FindGadgetIndex(p, "CHC3", 0xe);
}

// Creates the CHOICE3.GUI dialog, copies the title and three choice strings
// into the CHC1 / CHC2 / CHC3 / TITL gadget entries (each 0x15b bytes, text
// at +0xb6), clears the TITL entry's +0x13 field and installs Choice3DialogHandler
// as the dialog handler. Returns 1 if the dialog was created, else 0.
// FUNCTION: 0x4ac170
int __stdcall OpenChoice3Dialog(Menu_004abb20* menu, const char* title, const char* choice1,
                           const char* choice2, const char* choice3)
{
    Layer_004abb20* dialog = LoadGuiLayer(menu, "CHOICE3.GUI", 0);
    if (dialog != 0) {
        Entry_004abb20* entries = menu->layer->entries;
        RenderLayer(menu, 1);
        Entry_004abb20* e1 = &entries[FindGadgetIndex(entries, "CHC1", 1)];
        Entry_004abb20* e2 = &entries[FindGadgetIndex(entries, "CHC2", 1)];
        Entry_004abb20* e3 = &entries[FindGadgetIndex(entries, "CHC3", 1)];
        Entry_004abb20* et = &entries[FindGadgetIndex(entries, "TITL", 5)];
        strcpy(e1->text, choice1);
        strcpy(e2->text, choice2);
        strcpy(e3->text, choice3);
        strcpy(et->text, title);
        et->x = 0xffff;
        dialog->handler = Choice3DialogHandler;
        return 1;
    }
    return 0;
}

// FUNCTION: 0x4ac300
void __stdcall InputDialogHandler(Menu_004abb20* param_1)
{
    Entry_004abb20* esi = param_1->layer->entries;
    int edi = param_1->field_60;
    IsGadgetNamed(esi, edi, "CHC1");
    IsGadgetNamed(esi, edi, "CHC2");
    IsGadgetNamed(esi, edi, "INPT");
}

// Opens the player setup dialog (INPUT.GUI), fills the TITL / INPT / CHC1 /
// CHC2 gadget texts and installs InputDialogHandler as its handler. Returns 1 if the
// dialog was created, else 0.
// FUNCTION: 0x4ac340
int __stdcall OpenInputDialog(Menu_004abb20* sub, char* title, char* input, char* chc2, char* chc1, int unused)
{
    Layer_004abb20* dialog = LoadGuiLayer(sub, "INPUT.GUI", 0);
    if (dialog) {
        Entry_004abb20* gadgets = sub->layer->entries;
        Entry_004abb20* g = &gadgets[FindGadgetIndex(gadgets, "INPT", 3)];
        strcpy(g->text, input);
        g = &gadgets[FindGadgetIndex(gadgets, "TITL", 5)];
        strcpy(g->text, title);
        g->x = -1;
        Entry_004abb20* g1 = &gadgets[FindGadgetIndex(gadgets, "CHC1", 1)];
        Entry_004abb20* g2 = &gadgets[FindGadgetIndex(gadgets, "CHC2", 1)];
        strcpy(g1->text, chc1);
        strcpy(g2->text, chc2);
        dialog->handler = InputDialogHandler;
        return 1;
    }
    return 0;
}

// Builds a word-wrapped copy of `text` in a buffer allocated from
// the pool: the number of characters per line is width / (width of one
// digit), and a line is broken at a space, newline or '-' when the measured
// line would exceed `width` pixels. `index` selects the current font entry
// (SelectFontForEntry) when it is not -1; measurements go through the same
// GetTextPixelWidth / GetFont+GetTextWidth pair the sibling text fitter
// 0x4ac610 uses.
// The preheader size estimate `len + 3 * (len / (width / w)) + 2` allocates
// room for the CRLF pairs; the wrap-back loop overwrites the break character
// with the CR, so `s` is only advanced past it once.
// FUNCTION: 0x4ac4c0
char* __stdcall WordWrapText(Menu_004abb20* menu, char* text, int width, int index)
{
    Entry_004abb20* gadgets = menu->layer->entries;
    // Net-zero update: makes the allocator count an early use of text.
    text += 1; text -= 1;
    int len = strlen(text);
    if (index != -1)
        SelectFontForEntry(gadgets, index);
    int w;
    if (index == -1)
        w = GetTextPixelWidth((unsigned char*)"d");
    else
        w = GetTextWidth(GetFont(), (unsigned char*)"d");
    int size = len + 3 * (len / (width / w)) + 2;
    char* buf = (char*)FUN_004d83b0("WordWrap", size);
    memset(buf, 0, size);
    int i = 0;
    char c = *text;
    char* s = text;
    char* p = buf;
    while (c != 0) {
        if (*s == (char)0xff)
            break;
        buf[i] = *s;
        char next = s[1];
        // i is incremented before s: the source order sets the instruction order.
        i++;
        s++;
        if (next == ' ' || next == '\n' || next == '-') {
            int wrapped = 0;
            int m;
            if (index == -1)
                m = GetTextPixelWidth((unsigned char*)p);
            else
                m = GetTextWidth(GetFont(), (unsigned char*)p);
            if (width <= m) {
                buf[i] = 0;
                wrapped = 1;
                char ch = s[-1];
                i--;
                s--;
                while (ch != ' ' && ch != '-') {
                    // Net-zero update: makes the allocator count an extra use of i.
                    i += 1; i -= 1;
                    buf[i] = 0;
                    ch = s[-1];
                    i--;
                    s--;
                }
                buf[i] = '\r';
                i++;
                buf[i] = '\n';
                i++;
                s++;
            }
            if (wrapped)
                p = buf + i;
        }
        c = *s;
        if (c == '\n')
            p = buf + i + 1;
    }
    buf[i] = 0;
    return buf;
}

// Fit `text` into `limit` pixels: if it is too wide, chop characters off the
// end until it (plus the "..." marker, when flag is set) fits, then append the
// marker. Charset -1 means the default font (GetTextPixelWidth), anything else
// selects the holder entry (SelectFontForEntry) and measures with GetTextWidth.
static inline int Measure_004ac610(unsigned char* text, int charset)
{
    if (charset == -1)
        return GetTextPixelWidth(text);
    return GetTextWidth(GetFont(), text);
}

// FUNCTION: 0x4ac610
void __stdcall TruncateTextWithEllipsis(Menu_004abb20* obj, unsigned char* text, int limit,
                            int charset, int flag)
{
    Entry_004abb20* entries = obj->layer->entries;
    if (charset != -1)
        SelectFontForEntry(entries, charset);

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

// Builds a 256-entry remap table: for each colour of the source palette,
// the index of the closest colour (sum of absolute RGB differences) in the
// destination palette.
// The seven unused extern declarations below stay: the declaration count in
// front of 0x4ac710, 0x4ac8c0 and 0x4acbe0 sets their register allocation
// (the file's symbol count; see docs/c2-regalloc.md).
extern int pad4abb20_0;
extern int pad4abb20_1;
extern int pad4abb20_2;
// FUNCTION: 0x4ac710
void __stdcall BuildColorRemapTable(PalEntry_004abb20* src, PalEntry_004abb20* dest, unsigned char* table)
{
    for (int n = 256; n != 0; n--) {
        int i = 0;
        int best = 9999999;
        PalEntry_004abb20* p = dest;
        int b = src->b;
        int g = src->g;
        int r = src->r;
        int bestIndex;
        for (; i < 256; i++, p++) {
            int d = abs(b - p->b) + abs(g - p->g) + abs(r - p->r);
            if (d < best) {
                best = d;
                bestIndex = i;
            }
        }
        src++;
        *table++ = (unsigned char)bestIndex;
    }
}

// Copies a 256-entry palette into the object and then builds a 256-byte remap
// table: for each colour of the copied palette, the index of the closest
// colour (sum of absolute RGB differences) in the source palette.
// FUNCTION: 0x4ac7d0
void __stdcall FUN_004ac7d0(Palette_004abb20* pal, PalEntry_004abb20* src, PalEntry_004abb20* copy)
{
    memcpy(pal->dest, copy, 0x400);

    unsigned char* table = pal->table;
    PalEntry_004abb20* p = pal->dest;
    for (int n = 256; n != 0; n--) {
        int i = 0;
        int best = 9999999;
        PalEntry_004abb20* q = src;
        int bestIndex;
        for (; i < 256; i++, q++) {
            int d = abs(p->r - q->r) + abs(p->b - q->b) + abs(p->g - q->g);
            if (d < best) {
                best = d;
                bestIndex = i;
            }
        }
        p++;
        *table++ = (unsigned char)bestIndex;
    }
}

// FUNCTION: 0x4ac8a0
void __stdcall FUN_004ac8a0(char* obj, void* dest)
{
    memcpy(dest, obj + 0xb2, 0x100 * sizeof(int));
}

// Draws the 16 x 16 palette grid: one 8 x 8 cell per colour, at the "COLS"
// gadget's position.
extern int pad4abb20_3;
// FUNCTION: 0x4ac8c0
void __stdcall FUN_004ac8c0(Menu_004abb20* obj)
{
    // grid before gadgets: operand order of the prologue sums follows symbol order.
    Entry_004abb20 *grid, *gadgets;
    gadgets = obj->layer->entries;
    int index = FindGadgetIndex(gadgets, "COLS", 6);
    grid = &gadgets[index];
    void* surface = gadgets->surface;
    int x0 = grid->x + gadgets->x;
    int y = gadgets->y + grid->y;
    Rect_004abb20 rect;
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            rect.left = x0 + col * 8;
            rect.top = y + row * 8;
            rect.right = rect.left + 7;
            rect.bottom = rect.top + 7;
            FillRectangle(surface, &rect, row * 16 + col);
        }
    }
}

// Writes the red, green and blue bytes of one entry of a palette (4 bytes per
// entry) into the field_140 word of the "RED", "GREN" and "BLUE" GUI entries,
// then refreshes the object's gadget state.
// FUNCTION: 0x4aca20
void __stdcall ShowPaletteEntryRgb(Menu_004abb20* obj, unsigned char* colors, int index)
{
    Entry_004abb20* entries = obj->layer->entries;
    // Entry via a local: addressing entries[i] directly changes the allocation.
    Entry_004abb20* e;
    e = &entries[FindGadgetIndex(entries, "RED", 4)];
    e->field_140 = colors[index * 4];
    e = &entries[FindGadgetIndex(entries, "GREN", 4)];
    e->field_140 = colors[index * 4 + 1];
    e = &entries[FindGadgetIndex(entries, "BLUE", 4)];
    e->field_140 = colors[index * 4 + 2];
    RenderLayer(obj, 4);
}

// Interpolates the 256-entry RGB palette between entry `lo` and entry `hi`.
// Note: the two 0x400-byte copies run on a 768-byte stack buffer (see the bug
// note in the pull request); the original really does emit them, so they stay.
// FUNCTION: 0x4acae0
void __stdcall FUN_004acae0(unsigned char* data, int a, int b)
{
    unsigned char buf[768];
    memcpy(buf, data, 0x400);

    int lo = a < b ? a : b;
    int hi = a > b ? a : b;

    int base[3];
    int delta[3];
    int den = hi - lo;
    int i0 = lo * 3;
    base[0] = buf[i0 + 0];
    base[1] = buf[i0 + 1];
    base[2] = buf[i0 + 2];
    unsigned char* dst = &buf[i0];
    int i1 = hi * 3;
    delta[0] = buf[i1 + 0] - base[0];
    delta[1] = buf[i1 + 1] - base[1];
    delta[2] = buf[i1 + 2] - base[2];

    for (int i = lo; i < hi; i++) {
        for (int j = 0; j < 3; j++) {
            *dst = (unsigned char)(base[j] + delta[j] * (i - lo) / den);
            dst++;
        }
    }

    memcpy(data, buf, 0x400);
}

// Converts a screen position to the index of a cell in the 16x16 "COLS"
// colour grid gadget (row * 16 + column, each cell 8 pixels).
// (stdio.h only for its effect on register allocation; see tools/headers.py)
extern int pad4abb20_4;
extern int pad4abb20_5;
extern int pad4abb20_6;
// FUNCTION: 0x4acbe0
int __stdcall GetColorCellAt(Menu_004abb20* obj, int x, int y)
{
    Entry_004abb20* gadgets = obj->layer->entries;
    int index = FindGadgetIndex(gadgets, "COLS", 6);
    Entry_004abb20* grid = &gadgets[index];
    int col = (x - gadgets->x - grid->x) / 8;
    int row = (y - gadgets->y - grid->y) / 8;
    if (col < 0) {
        col = 0;
    }
    if (col > 15) {
        col = 15;
    }
    if (row < 0) {
        row = 0;
    }
    if (row > 15) {
        row = 15;
    }
    return row * 16 + col;
}

// Copies three slider values into one palette entry and applies it.
// FUNCTION: 0x4acc70
void __stdcall ApplySlidersToPaletteEntry(PaletteDialog_004abb20* obj, PalEntry_004abb20* palette)
{
    PalEntry_004abb20 c;
    c.r = obj->red->value;
    c.g = obj->green->value;
    c.b = obj->blue->value;
    palette[obj->index] = c;
    SetPaletteColors((unsigned char*)&c, obj->index, 1);
}

// Writes `depth` tab characters to a file (sibling of 0x4acda0).
// FUNCTION: 0x4accd0
void __stdcall WriteTabs(FileHandle* file, int depth)
{
    char tab = '\t';
    for (int i = 0; i < depth; i++)
        HAPI_WriteFile(file, &tab, 1);
}
