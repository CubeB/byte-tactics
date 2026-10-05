// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash, finished by GPT-6.1-sol, finished by Claude Opus 5.5. Names are provisional.
// Per-frame driver of a unit's command list (+0x5c): every node that is due
// (or has a pending bit) is handed to its kind's callback in DAT_00512344,
// and the answer decides what happens to it. The list head is re-read after
// every node, so a node the callback re-queues is seen again in the same
// pass. 0x43bad0 is the twin for the +0x60 list.
//
// The list helpers are the real neighbouring functions of this file,
// defined here (without FUNCTION lines, they match in their own files) so
// that /Ob2 makes the original's choices: FUN_00439f80 (unlink and delete)
// inlines in cases 5/8 and 9, FUN_00439fe0 (move to the end, 0 callers)
// inlines in case 6, FUN_00439eb0 (clear the lists) inlines in case 7 with
// its own FUN_00439f80 call left out of line, but stays an out-of-line call
// in the default case, and FUN_0043b730 (0 callers) inlines at the end with
// its FUN_0043ac60 call out of line. The Wait and ClearTargets helpers spend
// the budget that leaves the default case's FUN_00439eb0 out of line; with
// the target loop written out in place it is inlined there too. The
// countdown register of the target loop (ebx = 3) needs the char counter.
//
// `for (;;) { node = unit->list; if (node == 0) break; ... }` is the loop
// shape: it is what puts the early returns on the default case's epilogue
// (near jumps) and keeps node->next in edx across case 9's inlined unlink.
// The two Wait copies (cases 3 and 9) are merged by MSVC after allocation,
// so they only become one `push 0xf; jmp` when the temporaries' rotation
// agrees at both. That rotation, and the pending block's mask in cx and
// flags6 in eax, need one header before the declarations: without one the
// mask goes to eax (71.2%). headers.py finds 252 of 256 header sets that
// match, <windows.h>, <string.h> or <stdlib.h> alone among them.
#include <windows.h>

#pragma pack(push, 1)

struct Unit;

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;            // +0x4, index into DAT_00512344
    unsigned char count;           // +0x5
    unsigned int flags6;           // +0x6, bit 0 set while the node waits
    unsigned int wakeFrame;        // +0xa
    Unit* unit;                    // +0xe, handed to the callback
    char unknown_12[0x42 - 0x12];
    unsigned int flags;            // +0x42, bit 0x40000 picks the second list
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;          // +0x4a
    unsigned int field_4e;         // +0x4e
    char unknown_52[0x56 - 0x52];

    Class_0043a1f0(unsigned char k, void* o, void* p, int a, int b, int c);
    ~Class_0043a1f0();
};

struct Player_0043b7c0 {
    int active;                    // +0x0
    char unknown_4[0x73 - 0x4];
    char state;                    // +0x73
};

struct UnitDef_0043b7c0 {
    char unknown_0[0x230];
    unsigned char field_230;       // +0x230, the kind of a fresh command
};

struct Unit {
    char unknown_0[0x5c];
    Class_0043a1f0* list;          // +0x5c
    Class_0043a1f0* list2;         // +0x60, nodes with flag 0x40000
    char unknown_64[0x92 - 0x64];
    UnitDef_0043b7c0* def;         // +0x92
    Player_0043b7c0* player;       // +0x96
    char unknown_9a[0xba - 0x9a];
    unsigned short field_ba;       // +0xba, the unit's pending mask
};

struct Game {
    char unknown_0[0x38a47];
    unsigned int frame;            // +0x38a47
};

// One entry of the callback table, 0x19 bytes.
struct Callback_0043b7c0 {
    char unknown_0[4];
    int (__stdcall* notify)(Unit* unit, Class_0043a1f0* node, unsigned int pending);           // +0x4
    char unknown_8[0x19 - 0x8];
};

#pragma pack(pop)

extern Game* g_game;
extern Callback_0043b7c0* DAT_00512344;

void __stdcall FUN_0048a0f0(Unit* unit, int index);
int __stdcall FUN_004b6c30(int n);
void __stdcall FUN_00439f80(Unit* owner, Class_0043a1f0* node);

// 0x439eb0: deletes the nodes of the +0x5c list (all of them, or only those
// without flag 4), then with `all` every node of the +0x60 list.
void __stdcall FUN_00439eb0(Unit* owner, int all)
{
    Class_0043a1f0* first = owner->list;
    Class_0043a1f0** pp = &owner->list;
    Class_0043a1f0* node;
    while ((node = *pp) != 0) {
        if (!all && (node->flags & 4)) {
            pp = &node->next;
        } else {
            *pp = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
        }
    }
    if (all) {
        while ((node = owner->list2) != 0)
            FUN_00439f80(owner, node);
    }
}

// 0x439f80: unlinks `node` from the list its flag 0x40000 selects and
// deletes it.
void __stdcall FUN_00439f80(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0* first = owner->list;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->list2 : &owner->list;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
        if (n == node) {
            *link = node->next;
            if (node != first) {
                node->flags |= 0x10000;
            }
            delete node;
            return;
        }
        link = &n->next;
    }
}

// 0x439fe0: moves `node` to the end of the +0x5c list.
void __stdcall FUN_00439fe0(Unit* owner, Class_0043a1f0* node)
{
    Class_0043a1f0** pp = &owner->list;
    Class_0043a1f0* n = *pp;
    for (; n != node; n = n->next) {
        pp = &n->next;
    }
    n = node->next;
    *pp = n;
    for (; n != 0; n = n->next) {
        pp = &n->next;
    }
    *pp = node;
    node->next = 0;
}

// 0x43ac60: links `node` in front of `before`.
void __stdcall FUN_0043ac60(Unit* owner, Class_0043a1f0* node, Class_0043a1f0* before)
{
    Class_0043a1f0** link = (node->flags & 0x40000) ? &owner->list2
                                                   : &owner->list;
    while (*link != before)
        link = &(*link)->next;
    *link = node;
    node->unit = owner;
    node->next = before;
    if (before != 0)
        node->flags |= before->flags & 0x4000;
}

// 0x43b730: queues a fresh command of the given kind at the front.
void __stdcall FUN_0043b730(Unit* p, unsigned char type)
{
    Class_0043a1f0* child = new Class_0043a1f0(type, 0, 0, 0, 0, 0);
    child->flags |= 0x4000;
    FUN_0043ac60(p, child, (child->flags & 0x40000) ? p->list2 : p->list);
}

// Puts the node to sleep: it is due again a random delay (FUN_004b6c30(n))
// plus 30 frames from now.
static void Wait_0043b7c0(Class_0043a1f0* node, int n)
{
    unsigned int when = FUN_004b6c30(n) + 0x1e;
    node->flags6 |= 1;
    node->wakeFrame = g_game->frame + when;
}

// Clears the unit's three weapon targets.
static void ClearTargets_0043b7c0(Unit* unit)
{
    for (char i = 0; i < 3; i++)
        FUN_0048a0f0(unit, i);
}

// FUNCTION: 0x43b7c0
void __stdcall FUN_0043b7c0(Unit* unit)
{
    Class_0043a1f0* node;
    for (;;) {
        node = unit->list;
        if (node == 0)
            break;
        if (g_game->frame >= node->wakeFrame) {
            node->wakeFrame = 0xffffffff;
            node->field_4e |= 1;
        }
        unsigned int pending = (node->field_4e | unit->field_ba) & node->flags6;
        if (node->flags6 != 0 && pending == 0)
            return;
        unit->field_ba &= ~pending;
        node->field_4e &= ~pending;
        node->flags6 = 0;
        if (pending & 0x10000)
            ClearTargets_0043b7c0(unit);
        switch (DAT_00512344[node->kind].notify(node->unit, node, pending)) {
        case 3:
            Wait_0043b7c0(node, 0xf);
            break;
        case 1:
            node->count++;
            break;
        case 0:
            node->count = 0;
            break;
        case 2:
        case 4:
            break;
        case 5:
        case 8:
            FUN_00439f80(unit, node);
            break;
        case 9:
            node->flags |= 0x800000;
            if (node->next != 0) {
                FUN_00439f80(unit, node);
            } else {
                node->count = 0;
                Wait_0043b7c0(node, 0x1e);
            }
            break;
        case 6:
            FUN_00439fe0(unit, node);
            break;
        case 7:
            FUN_00439eb0(unit, 1);
            return;
        default:
            FUN_00439eb0(unit, 1);
            return;
        }
    }
    if (unit->player->active == 0)
        return;
    char state = unit->player->state;
    if (state != 1 && state != 2)
        return;
    if (unit->def->field_230 == 0)
        return;
    FUN_0043b730(unit, unit->def->field_230);
}
