// Decompiled by DeepSeek V4.1 Flash, finished by space-bunny-free. Names are provisional.
// PARTIAL (59.7%, 756 bytes against the original's 780). Per-frame driver of
// the unit's command list (+0x5c), the "main list" twin of 0x43bad0 (the +0x60
// list, matched, and the source of the Wait_0043b7c0 shape below). The list
// head is re-read after every node, so a node the callback re-queues is seen
// again in the same pass. The switch cases are written in the order the
// original emitted the bodies (3, 1, 0, 2/4, 5/8, 9, 6, 7, default), which is
// also the order of the jump table at 0x43baa4.
//
// What matches byte for byte: the whole prologue (unit in edi, node in esi,
// the list-head address in ebx, spilled to [esp+0x10]), the due check, the
// 3-iteration FUN_0048a0f0 countdown loop (it only matches as a do/while,
// which is what frees ebx for the counter and pushes the head into the
// spill), the case layout, the tail-merged Wait block shared by cases 3 and
// 9, both unlink searches, the delete sequence, ClearAll and the two
// FUN_0043ac60 arms.
//
// What still differs: one allocator state, seen as an eax/ecx swap in every
// block. The original keeps the 16-bit mask in ecx and flags6 in eax
// (`mov cx,word [edi+0xba]` / `mov eax,[esi+6]`); ours keeps the mask in eax
// and flags6 in ecx. The same swap reappears downstream: the callback-table
// base is in edx here and ecx in the original, `unit` is reloaded into ecx
// for the 3-iteration loop instead of eax, `unit->def` into eax instead of
// edx, `*link` in ClearAll into ecx instead of edx, and the default case's
// `unit` into edx instead of eax. Tried and rejected: the mask as `int`
// (55.5%, it also gains a `xor eax,eax; mov ax`), the field read twice with
// no local at all (identical score, identical code), the `unit` reload for
// the field_ba store hoisted differently. The one shape that did help was
// Wait_0043b7c0: with `when = FUN_004b6c30(n) + 0x1e` computed first and
// `flags6 |= 1` second it tail-merges cases 3 and 9 the way the original does
// (0x43b951); the other order duplicated the block at both sites (792 bytes).

#pragma pack(push, 1)

struct Unit_0043b7c0;
struct UnitDef_0043b7c0;
struct Player_0043b7c0;

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;            // +0x4, index into DAT_00512344
    unsigned char count;           // +0x5
    unsigned int flags6;           // +0x6, bit 0 set while the node waits
    unsigned int wakeFrame;        // +0xa
    Unit_0043b7c0* unit;           // +0xe, handed to the callback
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

struct Unit_0043b7c0 {
    char unknown_0[0x5c];
    Class_0043a1f0* list;          // +0x5c
    Class_0043a1f0* list2;         // +0x60
    char unknown_64[0x92 - 0x64];
    UnitDef_0043b7c0* def;         // +0x92
    Player_0043b7c0* player;       // +0x96
    char unknown_9a[0xba - 0x9a];
    unsigned short field_ba;       // +0xba, the unit's pending mask
};

struct Game_0043b7c0 {
    char unknown_0[0x38a47];
    unsigned int frame;            // +0x38a47
};

// One entry of the callback table, 0x19 bytes: the index arithmetic is
// kind*5*4 + kind*5 + 4.
struct Callback_0043b7c0 {
    char unknown_0[4];
    int (__stdcall* notify)(Unit_0043b7c0* unit, Class_0043a1f0* node, unsigned int pending);  // +0x4
    char unknown_8[0x19 - 0x8];
};

#pragma pack(pop)

extern Game_0043b7c0* g_game;
extern Callback_0043b7c0* DAT_00512344;

void* operator new(unsigned int size);
void operator delete(void* p);
int __stdcall FUN_0048a0f0(Unit_0043b7c0* unit, int index);
int __stdcall FUN_004b6c30(int n);
void __stdcall FUN_00439eb0(Unit_0043b7c0* unit, int all);
void __stdcall FUN_00439f80(Unit_0043b7c0* unit, Class_0043a1f0* node);
void __stdcall FUN_0043ac60(Unit_0043b7c0* unit, Class_0043a1f0* node, Class_0043a1f0* before);

// Unlinks `node` from the list `pp` starts (or the unit's second list when the
// node has flag 0x40000) and deletes it, marking it 0x10000 unless it is the
// head. Returns false when the node is not in the list at all.
static bool RemoveAndDelete(Unit_0043b7c0* unit, Class_0043a1f0** pp, Class_0043a1f0* node)
{
    Class_0043a1f0* first = *pp;
    Class_0043a1f0** link = (node->flags & 0x40000) ? &unit->list2 : pp;
    for (Class_0043a1f0* n = *link; n != 0; n = n->next) {
        if (n == node) {
            *link = node->next;
            if (node != first)
                node->flags |= 0x10000;
            delete node;
            return true;
        }
        link = &n->next;
    }
    return false;
}

// Takes `node` out of the list and puts it back at the end.
static void MoveToEnd(Class_0043a1f0** pp, Class_0043a1f0* node)
{
    Class_0043a1f0** link = pp;
    while (*link != node)
        link = &(*link)->next;
    *link = node->next;
    while (*link != 0)
        link = &(*link)->next;
    *link = node;
    node->next = 0;
}

// Deletes every node of the first list, then every node of the second one.
static void ClearAll(Unit_0043b7c0* unit, Class_0043a1f0** pp)
{
    Class_0043a1f0* first = *pp;
    Class_0043a1f0* node;
    while ((node = *pp) != 0) {
        *pp = node->next;
        if (node != first)
            node->flags |= 0x10000;
        delete node;
    }
    while ((node = unit->list2) != 0)
        FUN_00439f80(unit, node);
}

// Puts the node to sleep for `n` milliseconds. The `when` temporary is what
// keeps the sum from folding into one lea, and the flag is set after the call
// so that both call sites tail-merge (as in 0x43bad0).
static void Wait_0043b7c0(Class_0043a1f0* node, int n)
{
    unsigned int when = FUN_004b6c30(n) + 0x1e;
    node->flags6 |= 1;
    node->wakeFrame = g_game->frame + when;
}

// FUNCTION: 0x43b7c0
void __stdcall FUN_0043b7c0(Unit_0043b7c0* unit)
{
    Class_0043a1f0** pp = &unit->list;
    Class_0043a1f0* node = *pp;

    while (node != 0) {
        if (g_game->frame >= node->wakeFrame) {
            node->wakeFrame = 0xffffffff;
            node->field_4e |= 1;
        }
        unsigned short mask = unit->field_ba;
        unsigned int pending = (node->field_4e | mask) & node->flags6;
        if (node->flags6 != 0 && pending == 0)
            return;
        unit->field_ba = (unsigned short)(~pending) & mask;
        node->field_4e &= ~pending;
        node->flags6 = 0;

        if (pending & 0x10000) {
            int i = 0;
            int n = 3;
            do {
                FUN_0048a0f0(unit, i);
                i++;
            } while (--n);
        }

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
            RemoveAndDelete(unit, pp, node);
            break;
        case 9:
            node->flags |= 0x800000;
            if (node->next == 0) {
                node->count = 0;
                Wait_0043b7c0(node, 0x1e);
            } else {
                RemoveAndDelete(unit, pp, node);
            }
            break;
        case 6:
            MoveToEnd(pp, node);
            break;
        case 7:
            ClearAll(unit, pp);
            return;
        default:
            FUN_00439eb0(unit, 1);
            return;
        }
        node = *pp;
    }

    if (unit->player->active == 0)
        return;
    char state = unit->player->state;
    if (state != 1 && state != 2)
        return;
    unsigned char kind = unit->def->field_230;
    if (kind == 0)
        return;
    Class_0043a1f0* cmd = new Class_0043a1f0(kind, 0, 0, 0, 0, 0);
    cmd->flags |= 0x4000;
    FUN_0043ac60(unit, cmd, (cmd->flags & 0x40000) ? unit->list2 : *pp);
}
