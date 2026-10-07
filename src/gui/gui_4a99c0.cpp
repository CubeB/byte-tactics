// Decompiled by Space Bunny Free. Names are provisional.
// The list gadget's scroll-down step. First it does what 0x4a1810 does: picks
// the entry of type 7 whose group number matches entry `index` and makes that
// entry's id the current one (falling back to the current id of the list
// holder). Then it works out how far the visible window moves per line and,
// when the selected line has fallen below the window but is still inside the
// list, moves the selection one line down, scrolls the window if needed and
// refreshes the gadget. When the selection is already at the last line
// nothing happens. A selection sitting on a line whose text starts with
// "&G" is not moved either.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004a99c0 {                // 0x15b bytes
    unsigned char type;                // +0x00
    char unknown_01[0x19 - 0x01];
    short field_19;                    // +0x19
    char unknown_1b[0x28 - 0x1b];
    char group;                        // +0x28
    char unknown_29[0xb6 - 0x29];
    short count;                       // +0xb6 (only meaningful in entry 0)
    char unknown_b8[0xba - 0xb8];
    short field_ba;                    // +0xba
    short field_bc;                    // +0xbc
    short field_be;                    // +0xbe
    short field_c0;                    // +0xc0
    char* field_c2;                    // +0xc2
    char unknown_c6[0xd6 - 0xc6];
    int id;                            // +0xd6
    char unknown_da[0x15b - 0xda];
};
#pragma pack(pop)

struct List_004a99c0 {
    char unknown_0[0xc];
    unsigned short* field_0c;          // +0x0c
};

struct Holder_004a99c0 {
    int current;                       // +0x00
    Entry_004a99c0* entries;           // +0x04
    char unknown_08[0x14 - 0x08];
    List_004a99c0* list;               // +0x14
};

struct Dialog {
    char unknown_0[0x18];
    Holder_004a99c0* holder;           // +0x18
};

extern Holder_004a99c0* g_guiContext;
extern char DAT_00502a20[];

void __stdcall SetFont(int id);
int __stdcall GetGafFrame(unsigned short* param_1, int param_2);
int GetFontHeight();
char* __stdcall SkipTextLines(char* text, int line);
void __stdcall DrawListBox(Dialog* param_1, int param_2);
void __stdcall FUN_004a2be0(Dialog* param_1, int param_2);
void __stdcall FUN_004a2e40(Dialog* param_1, char* name, int line);

// FUNCTION: 0x4a99c0
void __stdcall FUN_004a99c0(Dialog* param_1, int index)
{
    Entry_004a99c0* entries = param_1->holder->entries;
    Entry_004a99c0* me = &entries[index];
    int n = 0;
    int i = 1;
    for (; i < entries->count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == me->group) {
                SetFont(entries[i].id);
                break;
            }
            n++;
        }
    }
    if (i == entries->count + 1) {
        SetFont(g_guiContext->current);
    }
    int size = (g_guiContext->list == 0) ? GetFontHeight()
        : (*(unsigned short*)(GetGafFrame(g_guiContext->list->field_0c, 0x49) + 2) + 2);
    size++;
    int step = (me->field_19 - 2) / size;
    short last = me->field_bc;           // last line of the window
    short sel = me->field_ba;            // selected line
    // Int copy of sel, kept live across the calls below: using sel directly changes the code.
    int isel = sel;
    if (isel < last + step && sel >= last) {
        if (me->field_c0 != 0) {
            if (isel == me->field_be + step - 1) {
                return;
            }
            short next = sel + 1;
            me->field_ba = next;
            if (next > last + step - 1) {
                me->field_bc = last + 1;
            }
            if (next >= me->field_c0 - 1) {
                me->field_ba = me->field_c0 - 1;
            }
            if (me->field_c2 != 0) {
                char* line = SkipTextLines(me->field_c2, me->field_ba);
                if (strncmp(DAT_00502a20, line, 2) == 0) {
                    me->field_ba = isel;
                }
            }
            DrawListBox(param_1, index);
            FUN_004a2be0(param_1, index);
            return;
        }
    }
    if (me->field_c0 != 0) {
        FUN_004a2e40(param_1, (char*)me + 2, isel);
    }
}
