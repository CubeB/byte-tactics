# Consolidation notes

Things to resolve when the per-function files under `src/` are merged
into real classes and translation units. Agents name unknown classes after
single addresses, so one real class often appears under several names.

## The unit map

`tools/unitmap.py` groups the matched members under `src/` into the classes
they belong to and builds a map in memory on every run. A unit is a class whose
matched members are spread over more than one file under `src/`, which is
the state every class is in before consolidation. For each unit the map gives its
members in address order, the file defining each, every file that declares the
class, the union of the field ledgers those declarations give, and every offset
where two of them disagree.

```sh
uv run tools/unitmap.py            # summarise and write build/units.json (ignored)
uv run tools/unitmap.py --list     # the units, largest first
uv run tools/unitmap.py --at 0x401070   # the unit nearest an address
```

The map is rebuilt from `data/progress.csv` and `src/` on each run, so nothing
has to be kept current. `build/units.json` is written for inspection only.

A class whose members already live in one file is not a unit, so a consolidated
class drops out of the map rather than staying as a one-file entry.

`tools/unitgen.py <unit>` reads the map and writes one candidate file under
`build/units/`:

- every type the unit's files declare is emitted once. For each name the views
  are merged: the base list comes from the view that has one, fields and their
  order from the reference file's view (the layout its matched functions were
  built against), any field another view declares that a member body actually
  names is added, and methods are unioned by name, so a view that left one out
  does not remove it;
- the reference file (the one carrying the most fields) keeps its order, so
  `#pragma pack` regions and the order of virtual slots survive;
- every other file's remaining declarations are inserted above the first place
  that needs them, rather than at the end;
- every member's definition is copied from the file that holds it, in address
  order.

It never invents a line, and it prints the offsets in dispute and any base class
it could not find. A type whose name no file in the unit declares is not pulled
in from elsewhere, so a unit that needs one still fails; none does today.

The candidate is the start of a unit, not a match. Merging changes the compiler
state and the inline budget, so once the file compiles, every function it holds
has to be re-checked:

```sh
uv run tools/check.py <address> build/units/<unit>.cpp
```

Where two declarations disagree, the map records every view and the generator
says which one it used; that choice is a hypothesis until the checker agrees.
Regenerate the map after each merge. A disagreement the generator cannot settle
from the code (for example a field one file reads as an `int` and another indexes
as a pointer, with a body naming both) is a decision for the reviewer, not for
the tool.

## Using the map while decompiling

`docs/agent-guide.md` has the short version: `uv run tools/unitmap.py --at
0x<addr>` names the class nearest an address and prints its ledger, and
`tools/unitgen.py <unit>` writes that class with its matched members under
`build/units/`. What the map cannot tell you:

- it only holds matched members, so a class whose methods are all unmatched does
  not appear, and a class still spelt under several placeholder names is either
  split between entries or is not a unit at all;
- a conflict is only found when a file carries a `// +0xN` comment. Two views of
  one offset that both omit it (compact packed structs) are stored as two fields
  and reported as clean, so an empty dispute list means "nothing was flagged",
  not "the views agree";
- a disputed offset is worth reading before it is used: every view is listed with
  the file it came from, and settling one is what unblocks the unit.

## Classes to merge

- `Class_004b2fb0` is the tree of a `std::map<int, int>` (`std::_Tree<int,
  std::pair<const int, int>, map::_Kfn, std::less<int>, std::allocator<int> >`),
  and `Class_004b3590` is its iterator. 0x4b2ac0 is its `erase(iterator)` and
  0x4b3590 its `iterator::_Inc`, now under their real names. 0x4b2340, 0x4b2540
  and 0x4b2850 still declare the tree by hand under the placeholder names, and
  two rows in `data/aliases.csv` accept both spellings until those files use
  `std::map<int, int>`.
- `Class_0044cf60` and `Class_0044d010`: both constructors store vtable
  `DAT_004fd328` and fill the same fields (+8 packed point, +0xc radius,
  +0x10 radius squared). Probably overloaded constructors of one class.
- `Class_004c91a0` (copy constructor, 0x4c91a0), `Class_004c9390` (destructor:
  decrement and free, 0x4c9390) and `Class_004c93b0` (assignment,
  0x4c93b0) are the same reference-counted string handle.
- `UnitScript` (vtable 0x4fd698, created by 0x485d40) derives from
  `CobScript`; its 20 overrides (0x480770-0x481470) are matched under
  separate placeholder classes. 0x485e30.cpp keeps a static `new` to emit its
  `??_G` until 0x485d40 is decompiled. The run 0x4b0720-0x4b1c00 is probably
  more non-virtual methods of `CobScript` (0x485d40 calls 0x4b0940).
- `ObjectPool` (vtable 0x4fd580, `??_G` at 0x470ae0): its constructor is
  0x470a90 (`Class_00470a90::Construct`) and its destructor 0x470b80
  (`Class_00470b80::Destroy`).
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

- `Class_004402e0` (constructor 0x4402e0) is the class 0x440290.cpp calls
  `MovementClass`, while 0x440320 is recorded as the free function
  `FUN_00440320`.

- `Class_0046e4d0::AllowUnit` and `Class_0046e450` are the same object's
  class (both called on g_game+0x2a30 from the two arms of one branch at
  0x44c4f8).
- 0x4352d0, 0x463730 and 0x45ca50 have no callers and no pointers to them:
  probably dead code.

- 0x438b90's `Class_00438b90` has `Class_0043a1f0`'s layout (kind at +4,
  flags at +0x42).
- `Class_0043a1f0` (vtable 0x4fd2c8, constructors 0x43a0c0 and 0x43a420,
  destructor 0x43a1f0) derives from `Class_0043a1e0` (vtable 0x4fd2cc). Each
  vtable has one slot: the base's is the empty 0x43a1e0, and the derived
  class overrides it with 0x438870 (still filed as
  `Class_00438870::FUN_00438870`; neither has a direct caller). Both
  constructors store 0x4fd2cc and then 0x4fd2c8 (the inline base
  constructor first); the destructor stores only 0x4fd2c8, so the base
  declares no destructor. #291 read 0x4fd2c8 as a 2-slot vtable, which runs
  into 0x4fd2cc (#294). The link member at +0x12 is a `Class_004895c0`; its
  destructor 0x489650, filed as `Class_00489650::FUN_00489650`, is called by
  hand at the end of 0x43a1f0. When 0x43a420 is decompiled it must also be
  `Class_0043a1f0::Class_0043a1f0` (a class named after its own address
  could not store `??_7Class_0043a1f0`), so its caller 0x487080 will need a
  data/aliases.csv row for that name at 0x43a420.

- `Class_00415b60`, `Class_00415b90` and `BitWriter` are one bit-writer
  class (0x48b710 calls all three on one 0x410-byte stack object).


- The 0x4fd5a8 family is consolidated (table in 0x471cc0.cpp): base
  `ParticleSystem` (destructor 0x471d00, class `operator new` 0x471d10 and
  `operator delete` 0x471d50) and six derived classes. Left over: the slot
  methods keep their placeholder classes (0x472f90 is still
  `Class_00472fd0::FUN_00472f90`); 0x471d70 is a non-virtual base method;
  0x475330 is recorded as a free function but is slot 3 of `Class_004750b0`;
  four derived `??_G` files use a static `new` until the real `new` sites
  (0x471340 and others) are decompiled; the pool at DAT_0051e610 is
  `Class_00470ed0`/`Class_00470eb0` in some files and `ObjectPool` in
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
- The family that stores vtable 0x4fd2f8 and then 0x4fd3b8 (a base and a
  derived class) does the same: constructors 0x44de80, 0x44e080, 0x44e190,
  0x44e250 and 0x44e2d0, each matched under its own placeholder class. They
  should become one derived class with real virtual slots, whose base
  constructor stores 0x4fd2f8.

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

- The network flags word at g_game+0x38d75 is declared `volatile` in
  0x494e70.cpp and 0x452800.cpp. That is an exception to the no-`volatile`
  rule, accepted on evidence: every write to it in the exe (0x496861,
  0x4975c0, 0x497c57) is a word load, bit change and store through a register,
  and every pair of bit tests (0x45290e, 0x4550c2, 0x494e8b) re-reads memory,
  which MSVC 5 does only for a volatile field (#436). Declare it `volatile` in
  the game struct when the files are merged.

## Signatures that disagree

The checker compares names, not parameter types, so callers and definitions
can disagree on types (a real link would fail). Known cases:

- `SetBrightness`: its file takes `int`; callers such as 0x417290 pass `float`.
- `FUN_004d0620`: its file returns `void`; 0x47efe0 uses a `void*` result.
- `Class_00438b90::FUN_00438b90` takes the 1-byte class `Class_00438760` by value
  (see 0x403190); its own file declares `int k`. FUN_0043f0e0 returns the same
  class through a hidden buffer.
- `FUN_004d83b0` returns a pointer (0x481500) but its file says `void`.
- `Class_0043a1f0`'s constructor 0x43a0c0: its own file takes `int`, 0x43a020
  and 0x43b730 declare its first parameter `unsigned char`, but 0x401c20
  shows it is a 1-byte class passed by value, built by
  `Class_00438760::Class_00438760`.

- 0x40d7b0 returns `int` in its own file, but 0x40da70 uses its result as
  unsigned (`cmp eax, 1; jae` and `cmp 1, eax; sbb`), so 0x40da70.cpp
  declares it `unsigned int`. Settle on `unsigned int` when merging the
  pathfinder class.

- DrawLine's colour parameter is declared `int` in 0x417c70.cpp,
  0x417e00.cpp and 0x4181d0.cpp so that `color & 0xff` is not folded, though
  the callee probably takes `unsigned char`. Settle it when 0x4be950 is
  decompiled. (0x4181d0.cpp also holds 0x417f60, defined above it as the
  original file did; see #111.)

- FUN_004103a0's second parameter is `int scale` in 0x4103a0.cpp, but all
  four callers (0x40fc53, 0x4108bf, 0x413018, 0x4131a7) load the constant into
  a register and push it, which only a 4-byte struct passed by value does;
  0x412d40.cpp declares it as a union. Settle on the struct.

- 0x4118e0 passes the landing pad index as a full dword to
  `Class_0044e250::Class_0044e250` and `AttachUnitToPiece`, whose own files declare
  that parameter `short` and `char`; 0x4118e0.cpp declares them `int`. The
  real parameters are probably `int`. 0x44e190 (unnamed) is a constructor
  (stores vtables, returns `this`, called on `operator new(0x36)`); 0x4118e0
  and 0x411f50 call it `Class_0044e190::Class_0044e190(Order*, Unit*)`.

- 0x44e190, 0x44e250 and 0x44e2d0 all store vtables 0x4fd2f8 then 0x4fd3b8
  and are called on `operator new(0x36)`: overloaded constructors of one
  class, matched under three placeholder names (see #96, #97).
- 0x438760 is the constructor of `Class_00438760`, an order type held as its
  index in the sorted order-type table and passed by value (#31).

- SetGadgetStatus's third parameter is `short` in 0x4a11c0.cpp, but 0x41a120
  only matches with `int` (#175); it is probably `int`.

- `Class_00460f60`, the object at g_packetManager: eleven 0x1044-byte channels
  from +0x08, a {buffer, used, capacity} triple at +0xb2f4 and a
  `Class_00462d30` member at +0xb300; 0x460e20.cpp has the full class (#225).
  Its destructors 0x461340 and 0x461420 view each channel's items/count pair
  (its +0x08) as entries at +0x10, which matches but should be unified with
  that class. 0x462860 and 0x4628a0 are methods of the channel class (a
  timeout and the send pacing), matched under separate placeholder classes.

- InitUnit's unit type parameter is `int` in 0x485e90.cpp but must be
  `unsigned short` where 0x4861d0 inlines it (#333); 0x485e90 also matches
  with `unsigned short`, so settle on that.

- 0x437800 is recorded as `Class_00437800::Class_00437800` but is
  `Class_00437820::operator=` (`??4Class_00437820@@QAEAAV0@ABV0@@Z`, the
  compiler-generated assignment of `{Class_004c91a0 handle; int field_4;}`,
  27 bytes, called only from 0x437580's fill and copy_backward); 0x4c93b0
  (`Class_004c93b0::Assign`) is the string handle's
  `Class_004c91a0::operator=`. Rename both before 0x437580 can match; its twin
  at 0x488fb0 (with 0x489240) is the same instantiation for another vector
  (#428).

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
  files that use them define them identically. `PlayerAI`,
  `Class_00409470`, `Class_00409730`, `Class_0040a150` and `Class_0040a7b0`
  are all that AI object (g_playerAI[player]).
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
- The tables read from gamedata\los.tdf (0x433130, 0x433380), around
  0x4330b0-0x434a30 (#187): the 4-byte element (two `unsigned short`s, see
  0x4339e0) is `Elem_00434020`, and `Elem_00434360` is a struct holding one
  `std::vector<Elem_00434020>`, with implicit members (0x434770 calls its
  `??_G`, 0x4349f0, and its copy constructor, 0x434470).
  The global at 0x51e6a0 is `vector<vector<Elem_00434360>>` (insert 0x4340f0,
  erase 0x434360). `vector<Elem_00434360>` owns 0x433a80 (destructor),
  0x433b00 (`size`), 0x433db0 (`insert`), 0x434020 (`erase`), 0x4340b0
  (`_Destroy`), 0x4344e0 (copy constructor) and 0x434770 (`operator=`), plus
  0x434400 (`allocator::destroy`) and 0x434440 (`std::_Destroy`, `/Gz`).
  `vector<Elem_00434020>` owns 0x433a10 (constructor), 0x433a30 (destructor),
  0x433a60 (`size`), 0x433d50 (`erase`), 0x433d90 (`_Destroy`), 0x4345e0
  (`operator=`) and 0x4349c0 (`_Ucopy`), plus 0x433da0 and 0x4343f0
  (allocator) and 0x434430 (`std::_Destroy`). Evidence: 0x433380 fills a
  `vector<Elem_00434360>` with a value it destroys through 0x433a30, and
  0x4336f0, called on each of its elements, uses 0x433a60 and 0x433d50 on the
  element itself; 0x433270 frees each element of its local column through
  0x433d90 and 0x433da0. `Elem_00433d50`, `Elem_004349c0` and
  `Elem_004340b0` became `Elem_00434020`, and the middle level, spelt
  `vector<vector<Elem_00434020>>`, became `vector<Elem_00434360>`. Where the
  implicit destructor costs an inline level (0x433a80, 0x434020), a
  `std::_Destroy(Elem_00434360*)` overload running the held vector's
  destructor keeps the call to 0x434430 inline. Left over: 0x433500 spells
  the global one level short (only that spelling schedules like the original;
  its return type is not compared), 0x433270 wraps the held vector in a
  second struct for inline depth, 0x433540 (no callers) destroys a vector of
  `vector<Elem_00434020>`, and 0x4335f0 (partial) calls 0x433db0 and 0x433a30
  under `Class_` placeholders.
- The map around 0x46e160-0x46ff90 (#201, #249) is
  `std::map<unsigned int, UnitSyncEntry>`, a 16-byte value (`x`, `y`, short
  `w` and `h`, one int) keyed by the dword at +0x13e of the 0x249-byte unit
  definitions (0x46e160); its `_Tree` has `_Nil` at 0x51e598 and
  `_Nilrefs` at 0x51e59c. The object holding it at +0 is destroyed by
  0x46c920, 0x46ca60 and 0x46d1a0, which inline `~_Tree()` and call the
  out-of-line `erase(first, last)` (0x46e890).
  Real template names so far: 0x46e890 (`erase(iterator, iterator)`),
  0x46f6d0 (`_Erase`) and 0x46ea10 (`iterator::_Inc`), renamed in #249 from
  `Class_0046f6d0::FUN_0046f6d0` and `Class_0046ea10::FUN_0046ea10`; the
  real `<map>` compiles all three to the same bytes. Still placeholders for
  the same tree: 0x46e9b0 (`find`), 0x46f720 (`_Init`), 0x46fe60 (`_Lbound`,
  called by the inlined `find()` in 0x46e160, 0x46e280, 0x46e330, 0x46e3c0,
  0x46e450, 0x46e4d0, 0x46e550 and 0x46e9b0), 0x46feb0 (`_Lrotate`),
  0x46ff10 (`_Rrotate`), 0x46ff70 (`_Buynode`) and 0x46ff90
  (`iterator::_Dec`); not matched yet: 0x46f1e0 (`erase(iterator)`) and
  0x46ef50 (which calls `_Buynode`). `Rect_0046e330`, `Rect_0046e450` and
  `Event_0046e280` in those files are the same value type.
- Overloads share one name in `tools/check.py` (`base_name` drops the
  argument list), so 0x46e890, which calls the other `_Tree::erase`
  overload (0x46f1e0) from `erase(_F++)`, is byte-identical with every
  reference right but is reported "a reference is wrong" once its own name
  is learned. It needs
  `IURect_0046e160::IU?$pair::?$_Tree::erase,0x46f1e0` in
  `data/aliases.csv` (checked in #249: with it 0x46e890 matches and no other
  status changes), or overload-aware names in the checker.
- The element of the vector erased in 0x46dad0 is `Class_0046eaa0`
  (0x5c bytes, operator= 0x470040, `_Destroy` 0x46eaa0): an int, two
  `vector<Elem_004702a0>` (+0x4, +0x14), three dwords, and at +0x30 a
  struct `PacketSequencer` (its operator= is 0x470560) holding three dwords
  and two `vector<Elem_0046faf0>` (+0x3c, +0x4c; the second's operator= is
  0x4707a0). FUN_00470030 is `std::_Destroy(Elem_0046faf0*)` from a `/Gz`
  file; 0x46eaa0 reaches it one inline level deeper than /Ob2 would go,
  so it stands in for the template with an explicit specialisation of
  `allocator<Elem_0046faf0>::destroy` that calls FUN_00470030.
- Two element types for one 2-byte scalar (#335): the exe has two
  byte-identical sets of `std::vector` members for 2-byte scalars, and the
  linker keeps one copy per mangled name, so each set is its own element
  type. `vector<short>` is the per-unit-type count table at +0x7d of the
  player AI object (every read sign-extends it: 0x409730, 0x40bb00,
  0x40c200; 0x40aa40 fills it as `vector<short>`) and owns 0x40d000
  (`size`), 0x40d020 (`insert`), 0x40d240 (`erase`) and 0x40d280
  (`_Destroy`), which were spelt `vector<unsigned short>`.
  `vector<unsigned short>` is 0x424c00's feature type remap table (0xffff
  is "none") and owns 0x4251f0 (`size`, was `Class_004251f0::FUN_004251f0`),
  0x425210 (`insert`, not matched), 0x425430 (`erase`) and 0x425470
  (`_Destroy`), both of which were `vector<Elem_00425430>`. The feature list
  DAT_00511fb4 is `vector<Class_004c2ea0*>`: 0x4251e0 (`_Destroy`, a member
  called with ecx set to the vector, was the free `__stdcall FUN_004251e0`)
  and 0x425480 (`insert`, was `Class_00425480::FUN_00425480`; 0x4222e0
  now reaches it through `push_back`).
- `std::copy<unsigned short*, unsigned short*>` (0x4256a0) comes from the
  same `/Gz` file as 0x424c00. MSVC 5 names a function template
  instantiation without its template arguments, so
  `template <> unsigned short* __stdcall std::copy(unsigned short*, ...)`
  with a body compiles to the `/Gz` name `?copy@std@@YGPAGPAG00@Z` (with
  warning C4666). Such a declaration cannot be called while the real
  `<xutility>`'s `__cdecl` template is visible (C2568, or the template is
  inlined instead), so 0x424c00 defines `_XUTILITY_` and declares the same
  `<xutility>` templates `__stdcall` before `<vector>`, a stand-in for `/Gz`
  until files are regrouped. Every instantiation of it is named `std::copy`
  by the checker, so the next matched one needs a row in `data/aliases.csv`.
- 0x424c00 (partial) still writes out the two vectors as explicit
  specialisations under their real names: with the real `<vector>` its
  second `resize` either inlines `copy` (64.3%) or, with the `__stdcall`
  `<xutility>`, calls `erase` out of line (64.6%), while the original
  inlines `erase` and calls `copy` and `_Destroy`.
- **`g_game->players` has 11 slots** of 0x14b bytes, +0x1b63 to +0x299c (the
  next field, `duplicateIds` in 0x450980, is at +0x299c). Most files declare
  `players[10]` with padding after it; 0x473590, 0x473a00, 0x474170,
  0x464060 and 0x41d920 declare all 11. Consolidate to 11.
- **`Class_004c42a0` was also called `Section_004c51b0` and
  `Section_004c3240`**: its destructor is 0x4c42a0 and its scalar deleting
  destructor 0x4c32f0. 0x4c51b0.cpp, 0x4c3120.cpp and 0x4c3240.cpp were
  renamed to `Class_004c42a0` when #381 landed.
- **`Class_0043a1f0` has two shapes**: 0x43a420 (#5538) gives it a second
  base class whose inline constructor clears the kind byte at `this + 4`
  (needed for the match), while the matched sibling 0x43a0c0 drops to 98.9%
  with that base. Reconcile when the class is consolidated. 0x43a420 also
  needed an alias row for `Class_0044e740`'s second constructor at 0x44e7d0.
- **0x410850 includes `ta_types.h` inside `namespace ta { }` without
  using its types** (#5664): its own views clash with the header by name, and
  the header flattens Class_00410830's `std::vector<Unit*>` base into plain
  fields. The include only sets symbol ids (unit at 65854, window 65808 to
  65919). Clean-up: have `tools/gametypes.py` keep base classes, include the
  header normally, and retune the files that depend on its size (see
  docs/c2-regalloc.md, "Files that depend on its exact size").
