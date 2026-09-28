// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Moves the element selected by node->index to the front (if any), then bubble
// sorts the remaining elements ascending by the integer average of
// node->table[3 * index + 1] over each element's `count` ushort indices.
//
// Still differs (best 73.6%): every instruction matches except the register
// that holds the `node` parameter. The original keeps `node` in ebp for the
// whole function (prologue `push ebp; mov ebp,[esp+0x3c]`, reused for
// node->table and for the `p` element pointer, reloaded from the argument slot
// at the loop bottom with `mov ebp,edx`); this compiler puts `node` in edx and
// then ebp is free for the first sum loop's count, which cascades into the
// rest of the register assignment. Everything else (struct copies, loop
// bounds `i < node->count - 1`, signed divisions, local stack slots e@0x10,
// i@0x14, swapped@0x18, a@0x1c, temp@0x20, frame sub esp,0x30) is identical.
// Tried with no effect: register/bool/int/unsigned/long types for node,
// swapped, i and the counts; declaration-order permutations; pointer vs
// reference parameters; an alias local for node; inline getters and helpers
// for elems/table/count/average/swap; memcpy and explicit copy helpers; direct
// node->elems vs a cached elems pointer; all 128 combinations of the common
// headers; do-while/while/for outer loop forms; names of the parameter and the
// locals; `register` on the parameter and locals.
// Retried by deepseek-v4.1-flash with no effect: each common header alone;
// __thiscall method; Node& parameter; a differently-named local copy of the
// parameter (`Node* node = param;`) so node is a local, not a parameter;
// label+goto and for(;;)+break outer loops; an explicit extra use of node to
// raise its register priority. Worse: re-declaring the two sum loops with the
// sibling 0x4cb2f0's named locals (`int n`, `unsigned short* p`, `*p++`),
// whether written inline or as an inlined static helper (48.6%, node lands in
// eax). Index-based `node->elems[i]` walking also much worse (46.6%). The
// source structure above is right; the allocator just refuses ebp for node.
// Note the sibling 0x4cb2f0 (the average helper, no direct callers) is inlined
// here, and its first sum loop's registers (count in esi, counter in edi) are
// what falls out of this source once node is in ebp.

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
            int a = 0;
            for (int k = 0; k < e->count; k++)
                a += node->table[e->indices[k] * 3 + 1];
            a /= e->count;
            Elem_004cb4c0* p = e + 1;
            int b = 0;
            for (int j = 0; j < p->count; j++)
                b += node->table[p->indices[j] * 3 + 1];
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
