// Decompiled by deepseek-v4.1-flash, finished by space-bunny-free. Names are provisional.
// Moves the element selected by node->index to the front (if any), then bubble
// sorts the remaining elements ascending by the integer average of
// node->table[3 * index + 1] over each element's `count` ushort indices.
//
// The one construct that decided the register allocation: each averaging loop
// caches the index pointer in a local and counts DOWN (`for (k = e->count;
// k > 0; k--)` over `*q++`). That is what puts `node` in ebp, the way the
// original holds it, and everything else then falls into place.

struct Elem_004cb4c0 {              // 0x20 bytes
    int unknown_0;                  // +0x0
    int count;                      // +0x4
    int ptr_8;                      // +0x8
    unsigned short* indices;        // +0xc
    int ptr_10;                     // +0x10
    int unknown_14[3];              // +0x14
};

struct Node_004cb4c0 {
    int unknown_0;                  // +0x0
    int unknown_4;                  // +0x4
    int count;                      // +0x8
    int index;                      // +0xc
    int unknown_10[5];              // +0x10
    int* table;                     // +0x24
    Elem_004cb4c0* elems;           // +0x28
};

// FUNCTION: 0x4cb370
void __stdcall FUN_004cb370(Node_004cb4c0* node)
{
    if (node->index != -1 && node->count > 0) {
        Elem_004cb4c0* elems = node->elems;
        Elem_004cb4c0 temp = elems[node->index];
        elems[node->index] = elems[0];
        elems[0] = temp;
        node->index = 0;
    }
    Elem_004cb4c0* e;
    int i;
    int swapped;
    do {
        swapped = 0;
        e = node->elems + 1;
        for (i = 1; i < node->count - 1; i++, e++) {
            unsigned short* q = e->indices;
            int a = 0;
            for (int k = e->count; k > 0; k--)
                a += node->table[*q++ * 3 + 1];
            a /= e->count;
            Elem_004cb4c0* p = e + 1;
            unsigned short* r = p->indices;
            int b = 0;
            for (int j = p->count; j > 0; j--)
                b += node->table[*r++ * 3 + 1];
            b /= p->count;
            if (a > b) {
                Elem_004cb4c0 t = *e;
                *e = *p;
                *p = t;
                swapped = 1;
            }
        }
    } while (swapped);
}
