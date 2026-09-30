// Decompiled by space-bunny-free. Names are provisional.
// Builds the MSGBOX.GUI dialog: word-wraps the (translated) message text, adds
// one TEXT entry per line, sizes the dialog from the longest line, centres it
// on the screen, stretches every text entry to the dialog width, positions the
// OK button and installs FUN_004abd00 as the handler.
//
// Suspected original bug: the max-line-width loop measures the entries at
// entries[count+1] .. entries[count+lines] (0x4abeb8 walks forward from
// count+1, one 0x15b step per line), while the text entries just added live at
// entries[count-lines+1] .. entries[count], so it measures one entry past the
// last line it added.
// Note: the two "OK" strings go to the table base's fields at +0xcc and +0xdc
// (0x4abfba, 0x4abfd8 address them from the table pointer, not from the entry
// that FUN_0049fdf0 returned, unlike the x/y fields just above them). If those
// are entry 0's own button-name fields that is deliberate, otherwise the OK
// entry is left unnamed.
#include <string.h>

#pragma pack(push, 1)
struct Entry_004abd90 {                  // 0x15b bytes
    unsigned char type;                  // +0x00
    unsigned char unknown_01[0x13 - 0x01];
    short x;                             // +0x13
    short y;                             // +0x15
    short w;                             // +0x17
    short h;                             // +0x19
    int flags;                           // +0x1b
    char unknown_1f[0xb6 - 0x1f];
    union {
        short count;                     // +0xb6 (entry 0 holds the entry count)
        char text[0x15b - 0xb6];         // +0xb6 (a text entry's string)
        struct {                         // entry 0's two gadget names
            char unknown_b6[0xcc - 0xb6];
            char name_cc[0x10];          // +0xcc
            char name_dc[0x10];          // +0xdc
            char unknown_ec[0x15b - 0xec];
        } head;
    } u;
};
#pragma pack(pop)

struct Layer_004abd90 {
    int unknown_0;
    Entry_004abd90* entries;             // +0x4
    void (__stdcall* handler)(void*);    // +0x8
};

struct Holder_004abd90 {
    int unknown_0;
    Entry_004abd90* entries;             // +0x4
};

struct Menu_004abd90 {
    char unknown_0[0x18];
    Holder_004abd90* holder;             // +0x18
};

Layer_004abd90* __stdcall FUN_004aa8f0(Menu_004abd90* menu, const char* name, int flags);
char* __stdcall FUN_004c5740(char* text);
char* __stdcall FUN_004ac4c0(Menu_004abd90* menu, char* text, int width, int index);
void __stdcall FUN_004a81e0(Menu_004abd90* menu, int flag);
void __stdcall FUN_004ab1b0(Layer_004abd90* layer, char* type, char* text, int x, short y,
                            int width, int attr);
int __stdcall FUN_004c1450();
int __stdcall FUN_004a5030(char* text);
int FUN_004b6700();
int FUN_004b6710();
int __stdcall FUN_0049fdf0(Entry_004abd90* entries, const char* name, int type);
void __stdcall FUN_004a0570(Menu_004abd90* menu, const char* name, int flag);
void __stdcall FUN_0049fb10(Menu_004abd90* menu, int flag);
void __stdcall FUN_0049fa90(Menu_004abd90* menu);
void __stdcall FUN_004a9fd0(Menu_004abd90* menu);
void __cdecl FUN_004d85a0(char* text);
void __stdcall FUN_004abd00(void* gadget);

// FUNCTION: 0x4abd90
int __stdcall FUN_004abd90(Menu_004abd90* gui, char* text, int wrapWidth, int centre, int autoHeight)
{
    Layer_004abd90* layer = FUN_004aa8f0(gui, "MSGBOX.GUI", 0x800);
    if (layer) {
        char name[0x100];
        char buf[0x100];
        strcpy(name, FUN_004c5740(text));
        char* wrapped = FUN_004ac4c0(gui, name, wrapWidth, -1);
        FUN_004a81e0(gui, 2);
        strncpy(buf, wrapped, 0xfe);
        Entry_004abd90* entries = gui->holder->entries;
        char* line = strtok(buf, "\n");
        int lines = 0;
        int y = 0x14;
        int next = layer->entries->u.count + 1;
        if (line) {
            do {
                FUN_004ab1b0(layer, "TEXT", line, 0, y, -1, 2);
                y += FUN_004c1450() + 5;
                line = strtok(0, "\n");
                lines++;
            } while (line);
        }
        int width;
        if (autoHeight) {
            width = 0;
            if (lines > 0) {
                char* p = entries[next].u.text;
                int i = lines;
                do {
                    if (width <= FUN_004a5030(p))
                        width = FUN_004a5030(p);
                    p += 0x15b;
                } while (--i);
            }
            width += 0x14;
        } else {
            width = wrapWidth;
        }
        entries->w = width;
        entries->h = lines * 25 + entries[1].h + 0x28;
        entries->x = (FUN_004b6700() - entries->w) / 2;
        entries->y = (FUN_004b6710() - entries->h) / 2;
        int i;
        for (i = 0; i <= entries->u.count; i++) {
            if (entries[i].type == 5) {
                entries[i].w = entries->w;
                entries[i].flags = 2;
            }
        }
        if (centre) {
            int k = FUN_0049fdf0(entries, "OK", 0xe);
            if (k != -1) {
                entries[k].y = entries->h - entries[k].h - 0xf;
                entries[k].x = entries->w - entries[k].w - 0xf;
                strcpy(entries->u.head.name_cc, "OK");
                strcpy(entries->u.head.name_dc, "OK");
            }
        } else {
            FUN_004a0570(gui, "OK", 0);
        }
        FUN_0049fb10(gui, 1);
        FUN_004a81e0(gui, 1);
        layer->handler = FUN_004abd00;
        FUN_0049fa90(gui);
        FUN_004a9fd0(gui);
        FUN_004d85a0(wrapped);
        return 1;
    }
    return 0;
}
