// Decompiled by deepseek-v4.1, finished by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// (earlier work by deepseek-v4.1-flash and GPT-6.1-sol)
//
// STATUS MATCH (1015/1015 bytes).
//
// The only block that ever differed was the char-insert default case
// (0x4ab972): the original evaluates GetTextPixelWidth(text) first, ours evaluated
// GetTextPixelWidth(c) first. The cause is argument-temporary allocation, not the
// expression's operand order: with `text` already live in ebp, the call whose
// argument needs a `lea` is the "harder" operand and MSVC evaluates it first,
// whichever way the sum is written. Spelling the first operand as
// `entry->text` instead of the `text` local reloads the entry pointer, which
// makes that operand the harder one, so MSVC evaluates the text call first.
// Everything downstream (the `add edi, eax` sum in edi, entry reloaded into
// ecx, and the `mov eax, ecx` copy at 0x4ab99e) then falls out of the same
// allocation change. Every two-statement split of the sum gives the right call
// order but loses 2 bytes and the ecx entry allocation, so it is not a near
// miss; do not spend time on it.
//
// Confirmed non-levers, all measured byte-identical to the combined expression:
// swapped operands `FUN(c) + FUN(text)`, `&c[0]` / `&text[0]` spellings, a
// local `char*` for c, `char c[2] = { (char)key, 0 }`, unsigned char c[2],
// `(char*)&c`, and the sum inlined in the if condition. `int w = F(text);
// int width = w + F(c);` gives the order but `add eax, edi` and 733 code bytes.
//
// The `char c[2]` buffer is not a separate frame local: MSVC 5 shares the dead
// `key` parameter's slot with it, so the stores land on the incoming argument
// home at frame+0x2c. A plain `char c[2]` reproduces that exactly; it is not a
// bug in Cavedog's code. The entry struct must be padded to 0x15b bytes or
// MSVC scales the subscript by 0x13a. The char-insert `int i;` local is
// declared before `char* text` so MSVC makes the subscript the addressing-mode
// index, giving `[ebp+eax]` rather than `[eax+ebp]` in the copy loops.
//
// Text-edit key handler for one GUI entry (stride 0x15b, text at +0xb6).
// __stdcall(control, entryIndex, key): when the holder has no pending
// event source it pulls keys from PopKey. Handles backspace, escape,
// delete, home, end, left, right, clipboard paste and plain character
// insertion, then saves the text back through DrawTextInput.
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

void __stdcall DrawTextInput(Control_004ab720* control, int index);
int __stdcall GetTextPixelWidth(char* text);
int PopKey();
void* GetDisplay();


// FUNCTION: 0x4ab720
int __stdcall HandleTextEditKey(Control_004ab720* control, int index, int key)
{
    Holder_004ab720* holder = control->holder;
    Entry_004ab720* entry = &holder->entries[index];
    int i;
    char* text = entry->text;
    int changed = 0;
    int last = 0;

    if (holder->field_18 == 0)
        key = PopKey();
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
                HWND hwnd = *(HWND*)((char*)GetDisplay() + 0x40);
                if (OpenClipboard(hwnd)) {
                    HANDLE hMem = GetClipboardData(1);
                    if (hMem != 0) {
                        DWORD size = GlobalSize(hMem);
                        if (size != 0) {
                            char* src = (char*)GlobalLock(hMem);
                            memset(text, 0, 0x80);
                            memcpy(text, src, (int)size < entry->capacity - 1 ? (int)size : entry->capacity - 1);
                            int width = GetTextPixelWidth(text);
                            while (width > entry->w) {
                                if (strlen(text) == 0)
                                    break;
                                text[strlen(text) - 1] = 0;
                                width = GetTextPixelWidth(text);
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
                int width = GetTextPixelWidth(entry->text) + GetTextPixelWidth(c);
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
            key = PopKey();
        }
    }
done:
    if (changed)
        DrawTextInput(control, index);
    return last;
}
