// Decompiled by Sonnet. Names are provisional.

#pragma pack(push, 2)
struct Node_0047cb00 {
    char unknown_0[0x8e];
    Node_0047cb00* next;                // +0x8e
};

class Class_0047cb00 {
public:
    char unknown_0[6];
    Node_0047cb00* head;                 // +6
    void FUN_0047cb00(Node_0047cb00* node);
};
#pragma pack(pop)

// FUNCTION: 0x47cb00
void Class_0047cb00::FUN_0047cb00(Node_0047cb00* node)
{
    Node_0047cb00** pp = &head;
    Node_0047cb00* n = head;
    while (n != node) {
        pp = &n->next;
        n = n->next;
    }
    *pp = node->next;
    node->next = 0;
}
