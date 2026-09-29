// Decompiled by space-bunny-free. Names are provisional.
// Retry #1342 worker pass: kept the 76.3% baseline. Register-handoff scratch variants scored 63.5% and 76.3%, with no improvement.
//
// Gave up at 76.3%. What is left is ONE thing: which register the constant 4
// is given. The store sequence, the store order, the 25 immediate stores and
// the whole epilogue are byte exact.
//
// Ours is 901 bytes, the original 915, and the whole 14 byte difference is the
// 14 stores of the constant 4: the original keeps 4 in edx and stores it with
// the 89 15 <abs> modrm form (6 bytes), we keep it in eax and store it with the
// a3 <abs> moffs form (5 bytes). So getting 4 out of eax fixes the size too.
//
// The register hand-out order (which follows the allocation order, not a fixed
// preference list) is:
//   original: 4=edx 7=ecx 1=esi FUN_0044fd40=eax 3=ebp 6=edi 5=ebx 2=edx
//   ours:     1=edx 4=eax 7=ecx FUN_0044fd40=eax 3=ebp 6=esi 5=ebx 2=edi
// Positions 4 to 8 (the pointer, 3, 6, 5, 2) already agree, so the whole
// mismatch is a 3 cycle among the first three constants. The original's order
// is exactly the source order of first use, and the first value allocated also
// gets the def hoisted above the memset's rep stosd (in the original that is
// 4, here it is 1). Ours promotes the constant 1 to the head of the live range
// list even though it is used 8 times against 4's 14 and 7's 20, so the list is
// not ordered by use count, by live range length, or by the front end's symbol
// order in any way I could find. Fixing this one 3 cycle fixes the function.
//
// Tried, all still 76.3% and all free to score with check.py --sym:
//   - named locals for the constants (`int one = 1;`, `int four = 4;`) and
//     using them in the stores: no change, the constants stay temps;
//   - static __inline int get1()/get4() accessors in place of the literals, to
//     see the value through a function boundary (an inlined boundary is not a
//     CSE boundary, so this was worth a try): no change;
//   - `= 1 + 0`, `= 0 + 1`, `= 4 + 0`, `= 0 + 4` to change the reference
//     count of a temp without changing the emitted store: no change;
//   - the memset target through a local pointer, and a (void*) cast on it: no
//     change;
//   - putting the memset after the 14 stores (MSVC does not hoist it, so the
//     memset has to be the first statement), return type int rather than bool,
//     #pragma pack(1) for the unaligned +0x1745 fields: no change.

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
    memset(&DAT_00512adc, 0, 0xb0);
    DAT_00512be4 = 4;
    DAT_00512c70 = 4;
    DAT_00512be8 = 4;
    DAT_00512bec = 4;
    DAT_00512bf0 = 4;
    DAT_00512bf4 = 4;
    DAT_00512bf8 = 4;
    DAT_00512bfc = 4;
    DAT_00512c00 = 4;
    DAT_00512b1c = 4;
    DAT_00512c04 = 4;
    DAT_00512c08 = 4;
    DAT_00512c10 = 4;
    DAT_00512c18 = 4;
    DAT_00512ae0 = 13;
    DAT_00512a28 = FUN_0044fd50;
    DAT_00512bc8 = 7;
    DAT_00512aec = 65;
    DAT_00512a34 = FUN_0044fd60;
    DAT_00512bd4 = 7;
    DAT_00512af0 = 1;
    DAT_00512a38 = FUN_0044fd70;
    DAT_00512bd8 = 7;
    DAT_00512af4 = 1;
    DAT_00512a3c = FUN_0044fd80;
    DAT_00512bdc = 7;
    DAT_00512af8 = 1;
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
    DAT_00512b14 = 6;
    DAT_00512a5c = FUN_0044fd40;
    DAT_00512b18 = 22;
    DAT_00512a60 = FUN_0044fd40;
    DAT_00512a64 = FUN_0044fd40;
    DAT_00512b20 = 5;
    DAT_00512a68 = FUN_0044fd40;
    DAT_00512b24 = 18;
    DAT_00512a6c = FUN_0044fd40;
    DAT_00512c0c = 7;
    DAT_00512b28 = 24;
    DAT_00512a70 = FUN_0044fd40;
    DAT_00512b2c = 1;
    DAT_00512a74 = FUN_0044fd40;
    DAT_00512c14 = 6;
    DAT_00512b30 = 17;
    DAT_00512a78 = FUN_0044fd40;
    DAT_00512b34 = 2;
    DAT_00512a7c = FUN_0044fd40;
    DAT_00512c1c = 7;
    DAT_00512b38 = 2;
    DAT_00512a80 = FUN_0044fd40;
    DAT_00512c20 = 7;
    DAT_00512b3c = 3;
    DAT_00512a84 = FUN_0044fd40;
    DAT_00512c24 = 7;
    DAT_00512b44 = 6;
    DAT_00512a8c = FUN_0044fd40;
    DAT_00512c2c = 7;
    DAT_00512b48 = 5;
    DAT_00512a90 = FUN_0044fd40;
    DAT_00512c30 = 7;
    DAT_00512b40 = 14;
    DAT_00512a88 = FUN_0044fd40;
    DAT_00512c28 = 1;
    DAT_00512b4c = 9;
    DAT_00512a94 = FUN_0044fd40;
    DAT_00512b6c = 5;
    DAT_00512ab4 = FUN_0044fd40;
    DAT_00512c54 = 1;
    DAT_00512b70 = 41;
    DAT_00512ab8 = FUN_0044fd40;
    DAT_00512c58 = 7;
    DAT_00512b64 = 14;
    DAT_00512aac = FUN_0044fd40;
    DAT_00512c4c = 7;
    DAT_00512b68 = 6;
    DAT_00512ab0 = FUN_0044fd40;
    DAT_00512c50 = 7;
    DAT_00512b50 = 2;
    DAT_00512a98 = FUN_0044fd40;
    DAT_00512c38 = 6;
    DAT_00512b54 = 5;
    DAT_00512a9c = FUN_0044fd40;
    DAT_00512c3c = 6;
    DAT_00512b58 = 186;
    DAT_00512aa0 = FUN_0044fd40;
    DAT_00512c40 = 7;
    DAT_00512b5c = 10;
    DAT_00512aa4 = FUN_0044fd40;
    DAT_00512c44 = 1;
    DAT_00512b60 = 6;
    DAT_00512aa8 = FUN_0044fd40;
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
    return param_1->field_1749 != 0;
}
