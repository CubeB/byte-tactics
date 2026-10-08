// Decompiled by space-bunny-free, GPT-6.1-sol, deepseek-v4.1-flash, Sonnet 5.5, GPT-6, deepseek-v4.1, Space Bunny Free, claude-sonnet-5-5, claude-opus-5-5, Opus, Fable 5.1, DeepSeek V4.1 Flash, muse-spark-1.3-free, Claude Sonnet 5.5 and Haiku. Names are provisional.
// The GUI layout engine: draws and handles the gadgets (buttons, list boxes,
// text inputs, sliders, labels) of a screen's 0x15b-byte entry table, renders
// and closes screens, and switches the current GUI context.
// <windows.h> and <math.h> are only for their symbol ids: DrawButton, RenderLayer
// and the list steps match only at this symbol count.
#include <windows.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#pragma pack(push, 1)

struct Glyph {                          // 8 bytes, returned by GetGafFrame
    unsigned short width;               // +0x00
    unsigned short height;              // +0x02
    short xoff;                         // +0x04
    short yoff;                         // +0x06
};

struct GafEntry {                       // 4 bytes: frame table header
    unsigned short count;               // +0x00
    unsigned short unknown_2;           // +0x02
};

struct Gui;

struct Entry {                          // 0x15b bytes, one GUI list entry
    unsigned char type;                 // +0x00
    unsigned char team;                 // +0x01
    char name[0x11];                    // +0x02 (strncpy 0x10)
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    int colours;                        // +0x1f
    int image;                          // +0x23
    char unknown_27;
    signed char tab;                    // +0x28
    unsigned char field_29;             // +0x29
    char unknown_2a;
    void* archive;                      // +0x2b
    GafEntry* gaf;                      // +0x2f
    char helpKey[0xb4 - 0x33];          // +0x33
    unsigned char resourceFlags;        // +0xb4
    char unknown_b5;
    union {
        short count;                    // +0xb6 (entry 0 only)
        char text[0x80];                // +0xb6
        struct {                        // entry 0
            char unknown_b6[2];
            void* saveUnder;            // +0xb8, the SAVE UNDER bitmap
            void* surface;              // +0xbc, the GUI SURFACE bitmap
            void* archive;              // +0xc0
            GafEntry* background;       // +0xc4
        } assets;
        struct {                        // type 12
            char unknown_b6[2];
            Glyph* glyph;               // +0xb8
            unsigned char flag;         // +0xbc
        } frame;
        struct {                        // type 2
            int sortKey;                // +0xb6
            short field_ba;             // +0xba
            short field_bc;             // +0xbc
            short field_be;             // +0xbe
            short field_c0;             // +0xc0
            char* field_c2;             // +0xc2
            char unknown_c6[4];
            GafEntry* gaf;              // +0xca
            void (__stdcall* callback)(Gui*, Entry*);   // +0xce
            char unknown_d2[4];
            union {
                int language;           // +0xd6
                void* filebuf;          // +0xd6, buffer type 7/8 loads
            };
            short scroll;               // +0xda
        } list;
        struct {                        // entry 0
            char unknown_b6[0x16];
            char choice[0x10];          // +0xcc
            char choice2[0x10];         // +0xdc
        } names;
        struct {                        // type 6
            int f_b6;
            int f_ba;
            int f_be;
            int f_c2;
            short f_c6;
        } t6;
        struct {                        // type 13
            char unknown_b6[4];
            int field_ba;               // +0xba
            int field_be;               // +0xbe
            int field_c2;               // +0xc2
            int field_c6;               // +0xc6
            float field_ca;             // +0xca
            int field_ce;               // +0xce
            int field_d2;               // +0xd2
        } anim;
    } u;
    union {                             // +0x136
        short field_136;
        struct {
            unsigned char stage;        // +0x136
            unsigned char stageIndex;   // +0x137
        };
    };
    short field_138;                    // +0x138 (the text length limit of a text input)
    union {                             // +0x13a
        struct {
            unsigned char field_13a;
            unsigned char field_13b;
            unsigned char field_13c;
            char unknown_13d;
        };
        GafEntry* inputGaf;
    };
    union {                             // +0x13e
        struct {
            char unknown_13e[0x142 - 0x13e];
            short sliderThumb;          // +0x142
            char unknown_144[0x14e - 0x144];
            GafEntry* sliderGaf;        // +0x14e
            unsigned char sliderStyle;  // +0x152
        };
        struct {
            char unknown_13e_b[2];
            short field_140;            // +0x140
            char unknown_142_b[2];
            void (__stdcall* callback)(Gui*, int);      // +0x144
            short unknown_148;
            int callbackArg;            // +0x14a
        };
        struct {
            char unknown_13e_c[0x147 - 0x13e];
            unsigned char field_147;    // +0x147
            unsigned char field_148;    // +0x148
        };
    };
    char unknown_153[0x157 - 0x153];
    int field_157;                      // +0x157
};

struct Layer {                          // a screen on the stack
    Layer* next;                        // +0x00
    Entry* entries;                     // +0x04
    void (__stdcall* handler)(Gui*);    // +0x08
    char unknown_0c[4];
    unsigned int flags;                 // +0x10
    int dirty;                          // +0x14
    int field_18;                       // +0x18
    void (__stdcall* cb1c)();           // +0x1c
    int current;                        // +0x20 (entry index, -1 for none)
    void* field_24;                     // +0x24
    char text[0xe];                     // +0x28
    char field_36;                      // +0x36
    char unknown_37[0x3b - 0x37];
    void (__stdcall* cb3b)(Gui*);       // +0x3b
};

struct Language {
    char unknown_0[0xc];
    GafEntry* glyphs;                   // +0x0c
};

struct Point {                          // 24 bytes, copied with rep movsd
    int x;
    int y;
    int unknown_08[4];
};

struct Gui {
    int font;                           // +0x00
    void* gaf;                          // +0x04
    Language* values[3];                // +0x08
    Language* language;                 // +0x14
    Layer* layer;                       // +0x18
    char unknown_1c[0x3c - 0x1c];
    Point point;                        // +0x3c
    char unknown_54[0x60 - 0x54];
    int field_60;                       // +0x60
    int focus;                          // +0x64
    int field_68;                       // +0x68
    int field_6c;                       // +0x6c
    int field_70;                       // +0x70
    char unknown_74[0x78 - 0x74];
    int field_78;                       // +0x78
    char unknown_7c[0x96 - 0x7c];
    int time;                           // +0x96
    int field_9a;                       // +0x9a
    int field_9e;                       // +0x9e
    int field_a2;                       // +0xa2 (nonzero = selection active)
    char unknown_a6[0x8b2 - 0xa6];
    unsigned char colours[0x100];       // +0x8b2
    char unknown_9b2[0x9b6 - 0x9b2];
    char str_9b6[0x100];                // +0x9b6
    char str_ab6[0x100];                // +0xab6
    char str_bb6[0x110];                // +0xbb6
    int field_cc6;                      // +0xcc6
    int changed;                        // +0xcca
    int field_cce;                      // +0xcce
    int field_cd2;                      // +0xcd2
    char field_cd6;                     // +0xcd6
};

#pragma pack(pop)

struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

extern Gui* g_guiContext;
extern char DAT_00502a20[];
extern char DAT_005119b8[];
extern int DAT_0051fbac;
extern int DAT_0051fbb0;
extern int DAT_0051fbb4;

int GetScreenWidth();
int GetScreenHeight();
unsigned int GetTicks();
int PeekKey();
int PopKey();
int __stdcall IsKeyDown(int key);
void ClearKeyQueue();
void HideSoftwareCursor();
void ShowSoftwareCursor();
int __cdecl tolower(int c);
int __cdecl toupper(int c);

void __stdcall SetFont(int id);
int GetFont();
int __stdcall GetTextWidth(int font, char* text);
int GetFontHeight();
void __stdcall SetTextColors(int colour, int font);
int GetTextKeyColor();
Glyph* __stdcall GetGafFrame(GafEntry* table, int index);
GafEntry* __stdcall FindGafEntry(void* gaf, const char* name);
void* __stdcall LoadGaf(char* path);
char* __stdcall ChangeExtension(char* out, char* in, const char* ext);
long __stdcall HAPI_FileLengthByName(char* path);
char* __stdcall HAPI_LoadFile(char* name, int* size);
char* __stdcall Translate(char* key);
char* __stdcall GetGadgetText(Gui* menu, char* name, char* buf);
char* __stdcall SkipTextLines(char* text, int line);

void* __stdcall AllocSurface(char* name, int width, int height);
void __stdcall FreeSurface(void* surface);
void __stdcall DrawSurface(void* dst, void* bmp, int x, int y);
void __stdcall DrawString(void* surface, char* text, int x, int y, int maxw);
int __stdcall FillRectangle(void* surface, Rect* rect, int colour);
void __stdcall GrayRectangle(void* surface, Rect* rect);
void __stdcall FadeRectangle(void* surface, Rect* rect, int level);
void __stdcall DrawLine(void* surface, int x1, int y1, int x2, int y2, int colour);
void __stdcall DrawFrame(void* surface, Glyph* glyph, int x, int y);
void __stdcall DrawFrameLit(void* surface, Glyph* glyph, int x, int y, int style);
void __stdcall FillBevelBox(void* surface, Rect* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FillBevelBoxDarkFirst(void* surface, Rect* rect, unsigned int a, unsigned int b, unsigned int c);
void __stdcall FUN_004a50e0(void* surface, char* text, int x, int y, int maxw, int style);
int __stdcall FUN_004a51d0(void* surface, char* text, int x, int y, int maxw, int rem, int style);

void __stdcall FUN_004a05e0(Gui* obj, int index);
void __stdcall FUN_004a0340(Gui* obj, int index);
void __stdcall FUN_004a16f0(Gui* obj, int index, int param3);
void __stdcall FUN_004a2580(Gui* obj, int index);
void __stdcall FUN_004a2be0(Gui* obj, int index);
void __stdcall FUN_004a2e40(Gui* obj, char* name, int line);
void __stdcall DrawListBox(Gui* obj, int index);
void __stdcall DrawSlider(Gui* obj, int index);
void __stdcall DrawTextInput(Gui* obj, int index);
void __stdcall DrawListboxFrame(Gui* obj, int index, void* bmp);
void __stdcall FUN_004a4660(Gui* obj, int index);
void __stdcall FUN_004a4980(Gui* obj, int index);
void __stdcall FUN_004a4c90(Gui* obj, int index, unsigned int param3);
int __stdcall FUN_004a4440(Gui* obj, int index, int key);
int __stdcall FUN_004a4b50(Gui* obj, int index);
int __stdcall FUN_0049fc50(Gui* obj, int index);
int __stdcall IsMouseButtonMessage(Gui* obj, unsigned char buttons);
int __stdcall HasMouseKeyFlags(Gui* obj, unsigned int mask);
void __stdcall SetClickMode(Gui* obj, int value);
void __stdcall CommitTextEdit(Gui* obj, int index, char* text, int maxLength, int clear);
void __stdcall UpdateCursorAndMouse(Gui* obj);
void __stdcall SetCursorHover(Gui* obj, int inside);
int __stdcall HandleListBoxInput(Gui* obj, int index, int key);
void __stdcall HandleSliderInput(Gui* obj, int index);
int __stdcall HandleTextEditKey(Gui* obj, int index, int key);
void __cdecl FUN_004d85a0(void* p);
int __stdcall SelectFontForEntry(Entry* entries, int index);

// The real GetTextPixelWidth (0x4a5030), which /Ob2 inlines into its callers here.
static inline int GetTextPixelWidth(char* text)
{
    int width = 0;
    if (text == 0)
        return 0;
    if (g_guiContext->language == 0)
        return GetTextWidth(GetFont(), text);
    char* p = text;
    while (*p != 0) {
        char ch = *p;
        Glyph* glyph = GetGafFrame(g_guiContext->language->glyphs, (unsigned char)ch);
        if (glyph != 0)
            width += glyph->width;
        ++p;
    }
    return width;
}

static inline int LineHeight()
{
    if (g_guiContext->language == 0)
        return GetFontHeight();
    Glyph* glyph = GetGafFrame(g_guiContext->language->glyphs, 0x49);
    return glyph->height + 2;
}

// The same height without the named glyph local; FUN_004a56b0 needs this form.
static inline int LineHeightDirect()
{
    if (g_guiContext->language == 0)
        return GetFontHeight();
    return (int)GetGafFrame(g_guiContext->language->glyphs, 0x49)->height + 2;
}

static inline int FindEntry(Entry* entries, char* name)
{
    for (int i = 1; i < entries->u.count + 1; i++) {
        if (strncmp(entries[i].name, name, 0x10) == 0)
            return i;
    }
    return -1;
}

// Walks the entry list of a layout object looking for the n-th tab stop
// (entries whose +0x00 byte is 7), sets the language from that entry, computes
// the line height, then lays the entry's text out right aligned (+0x1b bit 2),
// centred (bit 1) or left at its measured width (bit 0), writing the new x,
// width and line height back into the entry. +0xb6 is a union (count on entry
// 0, NUL terminated text elsewhere); +0x1b is a 4-byte field.
// FUNCTION: 0x4a53c0
void __stdcall FUN_004a53c0(Gui* obj, int index)
{
    Entry* entries = obj->layer->entries;
    Entry* entry = &entries[index];
    int i;
    int t = 0;
    for (i = 1; i < entries[0].u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entry->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].u.count + 1)
        SetFont(g_guiContext->font);
    int x = !entry->type ? 0 : entry->x;
    // Real variable declared here, assigned in each arm, one shared tail stores it.
    int nx = x;
    int lh;
    if (g_guiContext->language == 0)
        lh = GetFontHeight();
    else
        lh = GetGafFrame(g_guiContext->language->glyphs, 0x49)->height + 2;
    if (entry->flags & 4) {
        nx = entry->w + x;
        nx -= GetTextPixelWidth(entry->u.text);
    } else if (entry->flags & 2) {
        nx = entry->w / 2 + x;
        int half = GetTextPixelWidth(entry->u.text) / 2;
        nx -= half;
        // No named local for the width: it changes how lh is loaded.
        entry->w = (short)(half * 2);
    } else if (entry->flags & 1)
        entry->w = (short)GetTextPixelWidth(entry->u.text);
    // Stored via entries[index], not entry: pins the store order of x, w and h.
    entries[index].x = (short)nx;
    entry->h = (short)lh;
}

// FUNCTION: 0x4a56b0
void __stdcall FUN_004a56b0(Gui* obj, int index)
{
    obj->language = obj->values[1];
    Entry* entries = obj->layer->entries;

    int i = 1;
    int t = 0;
    for (; i < entries[0].u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (t == entries[index].tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            t++;
        }
    }
    if (i == entries[0].u.count + 1) {
        SetFont(g_guiContext->font);
        i = -1;
    }


    if (entries[index].x == -1)
        entries[index].x = (short)((entries[0].w - GetTextPixelWidth(entries[index].u.text)) / 2);

    Rect rect;
    if (entries[index].type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = entries[index].x;
        rect.top = entries[index].y;
    }
    rect.right = entries[index].w + rect.left - 1;
    rect.bottom = entries[index].h + rect.top - 1;

    if (entries[index].image != 0)
        FillRectangle(entries->u.assets.surface, &rect, obj->colours[entries[index].image]);
    int nx = rect.left;
    Entry* entry = &entries[index];

    if (entry->flags & 4) {
        nx = entry->w + rect.left;
        nx -= GetTextPixelWidth(entry->u.text);
    } else if (entry->flags & 2) {
        nx = entry->w / 2 + rect.left;
        int half = GetTextPixelWidth(entry->u.text) / 2;
        nx -= half;
    }

    if (i != -1 && (entries[index].flags & 8)) {
        SetTextColors(obj->colours[0], GetTextKeyColor());
        DrawString(entries->u.assets.surface, entries[index].u.text, nx + 1, rect.top + 3, -1);
    }

    SetTextColors(entries[index].colours, GetTextKeyColor());

    if (i == -1) {
        int lh = LineHeightDirect();
        if (rect.bottom - rect.top > lh * 2)
            FUN_004a51d0(entries->u.assets.surface, entries[index].u.text, nx, rect.top,
                         rect.right - rect.left + 1,
                         rect.bottom - rect.top + 1, entries[index].colours);
        else
            FUN_004a50e0(entries->u.assets.surface, entries[index].u.text, nx, rect.top,
                         rect.right - rect.left + 1, entries[index].colours);
    } else {
        DrawString(entries->u.assets.surface, entries[index].u.text, nx, rect.top, -1);
    }

    if (entries[index].field_148 & 1) {
        Entry* entries2 = obj->layer->entries;
        Rect rect2;
        if (entries2[index].type == 0) {
            rect2.left = 0;
            rect2.top = 0;
        } else {
            rect2.left = entries2[index].x;
            rect2.top = entries2[index].y;
        }
        rect2.right = entries2[index].w + rect2.left - 1;
        rect2.bottom = entries2[index].h + rect2.top - 1;
        GrayRectangle(entries2->u.assets.surface, &rect2);
        FadeRectangle(entries2->u.assets.surface, &rect2, -0x14);
    } else {
        unsigned char c = entries[index].field_147;
        if (c != 0) {
            char pat[2];
            pat[0] = (char)c;
            pat[1] = 0;
            char buf[0x80];
            strcpy(buf, entries[index].u.text);
            char* p = strstr(buf, pat);
            if (p != 0) {
                int y = rect.top;
                *p = 0;
                // Both x positions accumulate in x0, with x1 copied off it.
                int x0 = rect.left;
                x0 += GetTextPixelWidth(buf);
                int x1 = x0;
                x0 += GetTextPixelWidth(pat);
                int lh1 = LineHeightDirect();
                int lh2 = LineHeightDirect();
                DrawLine(obj->layer->entries->u.assets.surface, x1, lh2 + y - 1, x0 - 1,
                             lh1 + y - 1, obj->colours[2]);
            }
        }
    }

    // One tail after both arms of the field_148 if/else, not a copy per exit.
    obj->language = obj->values[0];
}

// FUNCTION: 0x4a5d30
void __stdcall FUN_004a5d30(Gui* p, int index)
{
    p->language = p->values[index];
}

// FUNCTION: 0x4a5d50
int __stdcall FUN_004a5d50(Gui* menu, int index)
{
    Entry* entries = menu->layer->entries;
    char* text = GetGadgetText(menu, entries[index].name, 0);
    if (text == 0)
        return 0;
    if (entries[index].type == 5 ||
        (entries[index].type == 1 && (entries[index].flags & 0x8000) != 0))
        menu->language = menu->values[1];
    while (1) {
        int w = GetTextPixelWidth(text);
        if (w <= entries[index].w - 6) {
            menu->language = menu->values[0];
            return w;
        }
        if (strlen(text) == 0)
            continue;
        text[strlen(text) - 1] = 0;
    }
}

// Draws GUI layout entry `index`: it turns on the manager's redraw flag, then
// blits the entry's glyph at the entry's rectangle offset, with the extra
// style argument when the entry's +0x1f count is positive, and finally fades
// the same rectangle when entry flag +0xbc has bit 0 set. The glyph's own x/y
// come from the loaded glyph's +4/+6.
//
// The +0xbc field is a union because entry 0 holds the
// destination surface pointer there while every other entry holds flag bits.
// FUNCTION: 0x4a5e50
void __stdcall FUN_004a5e50(Gui* obj, int index)
{
    if (obj->layer != 0)
        obj->layer->dirty = 1;
    Entry* entries = obj->layer->entries;
    Entry* e = &entries[index];
    Glyph* glyph = e->u.frame.glyph;
    if (glyph == 0)
        return;
    // One struct local, not four scalars: keeps right/bottom stored and the frame size.
    Rect rect;
    if (e->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = e->x;
        rect.top = e->y;
    }
    rect.right = e->w + rect.left - 1;
    rect.bottom = e->h + rect.top - 1;
    int count = e->colours;
    if (count > 0) {
        DrawFrameLit(entries->u.assets.surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top, count);
    } else {
        DrawFrame(entries->u.assets.surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top);
    }
    if (e->u.frame.flag & 1) {
        FadeRectangle(entries->u.assets.surface, &rect, -0x1c);
    }
}

// Draws one list-gadget entry: its glyph or frame, then the text (left,
// right, centred, or centred with an underlined hotkey letter, flags 1/4/2/0x20).
// Needed with the named glyph local in LineHeight: sets the flags 0x20 registers.
// FUNCTION: 0x4a5f40
void __stdcall DrawButton(Gui* menu, int index)
{
    char* p;
    char key2[2];
    void* surface;
    Entry* me;
    // Declaration order matters: rect before flagy and t, textw last.
    Rect rect;
    int x;
    int width;
    int border;
    int flagy;
    char* text;
    int pass;
    int saved;
    char buf[0x80];
    int y, i;
    int t;
    int textw;

    border = 0;
    if (menu->layer != 0)
        menu->layer->dirty = 1;
    Entry* entries = menu->layer->entries;
    me = &entries[index];
    if (me->type == 0) {
        rect.left = 0;
        rect.top = 0;
    } else {
        rect.left = me->x;
        rect.top = me->y;
    }
    rect.right = me->w + rect.left - 1;
    rect.bottom = rect.top + me->h - 1;
    if (me->flags & 0x8000)
        menu->language = menu->values[1];

    // Own counter, not t: it shares the slot of t.
    int tab = 0;
    for (i = 1; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (tab == me->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            tab++;
        }
    }
    if (i == entries->u.count + 1)
        SetFont(g_guiContext->font);

    textw = FUN_004a5d50(menu, index);
    surface = entries->u.assets.surface;
    if (me->gaf != 0) {
        Glyph* glyph;
        if (me->field_13c & 1) {
            if (me->flags & 0x100) {
                glyph = GetGafFrame(me->gaf, me->gaf->count - 1);
            } else if (me->stage != 0) {
                glyph = GetGafFrame(me->gaf, me->stageIndex);
                border = 1;
            } else if (me->flags & 0x1800) {
                glyph = GetGafFrame(me->gaf, me->field_13b);
                border = 1;
            } else {
                int val = me->gaf->count - 1;
                if (me->field_138 + 2 < val)
                    val = me->field_138 + 2;
                glyph = GetGafFrame(me->gaf, val + me->field_13b);
                if (!(me->flags & 0x80))
                    border = 1;
            }
        } else {
            if (me->field_138 != 0 && (unsigned short)me->gaf->count > (unsigned short)me->stage) {
                if (me->stage != 0)
                    glyph = GetGafFrame(me->gaf, me->gaf->count - 2);
                else
                    glyph = GetGafFrame(me->gaf, me->field_13b + me->field_138);
            } else if (me->stage != 0)
                glyph = GetGafFrame(me->gaf, me->stageIndex);
            else
                glyph = GetGafFrame(me->gaf, me->field_13b);
        }
        if (glyph != 0) {
            if (me->colours != 0)
                DrawFrameLit(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top, me->colours);
            else
                DrawFrame(surface, glyph, glyph->xoff + rect.left, glyph->yoff + rect.top);
        }
    } else {
        if (me->field_13c & 1) {
            FillBevelBox(surface, &rect, menu->colours[0], menu->colours[0x13], menu->colours[0x13]);
        } else if (me->field_138 != 0) {
            FillBevelBox(surface, &rect, menu->colours[0], menu->colours[0x11], menu->colours[0x14]);
        } else {
            FillBevelBoxDarkFirst(surface, &rect, menu->colours[0], menu->colours[0x11], menu->colours[0x14]);
        }
    }

    t = 0;
    flagy = 0;
    if (me->field_138 != 0) {
        t = 1;
        flagy = 1;
    }
    text = me->u.text;
    pass = 0;
    do {
        if (me->field_138 != 0)
            SetTextColors(menu->colours[0], GetTextKeyColor());
        else
            SetTextColors(menu->colours[me->colours], GetTextKeyColor());

        p = text;
        if (me->stage != 0) {
            for (unsigned int k = me->stageIndex; k != 0; k--) {
                while (*p != 0)
                    p++;
                p++;
            }
        }

        y = (rect.bottom - LineHeight() - rect.top) / 2 + flagy + rect.top;
        if (me->flags & 0x8000)
            menu->language = menu->values[1];

        if (me->flags & 1) {
            FUN_004a50e0(surface, p, t + rect.left + 3, y,
                         rect.right - rect.left + 1, 0);
        } else if (me->flags & 4) {
            x = rect.right - textw - 3;
            if (x < rect.left)
                x = rect.left;
            FUN_004a50e0(surface, p, x, y, rect.right - rect.left + 1, 0);
        } else if (me->flags & 2) {
            x = (rect.right - textw - rect.left) / 2 + t;
            x += rect.left + 1;
            if (me->field_13a == 0 || (me->field_13c & 1)) {
                FUN_004a50e0(surface, p, x, y, 1 + (rect.right - rect.left), 0);
            } else {
                // Declared in this block: key1[1] = 0 hoists into the inlined strcpy.
                char key1[2];
                // Each hotkey branch declares its own found: one frame slot.
                char* found;
                key1[0] = me->field_13a;
                width = rect.right - rect.left + 1;
                strcpy(buf, p);
                key1[1] = 0;
                found = strstr(buf, key1);
                if (found != 0) {
                    GetFont();
                    strcpy(buf, p);
                    *found = 0;
                    FUN_004a50e0(surface, buf, x, y, width, 0);
                    x += GetTextPixelWidth(buf);
                    saved = x;
                    if (me->field_138 != 0)
                        SetTextColors(menu->colours[0], GetTextKeyColor());
                    else
                        SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                    FUN_004a50e0(surface, key1, x, y, width, 0);
                    x += GetTextPixelWidth(key1);
                    if (me->field_138 != 0) {
                        DrawLine(surface, saved, LineHeight() + y - 1,
                                     x - 1, LineHeight() + y - 1,
                                     menu->colours[0]);
                    } else {
                        DrawLine(surface, saved, LineHeight() + y - 1,
                                     x - 1, LineHeight() + y - 1,
                                     menu->colours[2]);
                    }
                    if (me->field_138 != 0)
                        SetTextColors(menu->colours[0], GetTextKeyColor());
                    else
                        SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                    FUN_004a50e0(surface, found + 1, x, y, width, 0);
                } else {
                    FUN_004a50e0(surface, p, x, y, width, 0);
                }
            }
        } else if (me->flags & 0x20) {
            int xb;
            char* found;
            xb = (rect.right - textw - rect.left) / 2 + t;
            xb += rect.left + 1;
            int ys = flagy - LineHeight();
            ys += rect.bottom - 4;
            if (me->field_13a != 0 && (found = strchr(p, (signed char)me->field_13a)) != 0) {
                width = rect.right - rect.left + 1;
                key2[0] = me->field_13a;
                key2[1] = 0;
                GetFont();
                *found = 0;
                FUN_004a50e0(surface, p, xb, ys, width, 0);
                // Suspected original bug: this measures `text` (the first
                // string, [esp+0x4c] at 0x4a681e), not the drawn prefix `p`,
                // so the underline is misplaced when stage selects a later
                // string. The flags 2 branch measures its truncated copy.
                xb += GetTextPixelWidth(text);
                SetTextColors(menu->colours[10], GetTextKeyColor());
                FUN_004a50e0(surface, key2, xb, ys, width, 0);
                xb += GetTextPixelWidth(key2);
                SetTextColors(menu->colours[me->colours], GetTextKeyColor());
                FUN_004a50e0(surface, found + 1, xb, ys, width, 0);
            } else {
                FUN_004a50e0(surface, p, xb, ys, rect.right - rect.left + 1, 0);
            }
        }
    } while (pass--);

    menu->language = menu->values[0];
    if (border) {
        GrayRectangle(surface, &rect);
        FadeRectangle(surface, &rect, -0x14);
    }
}

// FUNCTION: 0x4a69d0
void __stdcall FUN_004a69d0(Gui* param_1)
{
    Entry* entries = param_1->layer->entries;
    Entry* e = &entries[1];
    for (int i = 1; i < entries->u.count + 1; i++, e++) {
        if (e->type == 1 && e->field_138 != 0) {
            e->field_138 = 0;
            DrawButton(param_1, i);
            param_1->changed = 1;
        }
    }
}

// Sibling of 0x4a69d0, limited to the entries on the same team as `index`.
// FUNCTION: 0x4a6a40
void __stdcall FUN_004a6a40(Gui* param_1, int index)
{
    Entry* entries = param_1->layer->entries;
    Entry* e = &entries[1];
    unsigned char team = entries[index].team;
    for (int i = 1; i < entries->u.count + 1; i++, e++) {
        if (e->type == 1 && e->team == team && e->field_138 != 0) {
            e->field_138 = 0;
            DrawButton(param_1, i);
            param_1->changed = 1;
        }
    }
}

static inline int FindKind(Entry* entries, unsigned char kind)
{
    for (int i = 1; i < entries->u.count + 1; i++) {
        if (entries[i].type == 4 && entries[i].team == kind)
            return i;
    }
    return 0;
}

// Command-button click/key handler for the 0x15b-byte entry table.
// FUNCTION: 0x4a6ae0
int __stdcall HandleButtonInput(Gui* obj, int index, int param_3)
{
    Entry* entries = obj->layer->entries;
    Entry* entry = &entries[index];
    if (entry->field_13c & 1)
        goto fail;

    Rect r;
    if (entry->type == 0) {
        r.left = 0;
        r.top = 0;
    } else {
        r.left = entry->x;
        r.top = entry->y;
    }
    r.right = entry->w + r.left - 1;
    r.bottom = entry->h + r.top - 1;

    Point point = obj->point;
    point.x -= entries->x;
    point.y -= entries->y;

    if (point.x >= r.left && point.x <= r.right
        && point.y >= r.top && point.y <= r.bottom) {
        obj->field_68 = index;
        if (IsMouseButtonMessage(obj, 1)) {
            obj->focus = -1;
            FUN_0049fc50(obj, index);
            SetClickMode(obj, 1);
            obj->field_cce = entry->field_138;
        } else if (IsMouseButtonMessage(obj, 2)) {
            obj->focus = -1;
            FUN_0049fc50(obj, index);
            SetClickMode(obj, 2);
            obj->field_cce = entry->field_138;
        }
    }

    if (obj->focus == index) {
        if (entry->flags & 0x10) {
            if (!HasMouseKeyFlags(obj, 3))
                goto fail;
            obj->focus = -1;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom) {
                entry->field_138 = obj->field_cce;
                DrawButton(obj, index);
                return 0;
            }
            entry->field_138 = 1;
            FUN_004a0340(obj, index);
            DrawButton(obj, index);
            return 1;
        }
        if (entry->flags & 0x40) {
            if (!HasMouseKeyFlags(obj, 3)) {
                obj->focus = -1;
                if (point.x < r.left || point.x > r.right
                    || point.y < r.top || point.y > r.bottom) {
                    entry->field_138 = obj->field_cce;
                    DrawButton(obj, index);
                    return 0;
                }
                entry->field_138 = (obj->field_cce == 0);
                FUN_004a0340(obj, index);
                DrawButton(obj, index);
                return 1;
            }
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom) {
                if (entry->field_138 == 0)
                    goto fail;
                entry->field_138 = 0;
                DrawButton(obj, index);
                return 0;
            }
            if (entry->field_138 != 0)
                goto fail;
            entry->field_138 = 1;
            DrawButton(obj, index);
            return 0;
        }
        if (entry->flags & 8) {
            if (HasMouseKeyFlags(obj, 3))
                goto fail;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom)
                goto fail;
            if (entry->field_138 == 1)
                entry->field_138 = 0;
            else if (entry->field_138 == 0)
                entry->field_138 = 1;
            FUN_004a0340(obj, index);
            DrawButton(obj, index);
            obj->focus = -1;
            return 1;
        }
        if (entry->flags & 0x100) {
            if (!IsMouseButtonMessage(obj, 1))
                goto fail;
            if (point.x < r.left || point.x > r.right
                || point.y < r.top || point.y > r.bottom)
                goto fail;
            GafEntry* p = entry->gaf;
            if (p != 0) {
                if (entry->field_138 < p->count - 1)
                    entry->field_138 += 1;
                else
                    entry->field_138 = 0;
            }
            FUN_004a0340(obj, index);
            DrawButton(obj, index);
            obj->focus = -1;
            return 1;
        }
        if (!HasMouseKeyFlags(obj, 3)) {
            obj->focus = -1;
            entry->field_138 = 0;
            FUN_004a0340(obj, index);
            if (point.x >= r.left && point.x <= r.right
                && point.y >= r.top && point.y <= r.bottom
                && !(entry->flags & 0x1800)) {
                if (entry->stage != 0) {
                    entry->stageIndex += 1;
                    if (entry->stageIndex >= entry->stage)
                        entry->stageIndex = 0;
                }
                DrawButton(obj, index);
                return 1;
            }
            DrawButton(obj, index);
            return 0;
        }
        // Keep this if / else-if / else chain: it gives the original block layout.
        if (entry->field_138 != 0 && (entry->flags & 0x2000)) {
            if (DAT_0051fbb0 == GetTicks())
                goto fail;
            DAT_0051fbb0 = GetTicks();
            if (DAT_0051fbac > 0) {
                DAT_0051fbac -= 1;
                return 0;
            }
        } else if (entry->field_138 == 0
                   && point.x >= r.left && point.x <= r.right
                   && point.y >= r.top && point.y <= r.bottom) {
            entry->field_138 = 1;
            DAT_0051fbac = 0xf;
        } else {
            if (entry->field_138 == 0)
                goto fail;
            if (point.x >= r.left && point.x <= r.right
                && point.y >= r.top && point.y <= r.bottom)
                goto fail;
            entry->field_138 = 0;
            DrawButton(obj, index);
            return 0;
        }
        DrawButton(obj, index);
        int flags = entry->flags;
        if (!(flags & 0x1800))
            goto fail;
        // Inline FindKind fed a byte local: fixes the obj/entry register choice.
        unsigned char team = entry->team;
        int found = FindKind(entries, team);
        // FindKind returns 0, not -1, when nothing matches, so this test can
        // never be true and a miss falls through to entry 0 (docs/bugs.md).
        if (found == -1)
            goto fail;
        Entry* f = &entries[found];
        short off = f->field_140;
        if (flags & 0x1000) {
            if (off > 0)
                f->field_140 = off - 1;
        } else {
            if (off < f->field_136 - 1)
                f->field_140 = off + 1;
        }
        if (obj->layer)
            obj->layer->dirty = 1;
        FUN_004a2580(obj, found);
        FUN_004a2be0(obj, found);
        if (f->callback)
            f->callback(obj, f->callbackArg);
        return 0;
    } else {
        if (!(obj->focus != -1 && entries[obj->focus].type == 3) || IsKeyDown(0xfb)) {
            if (obj->field_cc6 == 1) {
                if (param_3 != 0) {
                    if ((char)tolower((char)entry->field_13a) == (char)param_3
                        || (char)toupper((char)entry->field_13a) == (char)param_3) {
                        if (entry->flags & 0x40) {
                            entry->field_138 = (entry->field_138 == 0);
                            DrawButton(obj, index);
                        } else if (entry->flags & 0x10) {
                            if (entry->field_138 == 0) {
                                entry->field_138 = 1;
                                DrawButton(obj, index);
                            }
                        }
                        FUN_004a0340(obj, index);
                        PopKey();
                        return 1;
                    }
                }
            }
        }
    }
fail:
    return 0;
}

// FUNCTION: 0x4a7190
void __stdcall FUN_004a7190(Gui* obj, int index)
{
    Entry* entries = obj->layer->entries;
    Entry* target = &entries[index];

    SetTextColors(obj->colours[target->colours], GetTextKeyColor());

    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->font);
    }

    FUN_0049fc50(obj, index);
    obj->layer->current = index;
    CommitTextEdit(obj, index, target->u.text, target->field_138, 0);
    ClearKeyQueue();
}

// A copy of the function at 0x4a15c0 (another unit), which /Ob2 inlines below.
void __stdcall FUN_004a15c0(char* param_1, int param_2, Rect* param_3)
{
    char* e = param_1 + param_2 * 0x15b;
    if (*e == 0) {
        param_3->left = 0;
        param_3->top = 0;
    } else {
        param_3->left = *(short*)(e + 0x13);
        param_3->top = *(short*)(e + 0x15);
    }
    param_3->right = *(short*)(e + 0x17) - 1 + param_3->left;
    param_3->bottom = *(short*)(e + 0x19) - 1 + param_3->top;
}

// A copy of the function at 0x4a1810 (another unit), inlined in HandleTextInput.
static inline int SelectFontForEntry_inlined(Entry* entries, int index)
{
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entries[index].tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->font);
        i = -1;
    }
    return i;
}

// A copy of the function at 0x4a1920 (another unit), which /Ob2 inlines below.
int __stdcall FUN_004a1920(Rect* r, int px, int py)
{
    if (px >= r->left && px <= r->right && py >= r->top && py <= r->bottom) {
        return 1;
    }
    return 0;
}

// A copy of the function at 0x49fc40 (another unit), which /Ob2 inlines below.
void __stdcall FUN_0049fc40(Gui* obj)
{
    obj->focus = -1;
}

// A copy of the function at 0x49fcf0 (another unit), which /Ob2 inlines below.
int __stdcall FUN_0049fcf0(Gui* obj, int index)
{
    int result = 0;
    result = obj->focus == index;
    return result;
}

// One mouse button's action on the entry: the rect FUN_004a15c0 fills here is
// never read, but its block-scoped local shares the frame slot of the hit
// test's rect, as in the original.
static inline void Activate(Gui* obj, int index)
{
    Rect rect;
    Entry* ep = obj->layer->entries;
    FUN_004a15c0((char*)ep, index, &rect);
    SetTextColors(obj->colours[ep[index].colours], GetTextKeyColor());
    SelectFontForEntry(ep, index);
    FUN_0049fc50(obj, index);
    obj->layer->current = index;
    CommitTextEdit(obj, index, ep[index].u.text, ep[index].field_138, 0);
    ClearKeyQueue();
}

// Mouse and key handling for a text entry of the 0x15b-byte entry table: a
// click inside the entry's rect (left or right button) selects its group and
// gives it the focus; with the focus, the typed key is handled and Enter or
// Escape (which also clears the text) end the edit.
// FUNCTION: 0x4a7290
int __stdcall HandleTextInput(Gui* obj, int index, int key)
{
    Entry* entries = obj->layer->entries;
    Rect rect;
    FUN_004a15c0((char*)entries, index, &rect);
    SelectFontForEntry_inlined(entries, index);

    Point point = obj->point;
    int rel_x = point.x - entries->x;
    int rel_y = point.y - entries->y;

    if (FUN_004a1920(&rect, rel_x, rel_y)) {
        obj->field_68 = index;
        if (IsMouseButtonMessage(obj, 1)) {
            Activate(obj, index);
            SetClickMode(obj, 1);
        } else if (IsMouseButtonMessage(obj, 2)) {
            Activate(obj, index);
            SetClickMode(obj, 2);
        }
    }

    if (FUN_0049fcf0(obj, index)) {
        SetTextColors(obj->colours[entries[index].colours],
                     obj->colours[entries[index].image]);
        int r = HandleTextEditKey(obj, index, key);
        if (r == 13) {
            FUN_0049fc40(obj);
            return 1;
        }
        if (r == 27) {
            FUN_0049fc40(obj);
            entries[index].u.text[0] = 0;
            return 1;
        }
        if (obj->layer)
            obj->layer->dirty = 1;
    }
    return 0;
}

// Finds the next entry of type 3 after `index` in a 1-based list of 0x15b-byte
// entries (entry 0 holds the count at +0xb6), wrapping round to the start.
// The parameter itself is the loop counter (the original loads it first and
// keeps a copy of its old value for the wrapped search).
// FUNCTION: 0x4a7560
int __stdcall FindNextTextInput(Entry* list, int index)
{
    int old = index;
    for (index++; index < list[0].u.count + 1; index++) {
        if (list[index].type == 3)
            break;
    }
    if (index == list[0].u.count + 1) {
        for (index = 1; index < old; index++) {
            if (list[index].type == 3)
                break;
        }
    }
    return index;
}

// FUNCTION: 0x4a75d0
int __stdcall LoadScreenGaf(Gui* obj, char* name)
{
    char path[256];
    path[0] = 0;
    if (obj->str_ab6[0] != 0)
        strncpy(path, obj->str_ab6, 0x100);
    strcat(path, name);
    ChangeExtension(path, path, "GAF");
    if (HAPI_FileLengthByName(path)) {
        obj->layer->entries->u.assets.archive = LoadGaf(path);
        if (obj->layer->entries->u.assets.archive != 0)
            return 1;
    }
    return 0;
}

static inline void DoSelect(Gui* menu, Entry* entries, int sel)
{
    Entry* entry = &entries[sel];
    SetTextColors(menu->colours[entry->colours], GetTextKeyColor());
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == entry->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1)
        SetFont(g_guiContext->font);
    FUN_0049fc50(menu, sel);
    menu->layer->current = sel;
    CommitTextEdit(menu, sel, entry->u.text, entry->field_138, 0);
    ClearKeyQueue();
}

// Selects the menu entry named `name` (16 bytes of its name at +0x02 of the
// 0x15b-byte entry, so entry i's name is at entries + i*0x15b + 2). When the
// selected entry is type 3 it makes its group's type-7 entry current and
// refreshes its edit field.
// FUNCTION: 0x4a76b0
void __stdcall SelectGadgetByName(Gui* menu, char* name)
{
    Entry* entries = menu->layer->entries;
    int index = FindEntry(entries, name);
    if (index != -1) {
        menu->focus = -1;
        menu->layer->current = index;
        if (entries[menu->layer->current].type == 3) {
            // The test reads the local entries; the call re-reads layer->entries and current.
            DoSelect(menu, menu->layer->entries, menu->layer->current);
        }
    }
}

// Selecting the entry with the index the caller passes: remembers the index,
// and if the entry it names is a type 3 (text) control it makes the group's
// type 7 list entry current and puts the entry's own text back into the field.
// FUNCTION: 0x4a7830
void __stdcall SelectGadgetByIndex(Gui* menu, int index)
{
    // The array is loaded twice: this copy for the type test, again inside the branch.
    Entry* first = menu->layer->entries;
    menu->focus = -1;
    menu->layer->current = index;
    if (first[menu->layer->current].type == 3) {
        int i = menu->layer->current;
        Entry* entries = menu->layer->entries;
        Entry* entry = &entries[i];
        int font = GetTextKeyColor();
        SetTextColors(menu->colours[entry->colours], font);

        int n = 0;
        int j = 1;
        for (; j < entries->u.count + 1; j++) {
            if (entries[j].type == 7) {
                if (n == entry->tab) {
                    SetFont(entries[j].u.list.language);
                    break;
                }
                n++;
            }
        }
        if (j == entries->u.count + 1) {
            SetFont(g_guiContext->font);
        }

        FUN_0049fc50(menu, i);
        menu->layer->current = i;
        CommitTextEdit(menu, i, entry->u.text, entry->field_138, 0);
        ClearKeyQueue();
    }
}

// The real FUN_004a7190, inlined here by /Ob2 (the out-of-line function alone is not inlined).
static inline void FUN_004a7190_inlined(Gui* obj, int index)
{
    Entry* entries = obj->layer->entries;
    Entry* target = &entries[index];

    SetTextColors(obj->colours[target->colours], GetTextKeyColor());

    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->font);
    }

    FUN_0049fc50(obj, index);
    obj->layer->current = index;
    CommitTextEdit(obj, index, target->u.text, target->field_138, 0);
    ClearKeyQueue();
}

// FUNCTION: 0x4a7960
void __stdcall FUN_004a7960(Gui* menu, int dir)
{
    int used[200];

    Layer* layer = menu->layer;
    int index = layer->current;
    Entry* entries = layer->entries;
    if (index == -1)
        return;

    for (int k = 0; k < 50; k++)
        used[k] = 0;

    // Both loops index by i, with no cnt local: count + 1 is written at each use.
    for (int i = 1; i < entries->u.count + 1; i++) {
        int idx = -1;
        for (int j = 1; used[j] != 0; j++) {
            int d = used[j] - entries[i].x;
            if (d < 10 && d > -10) {
                idx = j;
                break;
            }
        }
        if (idx != -1)
            used[i] = used[idx];
        else
            used[i] = entries[i].x;
    }

    int start;
    int bound;
    switch (dir) {
    case 0:
        start = entries[index].x + entries[index].y * 5000;
        bound = start - 0x17d7840;
        break;
    case 2:
        start = entries[index].y + used[index] * 5000;
        bound = start - 0x17d7840;
        break;
    case 1:
        start = entries[index].x + entries[index].y * 5000;
        bound = start + 0x17d7840;
        break;
    case 3:
        start = entries[index].y + used[index] * 5000;
        bound = start + 0x17d7840;
        break;
    }

    {
    int pos;
    for (int i = 1; i < entries->u.count + 1; i++) {
        // up is declared before b, and b is an explicit pointer to w: induction variable order.
        int* up = &used[i];
        char* b = (char*)&entries[i].w;
        if (*(signed char*)(b + 0x12) != 0 && !(*(int*)(b + 4) & 0x400)
            && !(*(unsigned char*)(b - 0x17) == 1 && (*(unsigned char*)(b + 0x125) & 1))
            && !(*(unsigned char*)(b - 0x17) == 4 && *(int*)(b + 0x140) != 0)) {
            if (*(unsigned char*)(b - 0x17) == 3 || *(unsigned char*)(b - 0x17) == 4
                || *(unsigned char*)(b - 0x17) == 1
                || *(unsigned char*)(b - 0x17) == 6
                || *(unsigned char*)(b - 0x17) == 2) {
                if (!(*(unsigned char*)(b - 0x17) == 4
                      && *(short*)b < *(short*)(b + 2))) {
                    if (!(*(unsigned char*)(b - 0x17) == 2 && (*(int*)(b + 4) & 0x100))
                        && !(*(unsigned char*)(b - 0x17) == 1
                             && (*(unsigned char*)(b + 0x125) & 1))) {
                        switch (dir) {
                        case 0:
                        case 1:
                            pos = *(short*)(b - 4) + *(short*)(b - 2) * 5000;
                            break;
                        case 2:
                        case 3:
                            pos = *(short*)(b - 2) + *up * 5000;
                            break;
                        }
                        switch (dir) {
                        case 1:
                        case 3:
                            if (pos <= start)
                                pos += 0x17d7840;
                            if (pos < bound) {
                                bound = pos;
                                index = i;
                            }
                            break;
                        case 0:
                        case 2:
                            if (pos >= start)
                                pos -= 0x17d7840;
                            if (pos > bound) {
                                bound = pos;
                                index = i;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    }

    menu->focus = -1;
    layer->current = index;
    // Call FUN_004a7190_inlined(menu, menu->layer->current) behind this test; no sel local.
    if (entries[menu->layer->current].type == 3)
        FUN_004a7190_inlined(menu, menu->layer->current);
    if (entries[menu->layer->current].type == 3)
        FUN_004a7190_inlined(menu, menu->layer->current);
}

// FUNCTION: 0x4a7ee0
void __stdcall FUN_004a7ee0(Entry* entries, int index)
{
    Entry temp;
    if (index != -1) {
        temp = entries[index];
        for (int i = index; i > 1; i--) {
            entries[i] = entries[i - 1];
        }
        entries[1] = temp;
    }
}

// Finds the GAF entry that draws a button-like object: the object's own name
// (copied out of obj->name) is looked up in the screen's GAF, then in the
// object's own GAF, and only when both fail does it fall back to "CHECKBOX",
// "stagebuttn%d" or "BUTTONS0".
// FUNCTION: 0x4a7f70
void __stdcall FindButtonGaf(Gui* button, Entry* obj)
{
    char name[0x10];
    char str[0x20];
    int best;
    GafEntry* entry = 0;
    Entry* holder = button->layer->entries;
    strncpy(name, obj->name, 0x10);
    name[0xf] = 0;
    obj->field_13b = 0;
    void* gaf = holder->u.assets.archive;
    if (gaf)
        entry = FindGafEntry(gaf, name);
    if (entry == 0) {
        if (button->gaf != 0) {
            entry = FindGafEntry(button->gaf, name);
            if (entry == 0) {
                if (obj->flags & 0x80) {
                    entry = FindGafEntry(button->gaf, "CHECKBOX");
                } else if (obj->stage != 0) {
                    int n = obj->stage < 4 ? obj->stage : 4;
                    sprintf(str, "stagebuttn%d", n);
                    entry = FindGafEntry(button->gaf, str);
                    if (obj->stage == 1) {
                        obj->stage = 2;
                        obj->flags |= 0x4000;
                    }
                } else {
                    strcpy(str, "BUTTONS0");
                    entry = FindGafEntry(button->gaf, str);
                }
                // Both loops stay inside this block: early exits must jump to the tail.
                if (entry != 0) {
                    best = 1000;
                    for (int i = 0; i < entry->count; i++) {
                        Glyph* f = GetGafFrame(entry, i);
                        if (f != 0) {
                            f->yoff = 0;
                            f->xoff = 0;
                        }
                    }
                    for (int j = 0; j < entry->count; j += 4) {
                        Glyph* f = GetGafFrame(entry, j);
                        int d = abs(obj->h - f->height) + abs(obj->w - f->width);
                        if (d < best) {
                            obj->field_13b = (unsigned char)j;
                            best = d;
                        }
                    }
                }
            }
        }
    }
    obj->gaf = entry;
    if (entry != 0) {
        Glyph* f = GetGafFrame(entry, obj->field_13b);
        if (f != 0) {
            obj->w = f->width;
            obj->h = f->height;
        }
    }
}

// Appends a cleared entry of the given type to the GUI entry list (entry 0
// holds the count) and returns its index.
// The memset spelling and the type-before-field_29 order are what RenderLayer
// needs when it inlines this; the standalone function matches with either.
// FUNCTION: 0x4a8150
int __stdcall AddGadgetEntry(Gui* obj, unsigned char type)
{
    Entry* entries = obj->layer->entries;
    entries->u.count++;
    Entry* e = &entries[entries->u.count];
    memset(&entries[entries->u.count], 0, sizeof(Entry));
    e->type = type;
    e->field_29 = 1;
    return entries->u.count;
}

// FUNCTION: 0x4a81b0
void __stdcall FUN_004a81b0(Gui* obj, char* out)
{
    *out = 0;
    if (obj->str_ab6[0] != 0)
        strncpy(out, obj->str_ab6, 0x100);
}

// FUNCTION: 0x4a81e0
int __stdcall RenderLayer(Gui* menu, unsigned int flags)
{
    int savedType;
    int orientation;
    Entry* entries;
    char* name;
    int* pf;
    int i, force;
    GafEntry* g;

    if (!menu->layer)
        return 0;
    entries = menu->layer->entries;
    if ((flags & 0x100) && (flags & 1)) {
        entries[0].y = -1;
        entries[0].x = -1;
    }
    if ((flags & 0x1000) && (flags & 1)) {
        entries[0].y = -2;
        entries[0].x = -2;
    }
    if (-1 == entries[0].x) {
        entries[0].x = (short)((GetScreenWidth() - entries[0].w) / 2);
        entries[0].y = (short)((GetScreenHeight() - entries[0].h) / 2);
    }
    if (entries[0].x == -2) {
        entries[0].x = (short)(((GetScreenWidth() - 0x80 - entries[0].w) / 2) + 0x80);
        entries[0].y = (short)((GetScreenHeight() - entries[0].h) / 2);
    }
    if (entries[0].x + entries[0].w > GetScreenWidth())
        entries[0].x = (short)((GetScreenWidth() - entries[0].w) / 2);
    if (entries[0].y + entries[0].h > GetScreenHeight())
        entries[0].y = (short)((GetScreenHeight() - entries[0].h) / 2);

    force = flags & 1;
    if (force) {

    // The result goes through a variable and is compared against it.
    do {
        i = PopKey();
    } while (i != 0);
    if (menu->field_70 != 0) {
        for (i = 0; i <= entries[0].u.count; i++) {
            if (entries[i].type == 1)
                entries[i].field_13a = 0;
        }
    }
    if (entries[0].w > GetScreenWidth() || entries[0].h > GetScreenHeight())
        return 0;

    entries[0].u.assets.archive = 0;
    i = 0;
    while (i < entries[0].u.count + 1) {
        // Buffers stay declared at the top of the loop body (scheduling of textbuf stores).
        char stagebuf[0x20];
        char textbuf[0x100];
        char buf1[0x100];
        char buf2[0x100];
        char buf3[0x80];
        buf2[0] = 0;
        if (menu->str_9b6[0])
            strcpy(buf2, menu->str_9b6);
        g = 0;
        if (entries[i].resourceFlags & 1) {
            entries[i].archive = 0;
            entries[i].gaf = 0;
            FUN_004a81b0(menu, buf1);
            strcat(buf1, entries[i].name);
            strcat(buf1, "_gadget");
            ChangeExtension(buf1, buf1, "GAF");
            if (HAPI_FileLengthByName(buf1)) {
                entries[i].archive = LoadGaf(buf1);
                if (entries[i].archive)
                    entries[i].gaf = FindGafEntry(entries[i].archive, entries[i].name);
            }
        }
        switch (entries[i].type) {
        case 0:
        case 11: {
            if (0 > entries[0].y)
                entries[0].y += (short)GetScreenHeight();
            FUN_004a81b0(menu, buf1);
            strncpy(textbuf, entries[0].name, 0x10);
            textbuf[0x10] = 0;
            strcat(buf1, textbuf);
            ChangeExtension(buf1, buf1, "GAF");
            if (!entries[0].u.assets.archive) {
                if (HAPI_FileLengthByName(buf1))
                    entries[0].u.assets.archive = LoadGaf(buf1);
            }
            strncpy(textbuf, entries[0].u.text + 0x46, 0x10);
            textbuf[0x10] = 0;
            if (entries[0].u.assets.archive)
                g = FindGafEntry(entries[0].u.assets.archive, textbuf);
            if (g == 0) {
                if (0 != menu->gaf) {
                    g = FindGafEntry(menu->gaf, textbuf);
                    if (g == 0) {
                        g = FindGafEntry(menu->gaf, "BackTile");
                        if (g != 0) {
                            for (int frameIndex = 0; frameIndex < g->count; frameIndex++) {
                                Glyph* frame = GetGafFrame(g, frameIndex);
                                frame->yoff = 0;
                                frame->xoff = 0;
                            }
                        }
                    }
                }
            }
            entries[0].u.assets.background = g;
            break;
        }

        case 4: {
            entries[i].field_13b = 0;
            if (entries[0].u.assets.archive)
                g = FindGafEntry(entries[0].u.assets.archive, "SLIDERS");
            if (g == 0 && menu->gaf != 0) {
                g = FindGafEntry(menu->gaf, "SLIDERS");
                if (g != 0) {
                    for (int f = 0; f < g->count; f++) {
                        Glyph* frame = GetGafFrame(g, f);
                        frame->yoff = 0;
                        frame->xoff = 0;
                    }
                    orientation = entries[i].w > entries[i].h ? 10 : 0;
                    Glyph* frame = GetGafFrame(g, orientation);
                    if (entries[i].w < entries[i].h)
                        entries[i].w = frame->width;
                    else
                        entries[i].h = frame->height;
                    entries[i].sliderStyle = (unsigned char)orientation;
                }
            }
            entries[i].sliderGaf = g;
            if (g != 0) {
                Entry* firstEnd = &entries[AddGadgetEntry(menu, 1)];
                firstEnd->x = entries[i].x;
                firstEnd->y = entries[i].y;
                firstEnd->gaf = g;
                firstEnd->field_13b = entries[i].sliderStyle + 6;
                firstEnd->team = entries[i].team;
                Glyph* frame = GetGafFrame(g, entries[i].sliderStyle + 6);
                firstEnd->w = frame->width;
                firstEnd->h = frame->height;
                firstEnd->flags = 0x3400;
                firstEnd->field_29 = entries[i].field_29;
                Entry* secondEnd = &entries[AddGadgetEntry(menu, 1)];
                secondEnd->field_29 = entries[i].field_29;
                frame = GetGafFrame(g, entries[i].sliderStyle + 8);
                secondEnd->y = entries[i].y;
                secondEnd->gaf = g;
                secondEnd->field_13b = entries[i].sliderStyle + 8;
                secondEnd->team = entries[i].team;
                secondEnd->flags = 0x2c00;
                secondEnd->w = frame->width;
                secondEnd->h = frame->height;
                frame = GetGafFrame(g, entries[i].sliderStyle + 6);
                if (entries[i].w > entries[i].h) {
                    // Spelled x - (fw - w), not x - fw + w.
                    secondEnd->x = entries[i].x - (frame->width - entries[i].w);
                    entries[i].w += (short)(frame->width * -2);
                    entries[i].x += frame->width;
                    frame = GetGafFrame(g, entries[i].sliderStyle + 5);
                    entries[i].sliderThumb = frame->width;
                    entries[i].field_136 = entries[i].w - entries[i].sliderThumb - 4;
                } else {
                    secondEnd->y = entries[i].y - (frame->height - entries[i].h);
                    secondEnd->x = entries[i].x;
                    entries[i].h += (short)(-2 * frame->height);
                    entries[i].y += frame->height;
                }
            } else {
                entries[i].field_136 = (entries[i].w > entries[i].h ? entries[i].w : entries[i].h) - 6;
            }
            break;
        }

        case 3: {
            GafEntry* input = menu->gaf ? FindGafEntry(menu->gaf, "TEXTINPUT") : 0;
            if (input != 0) {
                for (int f = 0; f < input->count; f++) {
                    Glyph* frame = GetGafFrame(input, f);
                    frame->yoff = 0;
                    frame->xoff = 0;
                }
            }
            entries[i].inputGaf = input;
            if (entries[i].field_138 >= 0x80)
                entries[i].field_138 = 0x7f;
            memset(entries[i].u.text, 0, 0x80);
            break;
        }
        case 2: {
            GafEntry* list = menu->gaf ? FindGafEntry(menu->gaf, "LISTBOX") : 0;
            if (list != 0) {
                for (int f = 0; f < list->count; f++) {
                    Glyph* frame = GetGafFrame(list, f);
                    frame->yoff = 0;
                    frame->xoff = 0;
                }
            }
            entries[i].u.list.gaf = list;
            int j = 1;
            for (; j <= entries[0].u.count; ) {
                Entry* other = &entries[j];
                if (j != i && other->type == 2) {
                    if (other->team == entries[i].team) {
                        short scroll = other->u.list.scroll > entries[i].u.list.scroll
                                         ? other->u.list.scroll : entries[i].u.list.scroll;
                        entries[i].u.list.scroll = scroll;
                        other->u.list.scroll = scroll;
                    }
                }
                j = j + 1;
            }
            break;
        }
        case 12: {
            entries[i].colours = 0;
            strncpy(textbuf, entries[i].name, 0x10);
            textbuf[0x10] = 0;
            entries[i].u.frame.glyph = 0;
            if (entries[0].u.assets.archive != 0)
                g = FindGafEntry(entries[0].u.assets.archive, textbuf);
            if (0 == g)
                g = FindGafEntry(menu->gaf, textbuf);
            if (g != 0)
                entries[i].u.frame.glyph = GetGafFrame(g, 0);
            break;
        }

        case 1: {
            // pf is built from cur, not &entries[i].flags; the 0x80 test reads entries[i].flags.
            Entry* cur = &entries[i];
            cur->colours = 0;
            pf = &cur->flags;
            if ((cur->flags & 0x1800) || (cur->resourceFlags & 1))
                break;
            FUN_004a05e0(menu, i);
            strncpy(textbuf, entries[i].name, 0x10);
            entries[i].field_13b = 0;
            textbuf[0x10] = 0;
            if (entries[0].u.assets.archive)
                g = FindGafEntry(entries[0].u.assets.archive, textbuf);
            if (g == 0) {
                if (menu->gaf != 0) {
                    g = FindGafEntry(menu->gaf, textbuf);
                    if (0 == g) {
                        if (0x80 & entries[i].flags) {
                            g = FindGafEntry(menu->gaf, "CHECKBOX");
                        } else if (entries[i].stage != 0) {
                            if (strcmp(entries[i].u.text, "Off|On") != 0 && entries[i].stage != 1 && 0 == (*pf & 0x4000)) {
                                int n = entries[i].stage < 4 ? entries[i].stage : 4;
                                sprintf(stagebuf, "stagebuttn%d", n);
                            } else {
                                entries[i].stage = 2;
                                strcpy(stagebuf, "stagebuttn1");
                                *pf |= 0x4000;
                            }
                            g = FindGafEntry(menu->gaf, stagebuf);
                        } else {
                            strcpy(stagebuf, "BUTTONS0");
                            g = FindGafEntry(menu->gaf, stagebuf);
                        }
                        if (g) {
                            int best = 1000;
                            for (int f = 0; f < g->count; ++f) {
                                Glyph* frame = GetGafFrame(g, f);
                                if (frame != 0) {
                                    frame->yoff = 0;
                                    frame->xoff = 0;
                                }
                            }
                            for (int j = 0; j < g->count; j += 4) {
                                Glyph* frame = GetGafFrame(g, j);
                                int distance = abs(entries[i].h - frame->height) + abs(entries[i].w - frame->width);
                                if (distance < best) {
                                    entries[i].field_13b = (unsigned char)j;
                                    best = distance;
                                }
                            }
                        }
                    }
                }
            }
            entries[i].gaf = g;
            if (g != 0) {
                Glyph* frame = GetGafFrame(g, entries[i].field_13b);
                if (frame != 0) {
                    entries[i].w = frame->width;
                    entries[i].h = frame->height;
                }
            }
            if (0 != entries[i].stage) {
                char* p = entries[i].u.text;
                while (*p) {
                    if (*p == '|')
                        *p = 0;
                    p++;
                }
                cur = &menu->layer->entries[i];
                char* dst = buf3;
                char* src = cur->u.text;
                int k = 0;
                for (; k < cur->stage; k++) {
                    strcpy(dst, Translate(src));
                    dst += strlen(dst) + 1;
                    src += strlen(src) + 1;
                }
                memcpy(cur->u.text, buf3, sizeof(buf3));
                *pf = (*pf & 0x4000) | 1;
            }
            break;
        }

        case 7:
            strcpy(buf2, menu->str_bb6);
            strcat(buf2, entries[i].u.text);
            strcat(buf2, ".FNT");
            entries[i].u.list.filebuf = HAPI_LoadFile(buf2, 0);
            break;

        case 8:
            strcat(buf2, entries[i].u.text);
            entries[i].u.list.filebuf = HAPI_LoadFile(buf2, 0);
            break;

        case 13: {
            Entry* en = menu->layer->entries;
            en[i].u.anim.field_c6 = GetTicks() + en[i].u.anim.field_c2;
            break;
        }

        case 5:
            if (strlen((char*)&entries[i].field_136) == 0)
                entries[i].flags |= 0x10;
            else
                FUN_004a05e0(menu, i);
            entries[i].colours = 0;
            break;

        default:
            break;
        }
        i++;
    }

    name = entries[0].name;
    if (0 == name)
        name = "GUI SURFACE";
    entries[0].u.assets.surface = AllocSurface(name, entries[0].w, entries[0].h);
    DrawSurface(entries[0].u.assets.surface, 0, -entries[0].x, -entries[0].y);
    if (!(flags & 0x20)) {
        entries[0].u.assets.saveUnder = AllocSurface("SAVE UNDER", entries[0].w, entries[0].h);
        DrawSurface(entries[0].u.assets.saveUnder, entries[0].u.assets.surface, 0, 0);
    } else {
        entries[0].u.assets.saveUnder = 0;
    }
    }

    if ((flags & 4) != 0 || force || (flags & 0x40)) {
        if (force || (flags & 0x40)) {
            if (menu->layer->field_24)
                DrawSurface(entries[0].u.assets.surface, menu->layer->field_24, 0, 0);
            else if ((flags & 0x80) == 0)
                DrawListboxFrame(menu, 0, entries[0].u.assets.background);
        }

        for (i = 1; i < 1 + entries[0].u.count; i++) {
            if (entries[i].field_29 == 0)
                continue;
            switch (entries[i].type) {
            case 11:
                DrawListboxFrame(menu, i, entries[i].u.assets.background);
                break;
            case 12:
                FUN_004a5e50(menu, i);
                break;
            case 1:
                if (force != 0 || (flags & 0x48) != 0)
                    DrawButton(menu, i);
                break;
            case 2: {
                if (force) {
                    int fh;
                    Entry* base = menu->layer->entries;
                    int t = 0;
                    int j;
                    base[i].u.list.field_bc = 0;
                    base[i].u.list.field_ba = 0;
                    for (j = 1; j < base[0].u.count + 1; j++) {
                        if (base[j].type == 7) {
                            if (t == base[i].tab) {
                                SetFont(base[j].u.list.language);
                                break;
                            }
                            t++;
                        }
                    }
                    if (j == base[0].u.count + 1)
                        SetFont(g_guiContext->font);
                    if (g_guiContext->language == 0)
                        fh = GetFontHeight();
                    else
                        fh = GetGafFrame(g_guiContext->language->glyphs, 0x49)->height + 2;
                    int hh = base[i].h;
                    base[i].h = (short)(hh - ((int)base[i].h) % (fh + 2));
                    base[i].u.list.sortKey = GetTicks();
                }
                if (force || (flags & 0x40))
                    DrawListBox(menu, i);
                break;
            }
            case 3:
                if (force || (flags & 0x40))
                    DrawTextInput(menu, i);
                break;
            case 4:
                if (force) {
                    Entry* base = menu->layer->entries;
                    base[i].field_140 = 0;
                    base[i].callback = 0;
                    base[i].callbackArg = 0;
                }
                if (force || (0x40 & flags))
                    DrawSlider(menu, i);
                break;
            case 5:
                if (force || (flags & 0x40))
                    FUN_004a56b0(menu, i);
                break;
            case 6:
                if (force) {
                    Entry* base = menu->layer->entries;
                    base[i].u.t6.f_b6 = 0;
                    base[i].u.t6.f_be = 0;
                    base[i].u.t6.f_c2 = 0;
                    base[i].u.t6.f_c6 = 0;
                }
                if (force || (flags & 0x40))
                    FUN_004a4980(menu, i);
                break;
            case 13:
                if (force) {
                    Entry* base = menu->layer->entries;
                    base[i].u.anim.field_c6 = GetTicks() + base[i].u.anim.field_c2;
                }
                if (force || (flags & 0x40))
                    FUN_004a4660(menu, i);
                break;
            case 10:
                if (force || (flags & 0x40))
                    FUN_004a4c90(menu, i, flags);
                break;
            default:
                break;
            }
    }
        if (menu->layer->current != -1 && menu->field_a2 != 0) {
            savedType = entries[menu->layer->current].type;
            FUN_004a16f0(menu, menu->layer->current, 8);
            int j = FindEntry(entries, entries[0].u.text + 0x16);
            if (j != -1 && savedType != 1 && entries[i].field_29 != 0)
                FUN_004a16f0(menu, j, 8);
        }
    }

    if (flags & 2) {
        if (entries[0].u.assets.saveUnder != 0) {
            DrawSurface(0, entries[0].u.assets.saveUnder, entries[0].x, entries[0].y);
            FreeSurface(entries[0].u.assets.saveUnder);
            entries[0].u.assets.saveUnder = 0;
        }
        FreeSurface(entries[0].u.assets.surface);
        entries[0].u.assets.surface = 0;
        for (int j = 0; j < 1 + entries[0].u.count; j = j + 1) {
            if ((entries[j].resourceFlags & 1) && entries[j].archive)
                FUN_004d85a0(entries[j].archive);
            switch (entries[j].type) {
            case 0:
                FUN_004d85a0(entries[j].u.assets.archive);
                break;
            case 7:
                FUN_004d85a0(entries[j].u.list.filebuf);
                break;
            case 8:
                FUN_004d85a0(entries[j].u.list.filebuf);
                break;
            default:
                break;
            }
        }
    }

    return 1;
}

// Closes the top GUI screen: runs its close handler, pops it off the stack,
// activates the next one and frees the old node.
// FUNCTION: 0x4a9660
void __stdcall CloseTopScreen(Gui* gui)
{
    if (gui->layer) {
        unsigned int flags = gui->layer->flags;
        gui->field_68 = gui->field_60 = gui->focus = -1;
        if (gui->layer->handler)
            gui->layer->handler(gui);
        HideSoftwareCursor();
        RenderLayer(gui, 2);
        ShowSoftwareCursor();
        Layer* old = gui->layer;
        gui->layer = old->next;
        if (gui->layer)
            gui->layer->dirty = 1;
        FUN_004d85a0(old);
        if (flags & 0x800)
            RenderLayer(gui, 0x40);
    }
}

// Decrements the scroll offset (field_140) of GUI entry `index`, clamped to
// [0, field_136 - 1]. When the value actually changes it marks the object
// changed, refreshes the gadget and runs the entry's callback (if any).
// FUNCTION: 0x4a96d0
void __stdcall DecrementKnobPos(Gui* obj, int index)
{
    Entry* e = &obj->layer->entries[index];
    short raw = e->field_140;
    // Keep the int copy of the old value: it fixes the register order of the entry address.
    int old = raw;
    e->field_140 = raw - 1;
    if (e->field_140 > e->field_136 - 1) {
        e->field_140 = e->field_136 - 1;
    }
    if (e->field_140 < 0) {
        e->field_140 = 0;
    }
    if (e->field_140 != old) {
        obj->changed = 1;
        FUN_004a2580(obj, index);
        FUN_004a2be0(obj, index);
    }
    if (e->callback) {
        e->callback(obj, e->callbackArg);
    }
}

// Forward declarations of the list steps below: their symbol ids keep IncrementKnobPos matching.
void __stdcall FUN_004a9830(Gui* param_1, int index);
void __stdcall FUN_004a99c0(Gui* param_1, int index);

// Must stay a static inline helper: written inline it changes the load order.
static inline Entry* entry_at(Gui* obj, int index)
{
    return &obj->layer->entries[index];
}

// Increments the scroll offset (field_140) of GUI entry `index`, clamped to
// [0, field_136 - 1]. When the value actually changes it marks the object
// changed, refreshes the gadget and runs the entry's callback (if any).
// Note: the upper clamp still uses field_136 - 1, as the copy-paste source of
// this function did, even though this side scrolls the other way.
// FUNCTION: 0x4a9780
void __stdcall IncrementKnobPos(Gui* obj, int index)
{
    Entry* e = entry_at(obj, index);
    short raw = e->field_140;
    // Keep the int copy of the old value: it fixes the register used for it.
    int old = raw;
    e->field_140 = raw + 1;
    if (e->field_140 > e->field_136 - 1) {
        e->field_140 = e->field_136 - 1;
    }
    if (e->field_140 < 0) {
        e->field_140 = 0;
    }
    if (e->field_140 != old) {
        obj->changed = 1;
        FUN_004a2580(obj, index);
        FUN_004a2be0(obj, index);
    }
    if (e->callback) {
        e->callback(obj, e->callbackArg);
    }
}

// The list gadget's scroll-up step, the sibling of the scroll-down step
// 0x4a99c0. It first does what 0x4a99c0 does: picks the entry of type 7 whose
// group number matches entry `index` and makes that entry's id the current
// one (falling back to the current id of the list holder). Then it works out
// how far the visible window moves per line and, when the selected line is
// still inside the window and the window has not run off the top, moves the
// selection one line up, scrolls the window if needed and refreshes the
// gadget. A selection on line 0 is not moved, and a line whose text starts
// with "&G" is not moved either.
// FUNCTION: 0x4a9830
void __stdcall FUN_004a9830(Gui* param_1, int index)
{
    Entry* entries = param_1->layer->entries;
    Entry* me = &entries[index];
    int n = 0;
    int i = 1;
    // The `count + 1` condition keeps n in the dead argument slot.
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->font);
    }
    int size;
    if (g_guiContext->language == 0) {
        size = GetFontHeight();
    } else {
        // Indexed pg[1], not a pointer local: keeps the +2 offset in the load.
        unsigned short* pg = (unsigned short*)GetGafFrame(g_guiContext->language->glyphs, 0x49);
        size = pg[1] + 2;
    }
    size++;
    int step = (me->h - 2) / size;
    short last = me->u.list.field_bc;
    short sel = me->u.list.field_ba;
    int isel = sel;
    if (sel < last + step && sel >= last) {
        if (me->u.list.field_c0 == 0) {
            return;
        }
        if (sel == 0) {
            return;
        }
        short prev = sel - 1;
        me->u.list.field_ba = prev;
        if (prev < last) {
            last--;
            me->u.list.field_bc = last;
        }
        if (me->u.list.field_c2 != 0) {
            char* line = SkipTextLines(me->u.list.field_c2, prev);
            if (strncmp(DAT_00502a20, line, 2) == 0) {
                me->u.list.field_ba = isel;
            }
        }
        DrawListBox(param_1, index);
        FUN_004a2be0(param_1, index);
        return;
    }
    if (me->u.list.field_c0 != 0) {
        FUN_004a2e40(param_1, me->name, isel);
    }
}

// Forward declarations of the functions below: their symbol ids keep FUN_004a99c0 matching.
int __stdcall HandleGuiCommand(Gui* obj, int cmd);
int __stdcall UpdateMenu(Gui* menu);
void __stdcall SetCurrentGuiContext(Gui* ctx);
int FUN_004aa8d0(void);
void __stdcall FUN_004aa8e0(int* param_1, int param_2);

// The list gadget's scroll-down step. First it does what 0x4a1810 does: picks
// the entry of type 7 whose group number matches entry `index` and makes that
// entry's id the current one (falling back to the current id of the list
// holder). Then it works out how far the visible window moves per line and,
// when the selected line has fallen below the window but is still inside the
// list, moves the selection one line down, scrolls the window if needed and
// refreshes the gadget. When the selection is already at the last line
// nothing happens. A selection sitting on a line whose text starts with
// "&G" is not moved either.
// FUNCTION: 0x4a99c0
void __stdcall FUN_004a99c0(Gui* param_1, int index)
{
    Entry* entries = param_1->layer->entries;
    Entry* me = &entries[index];
    int n = 0;
    int i = 1;
    for (; i < entries->u.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->tab) {
                SetFont(entries[i].u.list.language);
                break;
            }
            n++;
        }
    }
    if (i == entries->u.count + 1) {
        SetFont(g_guiContext->font);
    }
    int size = (g_guiContext->language == 0) ? GetFontHeight()
        : (*(unsigned short*)((int)GetGafFrame(g_guiContext->language->glyphs, 0x49) + 2) + 2);
    size++;
    int step = (me->h - 2) / size;
    short last = me->u.list.field_bc;     // last line of the window
    short sel = me->u.list.field_ba;      // selected line
    // Int copy of sel, kept live across the calls below: using sel directly changes the code.
    int isel = sel;
    if (isel < last + step && sel >= last) {
        if (me->u.list.field_c0 != 0) {
            if (isel == me->u.list.field_be + step - 1) {
                return;
            }
            short next = sel + 1;
            me->u.list.field_ba = next;
            if (next > last + step - 1) {
                me->u.list.field_bc = last + 1;
            }
            if (next >= me->u.list.field_c0 - 1) {
                me->u.list.field_ba = me->u.list.field_c0 - 1;
            }
            if (me->u.list.field_c2 != 0) {
                char* line = SkipTextLines(me->u.list.field_c2, me->u.list.field_ba);
                if (strncmp(DAT_00502a20, line, 2) == 0) {
                    me->u.list.field_ba = isel;
                }
            }
            DrawListBox(param_1, index);
            FUN_004a2be0(param_1, index);
            return;
        }
    }
    if (me->u.list.field_c0 != 0) {
        FUN_004a2e40(param_1, me->name, isel);
    }
}

// The GUI layer's command handler, called by 0x4a9fd0 with a decoded key/command
// in `cmd`. It looks up the layer's currently selected gadget (layer->current,
// layer->entries) and switches on the command:
//   9     toggle a checkbox-ish gadget (IsKeyDown(0xf9))
//   0x1b  make the gadget whose stored name matches entries[0].choice2 current
//   0xd   same for entries[0].choice, then fall through into 0x20
//   0x20  activate the selected gadget (types 1, 2, 6); type 1 also cycles its
//         stageIndex sub-index and re-selects it
//   0xf4/0xf6  scroll the list one line up / down (type 4 gadgets)
//   0xf5/0xf7  page the list up / down (type 2 gadgets)
// A handled command is returned as 0, an unhandled one unchanged. The common
// tail refreshes the holder when nothing consumed the command and records the
// newly selected gadget in obj->field_60.
// FUNCTION: 0x4a9b90
int __stdcall HandleGuiCommand(Gui* obj, int cmd)
{
    int newsel = -1;
    int index = obj->layer->current;
    Entry* entries = obj->layer->entries;
    Entry* e = &entries[index];
    int type = e->type;

    // Case order follows the original emit order, not ascending.
    switch (cmd) {
    case 9:
        if (IsKeyDown(0xf9))
            FUN_004a7960(obj, 0);
        else
            FUN_004a7960(obj, 1);
        obj->changed = 1;
        cmd = 0;
        break;
    case 0x1b:
        {
            int found = FindEntry(entries, entries[0].u.names.choice2);
            if (found == -1 || entries[found].field_29 == 0)
                break;
            newsel = found;
        }
        cmd = 0;
        break;
    case 0xd:
        if (obj->focus != -1 && entries[obj->focus].type == 3)
            break;
        {
            int found = FindEntry(entries, entries[0].u.names.choice);
            if (found != -1 && entries[found].field_29 != 0
                && !(entries[found].type == 1 && (entries[found].field_13c & 1))) {
                newsel = found;
                cmd = 0;
                break;
            }
        }
        // fall through to case 0x20
    case 0x20:
        if (type == 3)
            break;
        if (type != 1 && type != 2 && type != 6)
            break;
        if (e->field_29 == 0)
            break;
        if (type == 1 && (e->field_13c & 1))
            break;
        newsel = index;
        if (type == 1) {
            if (e->flags & 0x10) {
                e->field_138 = 1;
                FUN_004a0340(obj, index);
                DrawButton(obj, index);
            }
        }
        if (type == 1 && e->stage != 0) {
            // Wrap through a pointer: a local copy makes the stage reload differ.
            unsigned char* p = &e->stageIndex;
            if (++*p >= e->stage)
                *p = 0;
        }
        cmd = 0;
        break;
    case 0xf5:
        if (type == 2) {
            FUN_004a9830(obj, index);
            if (e->u.list.callback)
                e->u.list.callback(obj, e);
        } else {
            FUN_004a7960(obj, 2);
        }
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf4:
        if (type == 3)
            break;
        if (type == 4 && e->w > e->h)
            DecrementKnobPos(obj, index);
        else
            FUN_004a7960(obj, 0);
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf7:
        if (type == 2) {
            FUN_004a99c0(obj, index);
            if (e->u.list.callback)
                e->u.list.callback(obj, e);
        } else {
            FUN_004a7960(obj, 3);
        }
        obj->changed = 1;
        cmd = 0;
        break;
    case 0xf6:
        if (type == 3)
            break;
        if (type == 4 && e->w > e->h)
            IncrementKnobPos(obj, index);
        else
            FUN_004a7960(obj, 1);
        obj->changed = 1;
        cmd = 0;
        break;
    }
    if (cmd == 0 && obj->layer->field_18 == 0)
        PopKey();
    if (newsel != -1) {
        obj->field_60 = newsel;
        obj->changed = 1;
    }
    return cmd;
}

// FUN_004a7190's body with its group scan left as a call to SelectFontForEntry, as
// on the case 5 path here (and twice in HandleTextInput).
static inline void SelectCurrentByName(Gui* menu, Entry* entries, int sel)
{
    Entry* entry = &entries[sel];
    SetTextColors(menu->colours[entry->colours], GetTextKeyColor());
    SelectFontForEntry(entries, sel);
    FUN_0049fc50(menu, sel);
    menu->layer->current = sel;
    CommitTextEdit(menu, sel, entry->u.text, entry->field_138, 0);
    ClearKeyQueue();
}

// The real UpdateHelpText (0x4a0090), inlined here by /Ob2.
static inline void UpdateHelpText(Gui* obj)
{
    char* text = DAT_005119b8;
    if (obj->field_68 != -1) {
        obj->field_6c = obj->field_68;
        text = obj->layer->entries[obj->field_68].helpKey;
    }
    int found = FindEntry(obj->layer->entries, "HELPTEXT");
    if (found != -1) {
        strcpy(obj->layer->entries[found].u.text, Translate(text));
        obj->changed = 1;
    }
}

// The real FUN_004a4890 (0x4a4890), inlined here by /Ob2.
static inline void FUN_004a4890(Gui* menu, int i)
{
    // Entries in their own local first: the one-expression form shifts registers.
    Entry* entries = menu->layer->entries;
    Entry* e = &entries[i];
    if (e->u.anim.field_ce && e->u.anim.field_ba < e->u.anim.field_be) {
        if ((int)GetTicks() > e->u.anim.field_c6) {
            e->u.anim.field_ba += (int)e->u.anim.field_ca;
            if (e->u.anim.field_ba > e->u.anim.field_be) {
                e->u.anim.field_ba = e->u.anim.field_be;
                e->u.anim.field_ce = 0;
            }
            e->u.anim.field_c6 = GetTicks() + e->u.anim.field_c2;
        }
        FUN_004a4660(menu, i);
    }
}

// The real SelectGadgetByIndex, inlined here by /Ob2.
static inline void SelectGadgetByIndex_inlined(Gui* menu, int index)
{
    Entry* first = menu->layer->entries;
    menu->focus = -1;
    menu->layer->current = index;
    if (first[menu->layer->current].type == 3)
        FUN_004a7190_inlined(menu, menu->layer->current);
}

// The real CloseTopScreen, inlined here by /Ob2.
static inline void CloseTopScreen_inlined(Gui* gui)
{
    if (gui->layer) {
        unsigned int flags = gui->layer->flags;
        gui->field_68 = gui->field_60 = gui->focus = -1;
        if (gui->layer->handler)
            gui->layer->handler(gui);
        HideSoftwareCursor();
        RenderLayer(gui, 2);
        ShowSoftwareCursor();
        Layer* old = gui->layer;
        gui->layer = old->next;
        if (gui->layer)
            gui->layer->dirty = 1;
        FUN_004d85a0(old);
        if (flags & 0x800)
            RenderLayer(gui, 0x40);
    }
}

// The per-frame update of the current screen: tracks the mouse, runs the key
// command handler, then gives every enabled entry its input handler by type
// and closes the screen when an entry was chosen.
// FUNCTION: 0x4a9fd0
int __stdcall UpdateMenu(Gui* menu)
{
    // Declared at the top of the function.
    int k;
    Entry* e;
    int i;
    Entry* entries = 0;

    // Real early returns: the original has several, so no shrink-wrapping.
    if (menu->layer == 0)
        return 0;

    int now = GetTicks();
    menu->field_9a = now - menu->time;
    menu->time = now;
    UpdateCursorAndMouse(menu);

    int key;
    if (menu->layer->field_18 == 0) {
        key = PeekKey();
        if (key >= 0xe2 && key <= 0xeb)
            key = 0;
    } else {
        key = PopKey();
    }

    if (menu->layer->field_18 != 0 && key != 0 && menu->field_a2 != 0) {
        key = HandleGuiCommand(menu, key);
        if (key != 0) {
            for (int n = 0; n < 0xe; n++)
                menu->layer->text[n] = menu->layer->text[n + 1];
            menu->layer->field_36 = (char)toupper(key);
            if (menu->layer->cb3b != 0)
                menu->layer->cb3b(menu);
            menu->field_60 = -1;
        }
    }

    int sel = menu->field_60;
    if (menu->layer == 0)
        return 1;
    if (menu->changed == 1) {
        menu->changed = 0;
        RenderLayer(menu, menu->layer->flags | 0x40);
    }

    // Read after the changed block, not at the top.
    entries = menu->layer->entries;
    if (entries == 0)
        return 1;

    Point& pt = menu->point;
    {
        // Rect local, not named ints: right/bottom spill into slots shared with `point`.
        Rect box;
        box.left = entries->x;
        box.top = entries->y;
        if (entries->type != 0) {
            box.left *= 2;
            box.top *= 2;
        }
        // Load y early, before right and bottom.
        int ptY = pt.y;
        box.right = entries->w + box.left - 1;
        box.bottom = entries->h + box.top - 1;
        SetCursorHover(menu, pt.x >= box.left && pt.x <= box.right &&
                           ptY >= box.top && ptY <= box.bottom);
    }

    int saved = menu->field_68;
    menu->field_68 = -1;

    Point point;
    memcpy(&point, &menu->point, 24);
    point.x -= entries->x;
    point.y -= entries->y;

    int elapsed;
    if ((int)GetTicks() - DAT_0051fbb4 > 0) {
        elapsed = 1;
        DAT_0051fbb4 = GetTicks();
    } else {
        elapsed = 0;
    }

    i = 1;
    for (; i < entries->u.count + 1; i++) {
        e = &entries[i];
        if (e->field_29 != 0) {
            int x, y;
            if (e->type == 0) {
                x = 0;
                y = 0;
            } else {
                x = e->x;
                y = e->y;
            }
            int right = e->w + x - 1;
            int bottom = e->h + y - 1;
            if (point.x >= x && point.x <= right && point.y >= y && point.y <= bottom)
                menu->field_68 = i;

            k = IsKeyDown(0xfb) == 0 ? key : 0;
            switch (e->type) {
            case 1:
                if (HandleButtonInput(menu, i, key) == 1)
                    sel = i;
                if (e->colours != 0 && elapsed) {
                    e->colours -= 2;
                    if (e->colours < 0)
                        e->colours = 0;
                    if (menu->layer != 0)
                        menu->layer->dirty = 1;
                }
                break;
            case 2:
                if (HandleListBoxInput(menu, i, 0) == 1)
                    sel = i;
                break;
            case 3:
                if (HandleTextInput(menu, i, k) == 1)
                    sel = i;
                break;
            case 4:
                HandleSliderInput(menu, i);
                break;
            case 5:
                if (FUN_004a4440(menu, i, key) != 0) {
                    sel = -1;
                    int found = FindEntry(entries, (char*)&e->field_136);
                    // Keep this if/else nesting, with sel = i in the else.
                    if (found != sel) {
                        Entry* me;
                        sel = found;
                        me = &entries[found];
                        if (me->type == 1) {
                            if (me->field_29 == 0 || (me->field_13c & 1) != 0) {
                                sel = -1;
                            } else {
                                me->stageIndex++;
                                if (me->stageIndex >= me->stage)
                                    me->stageIndex = 0;
                                DrawButton(menu, found);
                            }
                        } else {
                            if (me->field_29 != 0) {
                                if (me->type == 4 && me->field_157 != 0) {
                                    sel = -1;
                                } else {
                                    // Helper call, not the inline chain: keeps the group scan an explicit call.
                                    Entry* entriesNow = menu->layer->entries;
                                    menu->focus = -1;
                                    menu->layer->current = found;
                                    if (entriesNow[menu->layer->current].type == 3)
                                        SelectCurrentByName(menu, menu->layer->entries, menu->layer->current);
                                }
                            } else {
                                sel = -1;
                            }
                        }
                    } else {
                        sel = i;
                    }
                }
                break;
            case 6:
                if (FUN_004a4b50(menu, i) == 1)
                    sel = i;
                break;
            case 13:
                FUN_004a4890(menu, i);
                break;
            case 12:
                if (e->colours != 0 && elapsed) {
                        e->colours--;
                        FUN_004a5e50(menu, i);
                        if (menu->layer != 0)
                            menu->layer->dirty = 1;
                    }
                break;
            }
        }
        if (sel != -1)
            break;
    }

    if (menu->field_68 != saved)
        UpdateHelpText(menu);

    if (menu->layer->cb1c != 0)
        menu->layer->cb1c();

    if (sel != -1) {
        menu->field_60 = sel;
        SelectGadgetByIndex_inlined(menu, sel);
        if (menu->layer->handler != 0)
            menu->layer->handler(menu);
        if (menu->field_60 != -1)
            CloseTopScreen_inlined(menu);
    }
    return 1;
}

// Makes `ctx` the current context (g_guiContext) and resets its state.
// FUNCTION: 0x4aa850
void __stdcall SetCurrentGuiContext(Gui* ctx)
{
    g_guiContext = ctx;
    ctx->layer = 0;
    ctx->str_9b6[0] = 0;
    ctx->str_ab6[0] = 0;
    ctx->str_bb6[0] = 0;
    ctx->field_cc6 = 1;
    ctx->time = GetTicks();
    ctx->field_9a = 0;
    ctx->field_9e = 1;
    ctx->gaf = 0;
    ctx->field_cd2 = 0;
    ctx->field_cd6 = 0;
    ctx->field_78 = 0;
    ctx->field_68 = -1;
    memset(ctx->values, 0, 12);
    ctx->language = 0;
    ctx->field_a2 = 1;
}

// FUNCTION: 0x4aa8d0
int FUN_004aa8d0(void)
{
    return (int)g_guiContext;
}

// FUNCTION: 0x4aa8e0
void __stdcall FUN_004aa8e0(int* param_1, int param_2)
{
    *param_1 = param_2;
}
