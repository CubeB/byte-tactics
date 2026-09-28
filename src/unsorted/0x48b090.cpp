// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
// Sets or clears bits of the unit's state byte at +0x10e and reacts to the three
// bits that mean active (1), building (8) and working (4). The gained and the
// lost bits are tested separately: each gained bit plays its script event and
// its message, and losing the working bit (4) tells every object linked to this
// unit (the list head at +0xa2) to update, then a network packet (0x11) tells
// the owner when the owner is a real player (1 or 2).
//
// Not a match yet (93.2%, same size). What still differs:
// 1. In the set branch MSVC 5 gives the destination register to the other
//    operand than the original does: the original loads the old state into eax
//    and the mask into edx (`mov eax,[esp+0xc]; mov edx,[esp+0x14]; and both;
//    or eax,edx`), this version loads the mask into eax and the old state into
//    edx. Swapping the operand order in the source, casting both operands,
//    moving the mask through a local and writing the branch as two statements
//    all leave the code unchanged, so MSVC 5 canonicalises the commutative
//    order and something else in the original file must have decided it. The
//    same flip appears in the clear branch and in `lost`.
// 2. The clear branch here is `(unsigned char)(old & ~(unsigned char)mask)`,
//    which makes MSVC read the mask as a byte (`mov dl,[esp+0x14]; not dl`)
//    and keeps the old state in eax. The original does the whole thing at
//    dword width (`mov eax,[esp+0x14]; and eax,0xff; not eax; and eax,edx`
//    with the old state in edx). This byte form scores two lines higher only
//    because the diff aligns the two `and` lines; the plain
//    `old & ~(unsigned char)mask` (93.2% -> 92.3%) is closer in instructions,
//    so a future attempt should start from that instead.
// 3. `lost` is computed into al here (`not al; and al, cl`), the original
//    computes it into cl (`not al; and cl, al`).
// 4. The packet's type byte: the original stores it between the other two
//    (`mov word [E+5], cx; mov byte [E+4], 0x11; mov byte [E+7], dl`), this
//    version sinks the constant store past the argument pushes, just before
//    the call. Reordering the field assignments and aggregate initialisation
//    both leave the store sunk.
// The rest of the function (every call, both list walks, the frame, one dword
// of locals with `int now` in it) matches exactly.

#pragma pack(push, 1)

struct Player_0048b090 {
    int active;                         // +0x0
    int id;                             // +0x4
    char unknown_8[0x73 - 0x8];
    char kind;                          // +0x73, 1 or 2 for a real player
};

struct Packet_0048b090 {
    unsigned char type;                 // +0x0
    short field_1;                      // +0x1, the unit id
    unsigned char field_3;              // +0x3, the new state
};

#pragma pack(pop)

// One virtual slot, called on the object a link belongs to.
class Class_0043a1e0 {
public:
    virtual void FUN_0043a1e0(unsigned int value);
};

// The links of the owner's list; the head of a unit's list is at +0xa2.
class Class_004895c0 {
public:
    void* vptr;                         // +0x0
    void* owner;                        // +0x4
    Class_004895c0* next;               // +0x8
    Class_0043a1e0* value;              // +0xc
};

class Class_004b0940 {
public:
    void FUN_004b0940(const char* name, int a, int b);
};

#pragma pack(push, 1)
class Class_0048b090 {
public:
    char unknown_0[0x96];
    Player_0048b090* player;            // +0x96
    Class_004b0940* vars;               // +0x9a, the script
    void* block;                        // +0x9e
    Class_004895c0* head;               // +0xa2, the link list
    char unknown_a6[0xa8 - 0xa6];
    unsigned short id;                  // +0xa8
    char unknown_aa[0x10e - 0xaa];
    unsigned char state;                // +0x10e

    void FUN_0048b090(int mask, int set);
};
#pragma pack(pop)

void __stdcall FUN_0047f780(Class_0048b090* unit, int kind, char* text);
void __stdcall FUN_0041c110(Class_0048b090* unit);
int __stdcall FUN_00451df0(int player, void* data, int size);

// FUNCTION: 0x48b090
void Class_0048b090::FUN_0048b090(int mask, int set)
{
    unsigned char old = state;
    int now;
    if (set)
        now = old | (unsigned char)mask;
    else
        now = (unsigned char)(old & ~(unsigned char)mask);
    state = (unsigned char)now;
    if ((unsigned char)now != old) {
        unsigned char gained = ~old & now;
        unsigned char lost = old & ~now;
        if (gained & 1) {
            vars->FUN_004b0940("Activate", 0, 0);
            FUN_0047f780(this, 3, 0);
        }
        if (lost & 1) {
            vars->FUN_004b0940("Deactivate", 0, 0);
            FUN_0047f780(this, 4, 0);
        }
        if (gained & 8)
            vars->FUN_004b0940("StartBuilding", 0, 0);
        if (lost & 8)
            vars->FUN_004b0940("StopBuilding", 0, 0);
        if (gained & 4) {
            FUN_0047f780(this, 0xe, 0);
            for (Class_004895c0* link = head; link; link = link->next) {
                if (link->value)
                    link->value->FUN_0043a1e0(0x10000);
            }
        }
        if (lost & 4)
            FUN_0047f780(this, 0xf, 0);
        FUN_0041c110(this);
        Player_0048b090* p = player;
        if (p->active != 0 && (p->kind == 1 || p->kind == 2)) {
            Packet_0048b090 packet;
            packet.type = 0x11;
            packet.field_1 = id;
            packet.field_3 = state;
            FUN_00451df0(p->id, &packet, 4);
        }
    }
}
