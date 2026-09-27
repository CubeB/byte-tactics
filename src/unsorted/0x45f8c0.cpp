// Decompiled by space-bunny-free. Names are provisional.
// Fills a help page (HELP.TDF, node "Help", keys "Line<n>"): for every line of
// the page it looks the line up, cuts it at the '|' into a left and a right
// half and adds two TEXT entries for them, 0x12 pixels lower each time.
//
// PARTIAL (96%). Two spots in the loop preheader still differ:
// - the original loads both arguments into registers and multiplies them
//   (`mov eax,[page]; mov ecx,[lineCount]; imul eax,ecx`); here the second
//   argument is read into ecx and the first folded into the imul's memory
//   operand (`mov ecx,[lineCount]; mov eax,ecx; imul eax,[page]`), so the
//   whole int computation lands after the two pushes instead of before them.
//   MSVC always normalises a multiply this way when one operand is a plain
//   stack-argument read, and no source phrasing tried (both operand orders, a
//   temp per operand, unsigned, an inlined Range() helper, the multiply
//   written twice) changed it.
// - because of that, the `lea esi,[eax+ecx]` and the first byte store of the
//   blank strings swap places, and the address of the value buffer for the
//   first FUN_004b6af0 call lands in ecx instead of edx.
//
// A second pass, all still 96.0%: two separate locals holding the same
// `lineCount` (`int n1 = lineCount; int n2 = lineCount;` then
// `first = page * n1; last = first + n2`, which is the guide's "two weights
// sharing one local" idea applied to this), a local per operand
// (`a = page; b = lineCount;`), a local for only `lineCount`, the reversed
// operand order `lineCount * page`, the product written twice, an `unsigned`
// product, a `(long)` product, a pointer-arithmetic product, assigning into
// `lines.first` / `lines.last` before or after the locals, `last += n` as two
// statements, and a `for` loop instead of the do/while. `tools/headers.py`
// tried all 128 sets and the best is 96.0% with <windows.h>, <ddraw.h>,
// <windows.h>+<stdio.h>, +<stdlib.h> and +<string.h>, so this is not the
// headers-are-compiler-state effect either. The multiply is the only blocker
// and the other two differences are its knock-on effects.
#include <windows.h>
#include <string.h>

#pragma pack(push, 1)
struct Entry_0045f8c0 {              // 0x15b bytes
    char unknown_0[0x1b];
    int field_1b;                     // +0x1b
    char unknown_1f[0x15b - 0x1f];
};

struct Table_0045f8c0 {
    char unknown_0[0xb6];
    short count;                      // +0xb6
};

struct Layer_0045f8c0 {
    char unknown_0[4];
    char* entries;                    // +0x4
};

struct Sub_0045f8c0 {
    char unknown_0[0x18];
    Layer_0045f8c0* layer;            // +0x18
};
#pragma pack(pop)

// One help page's worth of lines: the strings used when a line has no
// translation, and the range of line numbers shown.
struct Page_0045f8c0 {
    char blank[2];                    // +0x0
    char blank2[2];                   // +0x2
    int first;                        // +0x4
    int last;                         // +0x8
};

class Class_004c48c0 {
public:
    char unknown_0[0x19];
    int FUN_004c48c0(char* dst, char* key, size_t size, char* def);
};

class Class_004c2ea0 {
public:
    int field_0;
    Class_004c48c0* current;          // +0x4
    int field_8;
    Class_004c2ea0();
    ~Class_004c2ea0();
};

class Class_004c2f60 {
public:
    int FUN_004c2f60(char* file);
};

class Class_004c3410 {
public:
    int FUN_004c3410(char* name);
};

extern int DAT_00512ef0;
extern char DAT_005119b8[];

void __stdcall FUN_004290f0(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_0049fa90(Sub_0045f8c0* sub);
int __stdcall FUN_004ab1b0(Layer_0045f8c0* layer, char* type, char* text, int x, int y,
                           int width, int attr);
char* __stdcall FUN_004b6af0(char* text, int n);
char* __stdcall FUN_004c5740(char* text);

// Emits one help line as its two text fields, left of the '|' and right of it,
// and marks both entries as used.
static inline void AddLine(Page_0045f8c0* page, Layer_0045f8c0* layer, char* value, int y)
{
    if (value[0] == '|') {
        strcpy(value, page->blank);
    } else {
        char* p = value + 1;
        char c;
        do {
            c = *p++;
        } while (c != '|');
        p[-1] = 0;
    }
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(FUN_004b6af0(value, 0)), 0x28, y, 0x4e, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
    FUN_004ab1b0(layer, "TEXT", FUN_004c5740(FUN_004b6af0(value, 1)), 0x7d, y, 0x12c, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
}

// FUNCTION: 0x45f8c0
void __stdcall FUN_0045f8c0(Sub_0045f8c0* sub, int page, int lineCount)
{
    Layer_0045f8c0* layer = sub->layer;
    ((Table_0045f8c0*)layer->entries)->count = DAT_00512ef0;
    Class_004c2ea0 parser;
    char path[256];
    char key[12];
    char value[0x80];
    FUN_004290f0(path, "gamedata", "help", "TDF");
    if (((Class_004c2f60*)&parser)->FUN_004c2f60(path)) {
        int y = 0x32;
        if (((Class_004c3410*)&parser)->FUN_004c3410("Help")) {
            Page_0045f8c0 lines;
            int first = page * lineCount;
            int last = first + lineCount;
            lines.blank[0] = ' ';
            lines.blank[1] = 0;
            lines.blank2[0] = ' ';
            lines.blank2[1] = 0;
            lines.first = first;
            lines.last = last;
            if (lines.first < lines.last) {
                do {
                    wsprintfA(key, "Line%d", lines.first);
                    if (parser.current->FUN_004c48c0(value, key, 0x80, DAT_005119b8)) {
                        AddLine(&lines, layer, value, y);
                        y += 0x12;
                    }
                } while (++lines.first < lines.last);
            }
        }
    }
    FUN_0049fa90(sub);
}
