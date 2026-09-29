// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash. Names are provisional.
//
// SKELETON for FUN_004a81e0 (0x4a81e0, 5248 bytes, "menu redraw / relayout").
//
// Purpose (from the disassembly): given a menu/pane object and a flags word,
// this walks the menu's entry list (entry 0 holds the count at +0xb6), lays
// out entry 0 against the screen, rebuilds each entry's GAF frame or text
// depending on its type, draws the entries, and finally tears the temporary
// entries back down. It is the big sibling of FUN_004a7f70 (one button) and
// FUN_004a5f40 (one entry draw). FUN_004a8150 appends an entry.
//
// SIGNATURE. ret 8, called from 62 places; every caller declares
//   void __stdcall FUN_004a81e0(Menu*, int)
// and passes &g_game->menu / &g_game->sub / g_game+0x519 as arg1 and 0x40,
// 0xc0 etc. as arg2. It is __stdcall, not __thiscall: the entry reads the
// first argument from [esp+4], never from ecx.
//
// FRAME. Original: sub esp, 0x3c0 (240 dwords). Ours: sub esp, 0x3c0.
// Pushed registers, in order: ebx, ebp, esi, edi. Both match.
// The frame is 0x10 + 0x20 + 0x20 + 0x100 + 0x100 + 0x100 + 0x70 = 0x3c0:
//   0x010  Scal_004a81e0 L: eight dwords the original keeps in memory, at
//          exactly the original's offsets: p4 0x10, g 0x14, force 0x18,
//          i 0x1c, pfield 0x20, text 0x24, e 0x28, name9b6 0x2c. They are one
//          struct because as eight separate locals MSVC kept five of them in
//          registers and the frame came out 0x18 short. Taking &L.i (a real
//          call argument) keeps the struct memory-resident and reproduces the
//          original's eight slots in the original's order.
//          entries is NOT a field. The original holds it in ebp
//          (mov ebp,[eax+4] at 0x4a8202) and never spills it, so it is an
//          ordinary local: keeping it in the escaping struct made MSVC store
//          it to the stack and reload it before every use. Moving it (and
//          p4/name9b6) out of the struct broke the frame by a dword, because
//          those three then stopped being memory-resident; measuring showed
//          the frame needs eight memory dwords, so p4, name9b6 and g stay in
//          the struct and only entries lives in a register.
//          g is the Glyph* of the SLIDERS case (the original's 0x14 slot was
//          a pointer there); it is a field so it stays in memory like the rest.
//   0x030  stagebuf[0x20]  (the original's name buffer)
//   0x050  textbuf[0x100]  (entry name / SAVE UNDER / GUI SURFACE scratch)
//   0x150  buf1[0x100]     (strncpy from menu+0xab6 at 0x4a8426)
//   0x250  buf2[0x100]     (strcpy + ".FNT" at 0x4a8f0c)
//   0x350  buf3            (0x20-dword text copy at 0x4a8ed8)
// The big buffers land at [esp+0x150] / [esp+0x250] / [esp+0x350], exactly as
// the original. The stack arguments are at [esp+0x3d4] (Menu*) and
// [esp+0x3d8] (flags), also as original.
//
// CALL SET (32 callees; the original makes 88 call sites across them).
// ret N means the callee pops N bytes, so all of these are __stdcall:
//   0x4a05e0 FUN_004a05e0(Menu*, int)             2 args, ret 8   (2)
//   0x4a16f0 FUN_004a16f0(Menu*, int, int)        3 args, ret 0xc (2)
//   0x4a1b40 FUN_004a1b40(Menu*, int)             2 args, ret 8   (1)
//   0x4a3ef0 FUN_004a3ef0(Menu*, int)             2 args, ret 8   (1)
//   0x4a4660 FUN_004a4660(Menu*, int)             2 args, ret 8   (1)
//   0x4a4980 FUN_004a4980(Menu*, int)             2 args, ret 8   (1)
//   0x4a4c90 FUN_004a4c90(Menu*, int, unsigned char) 3 args, ret 0xc (1)
//   0x4a4d70 FUN_004a4d70(Menu*, int)             2 args, ret 8   (1)
//   0x4a56b0 FUN_004a56b0(Menu*, int)             2 args, ret 8   (1)
//   0x4a5e50 FUN_004a5e50(Menu*, int)             2 args, ret 8   (1)
//   0x4a5f40 FUN_004a5f40(Menu*, int)             2 args, ret 8   (1)
//   0x4b0230 FUN_004b0230(Menu*, int, void* bmp)  3 args, ret 0xc (2)
//   0x4b7f30 FUN_004b7f30(void* gaf, int)         2 args, ret 8   (14)
//   0x4b8c60 FUN_004b8c60(char* name)             1 arg,  ret 4   (2)
//   0x4b8d40 FUN_004b8d40(void* gaf, const char*) 2 args, ret 8   (14)
//   0x4baff0 FUN_004baff0(char*, char*, char*)   3 args, ret 0xc (2)
//   0x4bbc40 FUN_004bbc40(char* name)             1 arg,  ret 4   (2)
//   0x4bbe50 FUN_004bbe50(char* name, int* size)  2 args, ret 8   (2)
//   0x4c1420 FUN_004c1420(int id)                 1 arg,  ret 4   (2)
//   0x4c5740 FUN_004c5740(char* key)              1 arg,  ret 4   (1)
//   0x4c69f0 FUN_004c69f0(char*, int, int)        3 args, ret 0xc (2)
//   0x4c6ac0 FUN_004c6ac0(void* obj)              1 arg,  ret 4   (2)
//   0x4c6b70 FUN_004c6b70(void*, void*, int, int) 4 args, ret 0x10 (4)
//   0x4d85a0 FUN_004d85a0(int*)                   1 arg,  plain ret, cdecl (2)
//   0x4b6340 FUN_004b6340()       plain ret, 0 args, cdecl   (3)
//   0x4b6700 FUN_004b6700()       plain ret, 0 args, cdecl   (5) screen width
//   0x4b6710 FUN_004b6710()       plain ret, 0 args, cdecl   (6) screen height
//   0x4c1450 FUN_004c1450()       plain ret, 0 args, cdecl   (1)
//   0x4c1ab0 FUN_004c1ab0()       plain ret, 0 args, cdecl   (1)
//   0x4e42b0 sprintf(char*, const char*, ...) cdecl          (1)
//   0x4e4760 strncpy(char*, const char*, size_t) cdecl       (6)
//   0x4e4b50 strncmp(const char*, const char*, size_t) cdecl (1)
// (strlen/strcpy are inlined as repne scasb / rep movsd, not calls.)
//
// GLOBALS: DAT_0051fba4 is the font root (FontRoot_004a81e0*), used at
// 0x4a925b and 0x4a9268. String literals at 0x502e38 "GAF", 0x509970
// "_gadget", 0x509964 "BackTile", 0x50995c "SLIDERS", 0x509950 "TEXTINPUT",
// 0x509948 "LISTBOX", 0x509940 "Off|On", 0x509934 "stagebuttn1", 0x509908
// "CHECKBOX", 0x5098f8 "stagebuttn%d", 0x5098ec "BUTTONS0", 0x509914
// "SAVE UNDER", 0x50992c ".FNT", 0x509920 "GUI SURFACE".
//
// TWO jump tables. First switch, at 0x4a84eb, is on entry->type (0..13),
// table at 0x4a95f4 with 14 entries, cases emitted in the order below.
// Second switch, at 0x4a917f, is on entry[i].type again (1..13), table at
// 0x4a962c with 13 entries. MSVC emits the case bodies in a different order
// from the source, so the regions below cut the EMITTED order (the case
// comments name the source value).
//
// REGIONS (emitted address ranges; rN names are local to this file):
//   r1  0x4a81e0-0x4a84f2  head: layer check, entry-0 geometry, flags&1 init
//   r2  0x4a84f2-0x4a8663  type 0/11: build name, GAF lookup
//   r3  0x4a8663-0x4a899f  type 4: SLIDERS
//   r4  0x4a899f-0x4a8a16  type 3: TEXTINPUT
//   r5  0x4a8a16-0x4a8aca  type 2: LISTBOX
//   r6  0x4a8aca-0x4a8b4b  type 12: name then GAF frame 0
//   r7  0x4a8b4b-0x4a8ef3  type 1: button frame (inlined FUN_004a7f70 logic)
//   r8  0x4a8ef3-0x4a8fa0  type 7: menu string + ".FNT" -> FUN_004bbe50
//   r9  0x4a8fa0-0x4a8fea  type 8: entry name + ".FNT" -> FUN_004bbe50
//   r10 0x4a8fea-0x4a900c  type 13: FUN_004b6340 timing
//   r11 0x4a900c-0x4a9045  type 5: stage name / FUN_004a05e0
//   r12 0x4a9045-0x4a90d1  loop latch + surface creation + "SAVE UNDER"
//   r13 0x4a90d1-0x4a9135  flags&4/&0x40 surface handling, FUN_004b0230
//   r14 0x4a9135-0x4a9186  second loop head + dispatch
//   r15 0x4a9186-0x4a9199  type 11: FUN_004b0230 draw
//   r16 0x4a9199-0x4a91a5  type 12: FUN_004a5e50
//   r17 0x4a91a5-0x4a91c7  type 1:  FUN_004a5f40
//   r18 0x4a91c7-0x4a92df  type 2:  FUN_004a1b40 text (font + wrap)
//   r19 0x4a92df-0x4a9301  type 3:  FUN_004a4d70
//   r20 0x4a9301-0x4a9345  type 4:  FUN_004a3ef0 (clears a rect)
//   r21 0x4a9345-0x4a9367  type 5:  FUN_004a56b0
//   r22 0x4a9367-0x4a93ab  type 6:  FUN_004a4980
//   r23 0x4a93ab-0x4a93eb  type 13: FUN_004a4660
//   r24 0x4a93eb-0x4a940c  type 10: FUN_004a4c90
//   r25 0x4a940c-0x4a9434  second loop latch
//   r26 0x4a9434-0x4a950a  type-1 retry (FUN_004a16f0) + strncmp
//   r27 0x4a950a-0x4a95c2  flags&2: free surfaces (FUN_004c6b70/6ac0/d85a0)
//   r28 0x4a95c2-0x4a95f4  epilogue, return 1
// (0x4a95f4-0x4a9660 is the two switch tables, not code.)
//
// WHAT IS STUBBED. Every region body is a plausible stub that calls the
// callees of that region with the right argument counts; it is not the
// original logic. The first switch, the second switch, the return values and
// the frame are in place. Regions r12, r13 and r27 are the roughest.
//
// STATUS 30 Sep, deepseek-v4.1-flash. Best 9.4%, no MATCH. The head from
// 0x4a81e0 through 0x4a82ee (end of the entry-0 clamp) is already correct: the
// sequence matcher lines it up instruction for instruction, so only two head
// problems remain, and both are consequences of the missing body:
//   1. Prologue is 8 bytes too long. Original is
//        mov eax,[esp+4] / sub esp,0x3c0 / mov eax,[eax+0x18] / push ebx /
//        push ebp / push esi / test eax,eax / push edi / jne
//      ours is sub esp / push ebx..edi / mov edi,[esp+0x3d4] / xor ebx,ebx /
//      mov eax,[edi+0x18] / cmp eax,ebx / jne. The argument load before the
//      frame and the interleaved `push edi` are the original's parameter
//      rematerialisation: with the full body ebx=flags, ebp=entries, esi, edi
//      are all live, so `menu` is never kept in a register and is reloaded
//      from [esp+0x3d4] at every use (0x4a8308, 0x4a837a, 0x4a83f0, 0x4a8502,
//      ...). Our skeleton caches menu in edi because its body is too small to
//      create that pressure. Do not chase this until the body exists.
//   2. The 8-byte shift moves every branch target, which is why the raw byte
//      comparison (bytes_match) fails even where the instruction text agrees.
// Remaining diff hunks by address (original addresses; all are the stubbed
// case bodies, largest first):
//   0x4a8b4b-0x4a8ef3  r7  type 1 (button frame, inlined FUN_004a7f70) ~950 B
//   0x4a8663-0x4a899f  r3  type 4 (SLIDERS)                            ~830 B
//   0x4a8b4b ..        r6/r8/r9/r10/r11 tails                         ~400 B
//   0x4a84f2-0x4a8663  r2  type 0/11 (name + GAF)                      ~370 B
//   0x4a9045-0x4a9135  r12/r13 surface creation + flags                ~240 B
//   0x4a9434-0x4a95c2  r26/r27 retry + frees                           ~400 B
//   0x4a899f-0x4a8b4b  r4/r5 TEXTINPUT + LISTBOX                      ~430 B
// The biggest missing instruction runs are r7 and r3; a next pass should
// transcribe those two from the disassembly before touching anything else.
// Verified this run: changing the head to a `layer` local left the frame and
// the 9.4% unchanged (scheduler/register pressure, not source shape).
#include <string.h>
#include <stdio.h>

// SHARED begin
#pragma pack(push, 1)

struct GafEntry_004a81e0 {              // 4 bytes: frame table header
    unsigned short count;               // +0x00
    unsigned short unknown_2;           // +0x02
};

struct Glyph_004a81e0 {                 // 8 bytes, returned by FUN_004b7f30
    short w;                            // +0x00
    short h;                            // +0x02
    short xoff;                         // +0x04
    short yoff;                         // +0x06
};

struct Entry_004a81e0 {                 // 0x15b bytes, one GUI list entry
    unsigned char type;                 // +0x00
    char unknown_01[1];                 // +0x01
    char name[0x11];                    // +0x02 (strncpy 0x10)
    short x;                            // +0x13
    short y;                            // +0x15
    short w;                            // +0x17
    short h;                            // +0x19
    int flags;                          // +0x1b
    unsigned char* colours;             // +0x1f
    char unknown_23[0x29 - 0x23];
    unsigned char field_29;             // +0x29 (second loop keep-test)
    char unknown_2a[0x2f - 0x2a];
    GafEntry_004a81e0* gaf;             // +0x2f
    char unknown_33[0xb6 - 0x33];
    union {
        short count;                    // +0xb6 (entry 0 only)
        char text[0x20];                // +0xb6
    } u;
    char unknown_d6[0x136 - 0xd6];      // +0xd6 language, +0xb8..+0xca surface
    short field_136;                    // +0x136
    short field_138;                    // +0x138
    unsigned char field_13a;            // +0x13a
    unsigned char field_13b;            // +0x13b
    unsigned char field_13c;            // +0x13c
    char unknown_13d[0x15b - 0x13d];
};

struct Layer_004a81e0 {
    char unknown_00[4];
    Entry_004a81e0* entries;            // +0x04
    char unknown_08[0x14 - 0x08];
    int field_14;                       // +0x14
};

struct Menu_004a81e0 {                  // the object callers pass as arg1
    char unknown_00[4];
    void* gaf;                          // +0x04
    char unknown_08[0x18 - 0x08];
    Layer_004a81e0* layer;              // +0x18
    char unknown_1c[0x70 - 0x1c];
    int field_70;                       // +0x70
    char unknown_74[0x9b6 - 0x74];
    char str_9b6[0x100];                // +0x9b6
    char str_ab6[0x100];                // +0xab6
    char str_bb6[0x100];                // +0xbb6
};

struct Language_004a81e0 {
    char unknown_00[0xc];
    void* glyphs;                       // +0x0c
};

struct FontRoot_004a81e0 {
    int current;                        // +0x00
    char unknown_04[0x14 - 0x04];
    Language_004a81e0* language;        // +0x14
};

#pragma pack(pop)

extern FontRoot_004a81e0* DAT_0051fba4;

int FUN_004b6700(void);
int FUN_004b6710(void);
int FUN_004c1ab0(void);
unsigned int FUN_004b6340(void);
int FUN_004c1450(void);

void __stdcall FUN_004a05e0(void* obj, int index);
void __stdcall FUN_004a16f0(void* obj, int index, int param3);
void __stdcall FUN_004a1b40(void* obj, int index);
void __stdcall FUN_004a3ef0(void* obj, int index);
void __stdcall FUN_004a4660(void* obj, int index);
void __stdcall FUN_004a4980(void* obj, int index);
void __stdcall FUN_004a4c90(void* obj, int index, unsigned char param3);
void __stdcall FUN_004a4d70(void* obj, int index);
void __stdcall FUN_004a56b0(void* obj, int index);
void __stdcall FUN_004a5e50(void* obj, int index);
void __stdcall FUN_004a5f40(void* obj, int index);
void __stdcall FUN_004b0230(void* obj, int index, void* bmp);
void* __stdcall FUN_004b7f30(void* gaf, int index);
void* __stdcall FUN_004b8c60(char* name);
void* __stdcall FUN_004b8d40(void* gaf, const char* name);
char* __stdcall FUN_004baff0(char* a, char* b, char* c);
long __stdcall FUN_004bbc40(char* name);
char* __stdcall FUN_004bbe50(char* name, int* size);
void __stdcall FUN_004c1420(int id);
char* __stdcall FUN_004c5740(char* key);
void* __stdcall FUN_004c69f0(char* name, int width, int height);
void __stdcall FUN_004c6ac0(void* obj);
void __stdcall FUN_004c6b70(void* dst, void* bmp, int x, int y);
void FUN_004d85a0(int* param_1);

// The memory-resident scalars. &L.i escapes to FUN_004bbe50, which keeps the
// whole struct in memory. entries is deliberately not a field; it lives in
// ebp. See the frame note in the header.
struct Scal_004a81e0 {
    void* p4;                          // +0x10
    Glyph_004a81e0* g;                 // +0x14
    int force;                         // +0x18
    int i;                             // +0x1c
    short* pfield;                     // +0x20
    char* text;                        // +0x24
    Entry_004a81e0* e;                 // +0x28
    char* name9b6;                     // +0x2c
};
// SHARED end

// FUNCTION: 0x4a81e0
int __stdcall FUN_004a81e0(Menu_004a81e0* menu, unsigned int flags)
{
    Entry_004a81e0* entries;
    Scal_004a81e0 L;
    char stagebuf[0x20];
    char textbuf[0x100];
    char buf1[0x100];
    char buf2[0x100];
    char buf3[0x80];

    // REGION r1 begin   0x4a81e0-0x4a84f2
    if (!menu->layer)
        return 0;
    entries = menu->layer->entries;
    if ((flags & 0x100) && (flags & 1)) {
        entries[0].y = -1;
        entries[0].x = -1;
    }
    if ((flags & 0x1000) && (flags & 1)) {
        entries[0].y = -2;
        entries[0].x = -2;
    }
    if (entries[0].x == -1) {
        entries[0].x = (short)((FUN_004b6700() - entries[0].w) / 2);
        entries[0].y = (short)((FUN_004b6710() - entries[0].h) / 2);
    }
    if (entries[0].x == -2) {
        entries[0].x = (short)(((FUN_004b6700() - 0x80 - entries[0].w) / 2) + 0x80);
        entries[0].y = (short)((FUN_004b6710() - entries[0].h) / 2);
    }
    if (entries[0].x + entries[0].w > FUN_004b6700())
        entries[0].x = (short)((FUN_004b6700() - entries[0].w) / 2);
    if (entries[0].y + entries[0].h > FUN_004b6710())
        entries[0].y = (short)((FUN_004b6710() - entries[0].h) / 2);

    L.force = flags & 1;
    if (L.force == 0)
        goto after_entries;

    while (FUN_004c1ab0() != 0)
        ;
    if (menu->field_70 != 0) {
        for (L.i = 0; L.i <= entries[0].u.count; L.i++)
            if (entries[L.i].type == 1)
                entries[L.i].field_13a = 0;
    }
    if (entries[0].w > FUN_004b6700())
        return 0;
    if (entries[0].h > FUN_004b6710())
        return 0;
    // REGION r1 end

    for (L.i = 0; L.i <= entries[0].u.count; L.i++) {
        L.e = &entries[L.i];
        switch (L.e->type) {
        case 0:
        case 11:
            // REGION r2 begin   0x4a84f2-0x4a8663
            strncpy(buf1, menu->str_ab6, 0x100);
            FUN_004baff0(buf1, textbuf, L.name9b6);
            if (L.p4 != 0) {
                if (FUN_004bbc40(buf1)) {
                    L.e->gaf = (GafEntry_004a81e0*)FUN_004b8c60(buf1);
                    if (L.e->gaf != 0)
                        FUN_004b8d40(L.e->gaf, L.e->name);
                }
            }
            // REGION r2 end
            break;

        case 4:
            // REGION r3 begin   0x4a8663-0x4a899f
            if (L.p4 != 0) {
                L.e->gaf = (GafEntry_004a81e0*)FUN_004b8d40(L.p4, "SLIDERS");
                if (L.e->gaf != 0) {
                    for (L.i = 0; (unsigned)L.i < L.e->gaf->count; L.i++) {
                        L.g = (Glyph_004a81e0*)FUN_004b7f30(L.e->gaf, L.i);
                        if (L.g != 0) {
                            L.g->h = 0;
                            L.g->w = 0;
                        }
                    }
                }
            }
            // REGION r3 end
            break;

        case 3:
            // REGION r4 begin   0x4a899f-0x4a8a16
            if (L.p4 != 0)
                L.e->gaf = (GafEntry_004a81e0*)FUN_004b8d40(L.p4, "TEXTINPUT");
            L.e->field_136 = 0x7f;
            // REGION r4 end
            break;

        case 2:
            // REGION r5 begin   0x4a8a16-0x4a8aca
            if (L.p4 != 0)
                L.e->gaf = (GafEntry_004a81e0*)FUN_004b8d40(L.p4, "LISTBOX");
            // REGION r5 end
            break;

        case 12:
            // REGION r6 begin   0x4a8aca-0x4a8b4b
            strncpy(textbuf, L.e->name, 0x10);
            textbuf[0x10] = 0;
            L.e->field_13b = 0;
            L.e->gaf = (GafEntry_004a81e0*)FUN_004b8d40(L.p4, textbuf);
            // REGION r6 end
            break;

        case 1:
            // REGION r7 begin   0x4a8b4b-0x4a8ef3
            FUN_004a05e0(menu, L.i);
            strncpy(textbuf, L.e->name, 0x10);
            textbuf[0x10] = 0;
            sprintf(stagebuf, "stagebuttn%d", 1);
            L.e->gaf = (GafEntry_004a81e0*)FUN_004b8d40(L.p4, textbuf);
            if (L.e->gaf != 0)
                FUN_004b7f30(L.e->gaf, 0);
            L.text = FUN_004c5740(L.e->u.text);
            FUN_004bbe50(buf2, &L.i);
            // REGION r7 end
            break;

        case 7:
            // REGION r8 begin   0x4a8ef3-0x4a8fa0
            strncpy(buf2, menu->str_bb6, 0x100);
            FUN_004bbe50(buf2, &L.i);
            // REGION r8 end
            break;

        case 8:
            // REGION r9 begin   0x4a8fa0-0x4a8fea
            strncpy(buf2, L.e->name, 0x10);
            FUN_004bbe50(buf2, &L.i);
            // REGION r9 end
            break;

        case 13:
            // REGION r10 begin   0x4a8fea-0x4a900c
            L.e->field_138 = (short)FUN_004b6340();
            // REGION r10 end
            break;

        case 5:
            // REGION r11 begin   0x4a900c-0x4a9045
            if (L.e->field_136 != 0)
                FUN_004a05e0(menu, L.i);
            // REGION r11 end
            break;

        default:
            break;
        }
    }

    // REGION r12 begin   0x4a9045-0x4a90d1
    L.e = &entries[0];
    L.p4 = FUN_004c69f0(L.e->name, L.e->w, L.e->h);
    FUN_004c6b70(L.p4, 0, -L.e->x, -L.e->y);
    L.text = L.e->name;
    if (L.text == 0)
        L.text = "GUI SURFACE";
    L.p4 = FUN_004c69f0("SAVE UNDER", L.e->w, L.e->h);
    FUN_004c6b70(L.p4, 0, 0, 0);
    // REGION r12 end

after_entries:
    // REGION r13 begin   0x4a90d1-0x4a9135
    L.e = &entries[0];
    if ((flags & 4) || L.force || (flags & 0x40)) {
        if (L.e->u.text[0] != 0) {
            FUN_004b0230(menu, 0, 0);
        } else if ((flags & 0x80) == 0) {
            FUN_004b0230(menu, 0, 0);
        }
    }
    // REGION r13 end

    // REGION r14 begin   0x4a9135-0x4a9186
    for (L.i = 1; L.i <= entries[0].u.count; L.i++) {
        L.e = &entries[L.i];
        if (L.e->field_29 == 0)
            continue;
        switch (L.e->type) {
        case 11:
            // REGION r15 begin   0x4a9186-0x4a9199
            FUN_004b0230(menu, L.i, 0);
            // REGION r15 end
            break;
        case 12:
            // REGION r16 begin   0x4a9199-0x4a91a5
            FUN_004a5e50(menu, L.i);
            // REGION r16 end
            break;
        case 1:
            // REGION r17 begin   0x4a91a5-0x4a91c7
            if (L.force || (flags & 0x48) != 0)
                FUN_004a5f40(menu, L.i);
            // REGION r17 end
            break;
        case 2:
            // REGION r18 begin   0x4a91c7-0x4a92df
            L.e->field_136 = 0;
            if (DAT_0051fba4->language == 0)
                FUN_004c1450();
            else
                FUN_004b7f30(DAT_0051fba4->language->glyphs, 0x49);
            FUN_004c1420(L.i);
            FUN_004a1b40(menu, L.i);
            // REGION r18 end
            break;
        case 3:
            // REGION r19 begin   0x4a92df-0x4a9301
            FUN_004a4d70(menu, L.i);
            // REGION r19 end
            break;
        case 4:
            // REGION r20 begin   0x4a9301-0x4a9345
            L.e->field_136 = 0;
            FUN_004a3ef0(menu, L.i);
            // REGION r20 end
            break;
        case 5:
            // REGION r21 begin   0x4a9345-0x4a9367
            FUN_004a56b0(menu, L.i);
            // REGION r21 end
            break;
        case 6:
            // REGION r22 begin   0x4a9367-0x4a93ab
            if ((L.force == 0) && (flags & 0x40) == 0)
                break;
            FUN_004a4980(menu, L.i);
            // REGION r22 end
            break;
        case 13:
            // REGION r23 begin   0x4a93ab-0x4a93eb
            if ((L.force == 0) && (flags & 0x40) == 0)
                break;
            FUN_004a4660(menu, L.i);
            // REGION r23 end
            break;
        case 10:
            // REGION r24 begin   0x4a93eb-0x4a940c
            if ((L.force == 0) && (flags & 0x40) == 0)
                break;
            FUN_004a4c90(menu, L.i, 0);
            // REGION r24 end
            break;
        default:
            break;
        }
        // REGION r25 begin   0x4a940c-0x4a9434
        // REGION r25 end
    }
    FUN_004c1420(DAT_0051fba4->current);
    // REGION r14 end

    // REGION r26 begin   0x4a9434-0x4a950a
    FUN_004a16f0(menu, 0, 8);
    if (L.i == entries[0].u.count + 1)
        FUN_004c1420(DAT_0051fba4->current);
    if (strncmp(buf1, buf3, 0x10) != 0)
        FUN_004a16f0(menu, 1, 8);
    // REGION r26 end

    // REGION r27 begin   0x4a950a-0x4a95c2
    if (flags & 2) {
        if (entries[0].u.text[0] != 0) {
            FUN_004c6b70(0, 0, 0, 0);
            FUN_004c6ac0(0);
        }
        FUN_004c6ac0(0);
        for (L.i = 0; L.i <= entries[0].u.count; L.i++) {
            L.e = &entries[L.i];
            if (L.e->gaf != 0)
                FUN_004d85a0((int*)L.e->gaf);
        }
    }
    // REGION r27 end

    // REGION r28 begin   0x4a95c2-0x4a95f4
    L.pfield = &entries[0].x;
    (void)L.pfield;
    return 1;
    // REGION r28 end
}
