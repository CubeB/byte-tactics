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
