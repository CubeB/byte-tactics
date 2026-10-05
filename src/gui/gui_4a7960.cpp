// Decompiled by deepseek-v4.1, finished by deepseek-v4.1-flash, edited by deepseek-v4.1-flash, finished by claude-opus-5-5, finished by Space Bunny Free, finished by Fable 5.1, finished by DeepSeek V4.1 Flash. Names are provisional.
// DeepSeek V4.1 Flash (#4855): 93.5% -> MATCH, 1404 bytes. Only the loop-2
// latch was left. The fix is one declaration: give loop 2 a source pointer
// `int* up = &used[i];` BEFORE `char* b = (char*)&entries[i].x1;` and read
// `*up` for the case 2/3 position. The extra source reference to `used[i]`
// (before the entry pointer's) changes the IV registration order, so MSVC
// updates the `up` IV before the entry IV at the latch (`mov eax,[esp+0x14];
// inc edi; add eax,4; add ecx,0x15b; mov [esp+0x14],eax; mov eax,[esp+0x24]`)
// instead of loading cnt into eax first and updating up in esi. Putting the
// `up` declaration after `b` (or spelling `used[i]` at the use) leaves the
// esi latch. Both pointers still strength-reduce to IVs with their setup after
// the loop guard, so the setup bytes are unchanged.
// Fable 5.1 (#4452): 84.6% -> 93.5%, 1404 bytes. Three structural changes:
//   (1) The tail is the real FUN_004a7190 (matched in 0x4a7190.cpp) inlined
//       twice, called as FUN_004a7190(menu, menu->layer->field_20) behind
//       `entries[menu->layer->field_20].type == 3`. Its own body reads
//       obj->layer->entries, which is the CSE with the test's menu->layer load
//       that the original shows (`mov ebp, [ecx+4]`). No `sel` local.
//   (2) `cnt` is not a local: `entries->data.count + 1` is written at each use
//       and MSVC's CSE temp is what sits in [esp+0x24]. A named `cnt` takes
//       esi away from `layer` (the 75% plateau of every earlier pass).
//   (3) Both loops index by `i`. Loop 1 as `for (i = 1; i < count + 1; i++)`
//       over used[i]/entries[i].x0 is what MSVC turns into the original's
//       countdown (`dec eax` into [esp+0x14]); loop 2's per-iteration
//       `int* up = &used[i]` (declared before `b`) and `char* b =
//       (char*)&entries[i].x1` become compiler induction variables, which is
//       why their setup sits AFTER the loop guard (`jle` then
//       `lea eax,[esp+0x2c]`), where a source pointer initialised before the
//       loop never goes.
//   The annotated SelectGadgetByIndex above the function is the real preceding
//   function (it also MATCHes here); without it the switch tails are not
//   shared (1440 bytes).
// IV bias rule measured on the way (build/scratch/0x4a7960/iv/): with
//   `entries[i].f` accesses MSVC 5 biases the walking pointer to the second
//   field with a non-zero offset in source order; a field accessed twice or
//   more (`type`) takes the bias instead (offset 0). Neither gives +0x17 for
//   this loop, hence the explicit x1 pointer.
#pragma pack(push, 1)

struct Entry_004a7960 {                // 0x15b bytes
    unsigned char type;                // +0x000
    char unknown_01[0x13 - 0x01];
    short x0;                          // +0x013
    short y0;                          // +0x015
    short x1;                          // +0x017
    short y1;                          // +0x019
    int field_1b;                      // +0x01b
    int colourIndex;                   // +0x01f
    char unknown_23[0x28 - 0x23];
    char group;                        // +0x028
    char field_29;                     // +0x029
    char unknown_2a[0xb6 - 0x2a];
    union {
        short count;                   // +0x0b6 (entry 0 only)
        char text[0x82];               // +0x0b6
        struct {
            char pad[0x20];
            int id;                    // +0x0d6
        } list;
    } data;
    short maxLength;                   // +0x138
    char unknown_13a[0x13c - 0x13a];
    unsigned char field_13c;           // +0x13c
    char unknown_13d[0x157 - 0x13d];
    int field_157;                     // +0x157
};

struct Layer_004a7960 {
    int unknown_00;
    Entry_004a7960* entries;           // +0x04
    char unknown_08[0x20 - 0x08];
    int field_20;                      // +0x20
};

struct Menu_004a7960 {
    char unknown_00[0x18];
    Layer_004a7960* layer;             // +0x18
    char unknown_1c[0x64 - 0x1c];
    int focus;                         // +0x64
    char unknown_68[0x8b2 - 0x68];
    unsigned char colors[16];          // +0x8b2
};
#pragma pack(pop)

struct Dialog {
    int group;                         // +0x00
};
extern Dialog* g_guiContext;

int GetTextKeyColor();
void __stdcall SetTextColors(int colour, int font);
void __stdcall SetFont(int id);
void ClearKeyQueue();
int __stdcall FUN_0049fc50(Menu_004a7960* menu, int index);
void __stdcall FUN_004ab6c0(Menu_004a7960* menu, int index, char* text,
                            int maxLength, int clear);

// The real FUN_004a7190 (matched in 0x4a7190.cpp), inlined here by /Ob2.
static inline void FUN_004a7190(Menu_004a7960* obj, int index)
{
    Entry_004a7960* entries = obj->layer->entries;
    Entry_004a7960* target = &entries[index];

    SetTextColors(obj->colors[target->colourIndex], GetTextKeyColor());

    int n = 0;
    int i = 1;
    for (; i < entries->data.count + 1; i++) {
        if (entries[i].type == 7) {
            if (n == target->group) {
                SetFont(entries[i].data.list.id);
                break;
            }
            n++;
        }
    }
    if (i == entries->data.count + 1) {
        SetFont(g_guiContext->group);
    }

    FUN_0049fc50(obj, index);
    obj->layer->field_20 = index;
    FUN_004ab6c0(obj, index, target->data.text, target->maxLength, 0);
    ClearKeyQueue();
}

// The function before this one in the original file (matched on its own in
// 0x4a7830.cpp): the compiler state it leaves behind is what lets the switch
// below share its case tails. Without it the tails are duplicated (1440 bytes).
void __stdcall SelectGadgetByIndex(Menu_004a7960* menu, int index)
{
    Entry_004a7960* first = menu->layer->entries;
    menu->focus = -1;
    menu->layer->field_20 = index;
    if (first[menu->layer->field_20].type == 3)
        FUN_004a7190(menu, menu->layer->field_20);
}

// FUNCTION: 0x4a7960
void __stdcall FUN_004a7960(Menu_004a7960* menu, int dir)
{
    int used[200];

    Layer_004a7960* layer = menu->layer;
    int index = layer->field_20;
    Entry_004a7960* entries = layer->entries;
    if (index == -1)
        return;

    for (int k = 0; k < 50; k++)
        used[k] = 0;

    for (int i = 1; i < entries->data.count + 1; i++) {
        int idx = -1;
        for (int j = 1; used[j] != 0; j++) {
            int d = used[j] - entries[i].x0;
            if (d < 10 && d > -10) {
                idx = j;
                break;
            }
        }
        if (idx != -1)
            used[i] = used[idx];
        else
            used[i] = entries[i].x0;
    }

    int start;
    int bound;
    switch (dir) {
    case 0:
        start = entries[index].x0 + entries[index].y0 * 5000;
        bound = start - 0x17d7840;
        break;
    case 2:
        start = entries[index].y0 + used[index] * 5000;
        bound = start - 0x17d7840;
        break;
    case 1:
        start = entries[index].x0 + entries[index].y0 * 5000;
        bound = start + 0x17d7840;
        break;
    case 3:
        start = entries[index].y0 + used[index] * 5000;
        bound = start + 0x17d7840;
        break;
    }

    {
    int pos;
    for (int i = 1; i < entries->data.count + 1; i++) {
        int* up = &used[i];
        char* b = (char*)&entries[i].x1;
        if (*(signed char*)(b + 0x12) != 0 && !(*(int*)(b + 4) & 0x400)
            && !(*(unsigned char*)(b - 0x17) == 1 && (*(unsigned char*)(b + 0x125) & 1))
            && !(*(unsigned char*)(b - 0x17) == 4 && *(int*)(b + 0x140) != 0)) {
            if (*(unsigned char*)(b - 0x17) == 3 || *(unsigned char*)(b - 0x17) == 4
                || *(unsigned char*)(b - 0x17) == 1
                || *(unsigned char*)(b - 0x17) == 6
                || *(unsigned char*)(b - 0x17) == 2) {
                if (!(*(unsigned char*)(b - 0x17) == 4
                      && *(short*)b < *(short*)(b + 2))) {
                    if (!(*(unsigned char*)(b - 0x17) == 2 && (*(int*)(b + 4) & 0x100))
                        && !(*(unsigned char*)(b - 0x17) == 1
                             && (*(unsigned char*)(b + 0x125) & 1))) {
                        switch (dir) {
                        case 0:
                        case 1:
                            pos = *(short*)(b - 4) + *(short*)(b - 2) * 5000;
                            break;
                        case 2:
                        case 3:
                            pos = *(short*)(b - 2) + *up * 5000;
                            break;
                        }
                        switch (dir) {
                        case 1:
                        case 3:
                            if (pos <= start)
                                pos += 0x17d7840;
                            if (pos < bound) {
                                bound = pos;
                                index = i;
                            }
                            break;
                        case 0:
                        case 2:
                            if (pos >= start)
                                pos -= 0x17d7840;
                            if (pos > bound) {
                                bound = pos;
                                index = i;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    }

    menu->focus = -1;
    layer->field_20 = index;
    if (entries[menu->layer->field_20].type == 3)
        FUN_004a7190(menu, menu->layer->field_20);
    if (entries[menu->layer->field_20].type == 3)
        FUN_004a7190(menu, menu->layer->field_20);
}