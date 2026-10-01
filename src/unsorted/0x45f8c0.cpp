// Decompiled by space-bunny-free, retried by deepseek-v4.1-flash, finished by space-bunny-free, finished by GPT-6.1-sol, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol. Names are provisional.
// GPT-6.1-sol retry in #4216: four checker invocations, best remains 98.7%; no MATCH. Precomputing the entries pointer scored 55.8%; restored source differs at 0x45f9f7, where `lea ecx`/`push ecx` remains instead of the original `lea edx`/`push edx`.
// GPT-6.1-sol retry: 3 checks retained 98.7%; only the first line-buffer LEA register still differs (EDX in the original, ECX here).
// GPT-6.1-sol (#3170 retry): 4 scored checks, including alternate loop form, helper-returned next y, and caller-held nextY, all retained 98.7% with the same LEA/PUSH register difference.
// Fills a help page (gamedata/help.TDF, node "Help", keys "Line<n>"): for every
// line of the page it looks the line up, cuts it at the '|' into a left and a
// right half and adds two TEXT entries for them, 0x12 pixels lower each time.
//
// 98.7%. One instruction left, at 0x45f9f7. The whole loop preheader now
// matches, which was the blocker for the two earlier passes. What finally made
// MSVC 5 emit the original's
//     mov eax,[page] / mov ecx,[lineCount] / imul eax,ecx
// is that NEITHER operand of the multiply may be a bare load: with
// `page * lineCount` it folds `page` into the imul's memory operand
// (`mov ecx,[lineCount] / mov eax,ecx / imul eax,[page]`), two bytes shorter.
// A ternary with identical arms (`page ? page : page`) is not folded away by
// MSVC 5, so it fails codegen's "this is a load" test while emitting nothing.
// An `& 0x7fffffff` on `page` also works but costs the 5-byte `and`. The two
// operands are read through locals declared in the opposite order to the
// multiply (`p2` then `n`, used `n * p2`) because the load order follows the
// declaration order and the original loads `page` first; all six other
// orderings give the right registers with the two loads swapped, which is the
// 98.0% version.
//
// What is left: the original materialises the value buffer's address for the
// first FUN_004b6af0 call into edx, this version into ecx. Same instruction,
// same operand, only the register, and the second call at 0x45fa45 uses edx in
// both, so it is the register pool state in the merged block, not the value.
// Things tried that did not change it: a named `char* v = value` local in
// AddLine, a local for the first call's string, an explicit `&value[0]`, an
// intermediate temp for FUN_004b6af0's result, a Table* local for
// layer->entries, swapping the '|' and non-'|' branches, moving the
// `int y = 0x32` declaration, an extra char buffer, and four further multiply
// spellings that all produce the identical preheader bytes.
//
// deepseek-v4.1-flash added these failed attempts: tools/headers.py (all 128
// sets, closest 98.7 with <windows.h>), the compiler-state sweep of 0 to 400
// unused `extern int dummyN;` declarations (flat 98.7 throughout), prepending
// 0x45f800's text (both its structs only and its full renamed body, flat 98.7),
// a loop-level `char* v`, an outer `char* vp`, an `int ok` temp for the lookup
// result, AddLine parameter reordering, `char (&value)[0x80]`, `char value[]`,
// `unsigned char` buffer, an inlined identity helper around `value` and around
// FUN_004c5740, an AddText helper wrapping FUN_004ab1b0 (93.4, arg order
// changed), a Layer* local inside AddLine, and a hoisted `char c`. The single
// lea/push register pair is compiler state this file cannot reach.
//
// deepseek-v4.1 (issue #2012 rerun) tried 20 check.py runs on this one hunk:
// `(char*)value`, `value ? value : value`, Page/Layer parameters taken by
// reference, `*value == '|'`, a pre-increment scan (`*++p`, loses 5 bytes),
// `(char)0` and `0L` for the second argument, FUN_004b6af0's first parameter
// typed void*, a cast on FUN_004c5740's argument, textual inlining of the whole
// AddLine body into the loop (no helper at all), `(LPCSTR)` and `(size_t)`
// casts on the wsprintf/lookup arguments, `&value[0]` on the first call only,
// a while-loop spelling of the scan (93.1), and a `char* v = value;` /
// `register char* v = value;` local live across the if/else merge. All of these
// stay at exactly 98.7 with the same `lea ecx` / `push ecx` pair, so the
// difference is not the argument expression: at the merge MSVC 5 has eax, ecx
// and edx free and picks ecx in every spelling probed, while the original has
// edx there and edx again at 0x45fa45. Since both files are 504 bytes and every
// other instruction (including the fragile imul preheader) is identical, the
// checker's remaining hunk is a whole-function coloring tie-break that no
// source-level change in this file reaches.
// deepseek-v4.1 (issue #2012 second rerun) tried 13 further check.py runs on the
// same hunk: plain `int p2 = page;` / `int n = lineCount;` without the ternary
// identities (98.0: the two imul loads swap order, so both operands need the
// identity), a plain `static` (not `inline`) helper, assigning `lines.first` /
// `lines.last` directly with no `first` / `last` locals (89.6: the blank stores
// disappear and the counter homes to edi), `'|' == value[0]` together with
// `'|' != c`, `strcpy(value, &page->blank[0])`, `sizeof(value)` for the lookup
// size, a while-with-assignment scan (93.1), the value buffer wrapped in a
// one-member struct passed as `value.text`, `register` on p2/n/first, and a
// `Page_0045f8c0* pp = &lines` pointer local. Every one leaves the merge-block
// hunk byte for byte the same, so the file stays at 98.7 with the one
// `lea edx` / `push edx` pair above. docs/agent-guide.md already describes this
// shape (an address-taken local whose displacement drifts by 4 per argument
// push): a scheduler tie-break, not a source-level lever.
// deepseek-v4.1 (issue #2465 rerun) ran 27 further check.py runs on that hunk.
// New families tried, all byte-identical at 98.7 with the same lea ecx /
// push ecx pair: address spellings `value + 0`, `0 + value`, `value - 0`,
// `&*(value + 0)`, `value + sizeof(value) - sizeof(value)`,
// `value + (y - y)`, `value + (page->first * 0)`, `&value[0 * y]`,
// `(char*)((int)value)`, `(char*)((unsigned)value)`;
// constant-propagation locals (`int which = 0;` as FUN_004b6af0's second
// argument, `int xx = 0x28, ww = 0x4e, aa = 2;` as the outer arguments);
// a `static void __stdcall` (not inline) helper; a while-loop scan with an
// empty body and a pre-increment scan (both lose bytes); `&page->blank[0]`
// in the strcpy; and the key/value pair wrapped in one anonymous struct
// local (`buf.key` / `buf.value`), which keeps the frame and every offset
// but still picks ecx. The one variation that moves the lea is splitting
// the text into `char* t = FUN_004c5740(FUN_004b6af0(value, 0));`, which
// hoists the lea to [esp+0x34] and still picks ecx (96.0), so the address
// temporary is not the lever: MSVC 5 always prefers ecx here and the
// original evidently had something occupying ecx at the merge.
//
// deepseek-v4.1-flash (issue #2980 retry): confirmed the same hunk with the
// compiler listing itself (tools/wcl /Fa), so no check.py budget was spent on
// dead ends. Further spellings that all still emit `lea ecx`: AddLine declared
// __fastcall (value 1st and 2nd), an int return type, an extra trailing char*
// parameter, value aliases (`char* vv = &value[0];` at the top and after the
// if/else, used for both FUN_004b6af0 calls), `(char*)(void*)value`, `*&value`,
// `value + 0 * y`, and arrays-of-struct forms. `(char*)&value` does move the
// lea, but to a NEW temporary slot at a different offset, so it cannot match.
// The merged-block instruction stream is otherwise byte-identical, so this is
// the compiler's register tie-break for a free ecx/edx and stays at 98.7%.
//
// deepseek-v4.1-flash (this run): 48 more scratch variants on the one hunk,
// none moved it. tools/headers.py --cpp swept all 768 sets (flat 98.7, best
// <windows.h>). Untried-until-now families, all flat: dead self-assignments
// (`int t = pp->first; pp->first = t;`) placed after pp->first, before the loop
// guard, in the loop body, after the success call and in the if condition (some
// reorder the two preheader stores, 98.0, but never the hunk); dead self-assigns
// on y, value, layer, page, n and first; a `char* v = value` before and after the
// split; `bool`-style split arms; `&value[0]` at the AddLine call; a MarkUsed
// helper and an AddText helper (68.1 and 93.4, helpers did not inline like the
// original); the count read through `*(short*)((char*)layer->entries + 0xb6)`
// (identical bytes); moving the `lines`/`pp`, `first`/`last`, `p2`/`n` and
// `int y` declarations to function scope (98.0 to 98.7); reordering the blank
// stores (98.0); chained blank stores. The register choice is a whole-function
// colouring tie that no source form of this function reaches.
// deepseek-v4.1-flash (issue #3405 rerun): 3 more scored variants, all flat 98.7
// with the same lea ecx / push ecx hunk: key/value buffers declared inside the
// do-while body, the '|' cut split into its own static inline CutAtBar helper,
// and dead int results kept from both FUN_004ab1b0 calls. The merge-block
// register pick is unchanged.
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
                    if (parser.current->FUN_004c48c0(value, key, 0x80, DAT_005119b8)) {
                        AddLine(pp, layer, value, y);
                        y += 0x12;
                    }
                } while (++pp->first < pp->last);
            }
        }
    }
    FUN_0049fa90(sub);
}
