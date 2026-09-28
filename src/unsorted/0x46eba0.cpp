// Decompiled by longcat-2.5-preview-free. Names are provisional.
// Class_0046eba0 is a byte-oriented vector of 0xe-byte Packet_0046cef0 elements
// (char* first/last/end, element size 14). FUN_0046eba0 is its
// insert(pos, count, val), out of line. The copy helpers guard against a null
// destination (allocation failure); fill and copy_backward do not.

#pragma pack(push, 1)
struct Packet_0046cef0 {         // 0xe bytes
    unsigned char type;          // +0x0
    unsigned char arg;           // +0x1
    unsigned int id;             // +0x2
    int field_6;                 // +0x6
    int field_a;                 // +0xa
};
#pragma pack(pop)

void* operator new(unsigned int size);
void operator delete(void* p);

class Class_0046eba0 {
public:
    char unknown_0[4];
    char* first;                 // +0x4
    char* last;                  // +0x8
    char* end;                   // +0xc
    void FUN_0046eba0(char* _P, unsigned int _M, Packet_0046cef0* _X);
};

static char* Ucopy(char* first, char* last, char* dest)
{
    for (; first != last; first += 14, dest += 14)
        if (dest)
            *(Packet_0046cef0*)dest = *(Packet_0046cef0*)first;
    return dest;
}

static void Ufill(char* dest, unsigned int n, Packet_0046cef0* val)
{
    for (; 0 < n; --n, dest += 14)
        if (dest)
            *(Packet_0046cef0*)dest = *val;
}

static void Fill(char* first, char* last, Packet_0046cef0* val)
{
    for (; first != last; first += 14)
        *(Packet_0046cef0*)first = *val;
}

static void CopyBackward(char* first, char* last, char* dest)
{
    while (last != first) {
        dest -= 14;
        last -= 14;
        *(Packet_0046cef0*)dest = *(Packet_0046cef0*)last;
    }
}

// FUNCTION: 0x46eba0
void Class_0046eba0::FUN_0046eba0(char* _P, unsigned int _M, Packet_0046cef0* _X)
{
    if ((end - last) / 14 < _M)
    {
        int oldsize = first ? (last - first) / 14 : 0;
        int newsize = oldsize + (_M < oldsize ? oldsize : _M);
        if (newsize < 0)
            newsize = 0;
        char* buf = (char*)operator new(newsize * 14);
        char* q = Ucopy(first, _P, buf);
        Ufill(q, _M, _X);
        Ucopy(_P, last, q + _M * 14);
        operator delete(first);
        end = buf + newsize * 14;
        last = buf + (_M + (first ? (last - first) / 14 : 0)) * 14;
        first = buf;
    }
    else if ((last - _P) / 14 < _M)
    {
        Ucopy(_P, last, _P + _M * 14);
        Ufill(last, _M - (last - _P) / 14, _X);
        Fill(_P, last, _X);
        last += _M * 14;
    }
    else if (0 < _M)
    {
        Ucopy(last - _M * 14, last, last);
        CopyBackward(_P, last - _M * 14, last);
        Fill(_P, _P + _M * 14, _X);
        last += _M * 14;
    }
}
