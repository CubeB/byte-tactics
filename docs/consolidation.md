# Consolidation notes

Things to resolve when the per-function files in `src/unsorted/` are merged
into real classes and translation units. Agents name unknown classes after
single addresses, so one real class often appears under several names.

## Classes to merge

- `Class_0044cf60` and `Class_0044d010`: both constructors store vtable
  `DAT_004fd328` and fill the same fields (+8 packed point, +0xc radius,
  +0x10 radius squared). Probably overloaded constructors of one class.
- `Class_004c91a0` (copy constructor, 0x4c91a0), `Class_004c9390` (destructor:
  decrement and free, 0x4c9390) and `Class_004c93b0` (assignment,
  0x4c93b0) are the same reference-counted string handle.
- `Class_00485e30` (vtable 0x4fd698, created by 0x485d40) derives from
  `Class_004b0610`; its 20 overrides (0x480770-0x481470) are matched under
  separate placeholder classes. 0x485e30.cpp keeps a static `new` to emit its
  `??_G` until 0x485d40 is decompiled. The run 0x4b0720-0x4b1c00 is probably
  more non-virtual methods of `Class_004b0610` (0x485d40 calls 0x4b0940).
- `Class_00470ae0` (vtable 0x4fd580, `??_G` at 0x470ae0): its constructor is
  0x470a90 (`Class_00470a90::FUN_00470a90`) and its destructor 0x470b80
  (`Class_00470b80::FUN_00470b80`).
- `Class_0044e250` and `Class_0044e330`: two constructors storing vtable
  `DAT_004fd3b8`.
- The pathfinder ("AISearch touched mapentries" is its grid): one class with
  a binary heap of 20-byte nodes at +0 and the grid at +0x1c (cells +0x1c,
  width +0x20, height +0x24, cell count rounded up to 8 at +0x28, one dirty bit
  per 8 cells at +0x2c). Its methods are matched under separate placeholder
  classes: 0x40e9e0 (constructor), 0x40d7b0, 0x40d880, 0x40d8b0, 0x40d900,
  0x40da40, 0x40e160, 0x40e630, 0x40e9a0, 0x40eb70, 0x40f000/0x40f060 (heap
  sift up and down, already `Class_0040f000`), 0x40ef20 (heap `Remove(k)`),
  0x40f1e0, and probably 0x40df00, 0x40e050 and 0x40f110. 0x40da40 is the
  out-of-line copy of the cost helper inlined into 0x40e160 and 0x40e630;
  0x40d880, 0x40d8b0 and 0x40e9a0 are inlined into 0x40e630. Found in #12, #81
  and #90. The object at +0x64 is called `owner` in 0x40eb70 and 0x40e630 but
  `map` in 0x40d7b0 and 0x40e050 (which read map origin shorts at +4/+6 from
  it); settle on one name when merging.
- Functions that store the same vtable address belong to the same class (or a
  base/derived pair); a tool listing every vtable store would find the rest.

## Duplicated library code

- Two copies of `std::_Lockit` (0x4e39b0 and 0x4e1480) and its cleanup are
  linked in; `data/aliases.csv` maps both. The code at 0x4d8000-0x4e3000 that
  calls the second copy is probably a separately built library.

## Context-dependent functions

- 0x4581e0 and 0x4335e0 match only with a header block (`windows.h`, `stdio.h`,
  `string.h`, `math.h`) at the top; 0x4d1820 and 0x438650 still differ in one
  operand order. Their original translation units probably decide this.
- 0x40f200, 0x40f2a0, 0x40f7d0 and 0x40fa20 (unit order handlers) share one
  original file: 0x40f2a0 and 0x40fa20 inline FUN_0040f200, so each of their
  files carries an unannotated copy of it next to the matched 0x40f200.cpp.
  When they are merged into one translation unit, keep one definition. The
  copies call the unit's type pointer at +0x92 `def` where 0x40f200.cpp says
  `info`.

## Matches that use suspicious constructs

These match byte-for-byte but use something Cavedog probably did not write;
revisit them once the surrounding code is known.


- The `std` exception classes in the C++ library block were misnamed by the
  signature matcher (their destructors are identical apart from the vtable).
  By RTTI: vtable 0x4fdca4 is `std::logic_error` (0x4c3730 is `what`, 0x4c38a0
  is `??1logic_error`), 0x4fdcb4 is `std::out_of_range` (0x4c3aa0 `??1`,
  0x4c3af0 `_Doraise`, 0x4c3c60 `??_G`), 0x4fdc7c is `std::length_error`.

- The 0x4fc980 family is consolidated (table in 0x407350.cpp): base
  `Class_00407350` and six derived classes, one per 2-slot vtable, owned by
  `Class_00408cb0`. `Class_004079d0` and `Class_00408810` are not yet named
  after their constructors (0x4079a0, 0x4087e0); 0x407d40 (a constructor) is
  unmatched at about 78%, its vtable stored between two vector computations.
- 0x417a60 (the debug crash command) divides by `(one >> 1)` with
  `volatile int one = 1`; plausible for a deliberate crash, but check once
  its file's other functions are known.

- The 0x4fd428 family is consolidated (table in 0x44ef60.cpp): base
  `Class_0044ef20`, derived `Class_0044f010`, `Class_0044f570`, middle
  `Class_00490630` and its children `Class_004907e0`, `Class_00490880`.
  Left over: 0x490880.cpp uses the name `Class_00490880` for what is
  `Class_004907e0`'s slot 2 override; 0x44f010.cpp and 0x44f570.cpp store their
  vtables by hand; 0x44ef90's class is spelt `Class_44ef90`.

- The timer class is `Class_004e2150` in 0x4e2150.cpp and `Class_004e2160`
  elsewhere; its getter 0x4e1e30 is `Class_004e1e30::FUN_004e1e30`.
- 0x434360.cpp's `Elem_00434360` looks like `std::vector<Elem_00434020>`: the
  operator= it calls (0x434770) destroys elements with 0x433a30, which is
  `~vector<Elem_00434020>`. So 0x434360 is probably an erase on a three-level
  vector, and 0x434770's recorded name is one level too shallow. Evidence
  since: with `Elem_00434360` as a struct holding a `vector<Elem_00434020>`,
  a rebuilt 0x434770 is byte-identical (0x4349f0.cpp), but it then calls
  0x434470 and 0x4349c0 under names other than their recorded ones
  (`Elem_004349c0` and `Elem_00434020` look like one 4-byte type).

- `Class_004402e0` (constructor 0x4402e0) is the class 0x440290.cpp calls
  `Class_00440320`, while 0x440320 is recorded as the free function
  `FUN_00440320`.

- `Class_0046e4d0::FUN_0046e4d0` and `Class_0046e450` are the same object's
  class (both called on g_game+0x2a30 from the two arms of one branch at
  0x44c4f8).
- 0x4352d0, 0x463730 and 0x45ca50 have no callers and no pointers to them:
  probably dead code.

- 0x438b90's `Class_00438b90` has `Class_0043a1f0`'s layout (kind at +4,
  flags at +0x42).

- `Class_00415b60`, `Class_00415b90` and `Class_00415c10` are one bit-writer
  class (0x48b710 calls all three on one 0x410-byte stack object).


- The 0x4fd5a8 family is consolidated (table in 0x471cc0.cpp): base
  `Class_00471cc0` (destructor 0x471d00, class `operator new` 0x471d10 and
  `operator delete` 0x471d50) and six derived classes. Left over: the slot
  methods keep their placeholder classes (0x472f90 is still
  `Class_00472fd0::FUN_00472f90`); 0x471d70 is a non-virtual base method;
  0x475330 is recorded as a free function but is slot 3 of `Class_004750b0`;
  four derived `??_G` files use a static `new` until the real `new` sites
  (0x471340 and others) are decompiled; the pool at DAT_0051e610 is
  `Class_00470ed0`/`Class_00470eb0` in some files and `Class_00470ae0` in
  0x470ae0.cpp.

- The victory condition with vtables 0x4fd890 (primary) and 0x4fd888 (visitor
  base) is `Class_0048f250`; its slots 4 and 5 (0x48f2f0, 0x48f330) are still
  filed as `Class_0048f2f0` and `Class_0048f330`.
- **A probable Cavedog bug** (see docs/bugs.md): the bit writer grows its buffer with `new
  unsigned int(capacity * 2)` (one dword initialised to the size) where an
  array was surely meant (0x415bb0, inlined in 0x415c10).

- Overloads share one name in data/symbols.csv, so the string handle's
  constructors are split across `Class_004c9180` (default), `Class_004c91a0`
  (copy) and `Class_004c91b0` (`const char*`); the timer's two constructors
  both use `Class_004e1d20::Class_004e1d20`, which the checker cannot tell apart.

- The `Class_0044ce20` family (vtables around 0x4fd3f8, constructors 0x44e740
  and 0x44e9c0 among others) still stores its vtables by hand
  (`vtable = DAT_004fd3f8;`), like the 0x4fc980 family before its
  consolidation.

- 0x43c360 is `vector::size()` of the global vector of 25-byte records at
  0x512340 but is named `Class_0043c360::FUN_0043c360`; it will clash when
  0x43bc90 or 0x43c050 is decompiled with a real `std::vector`.
- 0x44ec00 is a vtable slot of `Class_0044e740` recorded as a free function;
  tools/methods.py can't see it because it is only called through the vtable.
- 0x40e9e0 clears its pointer at +0x1c with `memset(&field_1c, 0, 4)` rather
  than `field_1c = 0`, which keeps MSVC from folding the following
  `delete field_1c` into a constant. The original probably cleared a larger
  struct or called an inline reset helper; revisit when the class's other
  methods are known.

## Signatures that disagree

The checker compares names, not parameter types, so callers and definitions
can disagree on types (a real link would fail). Known cases:

- `FUN_004ba590`: its file takes `int`; callers such as 0x417290 pass `float`.
- `FUN_004d0620`: its file returns `void`; 0x47efe0 uses a `void*` result.
- `Class_00438b90::FUN_00438b90` takes the 1-byte class `Class_00438760` by value
  (see 0x403190); its own file declares `int k`. FUN_0043f0e0 returns the same
  class through a hidden buffer.
- `FUN_004d83b0` returns a pointer (0x481500) but its file says `void`.
- `Class_0043a0c0`'s constructor: 0x43a020 and 0x43b730 declare its first
  parameter `unsigned char`, but 0x401c20 shows it is a 1-byte class passed by
  value, built by `Class_00438760::Class_00438760`.

- 0x40d7b0 returns `int` in its own file, but 0x40da70 uses its result as
  unsigned (`cmp eax, 1; jae` and `cmp 1, eax; sbb`), so 0x40da70.cpp
  declares it `unsigned int`. Settle on `unsigned int` when merging the
  pathfinder class.

- FUN_004be950's colour parameter is declared `int` in 0x417c70.cpp,
  0x417e00.cpp and 0x4181d0.cpp so that `color & 0xff` is not folded, though
  the callee probably takes `unsigned char`. Settle it when 0x4be950 is
  decompiled. (0x4181d0.cpp also holds 0x417f60, defined above it as the
  original file did; see #111.)

- FUN_004103a0's second parameter is `int scale` in 0x4103a0.cpp, but all
  four callers (0x40fc53, 0x4108bf, 0x413018, 0x4131a7) load the constant into
  a register and push it, which only a 4-byte struct passed by value does;
  0x412d40.cpp declares it as a union. Settle on the struct.

- 0x4118e0 passes the landing pad index as a full dword to
  `Class_0044e250::Class_0044e250` and `FUN_0048aac0`, whose own files declare
  that parameter `short` and `char`; 0x4118e0.cpp declares them `int`. The
  real parameters are probably `int`. 0x44e190 (unnamed) is a constructor
  (stores vtables, returns `this`, called on `operator new(0x36)`); 0x4118e0
  and 0x411f50 call it `Class_0044e190::Class_0044e190(Order*, Unit*)`.

- 0x44e190, 0x44e250 and 0x44e2d0 all store vtables 0x4fd2f8 then 0x4fd3b8
  and are called on `operator new(0x36)`: overloaded constructors of one
  class, matched under three placeholder names (see #96, #97).
- 0x438760 is the constructor of `Class_00438760`, an order type held as its
  index in the sorted order-type table and passed by value (#31).

## Third-party code

- zlib 1.0.4 occupies 0x4d1c80-0x4d7d70 and matches from its own source with
  `/Gz /Zp1`; 5 of its 58 functions (inlined statics or variants) did not match
  and are still listed as game code around that range. A rebuild should compile
  the real zlib source rather than decompiled copies.

## Per-file compiler options

- Some original files were compiled with `/Gz` (`__stdcall` by default): STL
  templates there (`copy_backward`, `fill`, `_Construct`, sort helpers around
  0x43c6b0-0x43cb20 and 0x4c5bc0-0x4c5d10) end in `ret N`. The staged files
  write those as explicit `__stdcall` functions; when files are regrouped, those
  translation units should get `/Gz` and the real `std::` templates instead.

## STL instantiations

- Small `std::vector` members matched early under placeholder classes block
  their callers' name checks. Shapes to look for: `if (!_First) return 0;
  return (_Last - _First) / sizeof(T)` is `size()` (with `_End` at +0xc,
  `capacity()`); `push ecx`, free `_First`, zero +4/+8/+0xc (34 bytes) is
  `~vector()` for a trivially destructible `T`; `mov eax, ecx`, copy the
  allocator byte, zero +4/+8/+0xc, `ret 4` is `vector(const allocator&)`, the
  default constructor. Name `T` after the other out-of-line members called on
  the same object (its `_Ucopy`, `erase`, ...), since the element type is part
  of every mangled name; a function that destroys a whole object (0x40b390 for
  the player AI object of 0x409160) maps each member's offset to its
  destructor. #88 renamed 0x40c510-0x40c5d0, 0x40cc80, 0x40d000, 0x40ca30 and
  0x40a5b0 this way.
- A constructor's address can't be taken, and no vector member calls
  `vector(const allocator&)`, so 0x40c510.cpp emits it with an explicit
  instantiation, `template class std::vector<Unit*>;`, which emits every member.
- One element type, one name: `Elem_0040cc40` (a cell and its float sort key,
  copy constructor 0x40a5b0) is the element of the vector at +0x4d of the
  player AI object, and `Elem_0040cfb0` (three bytes) the one at +0x65. The
  files that use them define them identically. `Class_00409160`,
  `Class_00409470`, `Class_00409730`, `Class_0040a150` and `Class_0040a7b0`
  are all that AI object (DAT_005119c0[player]).
- The unit list is `std::vector<Unit*>`, and `Unit` is its only element name
  (#135). Its out-of-line members are 0x406c00 (`_Destroy`), 0x406c10
  (`_Ucopy`), 0x406c40 (`_Ufill`), 0x408f30 (`insert`), 0x40c510 (the
  constructor), 0x40c530 (the destructor), 0x40c560 (`size`) and 0x40c9f0
  (`erase`). 0x40ad80 calls `insert`, `_Ucopy`, `_Ufill` and `size` on one
  vector, 0x40aa40 calls `erase` and `insert`, and 0x48d220 `_Destroy` and
  `erase`; the exe has separate byte-identical copies of each of these for
  other element types (the linker does not fold them), so one address is one
  element type. The placeholders `Elem_00406c10` (a 4-byte struct),
  `Elem_0040c9f0` and `Unit_00407560` were renamed to `Unit*` and `Unit`.
  0x412710 (partial) still uses `Elem_00406c10`, since `Unit*` alone moves
  registers there.
