// Decompiled by deepseek-v4.1, edited by deepseek-v4.1-flash. Names are provisional.
//
// PARTIAL: GUI layer loader (0x4aa8f0, 1762 bytes). Best 60.8%
// (ours 1781 bytes vs 1762). Win of the 0x3638 pass, 53.1 -> 60.8: the two
// `if (menu->layer != 0) menu->layer->field_14/18 = ...;` tests are written
// as tests of the existing `layer` local (identical at those points). That
// removes menu's whole-function EBX home: menu is no longer register-resident
// and the prologue now matches (`mov edi,[esp+0x230]` with menu in EDI, the
// cur pointer in ESI and `w` in EBX inside the flags & 0x800 block).
//
// First divergence still the ret/mask slot swap: three more declaration
// shapes (the swap `int mask; int ret = 1;`, the merge `int ret = 1, mask;`
// and the split `int ret; ... ret = 1;`) are all byte-identical at 60.8 /
// 1781 bytes, so no declaration spelling moves these two slots.
//
// First divergence now: `ret` occupies [S+0x18] where the original stores its
// 1 at [S+0x14] (mask takes the other int slot). Declaring `int mask;` before
// `int ret = 1;` and after it are both byte-identical at 60.8, so this slot
// order is not set by declaration order; the tail hunks that test mask vs a
// fresh `flags & 0x200` come from the same root. The old notes below still
// hold (mask-arm-first in the flags & 0x200 branch, menu reloads at five
// sites, flags never cached, the goto forms for the -1 loop results).
//
// Load-bearing find (38.3% -> 53.1%): the `flags & 0x200` sub-branch must be
// written with the mask != 0 arm FIRST, matching the original layout
// (0x4aaa2f `je 0x4aaa61` jumps over the mask arm to the alloc arm at
// 0x4aaa61). The earlier `if (mask == 0) {...} else {...}` spelling emitted
// the alloc arm first and a `jne` over it. Swapping the two arms also makes
// `layer` land in EDI for the FUN_004aeac0 block (`mov edi,[esi+0x18]` /
// `mov edi,[esp+0x18]`) as in the original.
//
// What still differs:
//  * `menu` is homed in EBX for the whole function here. The original homes
//    menu in EDI only for the flags & 0x800 block (0x4aa903..0x4aa989) and
//    then reloads it from its parameter home [S+0x230] at 0x4aa96e /
//    0x4aaa31 / 0x4aac38 / 0x4aae86 / 0x4aaf63, which frees EBX; ours keeps
//    it live everywhere, so `menu->layer` loads read `[ebx+0x18]` where the
//    original has `[edi+0x18]` or a fresh load from the home slot.
//  * `ret` still lands in [S+0x18] (original [S+0x14]), which flips the two
//    tests near 0x4aaea2 and the final `mov eax,[esp+0x10]` return.
//  * the tail here tests the `mask` local where the original re-derives
//    `flags & 0x200` (and `flags & 0x80`) from the reloaded flags at
//    0x4aac41..0x4aac68. Writing the tail as `(flags & 0x200) == 0` is
//    faithful and shrinks us to 1775 bytes, but check.py scores that 37.8%:
//    the extra flag reloads break more of the tail than the `mask` slot saves.
//  * `flags` is never cached in a register in the original: [S+0x238] is
//    re-loaded at 0x4aaa1d, 0x4aab29, 0x4aabdf, 0x4aac31.
//
// Tried and kept (0.9% total): `if (cur != 0) {...} else {...}` in the
// flags & 0x800 block, which matches the original's out-of-line
// FUN_004bf4d0(0,0,-0x18) at 0x4aa97e; `if (FUN_004aeac0(entry, layerName)
// != 0) {...} else {...}`, which matches 0x4aaaa5 `je 0x4aac22`; and goto
// forms that use one variable for both the loop counter and the found index
// in the PANEL and focusName searches, because the original materialises -1
// only on the loop fall-through (`or ecx,0xffffffff` at 0x4aab02, `or
// esi,0xffffffff` at 0x4aae7c). Without those the frame was 0x220 and 36.3%.
//
// Tried and rejected: reordering the locals (layer, ret, mask) changes
// nothing; a Menu layout whose padding array was sized from 0x64 instead of
// 0x68 put menu->name at +0x9ba, four bytes too high (fixed here).
//
// Structural facts recovered from the disassembly (kept because they are
// load-bearing for whoever tries next):
//  * the name setup is strncpy(layerName, menu->name, 0x100) followed by
//    strcat(layerName, name) (the inline rep movs at 0x4aa9d0 writes at the
//    NUL of layerName, `dec edi` at 0x4aa9cc), then
//    strncpy(guiName, name, 0x100).
//  * all the name probes are real strlen calls (repne scasb / not ecx /
//    dec ecx / jne), not `p[0] == 0`.
//  * the entry loops reload base->count / entry->count from memory on every
//    iteration (never a hoisted limit) and walk a moving pointer that is
//    bumped by 0x15b, while entry[i] indexing emits the shl/sub/lea chain.
//  * 0x4aab29 writes flags |= 0x20 back to the parameter home.
//
// Suspected original bug: when FUN_004bbc40(layerName) returns 0 (GUI file
// missing) the code jumps to 0x4aac2d, which loads `layer` from [S+0x10]
// before it was ever stored (the only store is the mask-path one at
// 0x4aaa3b) and then writes layer->entries/field_1c/field_24/field_3b
// through it and returns the garbage pointer.
#include <string.h>

#pragma pack(push, 1)

struct Layer_004aa8f0;

// 0x15b-byte GUI control record.
struct Entry_004aa8f0 {
    unsigned char type;            // +0x00
    char unknown_1;
    char name[0x10];               // +0x02
    char unknown_12;
    short x;                       // +0x13
    short y;                       // +0x15
    short w;                       // +0x17
    short h;                       // +0x19
    char unknown_1b[4];
    int field_1f;                  // +0x1f
    char unknown_23[5];
    char field_28;                 // +0x28
    unsigned char field_29;        // +0x29
    char unknown_2a[0x8c];
    short count;                   // +0xb6
    char unknown_b8[4];
    int handle;                    // +0xbc
    char unknown_c0[0x0c];
    char okName[0x10];             // +0xcc
    char prevName[0x10];           // +0xdc
    char focusName[0x10];          // +0xec
    char unknown_fc[0x5f];
};

struct Layer_004aa8f0 {
    Layer_004aa8f0* next;          // +0x00
    Entry_004aa8f0* entries;       // +0x04
    int field_08;
    int field_0c;
    int flags;                     // +0x10
    int field_14;                  // +0x14
    int field_18;                  // +0x18
    int field_1c;                  // +0x1c
    int field_20;                  // +0x20
    int field_24;                  // +0x24
    char unknown_28[0x13];
    int field_3b;                  // +0x3b
};

struct Menu_004aa8f0 {
    char unknown_0[0x18];
    Layer_004aa8f0* layer;         // +0x18
    char unknown_1c[0x48];
    int field_60;                  // +0x64
    char unknown_68[0x8b2 - 0x68];
    unsigned char field_8b2[0x104];// +0x8b2
    char name[0x100];              // +0x9b6
};

#pragma pack(pop)

extern void __stdcall FUN_004bf4d0(int handle, int* rect, int mode);
extern char* __stdcall FUN_004bb150(char* path);
extern char* __stdcall FUN_004baff0(char* out, char* in, char* ext);
extern int __stdcall FUN_004bbc40(char* path);
extern void* __cdecl FUN_004d83b0(const char* path, unsigned int size);
extern int __stdcall FUN_004aeac0(void* entry, char* path);
extern void __cdecl FUN_004d85a0(void* p);
extern int __stdcall FUN_004a81e0(Menu_004aa8f0* menu, unsigned int flags);
extern void __cdecl FUN_004c2470(void);
extern void __cdecl FUN_004c2870(void);
extern void __stdcall FUN_004a7960(Menu_004aa8f0* menu, int value);
extern void __stdcall FUN_0049fc50(Menu_004aa8f0* menu, int value);
extern int __cdecl FUN_004c13f0(void);
extern void __stdcall FUN_004c13a0(unsigned char a, int b);
extern void __stdcall FUN_004c1420(int a);
extern void __cdecl FUN_004c1a40(void);
extern void __stdcall FUN_004ab6c0(Menu_004aa8f0* menu, int a, char* text,
                                   int maxLength, int clear);
extern int* DAT_0051fba4;

// FUNCTION: 0x4aa8f0
Layer_004aa8f0* __stdcall FUN_004aa8f0(Menu_004aa8f0* menu, const char* name,
                                       unsigned int flags)
{
    Layer_004aa8f0* layer;
    int ret = 1;
    int mask;
    int rect[4];
    char layerName[0x100];
    char guiName[0x100];
    Entry_004aa8f0* entry = 0;

    if (flags & 0x800) {
        Layer_004aa8f0* cur = menu->layer;
        if (cur != 0) {
            Entry_004aa8f0* e = cur->entries;
            int x;
            int y;
            if (e->type == 0) {
                x = 0;
                y = 0;
            } else {
                x = e->x;
                y = e->y;
            }
            rect[0] = x;
            rect[1] = y;
            rect[2] = x + e->w - 1;
            rect[3] = y + e->h - 1;
            FUN_004bf4d0(cur->entries->handle, rect, -0x18);
            if (menu->layer != 0)
                menu->layer->field_14 = 1;
        } else {
            FUN_004bf4d0(0, 0, -0x18);
        }
    }
    strncpy(layerName, menu->name, 0x100);
    strcat(layerName, name);
    strncpy(guiName, name, 0x100);
    FUN_004bb150(guiName);
    FUN_004baff0(layerName, layerName, "GUI");
    if (FUN_004bbc40(layerName) != 0) {
        mask = flags & 0x200;
        if (mask != 0) {
            layer = menu->layer;
            entry = &layer->entries[layer->entries->count + 1];
        } else {
            layer = (Layer_004aa8f0*)FUN_004d83b0(guiName, 0x10f57);
            memset(layer, 0, 0x10f57);
            entry = (Entry_004aa8f0*)((char*)layer + 0x3f);
        }
        if (FUN_004aeac0(entry, layerName) != 0) {
          if (mask != 0) {
            Entry_004aa8f0* base = layer->entries;
            int idx;
            int i = 1;
            if (i <= base->count) {
                Entry_004aa8f0* e = &base[1];
                do {
                    if (strncmp(e->name, "PANEL", 0x10) == 0) {
                        idx = i;
                        goto panelFound;
                    }
                    i++;
                    e++;
                } while (i <= base->count);
            }
            idx = -1;
        panelFound:
            if (idx == -1) {
                int j = 1;
                if (j <= entry->count) {
                    Entry_004aa8f0* e = &entry[1];
                    do {
                        e->x += entry->x;
                        e->y += entry->y;
                        j++;
                        e++;
                    } while (j <= entry->count);
                }
            } else {
                base[idx].field_29 = 0;
                flags |= 0x20;
                int dx = (base[idx].w - entry->w) / 2 + base[idx].x;
                int dy = (base[idx].h - entry->h) / 2 + base[idx].y;
                int j = 1;
                if (j <= entry->count) {
                    Entry_004aa8f0* e = &entry[1];
                    do {
                        e->x += dx;
                        e->y += dy;
                        j++;
                        e++;
                    } while (j <= entry->count);
                }
            }
            layer->entries->count += entry->count;
            memcpy(entry, &entry[1], entry->count * 0x15b);
            entry = layer->entries;
          }
        } else {
            FUN_004d85a0(layer);
        }
    }
    layer->entries = entry;
    layer->field_1c = 0;
    layer->field_24 = 0;
    layer->field_3b = 0;
    if (mask == 0) {
        layer->next = menu->layer;
        menu->layer = layer;
    }
    layer->flags = 0;
    if (mask == 0 && (flags & 0x80) != 0)
        layer->flags = 0x80;
    layer->flags |= flags & 0x800;
    if (layer != 0)
        layer->field_14 = 1;
    if (layer != 0)
        layer->field_18 = 0;
    strncpy((char*)entry + 2, guiName, 0x10);
    menu->field_60 = -1;
    if ((flags & 0x400) == 0) {
        FUN_004c2470();
        ret = FUN_004a81e0(menu, flags | 1);
        FUN_004c2870();
    }
    {
        char* dst = entry->okName;
        if (strlen(dst) == 0) {
            int i = 1;
            if (i <= entry->count) {
                Entry_004aa8f0* e = &entry[1];
                do {
                    if (e->type == 1 && (_strnicmp(e->name, "OK", 2) == 0
                            || _strnicmp(e->name, "NEXT", 4) == 0)) {
                        strcpy(dst, entry[i].name);
                        break;
                    }
                    i++;
                    e++;
                } while (i <= entry->count);
            }
        }
        dst = entry->prevName;
        if (strlen(dst) == 0) {
            int i = 1;
            if (i <= entry->count) {
                Entry_004aa8f0* e = &entry[1];
                do {
                    if (e->type == 1 && (_strnicmp(e->name, "PREV", 4) == 0
                            || _strnicmp(e->name, "Cancel", 6) == 0)) {
                        strcpy(dst, entry[i].name);
                        break;
                    }
                    i++;
                    e++;
                } while (i <= entry->count);
            }
        }
        dst = entry->focusName;
        if (strlen(dst) == 0) {
            layer->field_20 = 0;
            FUN_004a7960(menu, 1);
        } else {
            int i = 1;
            if (i <= entry->count) {
                Entry_004aa8f0* e = &entry[1];
                do {
                    if (strncmp(e->name, dst, 0x10) == 0)
                        goto focusFound;
                    i++;
                    e++;
                } while (i <= entry->count);
            }
            i = -1;
        focusFound:
            layer->field_20 = i;
        }
    }
    menu->field_60 = -1;
    if (ret != 1) {
        FUN_004d85a0(layer);
        return 0;
    }
    if (entry->count == 1 && ((char*)entry)[0x15b] == 3) {
        Entry_004aa8f0* base = menu->layer->entries;
        Entry_004aa8f0* sub = &base[1];
        int r = FUN_004c13f0();
        FUN_004c13a0(menu->field_8b2[sub->field_1f], r);
        int i = 0;
        int j = 1;
        if (j < base->count + 1) {
            Entry_004aa8f0* e = sub;
            do {
                if (e->type == 7) {
                    if (i == base->field_28) {
                        FUN_004c1420(*(int*)((char*)&base[j] + 0xd6));
                        break;
                    }
                    i++;
                }
                j++;
                e++;
            } while (j < base->count + 1);
        }
        if (j == base->count + 1)
            FUN_004c1420(*DAT_0051fba4);
        FUN_0049fc50(menu, 1);
        menu->layer->field_20 = 1;
        FUN_004ab6c0(menu, 1, (char*)sub + 0xb6,
                     *(short*)((char*)sub + 0x138), 0);
        FUN_004c1a40();
    }
    return layer;
}
