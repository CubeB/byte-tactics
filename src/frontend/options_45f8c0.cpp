// Decompiled by space-bunny-free, retried by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by DeepSeek V4.1 Flash, verified by GPT-6, finished by Claude Opus 5.5. Names are provisional.
// Fills a help page (gamedata/help.TDF, node "Help", keys "Line<n>"): for every
// line of the page it looks the line up, cuts it at the '|' into a left and a
// right half and adds two TEXT entries for them, 0x12 pixels lower each time.
//
// MATCH. The '|' scan is `do {} while (*p++ != '|');` with the character
// left as an expression, not a `char c` local. The loaded byte is then an
// expression temporary, so code generation hands it a register from the
// eax/ecx/edx rotation (it lands in cl, the same instruction as before), and
// that one extra rotation step moves the first value-buffer `lea` at
// 0x45f9f7 from ecx to the original's edx. With `c = *p++; } while (c !=
// '|');` the byte is a register variable coloured by the global allocator,
// which does not advance the rotation. Found with a gdb trace of C2's
// FUN_00435c37 (the expression-temporary allocator): the first lea was taken
// with the rotation pointer on ecx, one step short.
//
// The loop preheader needs both multiply operands read through locals that
// are not bare loads: with `page * lineCount` MSVC folds `page` into the
// imul's memory operand (`mov ecx,[lineCount] / mov eax,ecx / imul
// eax,[page]`), two bytes shorter. A ternary with identical arms (`page ?
// page : page`) is not folded away by MSVC 5, so it fails codegen's "this is
// a load" test while emitting nothing. The two operands are read through
// locals declared in the opposite order to the multiply (`p2` then `n`, used
// `n * p2`) because the load order follows the declaration order and the
// original loads `page` first.
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

class TdfRecord {
public:
    char unknown_0[0x19];
    int GetFieldString(char* dst, char* key, size_t size, char* def);
};

class TdfFile {
public:
    int field_0;
    TdfRecord* current;               // +0x4
    int field_8;
    TdfFile();
    ~TdfFile();
    int LoadFile(char* file);
    int SelectRecord(char* name);
};

extern int DAT_00512ef0;
extern char DAT_005119b8[];

void __stdcall BuildDataPath(char* out, const char* dir, const char* name, const char* ext);
void __stdcall FUN_0049fa90(Sub_0045f8c0* sub);
int __stdcall AddTextGadget(Layer_0045f8c0* layer, char* type, char* text, int x, int y,
                           int width, int attr);
char* __stdcall SkipTextLines(char* text, int n);
char* __stdcall Translate(char* text);

// Emits one help line as its two text fields, left of the '|' and right of it,
// and marks both entries as used.
static inline void AddLine(Page_0045f8c0* page, Layer_0045f8c0* layer, char* value, int y)
{
    if (value[0] == '|') {
        strcpy(value, page->blank);
    } else {
        char* p = value + 1;
        do {
        } while (*p++ != '|');
        p[-1] = 0;
    }
    AddTextGadget(layer, "TEXT", Translate(SkipTextLines(value, 0)), 0x28, y, 0x4e, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
    AddTextGadget(layer, "TEXT", Translate(SkipTextLines(value, 1)), 0x7d, y, 0x12c, 2);
    ((Entry_0045f8c0*)layer->entries)[((Table_0045f8c0*)layer->entries)->count].field_1b = 1;
}

// FUNCTION: 0x45f8c0
void __stdcall FillHelpPage(Sub_0045f8c0* sub, int page, int lineCount)
{
    Layer_0045f8c0* layer = sub->layer;
    ((Table_0045f8c0*)layer->entries)->count = DAT_00512ef0;
    TdfFile parser;
    char path[256];
    char key[12];
    char value[0x80];
    BuildDataPath(path, "gamedata", "help", "TDF");
    if (((TdfFile*)&parser)->LoadFile(path)) {
        int y = 0x32;
        if (((TdfFile*)&parser)->SelectRecord("Help")) {
            Page_0045f8c0 lines;
            Page_0045f8c0* pp = &lines;
            int p2 = (page ? page : page);
            int n = (lineCount ? lineCount : lineCount);
            int first = (n ? n : n) * (p2 ? p2 : p2);
            int last = first + n;
            pp->blank[0] = ' ';
            pp->blank[1] = 0;
            pp->blank2[0] = ' ';
            pp->blank2[1] = 0;
            pp->first = first;
            pp->last = last;
            if (pp->first < pp->last) {
                do {
                    wsprintfA(key, "Line%d", pp->first);
                    if (parser.current->GetFieldString(value, key, 0x80, DAT_005119b8)) {
                        AddLine(pp, layer, value, y);
                        y += 0x12;
                    }
                } while (++pp->first < pp->last);
            }
        }
    }
    FUN_0049fa90(sub);
}
