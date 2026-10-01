// Decompiled by space-bunny-free, finished by deepseek-v4.1-flash, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Claude Sonnet 5.5 (#3273): 88.5% (was 85.9%), still 915 bytes. Found by an
// automated hill-climb over source mutations scored with check.py --sym (about
// 3100 variants): the gain comes from reordering independent stores (the
// registers handed to the constants 1, 6 and 2 follow where their first uses
// sit) plus `return 0 != field_1749`. The stores are to distinct globals so
// the order is only a source choice; the three register hunks listed below are
// reduced but not gone, and the same search stalled at 88.5% for 2500 variants.
// #3078 retry by GPT-6.1-sol: one check reconfirmed 85.9%; the three constant
// register assignments remain the only recorded differences.
// deepseek-v4.1-flash (#3429): moved `DAT_00512b80 = 2;` from the very end up
// into the store run, between `DAT_00512bcc = 7;` and `DAT_00512ac8 =
// FUN_0044fd40;`, which is exactly where the original emits that store (the
// stray trailing store hunk is gone, four hunks remain instead of five). The
// score is byte-flat at 88.5% / 915 bytes: only the 1/6/2 register hunks are
// left, and no tried source shape moves them.
// Partial: 85.9%, 915 bytes, exactly the original's size. The store sequence,
// every immediate and the whole prologue and epilogue match; what is left is
// only the register the allocator hands to three of the hoisted constants.
//
// The block is zeroed by a memset written AFTER nine of the fourteen stores of
// 4, not before them. MSVC 5 then hoists the rep stosd back to the top of the
// block anyway, but the constant 4 is live across it, so 4 keeps EDX. Written
// before the stores, the allocator picks the eight-use constant 1 for EDX and
// pushes 4 into EAX (76.3%, 901 bytes). Every memset position from 1 to 13 of
// the fourteen stores gives the same 85.9%.
//
// Remaining diff hunks, by original address:
//   0x452040  mov esi, 1  ->  mov edx, 1
//   0x45204f  mov edi, 6  ->  mov esi, 6
//   0x452059  mov edx, 2  ->  mov edi, 2
// and then every store of 1, 6 or 2 uses the register above. 4 keeps EDX, 7
// keeps ECX, FUN_0044fd40 keeps EAX and 3 keeps EBP in both. The original
// gives 1 ESI, 6 EDI and 2 EDX (reusing 4's dead slot last); ours gives 1 EDX
// (reusing 4's slot immediately), 6 ESI and 2 EDI. Tried and flat at 85.9%:
// memset through a char* or void*, length as 176, sizeof(int) * 44, 44 * 4, a
// zeroing for loop, the stores through a static set4() helper, long / unsigned
// / int globals, and explicit (int) casts.
//
// deepseek-v4.1 also tried and left flat at 85.9%: #include <windows.h>;
// a dead `if (0) { DAT_00512b34 = 2; }` after the memset to create the value 2
// earlier; `const int c1 = 1, c6 = 6, c2 = 2;` declared before the 4-block and
// used for the first store of each of 1, 6 and 2; 1u / 6u / 2u spellings of
// those stores; and vD_locals (see build/scratch/0x451fd0/). Swapping two
// adjacent independent stores (vA_swap) falls to 85.3%, which shows the
// scheduler does not reorder this store run, so the emitted order really is
// the source order and the original's source has the same interleaving. The
// only remaining freedom is the allocator's register choice, which no source
// shape tried so far moves.
//
// deepseek-v4.1-flash retry, all flat at 85.9% with the byte-identical diff:
// making 4 and 2 one reassigned variable `int v` (int v = 4; ... v = 2; and
// every 4/2 store through v), which should have kept EDX continuously live
// across the birth of 1 and so forced 1 into ESI; the same per constant
// (one/six/two declared at first use and reused for every store); declaring
// one, six and two at the very top of the function; and declaring two just
// before the memset so it is live across the rep stosd. MSVC 5 splits these
// pseudo-registers back into per-store constants, so all of them compile to
// the same 915 bytes. The fix is in the allocator, not the source shape.
//
// deepseek-v4.1-flash second retry, all flat at 85.9% with the same three
// register hunks: every header set (tools/headers.py, 128 sets); the
// N-declarations test (N unused `extern int dummyN;` for N = 0..400, all
// 85.9%, so the difference is not compiler symbol-table state); routing the
// 1/6/2 stores through `(unsigned char)`/`(short)` casts and through a
// `static inline int id(int)` (MSVC folds both back); the memset value through
// an `int zero` local; and declaring the 1 and 2 globals `unsigned`. Merging
// 4 and 2 into one reassigned `int v` drops to 85.3% / 914 bytes (MSVC keeps
// the variable in EDX but emits a different tail), so it is not the shape
// either. In the original the free list gives 4 EDX, then 7 ECX, then 1 ESI,
// 6 EDI, 2 EDX; ours reuses 4's just-freed EDX for 1 (so 6 ESI, 2 EDI). That
// is a one-slot rotation of the same three registers, and nothing that keeps
// all 915 bytes fixed moves it.

#include <string.h>

typedef int (__stdcall *EntryFunc)(int);

int __stdcall FUN_0044fd40(int arg1);
int __stdcall FUN_0044fd50(int arg1);
int __stdcall FUN_0044fd60(int arg1);
int __stdcall FUN_0044fd70(int arg1);
int __stdcall FUN_0044fd80(int arg1);
int __stdcall FUN_0044fd90(int arg1);

unsigned int __cdecl FUN_004b6340(void);
int __cdecl FUN_004d83b0(const char* name, int size);

extern EntryFunc DAT_00512a28;
extern EntryFunc DAT_00512a2c;
extern EntryFunc DAT_00512a34;
extern EntryFunc DAT_00512a38;
extern EntryFunc DAT_00512a3c;
extern EntryFunc DAT_00512a40;
extern EntryFunc DAT_00512a44;
extern EntryFunc DAT_00512a48;
extern EntryFunc DAT_00512a4c;
extern EntryFunc DAT_00512a50;
extern EntryFunc DAT_00512a54;
extern EntryFunc DAT_00512a58;
extern EntryFunc DAT_00512a5c;
extern EntryFunc DAT_00512a60;
extern EntryFunc DAT_00512a64;
extern EntryFunc DAT_00512a68;
extern EntryFunc DAT_00512a6c;
extern EntryFunc DAT_00512a70;
extern EntryFunc DAT_00512a74;
extern EntryFunc DAT_00512a78;
extern EntryFunc DAT_00512a7c;
extern EntryFunc DAT_00512a80;
extern EntryFunc DAT_00512a84;
extern EntryFunc DAT_00512a88;
extern EntryFunc DAT_00512a8c;
extern EntryFunc DAT_00512a90;
extern EntryFunc DAT_00512a94;
extern EntryFunc DAT_00512a98;
extern EntryFunc DAT_00512a9c;
extern EntryFunc DAT_00512aa0;
extern EntryFunc DAT_00512aa4;
extern EntryFunc DAT_00512aa8;
extern EntryFunc DAT_00512aac;
extern EntryFunc DAT_00512ab0;
extern EntryFunc DAT_00512ab4;
extern EntryFunc DAT_00512ab8;
extern EntryFunc DAT_00512abc;
extern EntryFunc DAT_00512ac0;
extern EntryFunc DAT_00512ac4;
extern EntryFunc DAT_00512ac8;
extern EntryFunc DAT_00512ad0;
extern int DAT_00512adc;
extern int DAT_00512ae0;
extern int DAT_00512ae4;
extern int DAT_00512aec;
extern int DAT_00512af0;
extern int DAT_00512af4;
extern int DAT_00512af8;
extern int DAT_00512afc;
extern int DAT_00512b00;
extern int DAT_00512b04;
extern int DAT_00512b08;
extern int DAT_00512b0c;
extern int DAT_00512b10;
extern int DAT_00512b14;
extern int DAT_00512b18;
extern int DAT_00512b1c;
extern int DAT_00512b20;
extern int DAT_00512b24;
extern int DAT_00512b28;
extern int DAT_00512b2c;
extern int DAT_00512b30;
extern int DAT_00512b34;
extern int DAT_00512b38;
extern int DAT_00512b3c;
extern int DAT_00512b40;
extern int DAT_00512b44;
extern int DAT_00512b48;
extern int DAT_00512b4c;
extern int DAT_00512b50;
extern int DAT_00512b54;
extern int DAT_00512b58;
extern int DAT_00512b5c;
extern int DAT_00512b60;
extern int DAT_00512b64;
extern int DAT_00512b68;
extern int DAT_00512b6c;
extern int DAT_00512b70;
extern int DAT_00512b74;
extern int DAT_00512b78;
extern int DAT_00512b7c;
extern int DAT_00512b80;
extern int DAT_00512b88;
extern int DAT_00512bc8;
extern int DAT_00512bcc;
extern int DAT_00512bd4;
extern int DAT_00512bd8;
extern int DAT_00512bdc;
extern int DAT_00512be0;
extern int DAT_00512be4;
extern int DAT_00512be8;
extern int DAT_00512bec;
extern int DAT_00512bf0;
extern int DAT_00512bf4;
extern int DAT_00512bf8;
extern int DAT_00512bfc;
extern int DAT_00512c00;
extern int DAT_00512c04;
extern int DAT_00512c08;
extern int DAT_00512c0c;
extern int DAT_00512c10;
extern int DAT_00512c14;
extern int DAT_00512c18;
extern int DAT_00512c1c;
extern int DAT_00512c20;
extern int DAT_00512c24;
extern int DAT_00512c28;
extern int DAT_00512c2c;
extern int DAT_00512c30;
extern int DAT_00512c38;
extern int DAT_00512c3c;
extern int DAT_00512c40;
extern int DAT_00512c44;
extern int DAT_00512c48;
extern int DAT_00512c4c;
extern int DAT_00512c50;
extern int DAT_00512c54;
extern int DAT_00512c58;
extern int DAT_00512c5c;
extern int DAT_00512c60;
extern int DAT_00512c64;
extern int DAT_00512c68;
extern int DAT_00512c70;

extern char s_PACKET_DATA_00506524[];

#pragma pack(push, 1)

class Class_00451fd0 {
public:
    char unknown_0[0x870];
    unsigned int field_870;      // +0x870
    char unknown_874[0x1745 - 0x874];
    int field_1745;              // +0x1745
    int* field_1749;             // +0x1749
};

#pragma pack(pop)

int __stdcall FUN_00451fd0(Class_00451fd0* param_1);

// FUNCTION: 0x451fd0
int __stdcall FUN_00451fd0(Class_00451fd0* param_1)
{
    DAT_00512be8 = 4;
    DAT_00512be4 = 4;
    DAT_00512bec = 4;
    DAT_00512c70 = 4;
    DAT_00512bf0 = 4;
    DAT_00512bf4 = 4;
    DAT_00512bf8 = 4;
    DAT_00512bfc = 4;
    DAT_00512c00 = 4;
    memset(&DAT_00512adc, 0, 0xb0);
    DAT_00512b1c = 4;
    DAT_00512c04 = 4;
    DAT_00512c10 = 4;
    DAT_00512ae0 = 13;
    DAT_00512c08 = 4;
    DAT_00512c18 = 4;
    DAT_00512a28 = FUN_0044fd50;
    DAT_00512bc8 = 7;
    DAT_00512aec = 65;
    DAT_00512a34 = FUN_0044fd60;
    DAT_00512af0 = 1;
    DAT_00512bd4 = 7;
    DAT_00512a38 = FUN_0044fd70;
    DAT_00512bd8 = 7;
    DAT_00512a3c = FUN_0044fd80;
    DAT_00512af4 = 1;
    DAT_00512af8 = 1;
    DAT_00512bdc = 7;
    DAT_00512a40 = FUN_0044fd90;
    DAT_00512be0 = 7;
    DAT_00512afc = 23;
    DAT_00512a44 = FUN_0044fd40;
    DAT_00512b88 = 3;
    DAT_00512ad0 = FUN_0044fd40;
    DAT_00512b00 = 7;
    DAT_00512a48 = FUN_0044fd40;
    DAT_00512b04 = 9;
    DAT_00512a4c = FUN_0044fd40;
    DAT_00512b08 = 11;
    DAT_00512a50 = FUN_0044fd40;
    DAT_00512b0c = 36;
    DAT_00512a54 = FUN_0044fd40;
    DAT_00512b10 = 14;
    DAT_00512a58 = FUN_0044fd40;
    DAT_00512a5c = FUN_0044fd40;
    DAT_00512b18 = 22;
    DAT_00512a64 = FUN_0044fd40;
    DAT_00512b14 = 6;
    DAT_00512a60 = FUN_0044fd40;
    DAT_00512b20 = 5;
    DAT_00512a68 = FUN_0044fd40;
    DAT_00512b24 = 18;
    DAT_00512a6c = FUN_0044fd40;
    DAT_00512c0c = 7;
    DAT_00512b28 = 24;
    DAT_00512b2c = 1;
    DAT_00512a74 = FUN_0044fd40;
    DAT_00512c14 = 6;
    DAT_00512a70 = FUN_0044fd40;
    DAT_00512b34 = 2;
    DAT_00512b30 = 17;
    DAT_00512a7c = FUN_0044fd40;
    DAT_00512b38 = 2;
    DAT_00512a78 = FUN_0044fd40;
    DAT_00512c1c = 7;
    DAT_00512a80 = FUN_0044fd40;
    DAT_00512c20 = 7;
    DAT_00512b3c = 3;
    DAT_00512a84 = FUN_0044fd40;
    DAT_00512c24 = 7;
    DAT_00512a8c = FUN_0044fd40;
    DAT_00512c2c = 7;
    DAT_00512b48 = 5;
    DAT_00512a90 = FUN_0044fd40;
    DAT_00512b44 = 6;
    DAT_00512c30 = 7;
    DAT_00512b40 = 14;
    DAT_00512c28 = 1;
    DAT_00512a88 = FUN_0044fd40;
    DAT_00512b4c = 9;
    DAT_00512a94 = FUN_0044fd40;
    DAT_00512b6c = 5;
    DAT_00512c54 = 1;
    DAT_00512ab4 = FUN_0044fd40;
    DAT_00512b70 = 41;
    DAT_00512ab8 = FUN_0044fd40;
    DAT_00512c58 = 7;
    DAT_00512b64 = 14;
    DAT_00512aac = FUN_0044fd40;
    DAT_00512c4c = 7;
    DAT_00512b68 = 6;
    DAT_00512ab0 = FUN_0044fd40;
    DAT_00512c50 = 7;
    DAT_00512a98 = FUN_0044fd40;
    DAT_00512b50 = 2;
    DAT_00512b54 = 5;
    DAT_00512c38 = 6;
    DAT_00512a9c = FUN_0044fd40;
    DAT_00512b58 = 186;
    DAT_00512c3c = 6;
    DAT_00512aa0 = FUN_0044fd40;
    DAT_00512c40 = 7;
    DAT_00512b5c = 10;
    DAT_00512aa4 = FUN_0044fd40;
    DAT_00512b60 = 6;
    DAT_00512aa8 = FUN_0044fd40;
    DAT_00512c44 = 1;
    DAT_00512c48 = 1;
    DAT_00512b74 = 17;
    DAT_00512abc = FUN_0044fd40;
    DAT_00512c5c = 7;
    DAT_00512b78 = 58;
    DAT_00512ac0 = FUN_0044fd40;
    DAT_00512c60 = 7;
    DAT_00512b7c = 3;
    DAT_00512ac4 = FUN_0044fd40;
    DAT_00512c64 = 7;
    DAT_00512ae4 = 3;
    DAT_00512a2c = FUN_0044fd40;
    DAT_00512bcc = 7;
    DAT_00512b80 = 2;
    DAT_00512ac8 = FUN_0044fd40;
    DAT_00512c68 = 7;
    param_1->field_870 = FUN_004b6340();
    param_1->field_1745 = 0x2000;
    param_1->field_1749 = (int*)FUN_004d83b0(s_PACKET_DATA_00506524, 0x2000);
    return 0 != param_1->field_1749;
}