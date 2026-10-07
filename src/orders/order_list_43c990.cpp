// Decompiled by space-bunny-free. Names are provisional.
// Insertion sort over 25-byte records with a __stdcall comparison callback,
// the same element type and callback style as the rest of this sort family
// (0x43c940 the unguarded insert, 0x43cb20 the unguarded partition). The
// insert of one element into the sorted prefix is the body of 0x43c940.

#pragma pack(push, 1)
struct Elem_0043c990 {
    char data[0x19];
};
#pragma pack(pop)

typedef int (__stdcall* Pred_0043c990)(const Elem_0043c990&, const Elem_0043c990&);

// The value is passed by value.
static inline void Insert_0043c990(Elem_0043c990* last, Elem_0043c990 value,
                                    Pred_0043c990 pred)
{
    for (Elem_0043c990* m = last; pred(value, *--m); last = m)
        *last = *m;
    *last = value;
}

// FUNCTION: 0x43c990
void __stdcall FUN_0043c990(Elem_0043c990* first, Elem_0043c990* last,
                            Pred_0043c990 pred, int*)
{
    if (first == last)
        return;
    if (first + 1 == last)
        return;
    for (Elem_0043c990* cur = first + 1; cur != last; ++cur) {
        Elem_0043c990 value = *cur;
        if (!pred(value, *first)) {
            Insert_0043c990(cur, value, pred);
        } else {
            Elem_0043c990* i = cur;
            if (first != i) {
                do {
                    --i;
                    i[1] = *i;
                } while (i != first);
            }
            *first = value;
        }
    }
}
