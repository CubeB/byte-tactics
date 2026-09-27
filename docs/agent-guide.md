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
   Addresses the linker fills in show as `<addr>` in the diff; in your own
   `/Fa` listings or object file they appear as 0 or as a symbol name. That is
   normal and not a bug in your declarations.
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
- A callee that is a **constructor** (called on the result of `operator new`,
  or one that stores a vtable and returns `this`) is named as a constructor,
  `Class_<addr>::Class_<addr>`, because that is what its own author will call it.
- A function that is only `ret` or `ret N` is an empty function: an empty body
  with N/4 dword-sized parameters (as a `__thiscall` method if unsure).
  If every caller sets `lea ecx, [esp+N]` to a local object just before calling
  it, at the end of that object's scope, it is that class's empty out-of-line
  destructor (see 0x4e2cb0.cpp); check with
  `objdump -d -M intel orig/TotalA.exe | grep -B14 "call   0x<addr>"`.
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

If the code you matched looks like a mistake in the original game (a wrong
allocation size, a read through a null pointer, a result that can never be
true, a field written twice), keep it exactly as the original does, explain it
in a comment in your file, and list it under a "Suspected original bugs"
heading after your table, with the address and your evidence. The orchestrator
records these in `docs/bugs.md`.

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
  vtable itself. Don't assign it by hand. The checker accepts the compiler's
  vtable name (`??_7Class@B@`) even where an earlier file named that address
  `DAT_...`: a `DAT_<address>` placeholder agrees with any real name for the
  same address, and the real name then replaces it.
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
- **A `std::vector` member starts 4 bytes before its `_First`**: the empty
  allocator byte sits at +0, padded to 4, even inside a `pack(1)` class (the
  header's own packing applies). When the original re-reads `_First` after an
  inlined `size()`, use a real `std::vector` member rather than raw pointer
  fields, which let the compiler reuse the loaded value (see 0x4c45e0.cpp).
- **A matched sibling's wording can fail once inlined**: a helper phrasing that
  matches out of line may allocate registers differently when inlined into a
  bigger function; try the plainest form (`if (i >= N) i = 0; return v;`).
- **Addresses are always symbols**: never write an address as a number (a
  vtable, string, global or function). Declare it (`extern void* DAT_004fd458[];`,
  a string literal, `extern Class_x DAT_00528a78;`) and use the name. The
  checker rejects hard-coded addresses.
- **`g_game` (0x511de8) is a pointer**: `mov eax, [0x511de8]` loads it, then
  fields are read at `[eax+N]`. Declare `extern char* g_game;` (or a struct
  pointer) and write `*(int*)(g_game + N)`. Never `&DAT_00511de8 + N`: that is a
  constant address with no load, and can never match.
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

The per-function budget in your instructions counts real attempts. Scoring a
scratch file with `uv run tools/check.py <addr> <scratch.cpp> --sym <name>`
(or checkall.py) is cheap and fine to repeat; there is no need to write your
own objdump-normalising diff script, since check.py's diff already masks
addresses.

Several agents work at once, so keep scratch files in your own folder:
`build/scratch/<your first address>/` (for example `build/scratch/0x4635b0/`),
never a shared name like `/tmp/a.cpp`.

`tools/wcl /c /O2 /Ob2 /MT /Fa<file>.asm /Fo<file>.obj <file>.cpp` compiles a scratch file
and writes an assembly listing you can read directly; iterate that way, then
confirm with one `check.py` run. `uv run tools/checkall.py <addr> <addr> ...` checks many
functions at once (in parallel, one summary line each), which suits a batch of
small functions.
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
  `neg; sbb; inc`. `neg; sbb; neg; dec` (0 or -1) is `return p ? 0 : -1;`.
- **Zero-init order**: a chained `a = b = c = d = 0;` initialises right to left.
- **Operand order that nothing changes**: only when the single remaining
  difference is which of two loads in one commutative `a + b` (or `x ^ y`) comes
  first, and you have tried swapping operands, helpers and the header block
  below, is the cause compiler state from earlier functions in the original
  file; say so and move on. This is rare. Ordinary register differences
  (a different register for a value, different instruction order elsewhere)
  are almost always fixable from the source: keep using the techniques above.
  Never add unused code to change the compiler state.
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
- **Inlined `std::map::find`**: declare the lower-bound callee as returning a
  node pointer and wrap it in a small iterator class (returning the iterator by
  value adds a hidden return pointer). `cmp; sbb; neg; test al, al` needs a
  `less`-style functor with `bool operator()`; `(p == End() || cmp(...)) ? End() : p`
  gives the `lea eax, [temp]` selection. See `src/unsorted/0x46e330.cpp`.
- **Assigning to a 1-bit bitfield**: an `int` value gives `xor/and 1/xor`; a
  `char` value gives `and/or`.
- **An argument `push` in the middle of a run of field stores**: MSVC hoists the
  push to just after the last inlined constructor before the call, so the stores
  before it came from member objects' inline constructors. Split those fields
  into member structs with inline constructors (see `0x4635b0.cpp`).
- **The same argument setup on both sides of a branch, then a jump to one
  call**: the source called one inlined helper in both branches of an if/else
  and MSVC merged the tail. A ternary argument gives a single push sequence.
- **Calls into "gap" regions** (hand-written assembly, e.g. the fixed-point trig
  routines at 0x4b70a0-0x4b7200): run `ctx.py` on the gap start to read the
  routine, take argument types from it (`movsx` of a word means `short`), and
  declare it `__cdecl FUN_<addr>`.
- **DirectX**: `<ddraw.h>`, `<dsound.h>` and `<dplay.h>` are available; a COM
  call (`call [ecx+N]` with the interface pointer pushed) is the real interface
  method, e.g. `IDirectDrawPalette::SetEntries`. The toolchain's `<dplay.h>` only
  has DirectX 3's `IDirectPlay`; the game's `IDirectPlay2`/`3` calls (e.g.
  `SetPlayerData` +0x74, `EnumConnections` +0x8c) need the interface declared by
  hand with padding slots, as in `0x4ca250.cpp` and `0x4c9d30.cpp`.
- **Siblings first**: unnamed functions next to a matched one often differ only
  in a string literal or a constant (a "METAL" version next to an "ENERGY"
  one), so check neighbouring addresses in `src/unsorted/` before starting.
- **Registers swapped in `base + index * size`**: try writing the full
  `obj->a->arr[i].field` expression each time it is used; a shared
  `Entry* e = &...[i]` local or getter changes which register holds the base.
- **Read constants from the exe** to learn what a function does, e.g. a 16-byte
  `.rdata` value compared with `memcmp` may be a DirectPlay service-provider GUID.
- **Scalar deleting destructors** (call the destructor, `operator delete(this)`
  if `flag & 1`, return `this`): call the destructor by its real name,
  `((Base*)this)->~Base();`, so it agrees with the destructor's own file.
- **Sizes pushed to `new` that are not multiples of 4** (e.g. 0x36): the class
  needs `#pragma pack(push, 2)` or MSVC rounds `sizeof` up.
- **The STL source is local**: `toolchain/msvc5-sp3/INCLUDE/XTREE`, `VECTOR`,
  `XSTRING` and friends show exactly where locks and helpers sit in inlined STL
  code (e.g. `lower_bound` is `iterator(_Lbound(k))`, and `_Lbound` takes the lock).
- **A parameter loaded into `ecx` early, with other registers used for the
  pointer chain**: a later callee is a `__thiscall` method on that parameter,
  even when `ecx` is set long before the call.
- **Adjacent `a += b` field updates whose last store is not sunk past a later
  load**: an inlined `operator+=` on an embedded vector struct.
- **Lazy singletons**: `if (!g) g = new T; return g;`. A failed-allocation path
  doing `xor eax, eax; mov [g], eax` means the global is returned; a
  `GlobalAlloc` null check around constructor stores is `new` with a class
  `operator new` (see `0x4da9f0.cpp`).
- **A small struct field stored to the stack and re-read as a dword before an
  add**: C-style inline helpers that take and return the struct by value
  (`MakePoint(x, y)`, `AddPoints(a, b)`), not constructors and `operator+=`.
- **`mov al, [m]; shr al, N; test al, 1` at an odd offset**: an `unsigned short`
  bitfield whose storage starts there, in a packed struct.
- **A parameter pointer loaded before the first branch** while yours loads it
  in each branch: take a reference to the field at the top
  (`int& m = obj->field;`).
- **Two pushes merging into one call** (`push edx; jmp L` / `L0: push imm` /
  `L: push ...; call`): an if/else calling the same function in both branches
  with one argument different.
- **Scalar deleting destructors that free through a pool** instead of
  `operator delete`: call the pool object's method (see `0x471cd0.cpp`).
- **Register priority**: when MSVC gives the preferred callee-saved register to
  the wrong variable, the original may have used the other variable once more
  in a way that folds away (e.g. an inlined sibling getter with its own range
  check inside an identical explicit check). A throwaway extra use in a scratch
  copy confirms the diagnosis; then find the natural construct, never commit
  the throwaway.
- **A callee that starts `mov eax, ecx` and ends `ret N`** is a method, usually a
  constructor, even if your call site happens to leave the right value in
  `ecx`. Declaring it as a free function can still produce matching bytes, but
  gives it a wrong name that later callers trip over.
- **Protected STL members out of line** (e.g. `vector::_Ucopy`): derive a struct
  from the container and initialise a static member pointer inside it,
  `Fn Access::fn = &Access::_Ucopy;`.
- **`or byte ptr [m], K` straight to memory** is setting a 1-bit
  `unsigned short` bitfield; `unsigned char` bitfields go through a register.
- **Loops with several induction variables**: which one MSVC compares against
  the end follows the order the per-iteration pointer locals are computed.
- **Bitfield test polarity**: `if (!bitfield)` compiles to
  `test byte ptr [m], mask`, while `if (bitfield)` (including
  `if (bitfield) return;`) gives `mov reg, [m]; shr reg, N; test reg, 1`.
- **x87 results stored back into argument slots** (`fstp [esp+0xc]`) before being
  copied to a return buffer: the argument is a struct passed by value
  (`Vec3 f(Vec3 v)`).
- **x87 loads one step early in a sum of squares**: compute each product into
  its own float local first.
- **When a match needs the function before it compiled first** (a loop guard
  gets its own copy of a call, or a tail merge differs, only in a file with no
  earlier function): define the real preceding function (`ctx.py` on the
  address just before yours) in the same file, above yours, with its own
  `// FUNCTION:` annotation. That is how the original file was laid out, so it
  is not a trick; never define made-up functions for this. See
  `src/unsorted/0x4b0830.cpp`.
- **Ordinal-only imports** (DPLAYX, smackw32) are called through `jmp [iat]`
  thunks; declare the real API with `extern "C" ... __stdcall`.
- **Calling a constructor callee on `this` first, then copying fields and
  returning `this`**: a copy constructor of a class whose first member has that
  constructor. Write it with a member-initialiser list,
  `X::X(const X& o) : handle(o.handle), a(o.a) {}`; a constructor cannot be
  called through a pointer. See `src/unsorted/0x437820.cpp` and `0x4b7e30.cpp`.
- **Ordinal imports called directly** (`call [iat]` into smackw32 or DPLAYX):
  declare the real API as `extern "C" __declspec(dllimport) ... __stdcall`;
  `Original().pe.DIRECTORY_ENTRY_IMPORT` shows which DLL and ordinal a slot holds.
- **`sete` after a call**: `return x == 0 ? 1 : 0;` gives `sete` only when `x`
  is a local; applied to a call result it folds to `neg/sbb/inc`, so store the
  result in an `int` first.
- **Function-local statics** (`static T x(...);` inside a function, with its
  `$S1` guard) are file-local names: the checker never compares them with
  other files, so name them naturally.
- **A byte local widened through its stack slot** (`mov [esp+X], cl;
  mov edx, [esp+X]; and edx, 0xff`) is an `unsigned char` local used in more
  than one basic block; index loops (`for (i = 0; text[i]; i++) { unsigned char
  c = text[i]; ... }`) give that shape where pointer-walking loops do not.
- **Avoid `volatile`**: a store that looks dead, often with a `push ecx`
  reserved slot, usually comes from inlined STL code (the destroy loop of a
  `std::vector` of a trivial type leaves exactly that). Try the real STL
  construct first; `volatile` is a last resort that Cavedog almost certainly
  did not write. A loop whose body compiles to nothing
  (`for (i = n - 1; i >= 0; i--) {}`) also leaves just the store of its
  counter's first value (0x458d20). An explicit member destructor call works
  as `member.~vector();` (MSVC 5 rejects `~TypedefName()`).
- **Check whether the caller uses `eax`**: when the original keeps a value in
  `eax` (or avoids `eax` in a loop and pushes `ebx` instead), the function
  probably returns that value; look for `mov reg, eax` after a call site.
  Returning it fixed 0x4d0c10 and 0x4800c0.
- **A `(float)` cast on a difference** can decide how a float struct result is
  copied to the return buffer, not just x87 order (0x4b6f70).
- **Placement-new copies with a null check** (`test esi, esi; je` then a copy
  constructor call on `esi`, `ret 8`) are `std::allocator<T>::construct`; emit
  it out of line by taking its address (see 0x432cf0.cpp).
- **Naming a vtable from RTTI**: the dword before a vtable points to the RTTI
  locator; locator +0xc points to the type descriptor, whose ".?AV...@@" string
  names the class. `<stdexcept>` classes are emitted by a static object of the
  class (see 0x4c38f0.cpp).
- **Zeroing a fixed int array**: a `for` loop gives `mov ecx, N; lea edi; xor
  eax, eax` for `rep stosd`; `memset` puts `xor eax, eax` before the `lea`.
- **An int call result stored into a `bool`**: `x ? true : false` gives
  `test eax, eax; setne al`; `x != 0` and `(bool)x` give `neg; sbb; neg`.
- **`fsub qword [-1.0]`** is `f += 1.0` (MSVC 5 adds 1.0 by subtracting -1.0).
- **A function that opens with a copy of a recursive callee's body**: `/Ob2`
  inlined one level of the recursion; write that level out by hand.
- **A `??_G` with the destructor inlined**: give the class an inline virtual
  destructor and add a static object whose constructor is only declared; MSVC
  then emits the vtable and the `??_G` (see 0x470ae0.cpp). Annotate the atexit
  destructor of a global as `_$E2` next to its `_$E4` (see 0x44f720.cpp).
- **Ghidra's return value can be a leftover**: when `eax` only holds what a
  final `idiv` or call left there and no caller reads it (`called from 0
  place(s)`, or callers ignore `eax`), the function returns `void` (0x47a8e0:
  `*p = (*p + 1) % n;`, not a quotient and remainder pair).
- **A pointer computed into `ecx` before a float argument's `push ecx; fstp
  [esp]`, then pushed again as an argument**: the callee is a `__thiscall`
  method called on that pointer (0x41bd10).
- **A parameter loaded after `operator new` that the original loads before**:
  bind a reference to the global slot first (`T*& slot = arr[i]; slot = new
  T(i);`), as in 0x40b320.
- **Dead sums in a loop**: MSVC 5 keeps unused accumulations inside loops;
  write them as unused locals rather than looking for a consumer.
- **A dead `lea reg, [base+K]` next to stores at `[base+K+n]`**: a struct
  pointer local (`S* p = &g_game->s; p->a = 0;`); MSVC folds the offsets into
  the stores but keeps the `lea` (0x4679a0).
- **An inlined `strcpy` whose destination `lea` sits between the `test` and the
  `je` choosing the source**: an if/else with one `strcpy` per branch, tail
  merged; a ternary source puts the `lea` after the merge (0x45ba60).
- **String arguments that are addresses of the function's own stack
  arguments**: a struct passed by value; a caller's `sub esp, K; rep movsd`
  gives its size (0x4d8790).
- **An out-of-line destructor (`??1`)** stores the derived vtable, runs the
  body, then stores the base vtable (the inlined base destructor). If the
  class's `??_G` already has a file, define the same destructor with its own
  `// FUNCTION:` line there or in a copy of that class declaration (0x4909e0).
- **Check a small batch before spending check.py runs**: compile every file in
  one `tools/wcl` loop and compare each object's `objdump -d --no-show-raw-insn`
  with ctx.py; mismatches show up before the first check.py run.
- **A constant hoisted into a register** (`mov eax, 1` then `test [m], al`)
  where the original uses immediates: put the final test and `return 1/0` in
  their own `static inline` helper (0x457a50).
- **Finding a class's layout from its destructor**: grep the disassembly for
  the vtable address to find the constructor's store site; the constructor
  shows where member arrays start (0x462d30).
- **Call order of `f() + g()`** (two calls without arguments): MSVC 5 calls
  the one declared later first, whatever the source order; reorder the
  declarations, not the expression (0x4468c0).
- **A float field spilled with `fld; fstp [esp+N]` before a call it is compared
  with**: only a non-leaf expression such as `(cap = p->x) < f()` does that; a
  plain field or a local copy is loaded after the call (0x419400).
- **`__DATE__`/`__TIME__` strings**: write the literals ("Jul 30 1998",
  "11:16:36"); the macros give today's date (0x41d920).
- **A hand-stored vtable** (`vtable = DAT_x;`) is a last resort: declare the real
  virtual slots with the names they already have (placeholder `FUN_` names in
  different classes are compatible) and let the constructor or destructor store
  it (0x43a1f0).
- **A derived class's `??_G` when its destructor is trivial**: the static
  object trick does not emit it (the derived vtable store is dead, so the
  vtable is never emitted). Define the real constructor again, unannotated, in
  the `??_G` file (0x44f590, 0x490840, 0x490630); see 0x44ef60.cpp for the
  whole family.
- **Variants in one scratch file influence each other**: earlier functions in a
  file change how later ones compile. Recompile the winning variant alone (or
  run tools/headers.py) before the check.py run (0x4c23e0).
- **Emitting a `std::vector` copy constructor out of line**: its address can't
  be taken, but the address of a member that calls it (the outer vector's
  `insert(iterator, size_type, const T&)`) works (0x434470).
- **A field load hoisted above stores the original keeps it after**: put the
  stores and the test in an inline method of a member sub-object (0x463610).
- **A 1-bit bitfield assigned from a byte parameter** (`mov bl, [esp+N]; and
  ebx, 1; shl`): the parameter is `int`; `char` gives `and bl, 1; movsx`.
- **A method with an established placeholder name that is really an out-of-line
  destructor**: write the named method as `((Real*)this)->Real::~Real();` with
  an inline destructor; MSVC inlines it with its vtable store (0x470b80).
- **A `bool` return from a call**: `return f() == 0 ? true : false;` gives
  `test eax, eax; sete al`; an int local first gives `xor edx, edx; sete dl;
  mov al, dl`, and `return f() == 0;` gives `neg; sbb; inc` (0x4de810).
- **Two parameters clamped through one reused stack slot**: a `static inline`
  clamp helper per value, not in-place changes to the parameters (0x496e90).
- **Two byte-identical out-of-line STL helpers**: the call site tells them
  apart; `ecx` set to the container means `allocator<T>::destroy`, no `ecx`
  means `std::_Destroy<T>` (0x434400).
- **A callee with one unused extra stack argument whose caller reads `[eax]`
  right after the call**: a postfix `operator++(int)` / `operator--(int)` on an
  STL iterator, returned through a hidden buffer (0x46fac0).
- **A byte constant hoisted as `mov dl, K`**: every use must be byte-typed;
  route the result through an `unsigned char` local (0x4897e0).
- **A tail returning K or 0 compiled branchless** (`setcc; dec; and`) where
  the original branches: give the zero case its own explicit `return 0;` before
  the final `return 0;` (0x480720).
- **`while (1)` vs `for (;;)`**: a loop that breaks in the middle stays tested
  at the top only as `while (1)`; MSVC 5 rotates `for (;;)` (0x4356f0).
- **A loop whose body reloads `*p` at the top and compares later bytes with a
  register constant**: the body was an inlined helper returning the new
  pointer, `while (*p) p = Helper(p);` (0x4c33a0).
- **`push ecx; mov ecx, esp; push x; call F` before the other pushes**: F
  constructs a by-value class argument in place; name it
  `Class_<F>::Class_<F>` (0x401c20).
- **A pointer-walking loop whose exit returns with a bare `ret`** (the pointer
  is already 0 in `eax`): `while (p) { if (...) return 1; p = p->next; } return
  0;`; `do/while` peels a copy of the body and `break` adds a `setne` (0x481430).
- **Constructor order**: MSVC 5 stores the vtable after the member
  initialisers and before the body, so stores before the vtable store are
  initialisers and stores after it are body assignments (0x407930).
- **A bitfield assigned from a call result**: `xor eax, ecx; and eax, 1; xor
  eax, ecx` in `eax` comes from an `int` local holding the result; assigning
  the call directly copies the old field into a callee-saved register (0x4ae410).
- **Indexing through a pointer field**: load the array pointer into a local
  before indexing (`Entry* entries = obj->entries; entries[i]`) when the
  original loads it before the multiply (0x4a0ff0).
- **A fill loop that looks like `rep stosd` plus a separate counter loop and a
  `lea edi, [base+cnt*4]`**: not `memset` but a plain loop such as
  `while (n >= 4) { *d++ = v; n -= 4; }`, which MSVC 5 converts (0x4d82c0).
- **Out-of-line `std::vector<T>::~vector`** (`push ecx`, free `_First`, zero the
  three pointers, called from element destroy loops): emit it by taking the
  address of the outer `vector<vector<T>>::operator=` (0x433a30).
- **`f(g(a), b)` pushes `b` before calling `g`**: when the original loads `b`
  after `g` returns, store `g`'s result in a local first (0x445e20).
- **`cmp edx, edx` plus a dead store of the old `_Last` into an argument
  slot**: an inlined `vector::clear()` (`erase(begin(), end())`) on that
  argument (0x44ce90).
- **An uncalled out-of-line constructor just before a class's destructor**: the
  class's `new` site inlines the same body elsewhere; reuse the class that
  inlined copy already has (0x470f80, 0x4402e0).
- **A `this`-less `__thiscall` range destroy** (`ret 8`, ecx unused, called with
  `mov ecx, vec` before an inlined `_Ucopy`/insert tail): `vector<T>::_Destroy
  (iterator, iterator)`, emitted like `_Ucopy` through a member pointer (0x4c5b70).
- **A derived class that overrides every slot of a base whose vtable has other
  names**: declare the derived slot names as the base's virtuals (the base
  vtable is never emitted in that file) so the derived constructor emits a
  correctly named vtable (0x474cd0).
- **`push 0; mov ecx, elem; call X` in a destroy loop**: X is the element's
  `??_G` with an implicit destructor, called only at one exact inline depth
  (one level shallower gives `??1T`). Probe the depth with wrapper structs in
  scratch; rebuilding the real caller in the same file emits it, and the
  rebuilt caller may match too (0x4c51b0 and 0x4c2eb0).
- **A `ret 0xc` copy loop that never reads `ecx` but whose callers set `ecx` to
  a vector**: `vector<T>::_Ucopy`, a member, not a `__stdcall` function; the
  caller pattern `push &local; push n; mov ecx, vec` is `vector::resize`
  (0x40d550, 0x40c7f0).
- **An inlined lock/acquire loop tested at the top**: the success case returns
  from inside a `while (1)`; breaking out or a try-helper condition rotates the
  loop (0x4c2b20).
- **A function forwarding an STL iterator through a temporary plus a copy**:
  the returned iterator type differs from the callee's (a `set`'s iterator is
  the tree's `const_iterator`, converted by a constructor); a same-type forward
  (`map`) builds the result straight into the return buffer (0x4dbd00).
- **A leaf method ending with the stored value already in `eax`**: it probably
  returns that value, even with no callers to show it (0x4b4c50).
- **In a constructor, `mov dl, [esp+4]` stored at +K and three zeroed dwords at
  K+4..K+0xc**: a default-constructed `std::vector` member; the byte is its
  empty allocator temporary in a dead parameter slot (0x480160).
- **Before declaring a class's virtuals**, dump its vtable with
  `objdump -s --start-address=<vtable> --stop-address=<vtable+0x40>
  orig/TotalA.exe` to see what each slot holds.
- **All divisions done before three stores to an output struct**: assign a
  constructed temporary, `*out = Vec3(a / n, b / n, c / n);`; field-by-field
  stores interleave with the divisions (0x407410).
- **`if (bf && x)` vs nested ifs**: `if (bf && x)` tests the bitfield with
  `test byte ptr [m], mask`; nested `if (bf) { if (x) ... }` gives `mov ax, [m];
  shr eax, N; test al, 1` (0x4c2cc0).
- **A byte difference used as an `unsigned short` index**: compute it into an
  `int` in its own statement; `(unsigned short)(c - f)` does 16-bit arithmetic
  (0x4c1480).
- **`mov reg, [0]`**: an inlined helper was passed a null pointer and reads a
  field through it, e.g. `Send(0, &packet)` (0x46d530).
- **A pointed-to field re-read around stores into a local buffer**: declare
  the local at function scope; its address escapes to a call, so MSVC treats it
  as aliased and keeps the re-reads (0x450f90).
- **One code shape repeated across functions**: search the exe disassembly for
  a distinctive immediate (such as `push 0xba`); the copies show it is an
  inlined helper and which parts are fixed.
- **Block layout that no reordering changes**: move the loop's match test and
  its body into separate `static inline` helpers (0x43afc0).
- **A narrowed index into a vector** (`_First` loaded before `dec; movsx;
  shl`): index a real `std::vector` member (`&v[(short)(n - 1)]`); a raw
  pointer field loads `_First` after the arithmetic (0x433500).
- **What callers do with a function**: `objdump -d -M intel orig/TotalA.exe |
  grep -B8 "call   0x<addr>"` shows whether they set ecx, what they push and
  what the object is.
- **The same tail call in both arms of an if/else**, seen as a `push` hoisted
  above the `je` in both arms: write the call in each arm; early returns
  falling through to one shared call do not reproduce it (0x408920).
- **A byte copy loop that increments the destination before the load** (`inc
  edx; mov al, [ecx]; ...; mov [edx-1], al`): read into an `unsigned int` local,
  then `*d++ = c;` (0x4587b0).
- **Loops over `g_game->players[i]`** with the walking pointer at the entry
  start and the byte compare constant in a register: take a per-iteration
  `Player* p = &g_game->players[i];` (0x457b90).
- **A field read and stored back unchanged between two real updates**
  (`mov edx, [esi+4]; mov [esi+4], edx`): a component-wise `out->y -= d.y`
  where `d.y` is a constant 0 from an inlined vector helper (0x44d720).
- **`or ecx, -1; repne scasb; not ecx` pushed with no `dec ecx`**:
  `strlen(s) + 1`, the length including the terminator (0x49e640).
- **Pass-through parameters** (only forwarded, pushed from a callee-saved
  register): declare them `int`; a narrower type changes which parameter gets
  `ebx` or `ebp` even with no extension code (0x4c07b0).
- **A byte field pushed as `mov cl, [m]; push ecx`**: the callee's parameter is
  char-typed; declare it so, since an `int` parameter adds `movzx`/`movsx`
  (0x47bd70).
- **Two callers deleting the same object at different inline depths** (one
  calls `??1T`, the other `??_GT`): rebuilding the deeper caller unannotated
  emits the `??_G` (0x470300).
- **Stores through a pointer to an array element computed after the multiply**
  (`i * size`, then the `obj->a->b` chain, then the add): pass `&obj->a->b[i]` to
  a `static inline` helper that does the stores; a local pointer loads the
  chain first (0x4a3eb0).
- **`rep movsd` from `lea esi, [eax+K]` right after a call**: `memcpy(dst,
  &p->field, n)`; a struct assignment gives `mov esi, eax; add esi, K`
  (0x4c2380).
- **A byte global used as an index and a compare, kept in `bl`**: read the
  global each time rather than copying it to a local, which gets spilled
  (0x48ffd0).
- **Loads through a copied register interleaved with adds before the stores**:
  a whole-struct copy then `+=` on some fields (`Rect r = *rect; r.y1 += dy;`),
  not field initialisers (0x467b60).
- **The same call twice with identical arguments in a compare-then-select**:
  a `max()`/`min()` macro evaluating its argument twice; write the macro
  (0x48a7f0).
- **`add reg, 0xffff; shl reg, 16` for `(n - 1) << 16`**: every integer spelling
  folds to `shl; sub reg, 0x10000`; copy a local 16.16 bitfield struct
  (`{unsigned frac : 16; int whole : 16;}`, frac = 0 then whole = n) instead
  (0x4853b0).
- **An in-place copy loop kept as two walking pointers** (`inc ecx; inc eax`):
  write it with two int indices into the one array, `do { p[k] = p[j]; k++; }
  while (p[j++]);`; pointer versions become an offset from the source
  (0x4bb150).
- **A constructor callee whose existing file declares the class smaller than
  the size pushed to `new`**: declare the class again locally with the right
  size and the same constructor signature (0x40f200).
- **Out-of-line pool allocators** (0x4dddf0, 0x4e2b60): declare `unsigned int
  rem = 0x2000;` before the GlobalAlloc retry loop, and pop with `p = DAT; DAT =
  *(void**)DAT;`.
- **`push 4; call operator new` then `if (p) *p = n`** with `n` a size: `new
  T(n)`, a scalar with an initialiser, not an array allocation (0x415bb0).
- **A zero register stored three times through a copy of the destination
  pointer** (`mov ebx, esi; mov [ebx], edi` x3): an inlined `memset(p, 0, 12)`;
  field-by-field zeroing gives immediate stores (0x485330).
- **A null test of `this`, then `lea reg, [this+K]` with the call tail on both
  paths**: `this` converted to a non-primary base; declare the real two-base
  class and pass `this` (0x48f200).
- **Headers can decide which side of a comparison is evaluated first**, not just
  operand order inside `+` or `*`; run tools/headers.py as soon as a whole
  subexpression comes out in the wrong order (0x4c1320).
- **A field compared and then re-read at once** with nothing stored between:
  the check and the use were inline methods of an embedded member struct,
  called as `member.Method()` (0x40d8b0).
- **One call after an if/else vs one in each arm**: MSVC duplicates a shared
  tail call into both arms itself; writing it in each arm changes the argument
  load order. Try both forms (0x4ab6c0 vs 0x408920).
- **Inline helpers taking structs by value**: arguments are evaluated right to
  left, so the parameter order decides which copy is loaded first (0x47ddc0).
- **Overloaded constructors**: names are compared without signatures, so a
  second constructor of an already named class needs a class named after its
  own address (0x4c91b0).
- **Choosing between two element models for an emitted `??_G`**: rebuild the
  caller that emits it and score it with `--sym`; the model whose caller also
  matches gives the right class (0x4349f0).
- **One `sub` after an if/else merge**: `x - K` written in both branches; a
  single subtraction after the merge becomes `add reg, -K` (0x4bc320).
- **`cmp eax, <reg known to be 0>` instead of `test eax, eax` after a call**: the
  result was stored in a local first (`hr = f(); if (hr == 0)`) (0x4c9dd0).
- **An immediate `mov [m], 0` among stores of zeroed registers**: that
  assignment came before the zero temporaries in the source (0x43d210).
- **A literal argument pushed as a register** right after a compare with that
  value: MSVC reused the register; the source still had the literal.
- **A call to a named method with no `mov ecx` before it** in a function that
  never sets ecx: the caller is a method of the same class and `this` passes
  through (0x437b50).
- **A table whose existing `DAT_` symbol starts at element 1**: `arr[k - 1]`
  keeps the offset in a separate `lea`; declare the true base as a new `DAT_`
  symbol and index it directly (0x415ef0).
- **Min-select `mov ecx, b; cmp; jae; mov ecx, a`**: the ternary names the value
  loaded first as its true branch (`a >= b ? b : a`) (0x4dba40).
- **A bitfield or boolean computed at 32 bits for a `char`-typed callee
  parameter**: declare that parameter `int` in your file (0x446450).
- **A zero constant in the wrong register while the early `return 0` has its own
  `xor eax, eax`**: hoist the pointer local the original computes earlier; its
  live range pushes the zero out of `eax` (0x437be0).
- **Inlined `std::set`/`map` operations whose helpers already have placeholder
  names**: write hand-rolled Iter/Find classes (as in 0x46e330). An 8-byte
  `pair<iterator, bool>` comes back through a hidden pointer only with a
  user-declared constructor (0x4e1990).
- **Two struct locals copied from pointers**: the first declared gets a real
  stack slot and the later one reuses a dead parameter slot; swap the
  declarations if they come out reversed (0x421eb0).
- **Destroying a `std::vector` member of each array element**: call
  `arr[i].member.~vector()` directly for `lea esi, [base+idx+K]`; the element's
  implicit destructor gives `add esi, idx` (0x4801f0).
- **`g_game->f += call() << k` when the original loads `g_game` after the
  call**: put the result in an int local first (0x416860).
- **Find near-copies before writing**: grep src/unsorted for a distinctive
  offset or callee address; many functions differ from a matched sibling only
  in a callee, a key string or a value type.
- **Victory-condition classes (vtables 0x4fd800-0x4fd978)**: each has a
  visitor (second-base) vtable just before its main one (main 0x4fd870, visitor
  0x4fd868); 0x4fd940 is the pure visitor base. Copy the two-base class and
  ForEach helper from 0x48edb0 or 0x48f530.
- **`Vec3 v; v = Vec3(0, 0, 0);`** gives three separate zero registers, the last
  store after the argument pushes; `Vec3 v(0, 0, 0);` or field zeroing shares
  one zero register (0x44eb60).
- **A free function whose callers set `ecx` just before calling it**: it is
  really a method that ignores `this` (0x4ce190, 0x461610). Check the callers
  before trusting a free-function name.
- **A negative-offset override (`[ecx-8]`)**: search .rdata for the function's
  address, then grep the disassembly for stores of that vtable; the
  constructor's store sequence names the owning class and the base offset
  (0x48f790).
- **A value loaded into `eax` then copied to a callee-saved register**: use the
  parameter itself as the loop variable and keep a copy of its old value
  (0x4a7560).
- **Before writing a constructor, find a sibling constructor of the same
  family** (grep for a distinctive expression such as `<< 19`) and copy its
  local-variable layout; it decides which stack slots MSVC reuses (0x44d3b0).
- **A placeholder method name that is really a constructor**: write the method
  as `((Real*)this)->Real::Real(args); return (Real*)this;` with an inline
  constructor (0x470a90); the destructor counterpart is 0x470b80.
- **A sum the original computes twice**: MSVC 5 shares `a + b` even across
  branches, so one use was probably two `+=` steps (`z += off; z += x1;`)
  (0x4c0a90).
- **Which field MSVC 5 walks an array loop from**: with no pointer local, the
  walking register starts at the second field the source accesses, so an
  unexpected `lea reg, [base+K]` shows which access came second (0x49d1e0,
  0x450980).
- **Identical `switch` cases may need separate bodies** even when the original
  has one shared target: writing cases 1 and 3 separately let MSVC merge their
  calls at the right place and fixed register choice around them (0x406780).
- **An inline helper that must reload a pointer member after each call**:
  take the pointer by reference (`Owner*& o`); passing it by value keeps it in
  a register (0x4077e0).
- **`lea reg, [esi+K]` then stores at `[reg+4]`/`[reg+8]`**: a struct copy into
  an inlined constructor's `this` (`*this = Vec3(...)`); field initialisers, a
  user `operator=` or a helper give direct `[esi+K]` stores. The scheduler folds
  the first store to `[esi+K]` only when nothing can fill the slot after the
  `lea`, so an unfolded `mov [reg], x` means other instructions came between
  them in the compiler's input (0x407d40).
- **`strlen(text) > 0 ? text : 0`** gives `cmp eax, ecx; sbb esi, esi` for a
  pointer-or-null select (0x435320).
- **Scoring many variants**: `uv run tools/check.py <addr> <scratch.cpp> --sym <part
  of the mangled name>` checks a scratch file; put many variant functions in one
  file and score each.
- **Smacker video (smackw32.dll, imported by ordinal)**: the imports have no
  names in the exe, so declare them `extern "C" __declspec(dllimport) ...
  __stdcall` with the names below and keep them consistent. Inferred from call
  sites, not from an export table: 14 SmackOpen, 17 SmackSoundOnOff, 18
  SmackClose, 19 SmackDoFrame, 20 SmackSummary, 21 SmackNextFrame, 23
  SmackToBuffer, 27 SmackGoto, 28 SmackToBufferRect, 38
  SmackSoundUseDirectSound. Smack struct: Width +4, Height +8, Frames +0xc,
  FrameNum +0x374, LastRect +0x380..+0x38c (0x47c330).
- **A search returning 0 when nothing is found** with `xor eax, eax` before the
  loop: `int result = 0; for (...) if (...) { result = j; break; } return
  result;` (0x440c10).
- **HAPINET wrappers (0x4c97xx-0x4ca8xx)**: `mov eax, <HRESULT>` before the null
  test is `int result = K; if (dp) result = ...; return result;`; the constant
  only after the `je` is an early return inside the `if`, then `return K;`.
  IDirectPlay2 slots: EnumPlayers +0x30, GetPlayerName +0x54, Receive +0x64,
  Send +0x68.
- **DirectPlayCreate** is DPLAYX ordinal 1 (thunk at 0x4faffc); call it from
  the toolchain's `<dplay.h>`. The GUID at 0x4fcd78 is IID_IDirectPlay3A, which
  the DirectX 3 header lacks: declare `extern GUID DAT_004fcd78;` (0x4ca900).
- **COM calls by slot**: work out the DirectX interface from the vtable slot
  and call the real method (IDirectSoundBuffer: +0x24 GetStatus, +0x48 Stop).
- **Header sets are not monotonic**: one header can flip an operand order that
  a larger set does not; try several combinations in scratch with `/Fa`.
- **A `new` of a class with two bases**: the second base's vtable store survives
  in the listing while the first base's disappears; declare both bases as real
  classes (the second with a pure virtual).
- **Static vs external global objects**: if the atexit destructor of a global
  `std::vector` keeps `_First` in a callee-saved register on the empty path, the
  vector is a file-scope `static` (see `0x434a30.cpp`); the checker accepts the
  compiler's `$S`-suffixed name.
- **Two pointers walking one struct array**, one at +0 and one into the middle
  of a group of fields: the group was accessed through an inlined helper taking
  the sub-struct by reference.
- **Three zeroed registers stored through `lea reg, [this+K]`**: a body assignment
  of a temporary, `v = Vec3(0, 0, 0);`, not a member initialiser.
- **Packing blocks**: keep a struct with a dword at an odd offset (e.g. +0x38a47)
  in its own `pack(1)` block; `pack(2)` silently moves the field.
- **Base and index swapped in an address** (`[esi+eax]` vs `[eax+esi]`, a different
  SIB byte): the same header dependence as commutative operands; adding a
  header fixed it. The `/Fa` listing prints both the same way, so compare
  encodings with `objdump -d -M intel file.obj` (installed).
  `uv run tools/headers.py <addr>` (each line it prints is one complete header
  set) compiles your file with every combination
  of `<windows.h>`, `<stdio.h>`, `<stdlib.h>`, `<string.h>`, `<math.h>` and
  `<memory.h>` in a few seconds and prints the sets that match; try it as soon
  as every rewrite gives the same wrong register or operand order (0x471f90
  needed exactly `<windows.h>` plus `<math.h>`).
- **"-2 jumps away, -1 skips, default stores"**: a `switch` with `case -2`,
  `case -1` and `default`, not an if/else chain.
- **A field tested in one register, then re-read before a COM call** (or copied
  with `mov eax, ecx` when inlined): the call went through an inline method of
  an embedded struct (`d->screen.UnlockSurface()`). The screen lock/unlock pair
  is FUN_004c5e70/FUN_004c5fa0 (`IDirectDrawSurface::Lock` +0x64 and `Unlock`
  +0x80 on the surface at display+0x8c), used by many functions around
  0x4c6b70-0x4c6dc0; see `src/unsorted/0x4c6d20.cpp`.
- **STL templates ending in `ret N`**: that original file was compiled with
  `__stdcall` as the default. Write the template body as an explicit
  `__stdcall` free function (the real `std::` template gives a plain `ret`).
- **Inlined GlobalAlloc pool allocators**: carve n-byte pieces generically
  (`for (rem = 0x2000; rem >= n; rem -= n)`, as in 0x4e2b60), not a fixed count.
- **`mov eax, fs:[0x2c]`** then an indexed load: thread-local storage. Declare the
  variable `__declspec(thread)` (the exe has a `.tls` section).
- **`mul` by a large odd constant, then a shift of `edx`** (`mov eax, 0x10624dd3;
  mul ...; shr edx, 6`): plain unsigned division by a constant (`v / 1000u`).
  The high half of a `mul` by a variable (`mul reg` then using `edx`) is
  `(unsigned int)(((unsigned __int64)a * b) >> 32)`.
- **`cmp eax, ecx; sbb eax, eax` after an inlined `strlen`** (with `ecx` zero):
  write the test as `0 < strlen(s)`; `strlen(s) > 0` or `!= 0` give `neg; sbb`.
- **`lea esi, [base+K]` then `[esi]` accesses, with the base register reused as a
  loop pointer**: the source took a pointer to the field plus a separate array
  pointer and never used the object pointer directly afterwards.
- **An index loop over a global array that should stay indexed**: address the
  array as a member of an enclosing global struct (`g.arr[i]`); the checker
  accepts the struct symbol plus a displacement.
- **Diff against siblings before writing**: compare your function's `ctx.py`
  disassembly with already-matched neighbours (ignoring addresses); several
  functions are byte-identical copies apart from jump targets or `ret N`.
- **A global object with a constructor and destructor**: write
  `Class_x DAT_y;` and annotate the compiler-generated initialiser and atexit
  destructor (`// FUNCTION: 0x49e610 _$E4` / `_$E2`); see `0x49e610.cpp`.
- **An unreferenced `??_E` function** (vector deleting destructor) is emitted by
  `new T[n]` on a class with a destructor; `new T[1]` in the file makes the
  compiler emit it (annotate it with its mangled name).
- **16-bit compares** (`cmp word ptr [m], reg`) against an `int` parameter: cast
  the parameter to the field's type (`field == (unsigned short)p`), or MSVC
  widens with `movzx` and compares 32 bits.
- **Indexing by a global counter**: index with the global itself
  (`arr[g_count]`); keep a local copy only for later comparisons.
- **`mov eax, 0xffff; cmp ax, 0xffff`**: an `unsigned short` helper returning
  0xffff (not `-1`).
- **Parameter width from a byte use**: `mov al, byte ptr [esp+N]` feeding an
  inlined `memset` fill value means an `int` parameter (`unsigned char` adds
  `and eax, 0xff`, `char` gives `movsx`).
- **An erase loop that reads `_First` once, before the loop**: take the iterator
  into a local before the loop (`v.begin()` in the condition reloads it).
