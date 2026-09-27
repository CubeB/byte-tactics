# Consolidation notes

Things to resolve when the per-function files in `src/unsorted/` are merged
into real classes and translation units. Agents name unknown classes after
single addresses, so one real class often appears under several names.

## Classes to merge

- `Class_0044cf60` and `Class_0044d010`: both constructors store vtable
  `DAT_004fd328` and fill the same fields (+8 packed point, +0xc radius,
  +0x10 radius squared). Probably overloaded constructors of one class.
- `Class_004c91a0` (copy constructor, 0x4c91a0) and `Class_004c93b0` (assignment,
  0x4c93b0) are the same reference-counted string handle.
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

- 0x458d20 and 0x474d10 force a dead stack store with `volatile`. A reserved
  `push ecx` slot plus a store that is never read is the signature of an
  inlined `std::vector<int>` destructor (compare 0x46e610 and 0x438480); the
  real source is probably a vector member or local.
