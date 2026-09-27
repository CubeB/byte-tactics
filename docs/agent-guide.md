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
  fixed (`/O2 /Ob2 /MT`: `/Ob2` means the compiler inlines small
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
  that `ctx.py` shows without a name is always `Class_<callee address>::FUN_<callee address>`,
  the same name its own author will give it. If your object has a different
  class, cast: `((Class_00437a20*)obj)->FUN_00437a20()`. Once a callee has a
  name in `data/symbols.csv`, `ctx.py` shows it and you must use it.
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
- **`xor eax, eax` then a byte/word load into `al`/`ax`**: declare
  `unsigned int result = 0;` before the load and assign into it
  (`result = *(unsigned char*)p;`). A plain `return *(unsigned char*)p;` loads
  with a different register choice.
- **Global object vs. pointer**: `mov ecx, <addr>` passes the address of a
  global object (`extern Class_x DAT_...;`, call with `.`); `mov ecx, [<addr>]`
  loads a global pointer (`extern Class_x* DAT_...;`, call with `->`). The
  same goes for vtables and tables: storing the address itself needs an array
  declaration (`extern void* DAT_...[];`).
- **Never add your own `if (n > 0)` guard around a loop**: MSVC rotates a plain
  `for` loop into test-at-bottom form and adds that single guard itself; a
  guard in the source makes the test appear twice.
- **A pointer chain loaded after an index multiply** (`[edx + eax + disp]`,
  with `obj->a->b` read after `i * size` is computed): wrap the chain in a
  `static inline` getter and index its result, `GetEntries(obj)[i].field`.
- **Embedded structs at odd offsets**: a struct embedded at an offset that is
  not a multiple of 4 needs `#pragma pack(push, 2)` (or 1) on the outer struct.
  A `mov eax, edx; cmp eax, K` right after a store means the source re-reads
  the field it just assigned (typical inside an inline method on that struct).
- **Structs returned by value**: a callee returning a struct has a hidden first
  argument (the return buffer), so `ctx.py` shows one more argument dword than
  it really has, and a function that returns a struct returns `eax` = that
  pointer. Declare the real return type (`Vec3 __stdcall f(Obj*, int)`).
- **Structs passed by value**: a plain struct is pushed dword by dword; a class
  with a user-defined copy constructor is built in place
  (`sub esp, 8; mov eax, esp; mov [eax], ...`).
- **Callee types**: when a callee already has a name, look for its file in
  `src/unsorted/` and copy its parameter types (for example a `char`
  parameter), since they decide how arguments are prepared.
- **Bit tests**: a single-bit test on a byte folds to `test byte ptr [m], mask`;
  `shr reg, N; test al, 1` means a bitfield in a wider (`int`) field. Setting
  a bit in a dword bitfield is `mov eax, [m]; or al, K; mov [m], eax`.
- **`abs()`**: the `cdq; xor eax, edx; sub eax, edx` idiom is `abs()` from
  `<stdlib.h>`; a hand-written `if (x < 0) x = -x;` compiles differently.
- **Keep notes above the annotation**: put comments before the
  `// FUNCTION:` line, not between it and the definition.

## Library and STL idioms (write the call, not the loop)

MSVC inlines these, so the disassembly shows a loop, but the source was a
single call:

- **`strcmp`**: a byte-compare loop unrolled by 2 ending in
  `sbb eax, eax; sbb eax, -1`. `memcmp`: `repe cmpsb` after `xor edx, edx`.
  `strlen`: `repne scasb` with `or ecx, -1`. Call the function with
  `const char*` arguments.
- **`vec.empty()`**: `sete al; ... and eax, 0xff` on a value that is 0 when
  `_First` is null, else `(_Last - _First) / sizeof(T)`.
- **`vector::erase(first, last)` out of line**: `eax` = the first argument, and a
  dead `mov [esp+8], <old _Last>` just before `ret 8` (left by the inlined
  `_Destroy`). See `src/unsorted/0x40cfb0.cpp` and `0x40c9f0.cpp`. For vectors of
  pointers, define the pointed-to struct (MSVC 5's `<xmemory>` needs it).
- **`while (n--)`**: `mov esi, ecx; dec ecx; test esi, esi; je`, then
  `lea esi, [ecx+1]` inside the guarded block.
- **Widened returns**: `and eax, 0xff` before `ret`, after a `sete al` or a
  byte loaded into `al`, means the function returns `int` holding a `bool` or
  `unsigned char`; the return type is `int`.

## When your version is "more optimised" than the original

It never is. If MSVC merges two branches, hoists a load, rotates a loop or
peels an iteration that the original didn't, the original source contained
something the optimiser saw early that leaves no trace in the final code.
Look for it instead of blaming the compiler:

- **Re-check the semantics first.** An off-by-one start (scanning from
  `strlen(s)`, not `strlen(s) - 1`) or a return value you dropped can look
  exactly like an optimisation difference.
- **Paths kept apart that you merge**: give them different return values
  (`return 0;` costs nothing when `eax` already holds 0), or add a no-op
  conversion; with x87 code a `(float)` cast of a double emits nothing but
  changes operand order and stops a load being hoisted.
- **A loop tested at the top with a `jmp` back from every branch**: the loop
  condition was an inlined helper with one `return` per outcome
  (`static inline int NotDone(...) { if (a == b) return 0; return 1; }`).
  A plain `while` gets rotated and its identical branches merged.
- **A global "vector" whose atexit destructor has no destroy loop** (no
  `push ecx`/dead store): a vector-shaped custom container, not `std::vector`.
  See `src/unsorted/0x438450.cpp` and `0x438480.cpp`.

## Saving check.py runs

`tools/wcl /c /O2 /Ob2 /MT /Fa<file>.asm <file>.cpp` compiles a scratch file
and writes an assembly listing you can read directly; iterate that way, then
confirm with one `check.py` run.
- **Empty functions called with a format string** are debug-print stubs whose
  body was compiled out: declare and define them variadic,
  `void FUN_x(const char* fmt, ...)`.
- **A function that "writes `*p = x`" but keeps `p` out of `eax` until the
  end** returns `p` (`return p;`), like an assignment operator.
- **Windows API calls** (`call [0x4fc0e0]` that `ctx.py` labels "import Sleep from
  KERNEL32.dll"): include `<windows.h>` (or `<mmsystem.h>` for sound APIs) and
  call the function normally. Never declare an import slot as a `DAT_` global;
  the checker knows every import by name.
- **Calls through a game global holding a function pointer** (`call [DAT_x]`
  where `DAT_x` is not an import): declare it with its real type,
  `extern void (__stdcall* DAT_x)(int);`, and call through it. A table of them
  is an array of function pointers.
- **A call through a vtable** (`mov eax, [ecx]; call [eax+N]`) is a C++
  virtual call: declare a class with virtual methods (N/4 slots) and call the
  method; a hand-cast function pointer moves `this` to the wrong register.

## The STL and C++ exceptions

Cavedog compiled without `/GX` (no C++ exception handling), and so does the
checker. Use the real MSVC 5 STL headers (`<vector>`, `<map>`, `<string>`,
`<list>`): a local `std::_Lockit` or a `std::string` compiles without an
exception frame, exactly as in the original. Code from the C++ library itself
(`std::string` internals, `_Lockit`, the std exception classes) is marked
`library` in `data/functions.csv` and needs no decompiling; call it by its real
name (`std::_Lockit::_Lockit` is 0x4e39b0).
- **Arguments loaded in the wrong order or registers**: copy them into locals
  just before the call; the order of those copies decides which load MSVC
  hoists.
- **A pointer stored, offset, and stored again** (`lea ecx, [eax+K]; mov [..], ecx;
  add ecx, esi`): use one pointer local updated with `+=`; two separate
  expressions let MSVC fold the offset into a fresh `lea`.
- **A callee whose result is used as a full `int`** even though its own file
  returns `unsigned short`/`char`: declare it returning `int` in your file (the
  checker compares names, not types); the narrower type adds a mask the
  original lacks.
- **Forcing a field to be re-read**: MSVC 5 reuses an already-loaded field only
  when it is read through the same pointer temporary. When the original
  re-reads fields it just tested, compute the pointer again into a second local
  (`q = &g_game->players[i];`).
- **Negative `this` offsets** (`[ecx-8]`) in a function with no direct callers:
  it overrides a virtual function of a non-primary base class, and `this` points
  at that base subobject. Write the real multiple-inheritance class.
- **Function-local statics**: a guard-byte test, a constructor call on a global,
  then `atexit` of an empty function is `static T x(args);` inside the function,
  where `T` has an empty inline destructor.
- **Two copies of one function**: the exe links two identical copies of
  `std::_Lockit` (0x4e39b0 and 0x4e1480). `data/aliases.csv` lists such
  duplicates, and the checker accepts either address for the name.
- **Base constructor inlined into a derived constructor**: a store to a field
  (e.g. +4) before the vtable store is the base's inline constructor (its own
  vtable store is dead and disappears), followed by the derived class storing
  its vtable. Declare the base constructor inline in the class
  (see `src/unsorted/0x44d010.cpp`).
- **Freeing and zeroing several {_First,_Last,_End} triples, last member first**:
  the empty destructor of a class with `std::vector` members.
- **A per-element call inside an inlined vector destroy loop**: the element type
  has a destructor that makes that call; write the element class with
  `~Elem() { FUN_x(this); }` (or, as in `0x434020.cpp`, an overload of
  `std::_Destroy` for the element type).
- **Copy matched siblings first**: look for already-matched neighbours that use
  the same inlined helper and copy it verbatim; small phrasing differences
  (`int r = f(); if (!r)` vs `if (!f())`) change the whole function's registers.
- **Two ways to write `== 0`**: `return x == 0 ? 1 : 0;` gives
  `xor edx, edx; test; sete dl; mov eax, edx`; `return x == 0;` gives
  `neg; sbb; inc`.
- **Zero-init order**: a chained `a = b = c = d = 0;` initialises right to left.
- **Stop early on operand-order-only differences**: when the only difference is
  which of two loads in a commutative `a + b` (or `x ^ y`) comes first, and
  swapping operands or wrapping them in helpers changes nothing, the cause is
  compiler state left by earlier functions in the original source file. It
  cannot be fixed from this function's source; say so in your notes and move
  on. Never add unused code to change that state.
- **Operand order that no rewrite changes can depend on the headers**: which
  operand of a commutative integer or x87 operation MSVC loads first can depend
  on how many declarations the file has seen. If nothing else works, try
  including the headers a real game file would have
  (`<windows.h>`, `<stdio.h>`, `<string.h>`, `<math.h>`) at the top. There was no
  single header set shared by every file, so only add them where they help.
- **A fresh loop variable**: when an inlined helper shifts array entries down
  from index `i` and the original copies `i` into a new register before the
  loop, write `for (int j = i; ...)`; reusing the parameter swaps which
  register holds the counter and which the destination pointer.
- **Struct copy vs field copies**: assigning a whole 16-byte struct member
  emits `lea eax, [esi+8]` and stores relative to `eax`; four field assignments
  give direct `[esi+8]..[esi+0x14]` stores.
- **Families of functions**: look for matched functions of the same shape (for
  example the pool allocators 0x4ddce0/0x4ddc00: GlobalAlloc 0x2000 plus the
  out-of-memory handler) and copy them, changing only sizes and globals.
