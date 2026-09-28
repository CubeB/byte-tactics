// Decompiled by deepseek-v4.1-flash. Names are provisional.

#pragma pack(push, 1)
struct Gadget_004ab0b0 {
    char unknown_0[0x13];
    short x;                           // +0x13
    short y;                           // +0x15
    short w;                           // +0x17
    short h;                           // +0x19
    char unknown_1b[0xbc - 0x1b];
    void* surface;                     // +0xbc
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Node_004ab0b0 {
    Node_004ab0b0* next;               // +0x0
    Gadget_004ab0b0* gadget;           // +0x4
    char unknown_8[0x14 - 0x8];
    int state;                         // +0x14
};
#pragma pack(pop)

struct Rect_004ab0b0 {
    int x1;                            // +0x0
    int y1;                            // +0x4
    int x2;                            // +0x8
    int y2;                            // +0xc
};

int __stdcall FUN_004b67d0(Rect_004ab0b0* a, Rect_004ab0b0* b);
void __stdcall FUN_004c6b70(void* dest, void* image, int x, int y);

// FUNCTION: 0x4ab0b0
int __stdcall FUN_004ab0b0(Node_004ab0b0* node, void* param_2, Rect_004ab0b0* param_3)
{
    Rect_004ab0b0 rect;
    if (node == 0) {
        return 0;
    }
    FUN_004ab0b0(node->next, param_2, param_3);
    Gadget_004ab0b0* g = node->gadget;
    rect.x1 = g->x;
    rect.y1 = g->y;
    rect.x2 = g->w + rect.x1 - 1;
    rect.y2 = g->h + rect.y1 - 1;
    if (param_3 == 0) {
        if (node->state == 1) {
            node->state = 0;
            FUN_004c6b70(param_2, g->surface, g->x, g->y);
        }
    } else {
        if (node->state != 1 && FUN_004b67d0(&rect, param_3) == 0) {
            goto finish;
        }
        node->state = 0;
        FUN_004c6b70(param_2, g->surface, g->x, g->y);
    }
finish:
    return 1;
}
