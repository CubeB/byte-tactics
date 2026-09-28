// Decompiled by space-bunny-free. Names are provisional.
// Drives a parent's list of attach/spot nodes (Class_0043a1f0) once per pass:
// every node that is due (or not waiting) is offered to the callback table
// DAT_00512344, and the answer decides what happens to it. The list is
// restarted from the head after every node, so nodes added by the callback
// are seen in the same pass. The field names follow 0x43a1f0.cpp and
// 0x43b730.cpp, which own Class_0043a1f0 and the list head at +0x5c/+0x60.
// The switch cases are in the order the original emitted the bodies
// (3, 1, 0, 2/4, 5/8/9, 6/7, default) and cases 6 and 7 return from the
// function while the other deleting cases just go on with the next node.

#pragma pack(push, 1)

class Class_0043a1f0;

// One entry of the callback table: a pointer to a per-kind notify function,
// in a 25-byte record (the index arithmetic is kind*5*4 + kind*5 + 4).
struct Callback_0043bad0 {
    char unknown_0[4];
    int (__stdcall* notify)(void* unit, Class_0043a1f0* obj, int code);    // +4
    char unknown_8[0x19 - 0x8];
};

class Class_0043a1f0 {
public:
    char unknown_0[4];
    unsigned char kind;             // +4, index into DAT_00512344
    unsigned char count;            // +5
    unsigned int flags_6;           // +6, bit 0 set while the node is waiting
    unsigned int wakeFrame;         // +0xa, frame the node becomes due at
    void* unit;                     // +0xe, passed to the callback
    char unknown_12[0x42 - 0x12];
    unsigned int flags;             // +0x42, bit 0x40000 picks the second list
    char unknown_46[0x4a - 0x46];
    Class_0043a1f0* next;           // +0x4a

    ~Class_0043a1f0();
};

struct Parent_0043bad0 {
    char unknown_0[0x5c];
    Class_0043a1f0* first;          // +0x5c
    Class_0043a1f0* firstTop;       // +0x60
};

struct Game_0043bad0 {
    char unknown_0[0x38a47];
    unsigned int frame;             // +0x38a47
};

#pragma pack(pop)

extern Callback_0043bad0* DAT_00512344;
extern Game_0043bad0* g_game;

int __stdcall FUN_004b6c30(int n);

// Takes the node out of the parent's list and frees it. The search can fail,
// when the node is not in the list at all, and then nothing is freed. The head
// of the list is read before the unlink, so a node that was the head does not
// get the 0x10000 flag. The local `first` is what forces that early read.
static void RemoveAndDelete(Parent_0043bad0* p, Class_0043a1f0* child)
{
    Class_0043a1f0* first = p->first;
    Class_0043a1f0** link = (child->flags & 0x40000) ? &p->firstTop : &p->first;
    Class_0043a1f0* node = *link;
    while (node != 0) {
        if (node == child) {
            *link = child->next;
            if (child != first)
                child->flags |= 0x10000;
            delete child;
            break;
        }
        link = &node->next;
        node = *link;
    }
}

// FUNCTION: 0x43bad0
void __stdcall FUN_0043bad0(Parent_0043bad0* p)
{
    Class_0043a1f0* child = p->firstTop;
    while (child != 0) {
        if (child->flags_6 == 0 || g_game->frame >= child->wakeFrame) {
            child->flags_6 = 0;
            switch (DAT_00512344[child->kind].notify(child->unit, child, 0)) {
            case 3: {
                // Ask again in a while. The temporary keeps the sum from being
                // folded into one lea, which is what the original does.
                unsigned int when = FUN_004b6c30(0xf) + 0x1e;
                child->flags_6 |= 1;
                child->wakeFrame = g_game->frame + when;
                break;
            }
            case 1:
                child->count++;
                break;
            case 0:
                child->count = 0;
                break;
            case 2:
            case 4:
                break;
            case 5:
            case 8:
            case 9:
                RemoveAndDelete(p, child);
                break;
            case 6:
            case 7:
                RemoveAndDelete(p, child);
                return;
            default:
                RemoveAndDelete(p, child);
                break;
            }
            child = p->firstTop;
        } else {
            child = child->next;
            continue;
        }
    }
}
