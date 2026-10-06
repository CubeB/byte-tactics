// Decompiled by Haiku, deepseek-v4.1-flash, GPT-5.6-Terra, GPT-6, GPT-6.1-sol, deepseek-v4.1, space-bunny-free, Claude Opus 5.5, mimo-v2.6-pro, longcat-2.5-preview-free, muse-spark-1.3-free, Sonnet 5.5, Claude Fable 5.1, Opus and Sonnet. Names are provisional.
// HapiBank: a bank of named accounts, each holding named items (integers,
// doubles and strings) and safe-deposit boxes (blocks of bytes under a name
// or a number), saved to and loaded from a HAPIBANK file.

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <io.h>
// Nothing here uses <stdlib.h>: its symbols put SaveAccount, LoadAccount and
// OpenAccount in the symbol-id windows they match in (docs/c2-regalloc.md).
#include <stdlib.h>

// A block of bytes an account keeps under a name or a number.
struct SafeDepositBox {              // 0x14 bytes
    int named;                       // +0x00
    union {
        char* name;                  // +0x04 when named
        int number;                  // +0x04 when not
    };
    int size;                        // +0x08
    int pos;                         // +0x0c, the read and write position
    char* data;                      // +0x10
};

// A named value of an account.
struct BankItem {                    // 0x10 bytes
    char* name;                      // +0x00
    int type;                        // +0x04, 1 = integer, 2 = double, 3 = string
    union {
        int value;                   // +0x08
        double real;                 // +0x08
        char* string;                // +0x08
    };
};

struct BankAccount {                 // 0x18 bytes
    char* name;                      // +0x00
    int itemCount;                   // +0x04
    int boxCount;                    // +0x08
    int currentBox;                  // +0x0c
    BankItem* items;                 // +0x10
    SafeDepositBox* boxes;           // +0x14
};

struct AccountList {                 // 0x0c bytes
    int count;                       // +0x00
    BankAccount* accounts;           // +0x04
    int current;                     // +0x08
};

// An open HAPI file.
struct HapiFile {
    void* field_0;                   // +0x00
    void* field_4;                   // +0x04
    void* field_8;                   // +0x08
    int field_c;                     // +0x0c
    void* field_10;                  // +0x10
    void* field_14;                  // +0x14
    char name[0x100];                // +0x18
};

HapiFile* __stdcall HAPI_OpenFileRead(char* name);
int __stdcall HAPI_CloseFile(HapiFile* file);
long __stdcall HAPI_SeekFile(HapiFile* file, long pos);
long __stdcall HAPI_TellFile(HapiFile* file);
void __stdcall HAPI_readfromfile(HapiFile* file, void* buf, int size);
long __stdcall HAPI_FileLength(HapiFile* file);

void* __cdecl FUN_004d8450(unsigned int size);
void* __cdecl GameCalloc(unsigned int count, unsigned int size);
void* __cdecl FUN_004d8580(void* ptr, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
char* __cdecl GameStrdup(char* s);
int __cdecl SetOutOfMemoryHandler(int handle);

int __stdcall SquashUnpackedSize(unsigned char* src);
int __stdcall SquashUnpack(void* dest, void* src);
char* __stdcall SquashErrorString(int code);
int __stdcall SquashMaxPackedSize(int size, int level);
int __stdcall SquashPack(void* dest, int* destSize, void* src, int srcSize, int param_5, int param_6);
void __stdcall FatalError(char* message);
char* __stdcall StripExtension(char* name);

// The file header. 0x22 bytes of it are read and written.
#pragma pack(push, 1)
struct BankFileHeader {              // 0x22 bytes
    char magic[8];                   // +0x00 "HAPIBANK"
    int nameOffset;                  // +0x08, of the bank's name in the string pool
    int poolOffset;                  // +0x0c, of the string pool in the file
    int headerSize;                  // +0x10, where the first account starts
    int version;                     // +0x14
    bool compressed;                 // +0x18, the string pool is squashed
    char unused_19[9];
};
#pragma pack(pop)

// The string pool as loaded: the original keeps it at the bottom of
// OpenBank's frame and hands it by address to LoadAccount.
struct PoolImage {                   // 0x08 bytes
    void* buf;                       // +0x00
    int size;                        // +0x04
};

// The string pool as SaveBank builds it.
struct StringPool {
    char* data;                      // +0x00
    int len;                         // +0x04
    int csize;                       // +0x08
};

struct AccountHeader {               // 0x20 bytes
    int size;                        // +0x00, of the whole account
    char* strOffset;                 // +0x04, of its name in the string pool
    int nInts;                       // +0x08
    int nDoubles;                    // +0x0c
    int nStrings;                    // +0x10
    int nBlobs;                      // +0x14
    int compressed;                  // +0x18
    int unknown_1c;                  // +0x1c
};

// The records of an account's body. The offsets are pointer-typed.
struct IntRecord {                   // 8 bytes
    char* name;
    int value;
};

#pragma pack(push, 4)
struct DoubleRecord {                // 12 bytes
    char* name;
    double value;
};
#pragma pack(pop)

struct BoxRecord {                   // 16 bytes
    char* name;                      // negative for a numbered box
    int number;
    int offset;                      // of the bytes in the file
    int len;
};

class HapiBank {
public:
    AccountList* bank;               // +0x00
    char unknown_4[4];
    int field_8;

    HapiBank* InitBank();
    void CloseBank();
    void NewBank();
    int OpenBank(char* filename, char* name, char* account);
    int SaveBank(char* filename, char* name, int compress, int audit);
    void SaveAccount(int index, FILE* file, StringPool* pool, int compress);
    void LoadAccount(HapiFile* file, int* image, char* name);
    int OpenAccount(char* name);
    int SetIntegerItem(const char* name, int value);
    int SetDoubleItem(const char* name, double value);
    int SetStringItem(const char* name, char* value);
    int GetIntegerItem(char* name, int def);
    double GetDoubleItem(char* name, double def);
    char* GetStringItem(char* name, char* def);
    int HasItem(const char* name);
    int FindItem(const char* name, int create);
    int FindNumberedBox(int number, int create);
    int FindNamedBox(char* name, int create);
    int OpenNumberedBox(int number);
    int OpenNamedBox(char* name);
    int GetBoxSize();
    void SeekBox(int pos);
    int SeekBoxEnd();
    int ReadBox(void* dst, int len);
    int WriteBox(void* src, int len);
    void WriteAuditFile(char* filename);
};

// FUNCTION: 0x4b3620
HapiBank* HapiBank::InitBank()
{
    bank = 0;
    return this;
}

// The out-of-line destructor of HapiBank: frees the account list and
// everything hanging off it.
// FUNCTION: 0x4b3630
void HapiBank::CloseBank()
{
    if (bank != 0) {
        for (int i = 0; i < bank->count; i++) {
            BankAccount* account = &bank->accounts[i];
            FUN_004d85a0(account->name);
            if (account->items != 0) {
                for (int j = 0; j < account->itemCount; j++) {
                    FUN_004d85a0(account->items[j].name);
                    if (account->items[j].type == 3) {
                        FUN_004d85a0(account->items[j].string);
                    }
                }
                FUN_004d85a0(account->items);
            }
            if (account->boxes != 0) {
                for (int k = 0; k < account->boxCount; k++) {
                    if (account->boxes[k].named != 0) {
                        FUN_004d85a0(account->boxes[k].name);
                    }
                    if (account->boxes[k].data != 0) {
                        FUN_004d85a0(account->boxes[k].data);
                    }
                }
                FUN_004d85a0(account->boxes);
            }
        }
        if (bank->accounts != 0) {
            FUN_004d85a0(bank->accounts);
        }
        FUN_004d85a0(bank);
    }
}

// FUNCTION: 0x4b3750
void HapiBank::NewBank()
{
    CloseBank();
    bank = (AccountList*)GameCalloc(1, 0xc);
    bank->current = -1;
}

//
// STATUS (mimo-v2.6-pro): MATCH. The current source compiles to 577 bytes,
// byte identical to the original. The residual described at length below
// (the strcmpi `add` destination) is RESOLVED by the local declaration order:
// `img` is declared AFTER `raw` and `h` instead of first, which gives the
// locals the original's stack slots and leaves the add as `add edx,ecx`
// (base is the destination), matching the original at 0x18e. Everything after
// this note is the historical record of the earlier 99.0% attempts and is kept
// only as documentation; it no longer describes the current output.
//
// Claude Opus 5.5 (found with tools/permute.py): MATCH. `img` is declared after `raw`
// and `h` instead of first, which gives the locals the original's stack slots
// (99.0% with `img` first).
// 99.0% (577 bytes against 577, only two instructions differ, see the end).
//
// SEVENTH PASS (deepseek-v4.1-flash). Confirmed the residual is unchanged:
// 206 of 208 instructions byte identical, the only diff at 0x18e/0x190:
//   0x18e add edx,ecx / push edx   (original)
//   0x18e add ecx,edx / push ecx   (ours)
// The #3148 GPT-6.1-sol retry independently rechecked the source and subscript-address variant (both 99.0%, no MATCH). All prior passes agree that with a base reloaded from the frame MSVC 5
// always names the int/offset as the add destination, so no spelling of
// `pointer + int` reaches the original's `add base, off`; the one shape that
// does (non-reloadable base read through a register) costs an extra `mov` the
// original does not have. Left partial at 99.0%.
//
// EIGHTH PASS (deepseek-v4.1-flash). Independently re-scored 13 more
// spellings through check.py's own compiled pipeline (script in
// build/scratch/0x4b3770/score.py), all still exactly 577 bytes and 99.0%
// with the same two-instruction residual: making Image_004b3770::buf a plain
// `int` (so the sum is a genuine int+int add, not a PTRADD) changes nothing,
// and neither do `off + base`, `base + off + 0`, `base + (+off)`,
// `*(int*)&buf + off`, `(unsigned)buf + off`, `base - (0 - off)` or
// `base + off*1`. This confirms the seventh pass: MSVC 5 names the second
// operand of this add as the destination and loads the base first, and no
// same-value respelling moves it. Left partial at 99.0%.
//
// The original's 0xb4-byte frame, read off the disassembly (offsets are from
// E0, the esp right after "sub esp,0xb4" plus the four register pushes, so a
// "[esp+X]" in the body is E0+X-0x10):
//   E0+0x00 img.buf      (0x4b37fb zeroes it, 0x4b38c1 tests it, 0x4b38db and
//                        0x4b388e store the malloc/realloc results, 0x4b38f6,
//                        0x4b3914 and 0x4b398e reload it, 0x4b396e takes &it)
//   E0+0x04 img.size     (zeroed at 0x4b37ff, then only ever written: dsize at
//                        0x4b3886, remaining at 0x4b38df.  Never read, so it
//                        only survives because &img escapes to LoadAccount)
//   E0+0x08 raw          (the decompression scratch: 0x4b3849 stores it,
//                        0x4b38a5 frees it)
//   E0+0x0c this         (spilled at the top by 0x4b3784, reloaded by
//                        0x4b38b8 after the compressed branch clobbers edi)
//   E0+0x10 the header   (0x24 bytes, 0x22 of them read by 0x4b37a4; its first
//                        dword is E0+0x10, so the fields sit 0x08..0x18 in it)
//   E0+0x34 the message  (0x80 bytes, 0x34+0x80 == 0xb4, the frame size)
//
// Register roles: ebp is the open File, ebx a register-resident 0 that the
// compressed branch reuses for src, edi this, esi the running size.  Two source
// choices buy that, and both were needed:
//   * `file` and `name` must be DIFFERENT parameters (param1 is the file name
//     for the open and the bank name for the strcmpi, param2 is only ever the
//     bank name, param3 goes to LoadAccount).  Reusing one variable for both
//     strings made it live across the whole function, and MSVC then hoisted it
//     into ebp before the prologue and demoted the file to ebx.
//   * `img.size = dsize;` has to come BEFORE the memcpy.  After the memcpy the
//     allocator spills the size to a home of its own, the frame grows to 0xb8
//     and every stack reference in the function moves 4 bytes.
//
// STILL DIFFERENT: the pointer+offset for the strcmpi.  The original emits
// "mov edx,buf / mov ecx,nameoff / add edx,ecx" and this emits the same two
// loads with "add ecx,edx", i.e. MSVC 5.0 always makes the integer offset the
// destination here.  See the PTRADD note at the end of this comment: no source
// spelling of `+` reaches the other order, so this is one unresolved 2-byte
// register choice, not a shape problem.  Tried and rejected for it: swapping the operands, casting
// the whole sum, subscripting, a char* member, an int member, int/long/
// unsigned/pointer-typed offset fields, a named local for the sum (both
// in-block and hoisted), a named local for the pointer with `+=`, a named local
// copy of the buffer pointer, reading the offset through a Header* reference,
// subscripting, an __inline accessor, comma-operator sequencing,
// `&&` instead of nested ifs, and non-const / void* / __stdcall declarations
// of _strcmpi.  Swapping the two strcmpi arguments is worse (96.6%).  0x4b4270 has the same shape and the same problem: the original
// has "mov edx,[edi] / mov ecx,[off] / add edx,ecx" while "*image + h.strOffset"
// compiles to "add edx,[ebx]", so whatever idiom Cavedog used is not a spelling
// of `pointer + int` that VC5 will accept here.
//
// Second pass over it, with /Fa listings for every spelling (all in
// build/scratch/0x4b3770/x_*, y_*): EVERY one of them puts the INTEGER offset
// in the `add` DESTINATION, which pins the cause.  The base is always loaded
// first into edx and the offset second into ecx, so the two operands keep their
// source order and only the destination flips.  That is MSVC 5's regalloc()
// swapping dst and src because genarith() was handed NO_REG for the
// destination, and a PTRADD only gets a destination register when the
// frontend knows where the result goes (a store address or the like), which a
// by-value call argument never is.  Register pressure is not the lever: adding
// a redundant `if (name == 0) return 1;` does rotate the roles (it produces
// "mov edx,buf / mov eax,off / add eax,edx / push eax / push ecx"), but the
// offset is still the destination.  Tried this pass and all still swapped: a
// named local for the sum at function scope and in the block, a named local
// for the base with `+=` and with `= base + off`, int and long locals for the
// offset, casting the whole sum, casting the base through unsigned, `&base[off]`
// and `base[off]`, `offset + base` with the operands swapped, a void* local,
// reading the offset through a pointer, a reference or a pointer to the header,
// a unary plus on the base, and the CAST-FREE case (Image::buf declared `char*`
// so the sum carries no cast node at all).  27 spellings, all swapped, so I do
// not think any source form of `pointer + int` reaches `add base, off` here.
//
// WHAT THE ADD DESTINATION ACTUALLY IS (measured from /Fa listings, not
// assumed; listings in build/scratch/0x4b3770/small/, probe2.py, probe3.py,
// plus 20 more spellings scored in g2.py..g5.py).  The rule is a property of
// the CODEGEN and it is absolute for every `+` that MSVC 5 turns into a
// PTRADD or a BUSOP ADD: THE DESTINATION REGISTER ALWAYS HOLDS THE
// INT/offset OPERAND, whatever the source order is, and the two operands are
// emitted in SOURCE ORDER:
//   base + off, or off + base -> the off is in the destination either way,
//       the emission order is the source order
//   an int add with no PTRADD at all ((unsigned)base + off, base held in an
//       unsigned field, the sum landing in a local) -> still off in dst
//   a short or char offset -> folds the BASE into the addressing mode:
//       add <dst>,[base], off in dst
//   base + a CONSTANT -> mov <dst>,[base] / add dst,CONST: the BASE is the
//       destination (this is the PTROFF fast path)
//   both operands already in registers -> lea <dst>,[base + off], base first
//       (this is how 0x4b39c0's `buf.data + oldlen` matches)
// So with a VARIABLE offset and both operands in memory the base is never the
// destination, and the original's "add edx,ecx" is exactly the one shape this
// codegen does not produce for a `+`.
//
// That leaves a compound assignment, since IR_ADDS names the variable's own
// register as the destination.  Tried: `char* b;` declared with the other
// locals (so allocvar can give it a register), then inside the block
// `b = (char*)img.buf; b += h.nameoff;` and `_strcmpi(name, b)`.  b is dead
// after the call so C1 stores nothing and the size stays 577, but the add is
// still "mov edx,[buf] / mov ecx,[off] / add ecx,edx" and it still scores
// 99.0%: C1 normalises the compound assignment back into the same tree.  The
// single-assignment forms (`char* b = img.buf + h.nameoff;`, with and without
// the `+=` split, and with b forced live so the store is emitted) score 95.2%
// or worse.
//
// The register allocation is already right: both loads land in the same two
// registers as the original in every spelling tried, and both movs are byte
// identical.  Only WHICH of the two registers the add names as its destination
// differs, so this is one 2-byte choice, not two.
//
// ---------------------------------------------------------------------------
// THIRD PASS (space-bunny-free).  40 further spellings, all byte-identical to
// each other and all still swapped.  What is new here is the SHAPE of the
// residual and the one thing that is now PROVEN rather than assumed.
//
// 1. The residual is exactly two instructions, both inside the strcmpi call:
//      0x18e  add edx,ecx   /  0x190  push edx      (original)
//      0x18e  add ecx,edx   /  0x190  push ecx      (ours)
//    577 bytes against 577, 206 of 208 instructions byte identical, so this is
//    class (a), an operand-order choice, and the 0x190 push is a cascade of
//    the add's destination, not a second defect.  There is NO size trap here:
//    the object is already the original's exact length, so no variant can
//    "score better by being shorter".
//
// 2. THE ORDER RULE, MEASURED (this is the new result).  With both operands
//    still in memory, MSVC 5.0 loads the POINTER operand first and the INT
//    offset second, and then names the INT as the add's destination.  I
//    confirmed the causal direction with an int-only add, where the source
//    order is under my control:
//      (char*)img.buf + h.nameoff       ->  mov edx,[buf] / mov ecx,[off]
//                                           / add ecx,edx   (int is dst)
//      (char*)(h.nameoff + (int)img.buf) ->  mov edx,[buf] / mov ecx,[off]
//                                           / add ecx,edx   (int is dst)
//    Writing the offset FIRST changes neither the load order nor the
//    destination, which pins the cause: it is not source order at all, it is
//    the front end canonicalising `+` so the pointer is always the lvalue,
//    and C1's gendiad then always naming the rvalue, which is the int.  So
//    no operand permutation of a `+` can reach "add base, off" here, and the
//    ORIGINAL's add must have had a destination register ASSIGNED to the
//    IR_ADDS node, which only happens when the sum is assigned to something
//    the register allocator gave a register to.
//
// 3. Why assigning a sum local does not pay (tested, not assumed).  A local
//    sum that is dead after the call is forwarded into the argument and the
//    tree is unchanged (`char* b; b = (char*)img.buf; b += h.nameoff;
//    _strcmpi(name, b);` is byte identical to the bare form).  A local sum
//    that IS live across the call does get a register, and then the add does
//    name the base -- but only because the value is promoted to a
//    callee-saved register, and in this function all four of ebp, ebx, esi
//    and edi are already pushed and in use, so the extra liveness costs a new
//    push/pop or a spill.  Measured: 87.0% and 212 bytes, with the whole
//    function's register assignment reshuffled.  Rejected.
//
// 4. Tried this pass and all still swapped, all at 577 bytes with the same
//    two-instruction diff (so none of them is a size trap, just no effect):
//    `*(char**)&img` and `*(int*)((char*)&h + 8)` for either operand, so a
//    deref instead of a direct member load, which is the one thing that would
//    have changed the operand's IR subtree size; a cast on the whole sum and
//    on the base through a second type; `char*` for Image_004b3770::buf so
//    the PTRADD carries no cast node at all (this needs the two allocator
//    results cast and LoadAccount's second parameter declared `char**`);
//    `int`/`unsigned`/`long` for the nameoff field; `(int)`, `(unsigned)`,
//    `(long)` casts of either operand; `offset + base` and
//    `base + offset` as plain int adds; `+(0 - off)` so the IR is a SUB;
//    a union-mediated read of either operand; seven different cast types on
//    the base (`unsigned char*`, `signed char*`, `void*`, `short*`, `long*`,
//    `char* const`, `const char*`); a named sum local declared at each of the
//    eight possible positions among the other locals, in both the
//    split-assignment and the single-assignment form; the sum local live
//    after the if via a call, a dereference, a comparison and a second
//    strcmpi; the base hoisted into a local that is live after the if.
//
// 5. The calling convention is NOT the cause, checked rather than assumed: the
//    function ends in `ret 0xc` and is a member with three parameters plus
//    `this` in ecx, and every _strcmpi call is followed by `add esp,8`, so
//    _strcmpi is __cdecl and the declaration is already right.  A
//    __stdcall spelling of _strcmpi was not needed and is not the answer.
//
// 6. It is not an STL instantiation: no template, no container, no
//    vector<T>::insert.  It is a plain C++ member function.
//
// FOURTH PASS (deepseek-v4.1-flash). An independent, scripted sweep scored 37
// expression spellings (`(int)`/`(unsigned)`/`(long)` casts of either operand,
// `(char*)(off + (int)base)`, `&base[off]`, derefs of `&img`, every field type
// for nameoff, and 9 control-flow shapes: `name != 0 && ...`, `if (name)`,
// `if (name == 0) {} else ...`, a named `char* p` with `p += off` in-block and
// hoisted, `!strcmpi`) through check.py's library. ALL 46 land on exactly
// 99.04%, byte for byte the same two-instruction residual. That pins it: the
// ADD destination is not a source-order choice at all, MSVC 5 always names the
// RIGHT operand of `ptr + int` as the destination, and only a plain `+` with
// the pointer on the right (unwritable in C++) reaches `add base, off`.
// tools/headers.py tried all 128 header sets: none changes it. Left partial.
//
// FIFTH PASS (deepseek-v4.1-flash). Re-ran headers.py (still no header set
// matches) and swept 38 more spellings through direct /Fa listings. The rule
// is confirmed and now has a positive control: `add dst, off` with the BASE in
// dst IS reachable, but only when the offset operand is forced to be
// materialised in a value register first. `(char*)img.buf + (h.nameoff ?
// h.nameoff : h.nameoff)` emits "mov eax,[off] / mov edx,[buf] / add edx,eax",
// i.e. the ADD gets a pre-assigned destination and names the base, but the
// register roles and the load order flip (off before buf, name in ecx) and the
// extra mov is not in the original. So the residual is still only:
//     0x18e add edx,ecx / push edx   (original)
//     0x18e add ecx,edx / push ecx   (ours)
// Nothing in this pass changed the file; 99.0% is the best.
//
// SIXTH PASS (space-bunny-free).  The `add` destination rule is now MEASURED,
// not assumed, and it closes the last open hypothesis.  Probes are in
// build/scratch/0x4b3770/probe.cpp, probe2.cpp, probe3.cpp (/Fa listings
// probe.lst, probe2.lst, probe3.lst), all compiled with the checker's flags.
//
//   A. With BOTH operands reloadable constant-address loads, the INT always
//      takes the add destination.  `char* + int`, `char* + (unsigned)int`,
//      `char* + (int+0)`, `char* + (int*1)`, `char* + (int&0xffff)`,
//      `*(char**)&img`, a `char*` field instead of `void*`, the operands
//      written in either order, `(int)base + off`, `off + (int)base`, a
//      subscript, `&subscript` and a named local with `+=` all emit
//      "mov <base>,mem / mov <int>,mem / add <int>,<base>".  Source order of the
//      two operands changes which load comes first, never the destination.
//   B. The base takes the destination ONLY when the pointer operand is an
//      INDIRECT through a REGISTER, i.e. not reloadable.  Measured twice:
//        probe2 q2  `p->buf` with p a parameter:  mov ecx,[p] / mov edx,[ecx]
//                                                        / mov ecx,[off]
//                                                        / add edx,ecx
//        probe2 q6  same with p kept live in esi across the call: same shape,
//                   still `add ecx,edx` with ecx holding the base.
//      That is the original's operand order and register roles, but it needs
//      the extra `mov <reg>,[p]`, so it cannot be it: the original loads the
//      base straight from [esp+0x10].
//   C. Reading the base through a pointer-to-local does not help, because C1
//      copy-propagates it back to a constant address and the node becomes
//      reloadable again.  Measured (probe3 s1..s4), all four of
//        `Img* p = &img; ... p->buf`        (address also passed to a call)
//        `Img& r = img; ... r.buf`
//        `(&img)->buf`
//        `void** pp = &img.buf; ... *pp`
//      fold to ONE direct `mov edx,[esp+0x10]` and then put the INT back in
//      the add destination.  So the "non-reloadable base" route needs a real
//      register, which costs an instruction the original does not have.
//   D. A constant offset is the one case where the base is the destination,
//      because the offset folds into the addressing mode / the immediate
//      (`mov edx,[buf] / add edx,imm`), and 0x4b39c0's `lea <dst>,[base+off]`
//      is the both-in-registers case.  The original's `add edx,ecx` with a
//      register source is in neither class.
//
// So for this function the residual is unreachable by any spelling of
// `pointer + int` where the pointer is read straight out of the frame, which
// is what the original does.  0x4b4270's `*image + h.strOffset` is case B and
// therefore reachable in principle; it differs there only in register roles,
// and it is still unmatched, so the same wall is likely structural.
// FUNCTION: 0x4b3770
int HapiBank::OpenBank(char* filename, char* name, char* account)
{
    void* raw;
    BankFileHeader h;
    PoolImage img;
    char errmsg[0x80];
    HapiFile* file;
    long remaining;
    long pos;

    file = HAPI_OpenFileRead(filename);
    if (file == 0) {
        return 0;
    }

    HAPI_readfromfile(file, &h, 0x22);
    if (strncmp(h.magic, "HAPIBANK", 8) != 0) {
        HAPI_CloseFile(file);
        return 0;
    }

    if (h.version != 1) {
        HAPI_CloseFile(file);
        return 0;
    }

    img.buf = 0;
    img.size = 0;
    remaining = HAPI_FileLength(file) - h.poolOffset;
    HAPI_SeekFile(file, h.poolOffset);

    if (h.compressed != 0) {
        void* src = FUN_004d8450(remaining);
        int dsize;
        int err;

        HAPI_readfromfile(file, src, remaining);
        dsize = SquashUnpackedSize((unsigned char*)src);
        raw = FUN_004d8450(dsize);
        err = SquashUnpack(raw, src);
        if (err != 0) {
            sprintf(errmsg, "[HapiBank::OpenBank] Decompression Error: %s",
                    SquashErrorString(err));
            FatalError(errmsg);
        }
        img.buf = FUN_004d8580(img.buf, dsize);
        img.size = dsize;
        memcpy(img.buf, raw, dsize);
        FUN_004d85a0(raw);
        FUN_004d85a0(src);
    } else {
        if (img.buf != 0) {
            FUN_004d85a0(img.buf);
        }
        img.buf = FUN_004d8450(remaining);
        img.size = remaining;
        HAPI_readfromfile(file, img.buf, remaining);
    }

    if (name != 0) {
        if (_strcmpi(name, (char*)img.buf + h.nameOffset) != 0) {
            HAPI_CloseFile(file);
            if (img.buf != 0) {
                FUN_004d85a0(img.buf);
            }
            return 0;
        }
    }

    CloseBank();
    bank = (AccountList*)GameCalloc(1, 0xc);
    bank->current = -1;

    HAPI_SeekFile(file, h.headerSize);
    pos = HAPI_TellFile(file);
    if (pos < h.poolOffset) {
        do {
            LoadAccount(file, (int*)&img.buf, account);
            pos = HAPI_TellFile(file);
        } while (pos < h.poolOffset);
    }

    HAPI_CloseFile(file);
    if (img.buf != 0) {
        FUN_004d85a0(img.buf);
    }
    return 1;
}

// MATCH. The 99.6% version carried a second local (`noff = oldlen`) and emitted
// `mov [esp+0x1c], edx` at 0x4b3aba where the original has `mov [esp+0x1c], esi`.
// Both registers hold the same value, so only the allocator's choice differed.
// Dropping the `noff = oldlen` statement and spilling `oldlen` itself fixes the
// store, but only while `noff` stays an unassigned local that the cbuf == 0 arm
// reads: that read is what keeps the spill slot alive, so the store is written
// from the register copy rather than from the loaded value. Every assigned form
// tried (`noff = oldlen` at four different points, `int noff = 0`, `err = 0`,
// `err = oldlen`, and `noff` as a 4th Buffer field) collapses the two names into
// one and loses the `mov esi, edx` copy, which costs 2 bytes.
// Suspected original bug: the cbuf == 0 arm at 0x4b3bb2 reads a stack slot that
// nothing assigns on that path (`mov eax, [esp+0x1c]`); it happens to hold the
// old buf.len spill from 0x4b3aba. Harmless, because err is only tested when
// cbuf != 0. Kept as written so the code matches.
// FUNCTION: 0x4b39c0
int HapiBank::SaveBank(char* filename, char* name, int compress, int audit)
{
    char path[256];
    BankFileHeader header;
    int noff;
    int err;
    StringPool pool;
    FILE* file;
    int i;
    if (bank == 0 || bank->count == 0) { return 0; }
    if (audit) {
        strcpy(path, filename);
        StripExtension(path);
        strcat(path, ".cpa");
        WriteAuditFile(path);
    }
    file = fopen(filename, "w+b");
    if (file == 0) { return 0; }
    pool.data = 0;
    pool.len = 0;
    memset(&header, 0, sizeof(header));
    strncpy(header.magic, "HAPIBANK", 8);
    header.version = 1;
    int oldlen = pool.len;
    pool.len = oldlen + strlen(name) + 1;
    pool.data = (char*)FUN_004d8580(pool.data, pool.len);
    strcpy(pool.data + oldlen, name);
    header.nameOffset = oldlen;
    header.headerSize = sizeof(header);
    fwrite(&header, sizeof(header), 1, file);
    for (i = 0; i < bank->count; i++) { SaveAccount(i, file, &pool, compress); }
    fseek(file, 0, 2);
    header.poolOffset = ftell(file);
    int dsize = pool.len;
    pool.csize = SquashMaxPackedSize(dsize, 2);
    int handle = SetOutOfMemoryHandler(0);
    char* cbuf = (char*)FUN_004d8450(pool.csize);
    SetOutOfMemoryHandler(handle);
    if (cbuf != 0) { err = SquashPack(cbuf, &pool.csize, pool.data, dsize, 1, 0); }
    else { err = noff; }   // noff is never assigned; see the note at the top
    if (cbuf != 0 && err == 0 && pool.csize < dsize) {
        fwrite(cbuf, pool.csize, 1, file);
        header.compressed = true;
    } else {
        fwrite(pool.data, pool.len, 1, file);
    }
    fseek(file, 0, 0);
    fwrite(&header, sizeof(header), 1, file);
    fclose(file);
    if (cbuf != 0) { FUN_004d85a0(cbuf); }
    if (pool.data != 0) { FUN_004d85a0(pool.data); }
    return 1;
}

// MATCH (1544 bytes). Three levers got here:
// (1) 95.7% to 98.6%: in the box record loop the dataOffset update must execute
// BEFORE the fwrite call (`int len = ...; rec[2] = dataOffset; rec[3] = len;
// dataOffset += len; fwrite(...)`), otherwise dataOffset stays live across the
// call in a callee-saved register and the whole function re-registers; with the
// increment first, dataOffset lives in [esp+0x64] as in the original and edi
// becomes the loop zero/index register. Also, in the string item case, declare
// `char* second = slot->items[i].string;` BEFORE `int oldlen2 = buf->len;`.
// (2) 98.6% to MATCH: the compression tail at 0x4b4125 was the last 6 bytes. The
// original keeps `off` in ebp from 0x4b412b (and reloads it at 0x4b421f) while
// spilling `raw` to [esp+0x5c]; the plain source keeps `off` in ebx and
// materialises `off + 0x20` into [esp+0x68]. That is a pure allocator tie between
// two callee-saved registers, and one extra reference to `off` inside the
// compress block splits it in the original's favour.
// Every dead spelling tried on 2026-09-30 is folded before allocation and does
// nothing: `(void)off;`, `off = off;`, a bare `off;`, an empty `if (off) { }`, an
// empty `switch (off) { }`, a comma use (`int d = (off, 0);`,
// `FUN_004d8450((off, len))`, `(off, h.size - 0x20)`), `len + (off & 0)`, a named
// `int start = off + 0x20;` local used by both fseeks, declaring `char* raw;`
// before `int len`, `unsigned off`, `0x20 + off` as the second fseek offset, a
// `char* raw = 0;` two-statement definition, an empty `for` loop bounded by
// `(off & 0)`, `off - off`, and a `sizeof`-style trick.
// The nudge that works is a redundant conditional re-assignment of `len`:
//     if (off < 0) len = h.size - 0x20;
// Both arms store the same value, so MSVC folds the assignment to nothing and
// the statement costs 0 bytes, but the extra `off` reference survives to the
// allocator and reproduces the original's ebp/ebx split exactly. This is the
// redundant self-correction lever of guide technique 5 (`if (v) v = 1; else
// v = 0;`), the same family as the original `if (off < 0) { len = 0; }` which
// also flips the tie but emits the guard (test/jge/xor, 6 bytes) for 98.6%.
// `if (off <= -1) len = h.size - 0x20;`, the braced body, and a version that
// stores through a throwaway `int z` all match too.
// FUNCTION: 0x4b3c60
void HapiBank::SaveAccount(int index, FILE* file, StringPool* buf, int compress)
{
    BankAccount* slot = &bank->accounts[index];
    if (slot->itemCount <= 0 && slot->boxCount <= 0) {
        return;
    }

    int off = ftell(file);
    AccountHeader h;
    memset(&h, 0, sizeof(h));
    fwrite(&h, sizeof(h), 1, file);

    {
        char* name = slot->name;
        int oldlen = buf->len;
        buf->len += strlen(name) + 1;
        buf->data = (char*)FUN_004d8580(buf->data, buf->len);
        strcpy(buf->data + oldlen, name);
        h.strOffset = (char*)oldlen;
    }

    int i;
    for (i = 0; i < slot->itemCount; i++) {
        if (slot->items[i].type == 1) {
            char* name = slot->items[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[2];
            rec[0] = oldlen;
            rec[1] = slot->items[i].value;
            fwrite(rec, 8, 1, file);
            h.nInts++;
        }
    }

    for (i = 0; i < slot->itemCount; i++) {
        if (slot->items[i].type == 2) {
            char* name = slot->items[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[3];
            rec[0] = oldlen;
            *(double*)&rec[1] = slot->items[i].real;
            fwrite(rec, 0xc, 1, file);
            h.nDoubles++;
        }
    }

    for (i = 0; i < slot->itemCount; i++) {
        if (slot->items[i].type == 3) {
            char* name = slot->items[i].name;
            int oldlen = buf->len;
            buf->len += strlen(name) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen, name);
            int rec[2];
            rec[0] = oldlen;
            char* second = slot->items[i].string;
            int oldlen2 = buf->len;
            buf->len += strlen(second) + 1;
            buf->data = (char*)FUN_004d8580(buf->data, buf->len);
            strcpy(buf->data + oldlen2, second);
            rec[1] = oldlen2;
            fwrite(rec, 8, 1, file);
            h.nStrings++;
        }
    }

    for (i = 0; i < slot->boxCount; i++) {
        if (slot->boxes[i].size > 0) {
            h.nBlobs++;
        }
    }

    int dataOffset = ftell(file) + h.nBlobs * 0x10;
    for (i = 0; i < slot->boxCount; i++) {
        if (slot->boxes[i].size > 0) {
            int rec[4];
            if (slot->boxes[i].named != 0) {
                char* nm = slot->boxes[i].name;
                int oldlen = buf->len;
                buf->len += strlen(nm) + 1;
                buf->data = (char*)FUN_004d8580(buf->data, buf->len);
                strcpy(buf->data + oldlen, nm);
                rec[0] = oldlen;
            } else {
                rec[0] = -1;
                rec[1] = slot->boxes[i].number;
            }
            rec[2] = dataOffset;
            int len = slot->boxes[i].size;
            rec[3] = len;
            dataOffset += len;
            fwrite(rec, 0x10, 1, file);
        }
    }

    for (i = 0; i < slot->boxCount; i++) {
        if (slot->boxes[i].size > 0) {
            fwrite(slot->boxes[i].data, slot->boxes[i].size, 1, file);
        }
    }

    h.size = ftell(file) - off;

    if (compress != 0) {
        int handle = SetOutOfMemoryHandler(0);
        int len = h.size - 0x20;
        // A redundant conditional re-assignment of `len`: it mentions `off` once
        // more and compiles to nothing (both arms store the same value), which
        // flips the off/raw register tie to the original's. See the note at the top.
        if (off < 0) len = h.size - 0x20;
        char* raw = (char*)FUN_004d8450(len);
        if (raw != 0) {
            fseek(file, off + 0x20, 0);
            fread(raw, len, 1, file);
            int csize = SquashMaxPackedSize(len, 1);
            char* cbuf = (char*)FUN_004d8450(csize);
            if (cbuf != 0) {
                int err = SquashPack(cbuf, &csize, raw, len, 1, 0);
                if (err == 0 && csize < len) {
                    fseek(file, off + 0x20, 0);
                    fwrite(cbuf, csize, 1, file);
                    h.size = csize + 0x20;
                    h.compressed = 1;
                    _chsize(file->_file, ftell(file));
                }
                FUN_004d85a0(cbuf);
            }
            FUN_004d85a0(raw);
        }
        SetOutOfMemoryHandler(handle);
    }

    fseek(file, off, 0);
    fwrite(&h, sizeof(h), 1, file);
    fseek(file, 0, 2);
}

// Reads one 0x20-byte section header out of a HapiBank archive (called in a loop
// by 0x4b3770) and, when the caller's name matches the section name, unpacks the
// section body and files its records away in the current section of the parsed
// bank: integers, doubles, strings and raw blobs, in that order.
//
// Claude Fable 5.1 (79.2% -> 93.0%): the image base is an integer and the
// offsets in the header and the records are pointer-typed (`char* name`), so
// every `offset + *image` names the base as the add's destination; the int and
// double loops copy each record into a struct local.
//
// Claude Opus 5.5 (93.0% -> MATCH): the blob loop is four calls to small
// methods of the same class that /Ob2 inlines: OpenNumberedBox /
// OpenNamedBox open a box by number or by name (their identical
// "set the current box" stores are tail-merged into the one store after the
// branch, and their `data != 0` results are dropped), WriteBox appends
// bytes to the current box and SeekBox seeks it (the `xor edx,edx` before
// the `rep movsb` is its constant 0). The record is a 16-byte struct copy
// (the original's 16 bytes between the header and the message buffer), and
// the source pointer is a named local computed before the append: passed
// straight as the argument, MSVC forwards it into the inlined memcpy and
// evaluates it late. The helpers are defined after this function, in address
// order, and still inline.
// FUNCTION: 0x4b4270
void HapiBank::LoadAccount(HapiFile* fh, int* image, char* name)
{
    int buf;
    int len;
    int base;
    int end;
    int* p;
    AccountHeader h;
    char message[1000];

    base = (int)HAPI_TellFile(fh);
    HAPI_readfromfile(fh, &h, 0x20);
    end = base + h.size;
    if (name != 0 && _strcmpi(name, h.strOffset + *image) != 0) {
        HAPI_SeekFile(fh, end);
        return;
    }
    len = h.size - 0x20;
    if (len > 0) {
        if (h.compressed == 1) {
            char* tmp = (char*)FUN_004d8450(len);
            HAPI_readfromfile(fh, tmp, len);
            buf = (int)FUN_004d8450(SquashUnpackedSize((unsigned char*)tmp));
            int err = SquashUnpack((void*)buf, tmp);
            if (err != 0) {
                sprintf(message, "[HapiBank::LoadAccount] Decompression Error: %s\nFile: %s",
                        SquashErrorString(err), fh->name);
                FatalError(message);
            }
            FUN_004d85a0(tmp);
        } else {
            buf = (int)FUN_004d8450(len);
            HAPI_readfromfile(fh, (void*)buf, len);
        }
        OpenAccount(h.strOffset + *image);
        p = (int*)buf;
        {
            for (int i = 0; i < h.nInts; i++) {
                IntRecord rec = *(IntRecord*)p;
                p += 2;
                SetIntegerItem(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nDoubles; i++) {
                DoubleRecord rec = *(DoubleRecord*)p;
                p += 3;
                SetDoubleItem(rec.name + *image, rec.value);
            }
        }
        {
            for (int i = 0; i < h.nStrings; i++) {
                int a = p[0];
                int b = p[1];
                p += 2;
                SetStringItem((char*)a + *image, (char*)b + *image);
            }
        }
        {
            for (int i = 0; i < h.nBlobs; i++) {
                BoxRecord rec = *(BoxRecord*)p;
                p += 4;
                if ((int)rec.name < 0)
                    OpenNumberedBox(rec.number);
                else
                    OpenNamedBox(rec.name + *image);
                char* src = (char*)(buf + (rec.offset - base) - 0x20);
                WriteBox(src, rec.len);
                SeekBox(0);
            }
        }
        HAPI_SeekFile(fh, end);
        FUN_004d85a0((void*)buf);
    }
}

// Finds the account named by the argument, or appends a new empty account
// when there is none, and makes it the current one; returns 1 when it
// already existed.
// FUNCTION: 0x4b4560
int HapiBank::OpenAccount(char* name)
{
    for (int i = 0; i < bank->count; i++) {
        if (_strcmpi(bank->accounts[i].name, name) == 0) {
            bank->current = i;
            bank->accounts[i].currentBox = -1;
            return 1;
        }
    }
    bank->current = bank->count;
    bank->count++;
    bank->accounts = (BankAccount*)FUN_004d8580(
        bank->accounts, bank->count * sizeof(BankAccount));
    memset(&bank->accounts[bank->current], 0, sizeof(BankAccount));
    bank->accounts[bank->current].name = GameStrdup(name);
    bank->accounts[bank->current].currentBox = -1;
    return 0;
}

// Stores an integer under a name in the current account (FindItem with 1
// finds or adds the item), freeing the old value first when it was a string;
// returns 0 when there is no current account.
// FUNCTION: 0x4b4630
int HapiBank::SetIntegerItem(const char* name, int value)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 1);
        if (bank->accounts[bank->current].items[i].type == 3)
            FUN_004d85a0(bank->accounts[bank->current].items[i].string);
        bank->accounts[bank->current].items[i].value = value;
        bank->accounts[bank->current].items[i].type = 1;
        return 1;
    }
    return 0;
}

// The double counterpart of SetIntegerItem.
// FUNCTION: 0x4b46c0
int HapiBank::SetDoubleItem(const char* name, double value)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 1);
        if (bank->accounts[bank->current].items[i].type == 3)
            FUN_004d85a0(bank->accounts[bank->current].items[i].string);
        bank->accounts[bank->current].items[i].real = value;
        bank->accounts[bank->current].items[i].type = 2;
        return 1;
    }
    return 0;
}

// The string counterpart of SetIntegerItem: stores a copy of the string, and
// does nothing when it is null.
// FUNCTION: 0x4b4750
int HapiBank::SetStringItem(const char* name, char* value)
{
    if (bank && bank->current >= 0 && value) {
        int i = FindItem(name, 1);
        if (bank->accounts[bank->current].items[i].type == 3)
            FUN_004d85a0(bank->accounts[bank->current].items[i].string);
        bank->accounts[bank->current].items[i].string = GameStrdup(value);
        bank->accounts[bank->current].items[i].type = 3;
        return 1;
    }
    return 0;
}

// Reads an integer item of the current account; returns `def` when there is
// no current account, the item is missing, or it is not an integer.
// FUNCTION: 0x4b4800
int HapiBank::GetIntegerItem(char* name, int def)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 0);
        if (i >= 0 && bank->accounts[bank->current].items[i].type == 1)
            return bank->accounts[bank->current].items[i].value;
    }
    return def;
}

// FUNCTION: 0x4b4850
double HapiBank::GetDoubleItem(char* name, double def)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 0);
        if (i >= 0 && bank->accounts[bank->current].items[i].type == 2)
            return bank->accounts[bank->current].items[i].real;
    }
    return def;
}

// FUNCTION: 0x4b48a0
char* HapiBank::GetStringItem(char* name, char* def)
{
    if (bank && bank->current >= 0) {
        int i = FindItem(name, 0);
        if (i >= 0 && bank->accounts[bank->current].items[i].type == 3)
            return bank->accounts[bank->current].items[i].string;
    }
    return def;
}

// FUNCTION: 0x4b48f0
int HapiBank::HasItem(const char* name)
{
    int i = FindItem(name, 0);
    return i >= 0 ? 1 : 0;
}

// Returns the index of the named item of the current account; when it is
// missing, adds it if `create` is set and returns -1 otherwise.
// FUNCTION: 0x4b4910
int HapiBank::FindItem(const char* name, int create)
{
    AccountList* f = bank;
    if (!f || f->current < 0)
        return -1;
    BankAccount* s = &f->accounts[f->current];
    for (int i = 0; i < s->itemCount; i++) {
        if (_strcmpi(s->items[i].name, name) == 0)
            return i;
    }
    if (!create)
        return -1;
    int n = s->itemCount;
    s->itemCount = n + 1;
    s->items = (BankItem*)FUN_004d8580(s->items, (n + 1) * sizeof(BankItem));
    memset(&s->items[n], 0, sizeof(BankItem));
    s->items[n].name = GameStrdup((char*)name);
    return n;
}

// FUNCTION: 0x4b49d0
int HapiBank::FindNumberedBox(int number, int create)
{
    AccountList* t = bank;
    if (!t || t->current < 0)
        return -1;
    BankAccount* s = &t->accounts[t->current];
    for (int i = 0; i < s->boxCount; i++) {
        if (s->boxes[i].named == 0 && s->boxes[i].number == number)
            return i;
    }
    if (!create)
        return -1;
    int n = s->boxCount;
    s->boxCount = n + 1;
    s->boxes = (SafeDepositBox*)FUN_004d8580(s->boxes, (n + 1) * sizeof(SafeDepositBox));
    memset(&s->boxes[n], 0, sizeof(SafeDepositBox));
    s->boxes[n].number = number;
    s->boxes[n].named = 0;
    return n;
}

// FUNCTION: 0x4b4a80
int HapiBank::FindNamedBox(char* name, int create)
{
    AccountList* t = bank;
    if (!t || t->current < 0)
        return -1;
    BankAccount* s = &t->accounts[t->current];
    for (int i = 0; i < s->boxCount; i++) {
        if (s->boxes[i].named && _strcmpi(s->boxes[i].name, name) == 0)
            return i;
    }
    if (!create)
        return -1;
    int n = s->boxCount;
    s->boxCount = n + 1;
    s->boxes = (SafeDepositBox*)FUN_004d8580(s->boxes, (n + 1) * sizeof(SafeDepositBox));
    memset(&s->boxes[n], 0, sizeof(SafeDepositBox));
    s->boxes[n].name = GameStrdup(name);
    s->boxes[n].named = 1;
    return n;
}

// Makes the numbered box (added when missing) the current one; returns
// whether it holds any bytes.
// FUNCTION: 0x4b4b50
int HapiBank::OpenNumberedBox(int number)
{
    int r = FindNumberedBox(number, 1);
    bank->accounts[bank->current].currentBox = r;
    return bank->accounts[bank->current].boxes[r].data != 0;
}

// FUNCTION: 0x4b4ba0
int HapiBank::OpenNamedBox(char* name)
{
    int i = FindNamedBox(name, 1);
    bank->accounts[bank->current].currentBox = i;
    return bank->accounts[bank->current].boxes[i].data != 0;
}

// FUNCTION: 0x4b4bf0
int HapiBank::GetBoxSize()
{
    AccountList* p = bank;
    BankAccount* a = &p->accounts[p->current];
    int box = p->accounts[p->current].currentBox;
    return a->boxes[box].size;
}

// Sets the current box's position, clamped to 0..size.
// FUNCTION: 0x4b4c10
void HapiBank::SeekBox(int pos)
{
    BankAccount* s = &bank->accounts[bank->current];
    SafeDepositBox* c = &s->boxes[s->currentBox];
    if (pos < 0) {
        pos = 0;
    }
    if (pos > c->size) {
        pos = c->size;
    }
    c->pos = pos;
}

// Moves the current box's position to its end and returns it (the returned
// value is what keeps it in eax).
// FUNCTION: 0x4b4c50
int HapiBank::SeekBoxEnd()
{
    BankAccount* e = &bank->accounts[bank->current];
    SafeDepositBox* it = &e->boxes[e->currentBox];
    it->pos = it->size;
    return it->pos;
}

// Reads up to len bytes from the current box into dst and advances its
// position.
// FUNCTION: 0x4b4c80
int HapiBank::ReadBox(void* dst, int len)
{
    SafeDepositBox* c = &bank->accounts[bank->current].boxes[bank->accounts[bank->current].currentBox];
    int avail = c->size - c->pos;
    if (avail <= 0) {
        return 0;
    }
    if (len > avail) {
        len = avail;
    }
    memcpy(dst, c->data + c->pos, len);
    c->pos += len;
    return len;
}

// Writes len bytes from src at the current box's position, growing the box
// as needed, and advances the position.
// FUNCTION: 0x4b4cf0
int HapiBank::WriteBox(void* src, int len)
{
    SafeDepositBox* c = &bank->accounts[bank->current].boxes[bank->accounts[bank->current].currentBox];
    int need = len + c->pos;
    if (need > c->size) {
        c->data = (char*)FUN_004d8580(c->data, need);
        c->size = need;
    }
    memcpy(c->data + c->pos, src, len);
    c->pos += len;
    return len;
}

// FUNCTION: 0x4b4d70
void HapiBank::WriteAuditFile(char* filename)
{
    FILE* file = fopen(filename, "w");
    int i;

    if (file == 0) {
        return;
    }

    fprintf(file, "HapiBank Audit File\n\n");
    fprintf(file, "Number of accounts: %i\n\n", bank->count);

    for (i = 0; i < bank->count; i++) {
        BankAccount* account = (BankAccount*)((char*)bank->accounts + i * 0x18);
        int j;

        fprintf(file, "Account:  \"%s\"\n{\n", account->name);
        for (j = 0; j < account->itemCount; j++) {
            BankItem* item = (BankItem*)((char*)account->items + j * 0x10);

            switch (item->type) {
            case 1:
                fprintf(file, "   Item \"%s\" Integer:  %i (0x%08x)\n", item->name, item->value, item->value);
                break;
            case 2:
                fprintf(file, "   Item \"%s\" Double:  %f\n", item->name, item->real);
                break;
            case 3:
                fprintf(file, "   Item \"%s\" String:  \"%s\"\n", item->name, item->string);
                break;
            default:
                fprintf(file, "   Item \"%s\" Uninitialized or Unknown\n", item->name);
                break;
            }
        }

        fprintf(file, "\n");
        for (j = 0; j < account->boxCount; j++) {
            SafeDepositBox* box = (SafeDepositBox*)((char*)account->boxes + j * 0x14);

            if (box->named != 0) {
                fprintf(file, "   Safe-Deposit Box \"%s\" contains %i bytes\n", box->name, box->size);
            } else {
                fprintf(file, "   Safe-Deposit Box #%i contains %i bytes\n", box->number, box->size);
            }
        }
        fprintf(file, "}\n\n");
    }

    fprintf(file, "----------EOF----------\n");
    fclose(file);
}
