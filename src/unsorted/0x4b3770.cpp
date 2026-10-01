// Decompiled by GPT-5.6-Terra, finished by space-bunny-free and deepseek-v4.1-flash. Names are provisional.
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
//                        only survives because &img escapes to FUN_004b4270)
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
//     bank name, param3 goes to FUN_004b4270).  Reusing one variable for both
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
//    results cast and FUN_004b4270's second parameter declared `char**`);
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
#include <string.h>
#include <stdio.h>

struct Table_004b3630 {
    int count;
    void* slots;
};

class Class_004b3630 {
public:
    Table_004b3630* table;
    void FUN_004b3630();
};

struct File_004bb5d0;

void* __stdcall FUN_004bb5b0(void* param1);
int __stdcall FUN_004bb5d0(File_004bb5d0* file);
long __stdcall FUN_004bb710(File_004bb5d0* file, long pos);
long __stdcall FUN_004bb7a0(File_004bb5d0* file);
void __stdcall FUN_004bb7c0(File_004bb5d0* file, void* buf, int size);
long __stdcall FUN_004bbd00(File_004bb5d0* file);
void* __cdecl FUN_004d8450(unsigned int size);
void* __cdecl FUN_004d8460(unsigned int count, unsigned int size);
void* __cdecl FUN_004d8580(void* ptr, unsigned int size);
void __cdecl FUN_004d85a0(void* p);
int __stdcall FUN_004d1b40(unsigned char* src);
int __stdcall FUN_004d1970(void* dest, void* source);
void* __stdcall FUN_004d1c60(int code);
void __stdcall FUN_004b6290(char* message);
int __cdecl _strcmpi(const char* s1, const char* s2);
int __cdecl sprintf(char* buf, const char* fmt, ...);

// The 0x8-byte image handle the original keeps at the bottom of its frame and
// hands by address to FUN_004b4270.
struct Image_004b3770 {          // 0x08 bytes
    void* buf;                   // +0x00
    int size;                    // +0x04
};

// The 0x24-byte bank header. 0x22 bytes of it are read from the file.
struct Header_004b3770 {         // 0x24 bytes
    char magic[8];               // +0x00 "HAPIBANK"
    int nameoff;                 // +0x08, of the bank name inside the image
    int dataoff;                 // +0x0c, end of the compressed data
    int seekoff;                 // +0x10, of the section index
    int version;                 // +0x14
    char compressed;             // +0x18
    char unknown_19[0x0b];
};

class Class_004b3770 : public Class_004b3630 {
public:
    char unknown_4[4];
    int field_8;

    void FUN_004b4270(File_004bb5d0* file, void** buf, void* arg3);
    int FUN_004b3770(char* filename, char* name, void* arg3);
};

// FUNCTION: 0x4b3770
int Class_004b3770::FUN_004b3770(char* filename, char* name, void* arg3)
{
    Image_004b3770 img;
    void* raw;
    Header_004b3770 h;
    char errmsg[0x80];
    File_004bb5d0* file;
    long remaining;
    long pos;

    file = (File_004bb5d0*)FUN_004bb5b0(filename);
    if (file == 0) {
        return 0;
    }

    FUN_004bb7c0(file, &h, 0x22);
    if (strncmp(h.magic, "HAPIBANK", 8) != 0) {
        FUN_004bb5d0(file);
        return 0;
    }

    if (h.version != 1) {
        FUN_004bb5d0(file);
        return 0;
    }

    img.buf = 0;
    img.size = 0;
    remaining = FUN_004bbd00(file) - h.dataoff;
    FUN_004bb710(file, h.dataoff);

    if (h.compressed != 0) {
        void* src = FUN_004d8450(remaining);
        int dsize;
        int err;

        FUN_004bb7c0(file, src, remaining);
        dsize = FUN_004d1b40((unsigned char*)src);
        raw = FUN_004d8450(dsize);
        err = FUN_004d1970(raw, src);
        if (err != 0) {
            sprintf(errmsg, "[HapiBank::OpenBank] Decompression Error: %s",
                    (char*)FUN_004d1c60(err));
            FUN_004b6290(errmsg);
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
        FUN_004bb7c0(file, img.buf, remaining);
    }

    if (name != 0) {
        if (_strcmpi(name, (char*)img.buf + h.nameoff) != 0) {
            FUN_004bb5d0(file);
            if (img.buf != 0) {
                FUN_004d85a0(img.buf);
            }
            return 0;
        }
    }

    FUN_004b3630();
    table = (Table_004b3630*)FUN_004d8460(1, 0xc);
    ((int*)table)[2] = -1;

    FUN_004bb710(file, h.seekoff);
    pos = FUN_004bb7a0(file);
    if (pos < h.dataoff) {
        do {
            FUN_004b4270(file, &img.buf, arg3);
            pos = FUN_004bb7a0(file);
        } while (pos < h.dataoff);
    }

    FUN_004bb5d0(file);
    if (img.buf != 0) {
        FUN_004d85a0(img.buf);
    }
    return 1;
}
