// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free. Names are provisional.
// Still differs in WM_COMMAND loop allocation and WM_INITDIALOG local layout (76.1%, best of 5 check runs).
// Remaining hunks, by first differing address:
//   0x4df630 : [esp+0x10]/[esp+0x14] swap, the 0x113 case stores `info` in the slot the original uses for
//              `sel` and vice versa. Both are 11-slot frame decisions over all four branches.
//   0x4df78f : `id` must compare SIGNED: the original is jg/jge/jle, this build is ja/jb/jbe. Declaring
//              `int id = wParam & 0xffff` fixes the mnemonics but MSVC then re-lays the 0x111 blocks out
//              of line and the whole function scores 72.4, so the nesting has to change with it.
//   0x4df7a4 : the `id > 0 && id <= 2` case is inline in the original, out of line here.
//   0x4df8ae : in the 0x111 case the original keeps ~mask in [esp+0x24] and mask in [esp+0x28];
//              here they are the other way round.
//   0x4df988 : the store of the toggled checkbox byte comes after the `push 0` in the original, before it here.
//   0x4dfa5b : the 0x3f4 case has NO Class_004e18c0::FUN_004e18c0 call in the original (only the 0x113
//              case calls it), and its set walk is a rotated do/while: `if (node != head) do { ... }
//              while (node != head)`, entry test at 0x4dfa8a, `jmp 0x4dfaa2` into the body, counter
//              in eax and `head` in esi. Dropping the call and using the do/while shape brings that whole
//              block to instruction-for-instruction agreement, only esi/eax and dl/al differ.
//   0x4dfb59 : 0x110 uses ecx for the entries base and edx for the mask; here they are swapped. Same for
//              the GetWindowRect hwnd/rect registers (ecx/eax against edx/ecx).
//   0x4dfb7a : `1 << i` stored to [esp+0x28] and `(i == 1) ? 0x3ee : 0x3ed` must stay a BRANCH
//              (`cmp ebp,1; mov esi,0x3ed; jne`): a plain ternary folds to `0x3ed + (i==1)`, and the
//              `DAT_00529e00[i]` index needs a typed Entry_004df590 array to keep `shl ecx,4; [ecx+..]`
//              instead of a hoisted pointer in a slot. Both of those changes are confirmed good and are
//              worth re-applying together with the signed `id` once the block layout is understood.
#include <windows.h>
#include <yvals.h>

struct Value_004df590 {
    const char* name;                  // +0x00
    char text[500];                    // +0x04
};

struct Node_004df590 {
    Node_004df590* left;               // +0x0
    Node_004df590* parent;             // +0x4
    Node_004df590* right;              // +0x8
    Value_004df590 value;              // +0xc
};

extern Node_004df590* DAT_005292c4;
struct Iterator_004df590 {
    Node_004df590* ptr;
    Iterator_004df590(Node_004df590* p) : ptr(p) {}
    bool operator==(const Iterator_004df590& other) const { return ptr == other.ptr; }
    bool operator!=(const Iterator_004df590& other) const { return !(*this == other); }
};


struct Map_004df590 {
    char compare;                      // +0x0
    char allocator;                    // +0x1
    Node_004df590* head;               // +0x4
    char multi;                        // +0x8
    int size;                          // +0xc
    char changed;                      // +0x10
};

class Class_004e17c0 {
public:
    Map_004df590 names;                // +0x0
};

Class_004e17c0* FUN_004e1a90();

class Class_004e1ac0 {
public:
    CRITICAL_SECTION cs;
};

Class_004e1ac0* FUN_004e1ac0();

class Class_004e18c0 {
public:
    void FUN_004e18c0();
};

class Class_004e1990 {
public:
    void FUN_004e1990(void* key);
};

class Class_004e0450 {
public:
    void* ptr;
    void FUN_004e0450();
};

class Class_004df280 {
public:
    HWND hwnd;
    char unknown_4[0x1c];
    unsigned char flag_20;
    void FUN_004df280(char show);
};

class Class_004df380 {
public:
    void FUN_004df380();
};

class Class_004df4e0 {
public:
    void FUN_004df4e0();
};

struct Entry_004df590 {
    int field_0;                       // +0x0
    LPARAM text;                       // +0x4
    int flags_8;                       // +0x8
    char* name;                        // +0xc
};

extern unsigned char DAT_00529dd8;
extern unsigned char DAT_00529dd4;
extern unsigned char DAT_00529ddc;
extern unsigned char DAT_00529e64;
extern unsigned char DAT_00529dc8;
extern unsigned char DAT_00529e00[];
extern unsigned char DAT_00529e10[];
extern char* DAT_0050d660;

void __cdecl FUN_004e1b10(int flag);
void __cdecl FUN_004e3400(HWND hwnd, char* name);
void __cdecl FUN_004da5b0(HWND hwnd, const char* url, const char* ext);

class Class_004df590 {
public:
    HWND hwnd;                         // +0x00
    int left;                          // +0x04
    int top;                           // +0x08
    char unknown_0c[0xc];
    int count;                         // +0x18
    Entry_004df590* entries;           // +0x1c
    char flag_20;                      // +0x20
    char unknown_21[3];
    Value_004df590 selected;           // +0x24
    Map_004df590 set;                  // +0x21c

    BOOL FUN_004df590(UINT msg, WPARAM wParam, LPARAM lParam);
};

static inline bool NamesEqual_004df590(const char* a, const char* b) { return a == b || strcmp(a,b) == 0; }

// FUNCTION: 0x4df590
BOOL Class_004df590::FUN_004df590(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case 0x312:
        if (wParam == 10) {
            ((Class_004df280*)this)->FUN_004df280(IsWindowVisible(hwnd) == 0);
        }
        return 0;

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        for (int i = 0; i < 2; i++) {
            int mask = 1 << i;
            int id = (i == 1) ? 0x3ee : 0x3ed;
            int sel = 0;
            int n = 0;
            for (int j = 0; j < count; j++) {
                Entry_004df590* e = &entries[j];
                if (entries[j].flags_8 & mask) {
                    if (e->field_0 == *(int*)(DAT_00529e00 + i * 0x10))
                        sel = n;
                    n++;
                    SendDlgItemMessageA(hwnd, id, 0x143, 0, e->text);
                }
            }
            SendDlgItemMessageA(hwnd, id, 0x14e, sel, 0);
        }
        RegisterHotKey(hwnd, 10, 1, 0x24);
        CheckDlgButton(hwnd, 0x3ef, DAT_00529dd8);
        CheckDlgButton(hwnd, 0x3f1, DAT_00529dd4);
        CheckDlgButton(hwnd, 0x3f6, DAT_00529ddc);
        CheckDlgButton(hwnd, 0x3f7, DAT_00529e64);
        CheckDlgButton(hwnd, 0x3f8, DAT_00529dc8);
        ((Class_004df4e0*)this)->FUN_004df4e0();
        if (flag_20)
            ((Class_004df280*)this)->FUN_004df280(1);
        return 1;
    }

    case 0x111: {
        unsigned int id = wParam & 0xffff;
        if (id <= 0x3ee) {
            if (id >= 0x3ed) {
                if ((wParam >> 16) != 1)
                    return 0;
                int b = 0;
                if ((short)wParam == 0x3ee) b = 1;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x147, 0, 0);
                int mask = 1 << b;
                int i = 0;
                int j = 0;
                for (int off = 0; j < count; j++, off += 0x10) {
                    if (entries[j].flags_8 & mask) {
                        if (i == sel) {
                            *(Entry_004df590*)(DAT_00529e00 + b * 0x10) = entries[j];
                            if (DAT_00529dc8 && entries[j].name != 0) {
                                int n = 0;
                                int k = 0;
                                for (int koff = 0; k < count; k++, koff += 0x10) {
                                    Entry_004df590* e2 = (Entry_004df590*)((char*)entries + koff);
                                    if (e2->flags_8 & ~mask) {
                                        if (e2->field_0 == (int)entries[j].name) {
                                            *(Entry_004df590*)(DAT_00529e10 - b * 0x10) = *e2;
                                            SendDlgItemMessageA(hwnd,
                                                ((short)wParam == 0x3ed) ? 0x3ee : 0x3ed,
                                                0x14e, n, 0);
                                        }
                                        n++;
                                    }
                                }
                            }
                            sel = -1;
                        }
                        i++;
                    }
                }
                FUN_004e1b10(0);
                return 0;
            }
            if (id > 0 && id <= 2) {
                ((Class_004df280*)this)->FUN_004df280(0);
                return 0;
            }
        } else {
            switch (id) {
            case 0x3fa:
                FUN_004da5b0(hwnd,
                    "http://10.0.150.18/programming/library/extras/performancestatusdialog.",
                    ".htm");
                return 0;

            case 0x3ef:
                DAT_00529dd8 = (DAT_00529dd8 == 0);
                FUN_004e1b10(0);
                ((Class_004df4e0*)this)->FUN_004df4e0();
                return 0;

            case 0x3f1:
                DAT_00529dd4 = (DAT_00529dd4 == 0);
                FUN_004e1b10(0);
                return 0;

            case 0x3f6:
                DAT_00529ddc = (DAT_00529ddc == 0);
                FUN_004e1b10(0);
                return 0;

            case 0x3f7:
                DAT_00529e64 = (DAT_00529e64 == 0);
                FUN_004e1b10(0);
                return 0;

            case 0x3f8:
                DAT_00529dc8 = (DAT_00529dc8 == 0);
                FUN_004e1b10(0);
                return 0;

            case 0x3f4: {
                if ((wParam >> 16) != 1)
                    return 0;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x188, 0, 0);
                ((Class_004e18c0*)&set)->FUN_004e18c0();
                int j = 0;
                Node_004df590* node = set.head->left;
                while (Iterator_004df590(node) != Iterator_004df590(set.head)) {
                    if (j == sel) {
                        selected = *(Value_004df590*)((char*)node + 0xc);
                    }
                    j++;
                    {
                        std::_Lockit lock;
                        if (node->right != DAT_005292c4) {
                            std::_Lockit lock2;
                            node = node->right;
                            while (node->left != DAT_005292c4)
                                node = node->left;
                        } else {
                            Node_004df590* p;
                            while (node == (p = node->parent)->right)
                                node = p;
                            if (node->right != p)
                                node = p;
                        }
                    }
                }
                ((Class_004df380*)this)->FUN_004df380();
                return 0;
            }
            }
        }
        return 0;
    }

    case 0x113: {
        if (!IsWindowVisible(hwnd))
            return 0;
        RECT rect;
        GetWindowRect(hwnd, &rect);
        if (rect.left != left || rect.top != top) {
            left = rect.left;
            top = rect.top;
            FUN_004e3400(hwnd, DAT_0050d660);
        }
        Class_004e1ac0* cs = FUN_004e1ac0();
        EnterCriticalSection(&cs->cs);
        Class_004e17c0* info = FUN_004e1a90();
        if (info->names.changed) {
            int sel = -1;
            int n = 0;
            Node_004df590* node = info->names.head->left;
            SendDlgItemMessageA(hwnd, 0x3f4, 0x184, 0, 0);
            ((Class_004e18c0*)&set)->FUN_004e18c0();
            while (Iterator_004df590(node) != Iterator_004df590(info->names.head)) {
                ((Class_004e1990*)&set)->FUN_004e1990(&node->value);
                SendDlgItemMessageA(hwnd, 0x3f4, 0x180, 0, (LPARAM)node->value.name);
                if (NamesEqual_004df590(node->value.name, selected.name))
                    sel = n;
                n++;
                ((Class_004e0450*)&node)->FUN_004e0450();
            }
            if (sel >= 0)
                SendDlgItemMessageA(hwnd, 0x3f4, 0x186, sel, 0);
            info->names.changed = 0;
        }
        ((Class_004df380*)this)->FUN_004df380();
        LeaveCriticalSection(&cs->cs);
        return 0;
    }
    }
    return 0;
}
