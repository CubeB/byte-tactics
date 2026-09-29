// Decompiled by deepseek-v4.1-flash. Names are provisional.
// Still differs: the body is right but MSVC picks different stack slots for
// the three median copies, the Median return temporary and the partition's
// by-value pivot. Original allocates c=0x10, b=0x25c, a=0x4a8, pivot=0x6f4,
// return temp=0x940, swap temp=0xb8c; ours puts the return temp at 0x25c and
// b at 0x6f4. Everything else (counts, branches, operator= swap) lines up.

// 585-byte GUI list entry (same class as 0x432fb0.cpp).
class Class_0042b370 {
public:
    char unknown_0[0x249];
    Class_0042b370& operator=(const Class_0042b370& src);
};

typedef int (__stdcall* Compare)(const Class_0042b370&, const Class_0042b370&);

static inline Class_0042b370 Median(Class_0042b370 a, Class_0042b370 b,
                                    Class_0042b370 c, Compare comp)
{
    if (comp(a, b)) {
        if (comp(b, c))
            return b;
        if (comp(a, c))
            return c;
        return a;
    }
    if (comp(a, c))
        return a;
    if (comp(b, c))
        return c;
    return b;
}

static inline Class_0042b370* Unguarded_partition(Class_0042b370* first,
        Class_0042b370* last, Class_0042b370 pivot, Compare comp)
{
    for (;;) {
        while (comp(*first, pivot))
            first++;
        last--;
        while (comp(pivot, *last))
            last--;
        if (last <= first)
            return first;
        Class_0042b370 tmp = *first;
        *first = *last;
        *last = tmp;
        first++;
    }
}

// FUNCTION: 0x432d40
void __stdcall FUN_00432d40(Class_0042b370* first, Class_0042b370* last,
                            Compare comp, int unused)
{
    int n = (int)((char*)last - (char*)first) / (int)sizeof(Class_0042b370);
    if (n <= 16)
        return;
    do {
        Class_0042b370* left = Unguarded_partition(first, last,
                Median(*first, *(first + n / 2), *(last - 1), comp), comp);
        int right_count = (int)((char*)last - (char*)left) / (int)sizeof(Class_0042b370);
        int left_count = (int)((char*)left - (char*)first) / (int)sizeof(Class_0042b370);
        if (right_count <= left_count) {
            FUN_00432d40(left, last, comp, 0);
            last = left;
        } else {
            FUN_00432d40(first, left, comp, 0);
            first = left;
        }
        n = (int)((char*)last - (char*)first) / (int)sizeof(Class_0042b370);
    } while (n > 16);
}
