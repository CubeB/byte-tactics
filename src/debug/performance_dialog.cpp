// Decompiled by Sonnet, deepseek-v4.1-flash, GPT-6, space-bunny-free, deepseek-v4.1 and Claude Opus 5.5. Names are provisional.

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

class NameTable {
public:
    Map_004df590 names;                // +0x0
};

NameTable* GetNameTable();

class CriticalSection {
public:
    CRITICAL_SECTION cs;
};

CriticalSection* FUN_004e1ac0();

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
    void SetPerformanceWindowVisible(char show);
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
extern Entry_004df590 DAT_00529e00[];
extern char* DAT_0050d660;

void __cdecl SyncPerformanceSettings(int flag);
void __cdecl SaveWindowPosition(HWND hwnd, char* name);
void __cdecl OpenUrl(HWND hwnd, const char* url, const char* ext);

class PerformanceDialog {
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

    BOOL HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    void CreatePerformanceDialog(void);
};

static inline Node_004df590* Min_004df590(Node_004df590* p)
{
    std::_Lockit lock;
    while (p->left != DAT_005292c4)
        p = p->left;
    return p;
}

static inline bool NamesEqual_004df590(const char* a, const char* b) { return a == b || strcmp(a,b) == 0; }

// Dialog procedure of the performance status dialog: WM_COMMAND (two
// combo boxes picking entries from a list, five check boxes, a list box of
// names, a help link), WM_TIMER (refills the name list box from the global
// name table when it changed), WM_INITDIALOG and WM_HOTKEY.
// FUNCTION: 0x4df590
BOOL PerformanceDialog::HandlePerformanceMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    // Case order WM_COMMAND, WM_TIMER, WM_INITDIALOG: decides register ids and frame slots.
    switch (msg) {
    case 0x111: {
        int id = LOWORD(wParam);
        switch (id) {
        case IDOK:
        case IDCANCEL:
            ((Class_004df280*)this)->SetPerformanceWindowVisible(0);
            return 0;

        case 0x3ed:
        case 0x3ee: {
                // HIWORD(wParam) for the notification tests: keeps the shr.
                if (HIWORD(wParam) != CBN_SELCHANGE)
                    return 0;
                int b = 0;
                if (LOWORD(wParam) == 0x3ee) b = 1;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x147, 0, 0);
                int mask = 1 << b;
                int i = 0;
                for (int j = 0; j < count; j++) {
                    if (entries[j].flags_8 & mask) {
                        if (i == sel) {
                            DAT_00529e00[b] = entries[j];
                            if (DAT_00529dc8 && entries[j].name != 0) {
                                int n = 0;
                                int nmask = ~mask;
                                for (int k = 0; k < count; k++) {
                                    if (entries[k].flags_8 & nmask) {
                                        if (entries[k].field_0 == (int)entries[j].name) {
                                            DAT_00529e00[1 - b] = entries[k];
                                            SendDlgItemMessageA(hwnd,
                                                (LOWORD(wParam) == 0x3ed) ? 0x3ee : 0x3ed,
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
                SyncPerformanceSettings(0);
                return 0;
            }

            case 0x3fa:
                OpenUrl(hwnd,
                    "http://10.0.150.18/programming/library/extras/performancestatusdialog.html",
                    ".htm");
                return 0;

            case 0x3ef:
                DAT_00529dd8 = (DAT_00529dd8 == 0);
                SyncPerformanceSettings(0);
                ((Class_004df4e0*)this)->FUN_004df4e0();
                return 0;

            case 0x3f1:
                DAT_00529dd4 = (DAT_00529dd4 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f6:
                DAT_00529ddc = (DAT_00529ddc == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f7:
                DAT_00529e64 = (DAT_00529e64 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f8:
                DAT_00529dc8 = (DAT_00529dc8 == 0);
                SyncPerformanceSettings(0);
                return 0;

            case 0x3f4: {
                if (HIWORD(wParam) != LBN_SELCHANGE)
                    return 0;
                int sel = (int)SendDlgItemMessageA(hwnd, id, 0x188, 0, 0);
                int j = 0;
                Node_004df590* node = set.head->left;
                while (Iterator_004df590(node) != Iterator_004df590(set.head)) {
                    if (j == sel) {
                        selected = *(Value_004df590*)((char*)node + 0xc);
                    }
                    j++;
                    {
                        // Min_004df590 inlined with node->right as argument: loads it before the lock.
                        std::_Lockit lock;
                        if (node->right != DAT_005292c4) {
                            node = Min_004df590(node->right);
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
            SaveWindowPosition(hwnd, DAT_0050d660);
        }
        CriticalSection* cs = FUN_004e1ac0();
        EnterCriticalSection(&cs->cs);
        NameTable* info = GetNameTable();
        if (info->names.changed) {
            int sel = -1;
            int n = 0;
            Node_004df590* node = info->names.head->left;
            SendDlgItemMessageA(hwnd, 0x3f4, 0x184, 0, 0);
            ((Class_004e18c0*)&set)->FUN_004e18c0();
            // Guarded do-while: a while or for loop moves the loop registers.
            if (Iterator_004df590(node) != Iterator_004df590(info->names.head)) {
                do {
                    ((Class_004e1990*)&set)->FUN_004e1990(&node->value);
                    SendDlgItemMessageA(hwnd, 0x3f4, 0x180, 0, (LPARAM)node->value.name);
                    if (NamesEqual_004df590(node->value.name, selected.name))
                        sel = n;
                    n++;
                    ((Class_004e0450*)&node)->FUN_004e0450();
                } while (Iterator_004df590(node) != Iterator_004df590(info->names.head));
            }
            if (sel >= 0)
                SendDlgItemMessageA(hwnd, 0x3f4, 0x186, sel, 0);
            info->names.changed = 0;
        }
        ((Class_004df380*)this)->FUN_004df380();
        LeaveCriticalSection(&cs->cs);
        return 0;
    }

    case 0x110: {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        left = rect.left;
        top = rect.top;
        for (int i = 0; i < 2; i++) {
            int mask = 1 << i;
            int id = 0x3ed;
            // Branch, not arithmetic: stops strength reduction of DAT_00529e00[i].
            if (i == 1)
                id = 0x3ee;
            int sel = 0;
            int n = 0;
            for (int j = 0; j < count; j++) {
                Entry_004df590* e = &entries[j];
                if (entries[j].flags_8 & mask) {
                    if (e->field_0 == DAT_00529e00[i].field_0)
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
            ((Class_004df280*)this)->SetPerformanceWindowVisible(1);
        return 1;
    }

    case 0x312:
        if (wParam == 10) {
            ((Class_004df280*)this)->SetPerformanceWindowVisible(IsWindowVisible(hwnd) == 0);
        }
        return 0;

    }
    return 0;
}

// Declared after the dialog procedure: before it, these symbols change its
// frame layout (the procedure follows the symbol ids, as its notes say).
HWND __cdecl CreateDialogFromTemplate(int id, HWND parent, DLGPROC proc, LPARAM param);
BOOL CALLBACK PerformanceDlgProc(HWND, UINT, WPARAM, LPARAM);

// FUNCTION: 0x4df250
void PerformanceDialog::CreatePerformanceDialog(void)
{
    if (CreateDialogFromTemplate(0x67, GetDesktopWindow(), (DLGPROC)PerformanceDlgProc, (LPARAM)this) == 0) {
        MessageBoxA(0, "Performance dialog failed to open", "Cavedog", 0);
    }
}
