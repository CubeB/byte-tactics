// Decompiled by deepseek-v4.1, finished by space-bunny-free. Names are provisional.
// (earlier work by deepseek-v4.1-flash and GPT-6.1-sol)
//
// STATUS 93.7%, partial. Every byte of the 1015 matches except the scheduling
// of the two FUN_004a5030 calls in the char-insert default case (0x4ab972).
// Original: push ebp / mov [esp+0x30],bl / mov [esp+0x31],0 / call FUN(text) /
//   mov edi,eax / lea eax,[esp+0x2c] / push eax / call FUN(c) / add edi,eax
// Ours:     lea eax,[esp+0x2c] / mov [esp+0x2c],bl / push eax /
//   mov [esp+0x31],0 / call FUN(c) / push ebp / mov edi,eax / call FUN(text) /
//   add edi,eax
// Same 29 bytes, same slots (c sits on the param_3 home at frame+0x2c in both),
// only the evaluation order of the sum differs: the compiler picks the FUN(c)
// subtree first here. Everything before 0x4ab972 matches byte for byte, so the
// allocator state at the block is identical; the choice is made inside the
// width expression.
// Tried and all byte-identical to the current file:
//   FUN(c) + FUN(text) (swapped operands), char c[2] = {(char)key,0},
//   char* cp = c, FUN(&c[0]) on either side, unsigned char c[2],
//   sum inlined in the if condition.
// `int w = FUN(text); int width = w + FUN(c);` reproduces the call order and
// every instruction of 0x4ab972..0x4ab98d exactly, but then the sum lands in
// eax (add eax,edi) and the insert block loses its `mov eax,ecx`
// (0x4ab99e entry-in-eax), and the function comes out 2 bytes short.
// `int width = FUN(text); width += FUN(c);` keeps the size but flips the
// insert block the same way. So the remaining diff is one allocator decision
// (which call is hoisted) that no rewrite of that expression moved.
//
// The char-insert block's `mov eax, ecx` (entry held in ecx, w in edx) only
// appears when the width is one combined expression `FUN(text) + FUN(c)`.
// The char-insert `int i;` local is declared before `char* text` so MSVC
// makes the subscript the addressing-mode index, giving the original
// `[ebp+eax]` instead of `[eax+ebp]` in the copy loops.
// The entry struct must be padded to 0x15b bytes or MSVC scales the index by
// 0x13a (compiler uses sizeof(Entry)).
//
// space-bunny-free notes, measured with a true instruction LCS over the code
// bytes only (735 code bytes + 280 of jump-table data in the 1015):
// this file is 735/735 bytes with instruction LCS 238/243, and the ONLY
// structural difference is the call order. Nothing in the width expression
// moves the call order except splitting it into two statements, and that
// costs the insert block:
//   * `int w = F(text); w = w + F(c);` and `w += F(c)`: the call block at
//     0x4ab972..0x4ab98d becomes byte exact, but the entry reload then lands
//     in eax instead of ecx, so `mov eax, ecx` (0x4ab99e) disappears, the
//     copy loop uses dl instead of cl, and the final store becomes
//     [ebp+eax] instead of [ebp+edx]. 733/735 bytes, LCS 232/243.
//   * `int w = F(text); int v = w + F(c);`: right order AND the entry stays in
//     ecx, but the sum lands in eax (`add eax,edi`, one byte) and `mov eax,ecx`
//     is still missing. 733/735 bytes, LCS 238/243, the closest of all.
//   * `int w1 = F(text); int w2 = F(c);` any combination: the sum becomes
//     `lea ecx,[eax+edi]` and the entry lands in eax. Worse.
//   * `if (w + F(c) > entry->w - 4)`: right order, sum in ecx, entry in eax.
// Also tried, all no better: comma in the sum, unsigned accumulator, a local
// `char*` for c, a local pointer to the entry, hoisting the capacity or the
// max-width read above or below the calls, the positive `if (w <= lim)` form,
// an inline two-call helper, and comma-forced `(F(text), F(c))`.
// The single remaining lead is a construct that makes MSVC colour the
// reloaded entry pointer ecx while the width accumulator keeps edi.
//
// The two-byte `c` buffer is NOT a frame local in the original: the frame is
// `sub esp,0x10` plus four pushes, so [esp+0x10] is the entry local and
// [esp+0x2c] is the incoming `key` argument slot, and the original stores at
// [esp+0x30]/[esp+0x31] and passes `lea eax,[esp+0x2c]`. A plain `char c[2]`
// reproduces that exactly, because MSVC 5 shares the dead `key` parameter's
// slot with the local. So that is not a bug in Cavedog's code, it is just
// slot sharing; worth knowing so nobody reads it as a wild pointer.
//
// space-bunny-free, second pass. Two things settled, one lead closed.
//
// 1. The `push ebp` at 0x4ab972 is NOT a frame grow and NOT a stack leak, so
//    do not chase an unbalanced-push construct. It is simply the first
//    FUN_004a5030's argument: `text` is in ebp, the callee is `ret 4`, so it
//    eats the push. The apparent `[esp+0x30]` stores are at esp = E-4, i.e.
//    E+0x2c, the same place ours writes at esp = E. The `lea eax,[esp+0x2c]`
//    at 0x4ab983 and the `mov ecx,[esp+0x10]` at 0x4ab98f are both at esp = E,
//    so they are right too. Everything in the block is consistent and there is
//    no bug in Cavedog's code here.
// 2. The call ORDER cannot be changed inside one expression. Measured on a
//    standalone probe (`build/scratch/0x4ab720/probe.cpp`): for `A + B` where
//    one call's argument is a bare register push and the other's needs a
//    `lea`, MSVC 5 always evaluates the `lea` one FIRST, whichever way round
//    the operands are written. `f(a) + f(c)` and `f(c) + f(a)` both put the
//    stack-array call first. So `FUN(text) + FUN(c)` can never produce the
//    original's order, and every parenthesisation is folded identically:
//    `A + (B + 0)`, `(A + 0) + B`, `A + B * 1`, `A + (B - 0)`, `0 + (A + B)`
//    and `B - -A` all compile byte-identically to the file below.
// 3. Lead closed. The two-statement form does give the original's order AND
//    `add edi, eax`, but it costs TWO independent things, so it is not a near
//    miss any more:
//      a. the sum's destination and the (entry, entry->w - 4) pair trade
//         places one preference step. Original: sum in edi (in place),
//         entry ecx, w-4 edx. `w += F(c)`: sum in edi, entry EAX, w-4 ecx.
//         `int w = F(text); int width = w + F(c);`: sum in eax, entry ecx,
//         w-4 edx. So the original needs the in-place edi sum AND a node that
//         keeps eax busy; neither spelling has both.
//      b. every two-statement spelling is 733 code bytes, not 735, so the
//         jump table after it shifts by two and the byte count can never
//         agree even if the block did.
//    Best of the two-statement family, `if (entry->w - 4 < w)` instead of
//    `if (w > entry->w - 4)`, keeps the 735-byte length and gets the order,
//    the edi sum and the entry in ecx, but sends the `- 4` to eax
//    (`movsx eax / add eax,-4` and `mov edx,ecx` where the original has
//    `movsx edx / sub edx,4` and `mov eax,ecx`): 60 differing bytes,
//    LCS 237/243, against 72 and 238/243 for the file below. Kept the file
//    below because it wins on shape, which is the better predictor.
//    Variants scored: `w = w + F(c)`, `w += F(c)`, `int w; int width = w +
//    F(c)`, `int w; int v; width = w + v`, the positive `if (w <= lim)` form
//    (which emits a THIRD FUN_004a5030 call), `w` at function scope, a live
//    `int n = entry->capacity - 1` across the calls, `(unsigned)w`, a local
//    `char*` for c, an inline `char* mk(char*,int)` builder, and an
//    `entry`-copy local. All worse or equal.
//
// Text-edit key handler for one GUI entry (stride 0x15b, text at +0xb6).
// __stdcall(control, entryIndex, key): when the holder has no pending
// event source it pulls keys from FUN_004c1ab0. Handles backspace, escape,
// delete, home, end, left, right, clipboard paste and plain character
// insertion, then saves the text back through FUN_004a4d70.
#include <string.h>
#include <windows.h>

#pragma pack(push, 1)
struct Entry_004ab720 {                 // 0x15b bytes
    char unknown_0[0x17];
    short w;                            // +0x17 (max pixel width)
    char unknown_19[2];
    unsigned char flags;                // +0x1b
    char unknown_1c[0xb6 - 0x1c];
    char text[0x80];                    // +0xb6
    char unknown_136[0x138 - 0x136];
    short capacity;                     // +0x138 (max text length)
    char unknown_13a[0x15b - 0x13a];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Holder_004ab720 {
    char unknown_0[4];
    Entry_004ab720* entries;            // +0x4
    char unknown_8[0x18 - 8];
    int field_18;                       // +0x18
};
#pragma pack(pop)

struct Control_004ab720 {
    char unknown_0[0x18];
    Holder_004ab720* holder;            // +0x18
    char unknown_1c[0x74 - 0x1c];
    int cursor;                         // +0x74
};

void __stdcall FUN_004a4d70(Control_004ab720* control, int index);
int __stdcall FUN_004a5030(char* text);
int FUN_004c1ab0();
void* FUN_004b6220();


// FUNCTION: 0x4ab720
int __stdcall FUN_004ab720(Control_004ab720* control, int index, int key)
{
    Holder_004ab720* holder = control->holder;
    Entry_004ab720* entry = &holder->entries[index];
    int i;
    char* text = entry->text;
    int changed = 0;
    int last = 0;

    if (holder->field_18 == 0)
        key = FUN_004c1ab0();
    if (key != 0) {
        changed = 1;
        while (key != 0) {
            last = key;
            int len = strlen(text);
            switch (key) {
            case 0xf1:
                control->cursor = len;
                break;
            case 0xf0:
                control->cursor = 0;
                break;
            case 8:
                if (control->cursor != 0) {
                    control->cursor--;
                    int n = strlen(text);
                    for (i = control->cursor; i < n - 1; i++)
                        text[i] = text[i + 1];
                    text[n - 1] = 0;
                }
                break;
            case 0xf4:
                if (control->cursor != 0)
                    control->cursor--;
                break;
            case 0xf6:
                if (control->cursor < len)
                    control->cursor++;
                break;
            case 0xef:
                if (len != 0 && control->cursor < len) {
                    int n = strlen(text);
                    for (i = control->cursor; i < n - 1; i++)
                        text[i] = text[i + 1];
                    text[n - 1] = 0;
                }
                break;
            case 0xbf:
            case 0xee: {
                HWND hwnd = *(HWND*)((char*)FUN_004b6220() + 0x40);
                if (OpenClipboard(hwnd)) {
                    HANDLE hMem = GetClipboardData(1);
                    if (hMem != 0) {
                        DWORD size = GlobalSize(hMem);
                        if (size != 0) {
                            char* src = (char*)GlobalLock(hMem);
                            memset(text, 0, 0x80);
                            memcpy(text, src, (int)size < entry->capacity - 1 ? (int)size : entry->capacity - 1);
                            int width = FUN_004a5030(text);
                            while (width > entry->w) {
                                if (strlen(text) == 0)
                                    break;
                                text[strlen(text) - 1] = 0;
                                width = FUN_004a5030(text);
                            }
                            GlobalUnlock(hMem);
                        }
                    }
                    CloseClipboard();
                }
                break;
            }
            case 0x1b:
                goto done;
            default: {
                if (len == entry->capacity)
                    break;
                if (key < 0x20 || key > 0x7f)
                    break;
                if (entry->flags & 2) {
                    if (!isalnum(key) && key != '_' && key != ' ' && key != '\'')
                        break;
                }
                char c[2];
                c[0] = (char)key;
                c[1] = 0;
                int width = FUN_004a5030(text) + FUN_004a5030(c);
                if (width > entry->w - 4)
                    break;
                int n = entry->capacity - 1;
                for (i = n; i > control->cursor; i--)
                    text[i] = text[i - 1];
                text[control->cursor] = (char)key;
                control->cursor++;
                break;
            }
            }
            key = FUN_004c1ab0();
        }
    }
done:
    if (changed)
        FUN_004a4d70(control, index);
    return last;
}
