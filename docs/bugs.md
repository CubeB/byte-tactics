# Bugs in the original game

Matching means reproducing Cavedog's code exactly, mistakes included. This file
records the mistakes found along the way: code that compiles to the original
bytes but almost certainly does not do what its author meant. The source keeps
the bug (it has to, to match) with a comment; the entry here says what goes
wrong and how sure we are.

Confidence: **likely** means the code is clearly wrong on its face; **possible**
means it looks wrong but the intent is not certain from the code seen so far.

## Bit writer grows its buffer into a single dword (likely)

**0x415bb0**, also inlined into 0x415c10. `Class_00415b60` is a bit writer with
a 0x100-dword inline buffer. When it runs out of space it allocates the bigger
buffer with

```cpp
unsigned int* grown = new unsigned int(newCapacity);   // one dword, set to newCapacity
```

where `new unsigned int[newCapacity]` was surely meant, then copies the old
contents (`capacity` dwords) into it. Anything that writes more than 1 KB of
bits therefore overruns a 4-byte heap block. It also frees the old buffer with
`delete` rather than `delete[]`. In practice the inline buffer is probably big
enough for the game's own data, which would explain why it went unnoticed.

## Send reads a target's id through a null pointer (possible)

**0x46d530**. The send helper `Class_0046d4c0::Send(target, packet)` (inlined)
reads `target->id` when the object is in "direct" mode. 0x46d530 calls it with
no target, so in direct mode it reads address 0 (`mov eax, [0]` in the
original) and would crash. Either direct mode is never on when this runs, or
it is a latent crash. Its sibling 0x46d630 passes a real target.

## Entry search always returns 0 (possible)

**0x4a18c0**. Counts the type-8 entries up to the one numbered by
`entries[index].field_27` and returns 0 whether or not it finds it; the found
path looks like it should return 1 (or the count). Its caller tests the
result, so that test can never succeed.

## Sprite reference overwrites its first field (possible)

**0x4b8ae0**. Initialising a 0x14-byte frame reference, the code writes
`dst->a` twice, the second time from `src->c`, so `src->a` is lost and
`src->c` probably had another destination field. The field meanings are not
known yet, so this may be deliberate.

## "any" difficulty is only recognised as the first argument (likely)

**0x406c90**, a console command that parses difficulty arguments. Its loop
compares each argument against "easy", "medium" and "hard" using the loop
index, but compares against "any" using argument 1 every time (the original
pushes the constant 1, `ebx`, where the other comparisons push the index,
`edi`). So "any" is ignored unless it is the first argument. Found by Codex /
GPT-6 in #8.

## Construction-assist radius adds y twice instead of squaring it (likely)

**0x403f70**, the order handler for helping another unit build. It works out a
target radius as `sqrt(x*x + y + y)` where `sqrt(x*x + y*y)` was surely meant:
the original's x87 sequence at 0x40401d is `fld st(1); fmul st(2); fadd st(1);
fadd st(1); fsqrt`, adding the second coordinate twice. The effect is a radius
that grows roughly with the square root of y rather than with y, so assisting
units stop at the wrong distance for large footprints. Found by ozgb's Codex /
GPT-6 Astra in #38.

## Group attack target used without a null check (likely)

**0x407ae0**, slot 0 of `Class_00407a90` (an AI unit group). It pushes
`&target->pos` straight after `Class_004071f0::FUN_004071f0`, which returns 0
when it finds no enemy unit (see 0x4071f0.cpp), so with no enemy the group is
sent towards address 0x6a. Found by Claude Opus 5.5 in #54.

## Group centre may drift after moving a unit (possible)

**0x407560**. After `FUN_00480250(*best, kind)` moves the farthest unit to the
other group, the code re-reads `*best` to subtract that unit's position from
the running sums. If FUN_00480250 erases the unit from this group's vector,
`*best` then names the next unit and the centre drifts. Not verified until
FUN_00480250 is decompiled. Found by Claude Opus 5.5 in #54.

## Base height overwritten while measuring flat distances (likely)

**0x408100**, slot 0 of `Class_004085d0` (one of the AI's unit groups). To get
a horizontal distance it overwrites the base position's y in place (at
0x408250 and 0x40834f) and never restores it, so every later unit in the loop
is measured against the previous unit's height instead of the base's. Found by
Claude Opus 5.5 in #55.

## Harmless oddities

Things that look wrong in the original but have no effect, kept for the record.

- **0x404db0** (resurrect order): the feature pointer is first set to
  `&features[0xffff]`, the "no feature" index far past the end of the table,
  before the state test; no path reads it in the state where it stays that way.
- **0x404db0**: the second failure message is spelt "Ressurection failed",
  the first "Resurrection failed".
- **0x407e90**: an unused `std::vector` local is constructed and destroyed.
- **0x40e9e0** (a map grid constructor): it clears the pointer at +0x1c and
  then immediately passes it to `operator delete`, a dead free of the buffer
  it is about to allocate. `delete 0` does nothing. Found by DeepSeek V4.1
  Flash in #12.
- **0x40e9e0**: the second buffer's size is `((cells + 0xff) >> 8) * 4`, and
  the code fills `size - 1` bytes and writes the last dword with no check, so
  a map with no cells would underflow to a 4 GB `memset`. Only reachable with
  zero map dimensions. Found by DeepSeek V4.1 Flash in #12.
