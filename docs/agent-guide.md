# Decompiling a function: guide for agents

You are turning functions from Total Annihilation's `TotalA.exe` (1998, compiled
with Microsoft Visual C++ 5.0 SP3) back into C++ that compiles to **exactly the
same machine code**. A result only counts when `tools/check.py` prints `MATCH`;
every claimed match is re-verified independently.

Work from the repository root: `~/repos/personal/byte-tactics`.

## The loop, per function

1. `uv run tools/ctx.py 0x<addr>`: disassembly annotated with names, strings and
   float constants, facts about arguments and calling convention, what each
   callee expects, and Ghidra's pseudo-C (a rough starting point; its types and
   control flow are often wrong).
2. Write `src/unsorted/0x<addr>.cpp` (lower-case hex, e.g. `0x4010b0.cpp`).
3. `uv run tools/check.py 0x<addr>`: compiles your file and prints `MATCH`, or a
   similarity % with a diff (`-` lines are the original, `+` lines are yours).
4. Adjust and repeat. Stop at `MATCH`, or when you run out of attempts for that
   function; leave your best (highest %) version in the file either way.

## Rules

- Only create or edit `src/unsorted/0x<addr>.cpp` for the addresses you were
  given. Do not touch anything else (tools, data, include, other files), do not
  run `tools/progress.py`, and do not commit.
- No inline assembly or byte emission (`__asm`, `_emit`) and no
  `#pragma optimize`/`code_seg`; the checker rejects them. Compiler flags are
  fixed (`/O2 /Ob2 /GX /MT`: `/Ob2` means the compiler inlines small
  functions on its own); do not try to change them.
- Each file must compile on its own: define the structs/classes you need in the
  file, and declare (don't define) the functions and globals you call or use.

## File template

```cpp
// Decompiled by <model>. Names are provisional.
#include <string.h>   // only what you need

struct Unit;                         // opaque when only passed around

class Weapon {                       // fields at the offsets the code uses
public:
    char unknown_0[0x10];
    int damage;                      // +0x10
    int FUN_004b4ba0(char* name);    // callee declared, not defined
    void FUN_00401234(Unit* target);
};

extern int DAT_00511de8;             // global, declared extern

// FUNCTION: 0x401234
void Weapon::FUN_00401234(Unit* target)
{
    ...
}
```

The `// FUNCTION: 0x<addr>` line must sit directly above the definition.

## Names

- If `ctx.py` shows a name for a callee or global (anything other than
  `FUN_...`/`DAT_...`), use exactly that name, including the class
  (`PlayerRef::Reset`); the checker fails references that disagree with names
  already established.
- Otherwise use `FUN_<8 hex digits>` for functions and `DAT_<8 hex digits>` for
  globals, e.g. `FUN_004b4ba0`, `DAT_00511de8`. Name your own function
  `FUN_<addr>` too unless its purpose is obvious.
- If your function is a method and its class has no known name yet, call the
  class `Class_<8 hex digits of your function's address>`, e.g.
  `Class_00401234::FUN_00401234`.
- A callee called as a method (ecx set to an object just before the call)
  that `ctx.py` shows without a name: declare it as a method of the class you
  use for that object, named `FUN_<callee address>`. If you have no class for
  that object either, use `Class_<callee address>`. Once a callee has a name
  in `data/symbols.csv`, `ctx.py` shows it and you must use it.
- A function that is only `ret` or `ret N` is an empty function: an empty body
  with N/4 dword-sized parameters (as a `__thiscall` method if unsure).
- Library calls (`sprintf`, `memset`, `strcpy`, `malloc`, ...) are the normal C
  runtime; include the header and call them.

## Reading the calling convention

- `ctx.py` says how the function returns. `ret N` means the callee removes N bytes
  of arguments: a `__thiscall` member function (if `ecx` is used as a pointer
  before being written) or a `__stdcall` free function. Plain `ret` means
  `__cdecl`, or a `__thiscall` method with no stack arguments.
- The FPO line gives the number of stack argument dwords.
- `__thiscall` is only available for member functions: make it a method of a
  class/struct. A function that only uses `ecx` (not `edx`) as an input
  is a `__thiscall` method, not `__fastcall`: both compile the same, but
  Cavedog wrote methods, and the name you choose is what callers will use.
  Free functions default to `__cdecl`; write `__stdcall`
  explicitly when needed.

## Getting MSVC 5 to produce the same code

- Types matter. `movzx`/`and reg, 0xff` loads mean `unsigned char`/`unsigned
  short`; `movsx` means signed. `shr`/`jb`/`ja` mean unsigned; `sar`/`jl`/`jg`
  mean signed. A `short` or `char` parameter still occupies a dword slot.
- Field offsets come from `[reg + 0x..]`; pad structs with `char unknown_X[N]`
  arrays so each field lands at the right offset.
- Register choice and instruction order follow the order of your statements and
  of declarations. Try: reordering statements, introducing or removing a
  temporary variable, swapping comparison operands (`a < b` vs `b > a`),
  `if/else` vs `?:`, `for` vs `while` vs `do/while`, early `return` vs one exit,
  pre vs post increment, array indexing vs pointer arithmetic.
- `rep stosd`/`rep movsd` sequences are usually `memset`/`memcpy` (inlined at
  `/O2`), or struct assignment.
- A `switch` usually becomes a jump table or a compare chain.
- x87 code (`fld`, `fstp`, `fmul`...) is `float`/`double` arithmetic; `ctx.py`
  shows constant values. `__ftol` calls are float-to-int casts.
- Small callees may have been inlined in the original, so their bodies appear
  in the disassembly without a `call`.

## When you finish

End your reply with one table row per function you were given:

```
| address | result | best % | check.py runs | notes |
| 0x401234 | MATCH | 100 | 3 | needed unsigned char param |
| 0x401260 | partial | 87.5 | 12 | register swap in loop I could not fix |
```

Keep notes short and specific: what made it hard, what fixed it.

## Patterns already solved in this game

Check these before fighting the compiler; each one has cost earlier agents
their whole budget.

- **`push ecx` as the first instruction** usually just reserves 4 bytes of
  stack for a local variable. It is not saving an argument, and `ecx` is not
  necessarily a parameter.
- **`mov eax, ecx` near the start, `this` returned in `eax`**: a C++
  constructor (constructors return `this`), or a method that returns `this` /
  `*this`. Write it as a real constructor, `Class::Class(...)`, or as a method
  returning `Class*`/`Class&`. `ctx.py` prints a hint when it sees this.
- **Storing a `.rdata` address into `[this]`**: the vtable pointer. `ctx.py`
  marks such addresses `vtable? [...]`. Declare the class with virtual methods
  (declared, not defined) and write the constructor; the compiler stores the
  vtable itself. Don't assign it by hand.
- **A global `std::vector`**: a function that copies one byte from an
  uninitialised stack slot (`push ecx; mov al, [esp+3]`), zeroes the next three
  dwords of a global, then calls `atexit` is the compiler-generated
  initialiser for `std::vector<T> global;`. See `src/unsorted/0x438450.cpp`.
  Compiler-generated functions have no definition to annotate, so put the
  symbol after the address: `// FUNCTION: 0x438450 _$E5`.
- **Division by a constant** compiles to a multiply by a "magic" number plus
  shifts. Write the plain division (`x / 48`); if registers or the shift
  sequence differ, the signedness of `x` is usually wrong (`int` adds a sign
  fix-up, `unsigned` doesn't). A guarded division such as "return 0 if the count is
  zero, otherwise a difference divided by 48" matched only when written as one
  ternary, `return n == 0 ? 0 : (b - a) / 48;`, not as an early `return 0`.
- **Loop compares**: `jbe`/`jae` in a loop test means the counter is
  `unsigned`; `sete dl; test dl, dl` means the result of a comparison was
  stored in a `bool` local first.

## When the registers or the order won't budge

Cavedog wrote many small helper functions and methods, and `/Ob2` inlined
them. An inlined function boundary changes the order MSVC evaluates things in
and which registers it keeps values in, so when source-level shuffling has no
effect, the missing piece is usually a helper that was inlined:

- If swapping the operands of `this->a + this->b` changes nothing, move the
  expression into a small `static inline` helper that takes the object
  pointer (`MidX(this)` doing `w->x1 + w->x2`); MSVC then keeps the source
  order. See `src/unsorted/0x44dc60.cpp`.
- A value that sits in a scratch register on one path, and is copied into
  place (`mov edx, ebp`) just before the paths merge on the other, is the
  return value of an inlined function with one `return` per path. A local
  assigned on both paths gets a callee-saved register for the whole function
  instead. See `src/unsorted/0x4c9290.cpp`.
- A loop that walks a pointer, where the offset is added after the loop
  guard (`add eax, K` after `test/jle`), is plain array indexing
  (`arr[i].field`) in the source; adding the offset yourself moves the `add`
  before the guard. A `cmp ptr, end; jl` loop over a global array is a signed
  `int i` for-loop that MSVC turned into a pointer loop.
- For `imul reg, [mem]`, the register operand is the left side of `*` in the
  source.
- **`ret N` with no matching stack reads**: the function has unused trailing
  parameters. Declare them (`int unused`) instead of fighting the cleanup.
- **Return types**: `mov al, cl` at the end means a `bool`/`char` return;
  `mov eax, ecx` means `int`.
- **Bit toggles**: a `not`/`xor`/`and`/`xor` sequence on one bit is
  `f->flag = !f->flag;` on a 1-bit bitfield. Place the bitfield so the bit
  lands where the mask says (mask 0x8 is bit 3).
- **x87 sums in the wrong order**: write the sum as sequential accumulation
  (`r = a*d; r = r + b*e; r = r + c*f;`) so the compiler cannot reassociate it.
- **MSVC STL templates**: a `__thiscall` that loops from a pointer argument to
  `[ecx+8]` and then stores into `[ecx+8]` is probably `std::vector<T>::erase`
  (members `_First` +4, `_Last` +8, `_End` +0xc). To make the compiler emit the
  template out of line, take its address in a global
  (`EraseFn g = &std::vector<T>::erase;`) and put the mangled symbol after the
  address in the `// FUNCTION:` line. See `src/unsorted/0x40cfb0.cpp`.
- **Addresses are always symbols**: never write an address as a number (a
  vtable, string, global or function). Declare it (`extern void* DAT_004fd458[];`,
  a string literal, `extern Class_x DAT_00528a78;`) and use the name. The
  checker rejects hard-coded addresses.
- **`mov ecx, <global>; jmp <method>`**: a tail call of a method on a global
  object. Declare the object (`extern Class_x DAT_00528a78;`) and write
  `DAT_00528a78.FUN_004e1650();`. See `src/unsorted/0x4de0f0.cpp`.
- **Locals in parameter slots**: MSVC 5 reuses the stack slot of a parameter
  that is no longer needed for a local. When the code writes into a
  parameter's slot (a buffer, an output value), declare an ordinary local and
  the compiler puts it there itself.
- **Keeping a narrow computation where it is**: if the original computes
  `add cl, 0x3f; shl cl, 2` before a test and yours folds it into the branch,
  compute it in separate statements on an `unsigned char` local
  (`h = n + 0x3f; h <<= 2;`).
- **Search loops ending in `or reg, -1` then `cmp reg, -1`**: an inlined
  helper returning an index or -1. Write it as a `static inline` function with
  an early `return i;`.
