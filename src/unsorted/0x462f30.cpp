// Decompiled by deepseek-v4.1-flash, finished by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// PARTIAL: 43.3%. Still differs: the frame is sub esp,0xc against sub esp,0x10 (three
// spilled locals instead of four: original homes i/entry in [esp+0x10], tick in
// [esp+0x14], flag in [esp+0x18] and a in [esp+0x1c]; ours homes i/entry, flag and
// tick, so every local and argument slot is 4 bytes low). That 4-byte shift is the
// bulk of the remaining diff, plus register allocation: the original keeps a
// long-lived zero in edi (cmp eax,edi, mov [eax+4],edi), tick in ebp and the walk in
// esi, while ours has the walk in ebp, tick in edi and no zero register, so
// comparisons/stores use immediates instead of edi.
// Earlier passes fixed: the ring-pop block declares `RingEntry* ee = 0` outside the
// `if (r->n > 0)` and assigns len/src after it (the original really loads
// ee->size/ee->data through the null ee on the r->n <= 0 path, 0x462fb5 and
// 0x463523), and the first loop walks &entries[0].tail.buffer (+0x34, id at [-0x28]).
// This pass fixed: the prev normalization `prev = a; if (prev >= -1) prev = -2;`
// (an if-statement, not `(a < -1) ? a : -2`, which let MSVC keep a in a register and
// gave the -0x1c slot), 42.2 -> 43.3%.
// Tried without gain (all 43.3% or worse): `int zero = 0` (41.1%), a `p += 13`
// int-pointer tail walk (40.1%), reordering flag/cur/prev (35.6-41.7%), declaring
// flag/a at point of use, long/unsigned types, moving flag/entry decls, `(void)a`,
// the Previous/Next helpers as if-statements, and 128 header sets via headers.py.
#include <string.h>

struct RingEntry_00462f30 {
    int v0;                            // +0x00
    void* data;                        // +0x04
    int size;                          // +0x08
};

// Ring of 0x200 12-byte entries, the object at entry+0x28.
struct Ring_00462f30 {
    int n;                             // +0x00
    int head;                          // +0x04
    int tail;                          // +0x08
    RingEntry_00462f30 entry[0x200];   // +0x0c
};

struct Tail_00462f30 {
    int field_0;                       // +0x18
    int field_4;                       // +0x1c
    int field_8;                       // +0x20
    int field_c;                       // +0x24
    Ring_00462f30* buffer;             // +0x28
    int field_14;                      // +0x2c
    int field_18;                      // +0x30
};

struct Entry_00462f30 {
    int field_0;                       // +0x00 the id
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    char* field_14;                    // +0x14
    Tail_00462f30 tail;                // +0x18
};

#pragma pack(push, 1)
struct Game_00462f30 {
    char unknown_0[0x38a47];
    int tick;                          // +0x38a47
};
#pragma pack(pop)

extern Game_00462f30* g_game;

class Class_0044f9c0 {
public:
    int FUN_0044f9c0(void* net, char* buf, int* len);
};
extern Class_0044f9c0 DAT_005129f8;

void __stdcall FUN_004568b0(int a, int b, int c);

static int Previous_00462f30(int n) { --n; return n >= -1 ? -2 : n; }
static int Next_00462f30(int n) { ++n; return n >= -1 ? -2 : n; }

struct Class_00463730 {
    int FUN_00463790(void* data, unsigned int size, int tick, int a4, int a5,
                     int flag);
};

void __cdecl FUN_00461170(const char* fmt, ...);
void* __cdecl operator new(unsigned int size);
void __cdecl operator delete(void* p);
int __stdcall FUN_004c9530(int rc);

class Class_00462d30 {
public:
    int field_0;                       // +0x00
    int field_4;                       // +0x04
    int field_8;                       // +0x08
    int field_c;                       // +0x0c
    int field_10;                      // +0x10
    int field_14;                      // +0x14
    char* buffer;                      // +0x18
    char* spare;                       // +0x1c
    Entry_00462f30 entries[10];        // +0x20
    int capacity;                      // +0x228
    int length;                        // +0x22c
    int field_230;                     // +0x230
    int field_234;                     // +0x234
    int field_238;                     // +0x238

    Entry_00462f30* FUN_00462d90(long id);
    int FUN_00462f30(void* packet, void* dest, unsigned int* size);
};

// FUNCTION: 0x462f30
int Class_00462d30::FUN_00462f30(void* packet, void* dest, unsigned int* size)
{
    int tick = g_game->tick;
    unsigned int i;
    Entry_00462f30* e;
    Ring_00462f30* r;
    int* p;
    void* src;
    unsigned int len;
    Entry_00462f30* entry = 0;
    int flag;
    int a;
    Tail_00462f30* t;

    e = entries;
    for (i = 0; i < 10; i++, e++) {
        if (e->field_0 == -1)
            break;
        r = e->tail.buffer;
        len = 0;
        if (r == 0 || r->n <= 0)
            p = 0;
        else
            p = (int*)((char*)r + (r->head + 1) * 12);
        src = 0;
        if (p != 0 && (tick == 0 || (*p - tick) <= 0 || (*p - tick) > 0x1e)) {
            RingEntry_00462f30* ee = 0;
            if (r->n > 0) {
                int h;
                r->n--;
                h = r->head + 1;
                r->head = h;
                ee = (RingEntry_00462f30*)((char *)r + h * 12);
                if (h >= 0x200)
                    r->head = 0;
            }
            len = ee->size;
            src = ee->data;
        }
        if (src != 0) {
            *(int*)((char*)packet + 0x4b5) = e->tail.field_14;
            *(int*)((char*)packet + 0x4b9) = e->tail.field_18;
            goto copy_out;
        }
    }

    entry = 0;
    if (length != 0)
        goto route_frames;
    if (length == 0) {
        Entry_00462f30* link = (Entry_00462f30*)field_14;
        int ecx = 0;
        if (link != 0) {
            if (spare != 0) {
                buffer = spare;
                if (field_230 > 0) {
                    length = field_230;
                    field_c = field_234;
                    field_10 = field_238;
                    ecx = length - 4;
                    if (ecx > 0)
                        link->field_8 = *(int*)buffer;
                } else {
                    length = 0;
                }
                spare = 0;
                ((Entry_00462f30*)field_14)->field_c = 0;
                field_14 = 0;
            } else if (link->field_c > 0) {
                spare = buffer;
                field_234 = field_c;
                field_230 = 0;
                field_238 = field_10;
                buffer = link->field_14;
                link->field_8 = *(int*)buffer;
                link = (Entry_00462f30*)field_14;
                length = link->field_c;
                field_c = link->field_0;
                field_10 = link->field_4;
                ecx = length - 4;
                link->field_c = 0;
            } else {
                field_14 = 0;
            }
        }
        if (ecx != 0)
            goto route_frames;
        if (ecx == 0) {
            int rc;
            length = capacity;
            rc = DAT_005129f8.FUN_0044f9c0((char*)g_game + 0x14, buffer, &length);
            while (rc != 0) {
                if (rc == (int)0x887700be)
                    goto fail832;
                if (rc != (int)0x8877001e) {
                    FUN_00461170("HAPINET_receivepacket failed (%s)\n", (char*)FUN_004c9530(rc));
                    length = 0;
                    return rc;
                }
                operator delete(buffer);
                capacity = length;
                length = 0;
                buffer = (char*)operator new(capacity);
                if (buffer == 0) {
                    capacity = 0;
                    return (int)0x8007000e;
                }
                length = capacity;
                rc = DAT_005129f8.FUN_0044f9c0((char*)g_game + 0x14, buffer, &length);
            }
        }
    }

    {
        field_c = *(int*)((char*)packet + 0x4b5);
        field_10 = *(int*)((char*)packet + 0x4b9);
        if (*(int*)((char*)packet + 0x4b5) == 0) {
            unsigned int n = length;
            if ((int)*size < (int)n) {
                *size = n;
                return (int)0x8877001e;
            }
            memcpy(dest, buffer, n);
            *size = length;
            length = 0;
            return 0;
        }
        if ((unsigned int)length < 4)
            return (int)0x80004005;
        if (length == 4)
            return (int)0x80004005;
        if (*(int*)buffer != -1) {
            entry = FUN_00462d90(field_c);
            if (entry == 0)
                return (int)0x80004005;
            if (entry->field_8 != -1) {
                int cur;
                int prev;
                flag = entry->field_c > 0;
                a = entry->field_8 - 1;
                cur = *(int*)buffer;
                prev = a;
                if (prev >= -1)
                    prev = -2;
                if (prev != cur && prev - cur > -1) {
                    if (entry->field_c <= 0) {
                        if (entry->field_10 < length) {
                            operator delete(entry->field_14);
                            entry->field_14 = (char*)operator new(length);
                            if (entry->field_14 == 0) {
                                FUN_00461170("no memory for allocating saved receive frame\n");
                                entry->field_10 = -1;
                                return (int)0x8007000e;
                            }
                            entry->field_10 = length;
                        }
                        memcpy(entry->field_14, buffer, length);
                        entry->field_4 = field_10;
                        entry->field_0 = field_c;
                        entry->field_c = length;
                        goto fail832;
                    }
                    if (a >= -1)
                        a = -2;
                    if (*(int*)entry->field_14 <= cur) {
                        if (a != cur)
                            FUN_004568b0(field_c, a, Next_00462f30(cur));
                        int t2 = Previous_00462f30(*(int*)buffer);
                        if (t2 != *(int*)entry->field_14)
                            FUN_004568b0(field_c, t2, Next_00462f30(*(int*)entry->field_14));
                        flag = 0;
                        field_14 = (int)entry;
                    } else {
                        if (a != *(int*)entry->field_14)
                            FUN_004568b0(field_c, a, Next_00462f30(*(int*)entry->field_14));
                        int t3 = Previous_00462f30(*(int*)entry->field_14);
                        if (t3 != *(int*)buffer)
                            FUN_004568b0(field_c, t3, Next_00462f30(*(int*)buffer));
                    }
                }
                if (flag && entry->field_c > 0) {
                    spare = buffer;
                    field_230 = length;
                    field_234 = field_c;
                    field_238 = field_10;
                    buffer = entry->field_14;
                    length = entry->field_c;
                    field_c = entry->field_0;
                    field_10 = entry->field_4;
                    field_14 = (int)entry;
                    entry->field_c = 0;
                }
            }
            entry->field_8 = *(int*)buffer;
        }
    }

route_frames:
    if (entry == 0) {
        entry = FUN_00462d90(field_c);
        if (entry == 0)
            goto fail832;
    }
    t = &entry->tail;
    if (((Class_00463730*)t)->FUN_00463790(buffer, length, tick, field_c, field_10,
                     field_14 == 0) == 0) {
        r = t->buffer;
        if (r == 0 || r->n <= 0)
            p = 0;
        else
            p = (int*)((char*)r + (r->head + 1) * 12);
    } else {
        length = 0;
        r = t->buffer;
        if (r == 0 || r->n <= 0)
            p = 0;
        else
            p = (int*)((char*)r + (r->head + 1) * 12);
    }
    src = 0;
    len = 0;
    if (p != 0 && (tick == 0 || (*p - tick) <= 0 || (*p - tick) > 0x1e)) {
        RingEntry_00462f30* ee = 0;
        if (r->n > 0) {
            int h;
            r->n--;
            h = r->head + 1;
            r->head = h;
            if (h >= 0x200)
                r->head = 0;
            ee = (RingEntry_00462f30*)((char*)r + h * 12);
        }
        len = ee->size;
        src = ee->data;
    }
    if (src != 0) {
        *(int*)((char*)packet + 0x4b5) = entry->tail.field_14;
        *(int*)((char*)packet + 0x4b9) = entry->tail.field_18;
        goto copy_out;
    }

fail832:
    length = 0;
    return (int)0x887700be;

copy_out:
    memcpy(dest, src, len);
    *size = len;
    return 0;
}
