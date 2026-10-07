// Decompiled by space-bunny-free. Names are provisional.
// Stays in its own file: its Class_00470250 and Class_00470270 views (const,
// with the inline assign helpers) cannot share unit_sync_46e9b0.cpp's plain
// views, and unifying them would change 0x470250's and 0x470270's signatures.
// Class_0046eaa0::operator= (0x470040). list_a and list_b are two vectors of
// 4-byte elements, each with an out-of-line capacity() (0x470250) and size()
// (0x470270) that both return the element count, i.e. a byte difference
// shifted right by 2. list_a's class inlines its size() for the first
// shifted right by 2.
#include <vector>

struct Elem_004702a0 {
    int unknown_0;
};

int* __stdcall FUN_004702d0(int* first, int* last, int* dest);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);

class Class_00470250 : public std::vector<Elem_004702a0> {
public:
    unsigned int FUN_00470250() const;

    unsigned int __inline count() const
    {
        if (!_First)
            return 0;
        return (int)((char *)_Last - (char *)_First) >> 2;
    }
};

class Class_00470270 : public Class_00470250 {
public:
    unsigned int FUN_00470270() const;

    __inline Elem_004702a0* begin() { return _First; }
    __inline Elem_004702a0* end() { return _Last; }
    __inline const Elem_004702a0* begin() const { return _First; }
    __inline const Elem_004702a0* end() const { return _Last; }

    // Three dead one-statement calls standing in for whatever the original
    // spent its inline budget on: /Ob2 inlines _Ucopy and _Destroy here only
    // as deep as this budget allows.
    static __inline void burn(int a)
    {
        int t = a;
        t = t * 3 + 1;
        (void)t;
    }

    // Separate from assign_second: list_a inlines size(), list_b does not.
    // the first list
    void __inline assign_first(Class_00470270* d, const Class_00470270* s)
    {
        if (d == s)
            ;
        else if (s->count() <= d->count()) {
            int* p = (int*)s->_First;
            int* e = (int*)s->_Last;
            int* q = (int*)d->_First;
            for (; p != e; ++p, ++q)
                *q = *p;
            d->_Last = d->_First + s->FUN_00470270();
        } else {
            unsigned int room = d->FUN_00470250();
            if (s->FUN_00470270() <= room) {
                Elem_004702a0* mid = s->_First;
                mid += d->FUN_00470270();
                FUN_004702d0((int*)s->begin(), (int*)mid, (int*)d->_First);
                d->_Ucopy(mid, s->_Last, d->_Last);

                d->_Last = d->_First + s->FUN_00470270();
            } else {
                d->_Destroy(d->_First, d->_Last);
                // begin()/end() here, raw fields in assign_second: sets the register split.
                operator delete(d->begin());
                int n = (int)s->FUN_00470270();
                if (n < 0)
                    n = 0;
                Elem_004702a0* p = (Elem_004702a0*)operator new(n * 4);
                d->_First = p;
                Elem_004702a0* q = d->_Ucopy(s->begin(), s->end(), p);
                d->_Last = q;
                d->_End = q;
            }
        }
    }

    // the second list
    void __inline assign_second(Class_00470270* d, const Class_00470270* s)
    {
        if (d == s)
            ;
        else if (s->FUN_00470270() <= d->FUN_00470270()) {
            Elem_004702a0* r = (Elem_004702a0*)FUN_004702d0(
                (int*)s->_First, (int*)s->_Last, (int*)d->_First);
            d->_Destroy(r, d->_Last);
            d->_Last = d->_First + s->FUN_00470270();
        } else {
            unsigned int room = d->FUN_00470250();
            if (s->FUN_00470270() <= room) {
                Elem_004702a0* mid = s->_First;
                mid += d->FUN_00470270();
                FUN_004702d0((int*)s->_First, (int*)mid, (int*)d->_First);
                d->_Ucopy(mid, s->_Last, d->_Last);

                d->_Last = d->_First + s->FUN_00470270();
            } else {
                d->_Destroy(d->_First, d->_Last);
                operator delete((void*)d->_First);
                int n = (int)s->FUN_00470270();
                if (n < 0)
                    n = 0;
                Elem_004702a0* p = (Elem_004702a0*)operator new(n * 4);
                d->_First = p;
                Elem_004702a0* q = d->_Ucopy(s->_First, s->_Last, p);
                d->_Last = q;
                d->_End = q;
            }
        }
    }
};

struct PacketSequencer {
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    char unknown_c[0x20];              // +0x0c

    PacketSequencer& operator=(const PacketSequencer& src);
};

struct Class_0046eaa0 {
    int field_0;                       // +0x00
    Class_00470270 list_a;             // +0x04
    Class_00470270 list_b;             // +0x14
    int field_24;                      // +0x24
    int field_28;                      // +0x28
    int field_2c;                      // +0x2c
    PacketSequencer sub;               // +0x30

    Class_0046eaa0& operator=(const Class_0046eaa0& src);
};

// FUNCTION: 0x470040
Class_0046eaa0& Class_0046eaa0::operator=(const Class_0046eaa0& src)
{
    field_0 = src.field_0;

    list_a.assign_first(&list_a, &src.list_a);
    list_b.assign_second(&list_b, &src.list_b);

    Class_00470270::burn(1);
    Class_00470270::burn(2);
    Class_00470270::burn(3);

    field_24 = src.field_24;
    field_28 = src.field_28;
    field_2c = src.field_2c;
    sub = src.sub;
    return *this;
}
