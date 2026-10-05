// Decompiled by Opus. Names are provisional.

#pragma pack(push, 1)
struct Node_0047e570 {
    char unknown_0[0x8e];
    Node_0047e570* next;     // +0x8e
    char unknown_92[0xf9 - 0x92];
    char id;                 // +0xf9
};

struct Struct_0047e570 {
    char unknown_0[0x86];
    int field_86;            // +0x86
    Node_0047e570* list;     // +0x8a
};
#pragma pack(pop)

// FUNCTION: 0x47e570
int __stdcall FUN_0047e570(Struct_0047e570* p, int id)
{
    if (p->field_86 != 0) {
        return 0;
    }
    for (Node_0047e570* n = p->list; n != 0; n = n->next) {
        if (n->id == id) {
            return 0;
        }
    }
    return 1;
}
