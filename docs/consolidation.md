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

## Matches that use suspicious constructs

These match byte-for-byte but use something Cavedog probably did not write;
revisit them once the surrounding code is known.

- 0x474d10 and 0x475110 (scalar deleting destructors of classes derived from
  `Class_00471cc0`) call the base destructor explicitly, as
  `((Class_00471d00*)this)->FUN_00471d00()`, after `records.~vector()`. The
  real source was an implicit destructor chain; that needs 0x471d00 renamed as
  the base class destructor, together with all its callers.

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
  vector, and 0x434770's recorded name is one level too shallow.

## Signatures that disagree

The checker compares names, not parameter types, so callers and definitions
can disagree on types (a real link would fail). Known cases:

- `FUN_004ba590`: its file takes `int`; callers such as 0x417290 pass `float`.
- `FUN_004d0620`: its file returns `void`; 0x47efe0 uses a `void*` result.
- `Class_0043a0c0`'s constructor: 0x43a020 and 0x43b730 declare its first
  parameter `unsigned char`, but 0x401c20 shows it is a 1-byte class passed by
  value, built by `Class_00438760::Class_00438760`.

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
