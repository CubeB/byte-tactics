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

## Full sound table reported as a successful insert (likely)

**0x429470**, which adds a sound to a 0x100-slot name table. When the table is
full (`soundCount + 1 >= 0x100`) it returns 0, which is also what the very
first successful insert returns (its index), so a caller cannot tell "full"
from "added at slot 0". The same guard stops at 0xfe, so slot 0xff is never
used. Found by Space Bunny Free in #25.

## Piece centre minimums seeded with 0 (likely)

**0x43e0b0**, which works out a 3D piece's bounds and centre. All six
accumulators (minimum and maximum per axis) start at 0
(`xor edx, edx; xor edi, edi; xor esi, esi` at 0x43e0cb), and the minimum test
has no large sentinel, so a minimum can never be above 0. For a piece whose
vertices are all positive on an axis, the centre is pulled towards the
piece's origin. The maximums are unaffected. Found by Space Bunny Free in #34.

## Texture pass uses the model as a texture entry (likely)

**0x42a140**, a model texture pass. Its `entry` variable is only set inside the
first lookup loop; when `g_game->blockCount` (+0x148df) is 0 or less the loop
is skipped and `entry` still holds the model pointer, which is then treated as
a GAF entry (compared with 10 and 1, passed to FUN_004b8b30 and stored). With
exactly one texture GAF loaded, 0x42a440 sets blockCount to 0, so the path is
reachable. Found by DeepSeek V4.1 Flash in #26.

## Division by zero with one texture file (likely)

**0x42a440** divides its progress value by `count - 1` (`cdq; idiv ebp`) with
no guard, so a single texture file divides by zero. Found by DeepSeek V4.1
Flash in #26.

## Off-by-one append past a 30-entry list (likely)

**0x42be30**: the append guard is `e->count <= 0x1e`, so at `count == 30` it
writes `e->items[30]`, but the items array is allocated with 60 bytes (30
shorts, `push 0x3c` at 0x42dac7), two bytes short. The guard should be
`count < 0x1e`. Found by DeepSeek V4.1 Flash in #26.

## Z offset computed from the rotated x (likely)

**0x43d0d0**: its second FUN_004b715a call, for the +0x68 (z) offset, reads the
same stack slot as the first (`[esp+8]` at 0x43d1a0, then `push edi;
mov ecx, [esp+0xc]` at 0x43d1ce), so both use the rotated x and the rotated z
is never read. Found by DeepSeek V4.1 Flash in #33.

## Player name overflows a 100-byte buffer (likely)

**0x446080** formats the localized "Reject" prefix and a player name into a
100-byte stack buffer with `sprintf` (`sub esp, 0x64`); a long enough name
overflows it. Found by DeepSeek V4.1 Flash in #126.

## Loading a save runs a destructor on file data (likely)

**0x44de80** constructs an embedded `Class_004895c0` at `rec+0xa`, then reads
a 0x36-byte record from the save file over it, and at the end runs that
object's destructor on whatever the file contained; non-zero bytes at +0xe
make the destructor follow a pointer taken from the file. Its saver 0x44dfb0
writes the embedded vtable pointer (0x4fd754) and 8 never-set stack bytes into
the file (so do 0x44d090, 0x44d500 and 0x44d9a0 with their first dword).
Found by DeepSeek V4.1 Flash in #136.

## Unit type fallback index off by one per skipped type (likely)

**0x43a360** returns `n`, which counts every unit type entry (`inc eax` at
0x43a404, outside the flag test), but matches on `k`, which skips types whose
+0x241 bit 5 is set (`inc ebp`, inside it). The fallback result is therefore
off by one for each skipped type before the match. Found by Space Bunny Free
in #32.

## Two sort helpers pop the wrong number of argument bytes (likely)

**0x43ca70** ends in `ret 0x5c` but both its callers (0x43be6a, 0x43c20c) push
0x60 bytes; **0x43cb20** ends in `ret 0x28` but its callers push only 0x24.
The two 4-byte errors cancel only because the calls always come in pairs, so
either function called alone would unbalance the stack: a declaration that
disagrees with its definition. Found by Space Bunny Free in #32.

## Unit text used as a format string (likely)

**0x40c250**, an AI diagnostic report, formats unit names and descriptions
into a buffer with `sprintf` and then passes that buffer to `fprintf` as the
format string (0x40c4b0 pushes only the `FILE*` and the buffer), so any `%` in
unit text is interpreted again. Found by ozgb's Codex / GPT-6 Astra in #78.

## Player cleanup loops run one past the array (likely)

**0x4453a0** and **0x445450** loop `for (i = 0; i <= 10; i++)` over
`g_game->players[10]` (`cmp eax, 0xcee; jle`, where 0xcee is 10 * 0x14b), so
the last pass touches players[10] at +0x2851, one slot past the array;
0x445450 writes to its field_146 (0x2997), clobbering whatever follows the
array. Found by Space Bunny Free in #125.

## Watching another player overwrites your own unit limit (likely)

**0x445b70**: when FUN_00456850 names a player other than the local one, the
code reads that player's maxunits (+0xa5) but stores it into the local
player's record (via +0x2a42), so watching someone else replaces your own
unit limit. It also stores the value twice. Found by Space Bunny Free in #125.

## Displaced piece vertices never restored (likely)

**0x45b030** (inlined into 0x45ab10) restores a piece's vertices only when its
flag at +0x26 is 0 (`cmp word ptr [ebx+0x26], bp; je` into the `rep movsd`),
then clears that flag, so the clear does nothing and a piece whose vertices
were actually displaced (flag set) is never restored. The test looks
inverted. Found by Space Bunny Free in #142.

## Segment vertices overflow a 25-entry stack buffer (likely)

**0x45a610** copies `seg->count` 12-byte vertices into a `Vertex tmp[25]` on
its stack and passes that count on to FUN_004c1000, with no bound; a segment
with more than 25 vertices overruns `tmp` into the vertex array above it.
Found by Space Bunny Free in #142.

## Both dialog choices named "CHOICE2" (likely)

**0x460680** stores the same literal, "CHOICE2" (0x503120), as the name of two
gadgets (at 0x4606d9 and 0x460710), where the matching dialog setup 0x464e70
names them "CHOICE1" and "CHOICE2": a copy-paste slip. Found by DeepSeek V4.1
Flash in #167.

## Cloak state always "mixed" for several cloakable units (likely)

**0x41b2e0**, which builds the order bar's combined state for the selected
units. For the cloak button it sets the state to 2 ("mixed") for the second
and every later cloakable unit without comparing its cloak bit
(`cmp [esp+0x18], 3; jne 0x41b497` goes straight to `mov [esp+0x18], 2` at
0x41b485), so two units that are both cloaked show as mixed. The fire order,
move order and on/off buttons compare first (`cmp esi, ecx; je` at 0x41b893).
Found by Claude Opus 5.5 in #208.

## Feature seeding uses the map height as the row divisor (likely)

**0x424050**, the per-tick feature update: when a scanned cell seeds a nearby
copy of its feature, the column is `scanIndex % width` (+0x14233) but the row
is `scanIndex / height` (+0x14237), while cells are indexed as
`row * width + column`. On a map that is not square the seed lands in the
wrong row (the two `idiv`s on the same index at 0x424137 and 0x424142). Found
by Claude Opus 5.5 in #275.

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
- **0x40d900** (clears the AI search grid's touched cells): in the last block
  the bounds check restarts at `(i << 8)` for every group of eight cells
  instead of advancing, so it only really tests the first group. Harmless,
  because the constructor 0x40e9e0 rounds the cell count up to a multiple of
  8 and allocates that many, so every group is either wholly valid or never
  marked. Found by Claude Opus 5.5 in #90.
- **0x419670**: the unit type's flag bit 11 is tested twice in a row
  (`test ah, 8; jne` at 0x41976e lands on `shr eax, 0xb; test al, 1; je` at
  0x419789), so the second test's branch can never be taken. A redundant
  condition in the source. Found by DeepSeek V4.1 Flash in #17.
- **0x40e630** (starts a path search): the start node's short at +0xc of its
  data is never set. The node is built on the stack with only its position and
  the word at +0xe (100) written, then copied into the pool, so +0xc is stack
  garbage, and FUN_0040da70 adds a node's +0xc into its cost (0x40db0c).
  Probably harmless, since the start node is popped and closed on the first
  expansion before anything reads it. Found by Claude Opus 5.5 in #81.
- **0x40eb70** (the per-tick path scheduler): the `r < 3` case and the final
  `else` set the same value, and its second call to 0x40ef20 can never run.
  Found by Claude Opus 5.5 in #81.
- **0x41d7b0** (builds a path on the CD): its format string at 0x502914 is
  `%c\\%s\%s`, two backslashes after the drive letter and one before the file
  name, so paths come out as `D:\\dir\file`. Windows accepts the doubled
  separator. Found by DeepSeek V4.1 Flash in #21.

- **0x42db90**: clears `field_152` together with `field_156` although only
  `field_156` is tested, so `field_152` is zeroed even when `field_156` is
  already null. Found by Space Bunny Free in #27.

- **0x428d10** (a script tokenizer): the number branch tests `c != '-'`, but
  the punctuation branch above it already returns on any `-`, so that test
  can never fail. Found by Space Bunny Free in #25.
- **0x428e90** and **0x428f60**: format a parse error into a 256-byte local
  buffer with `sprintf` and never use it, as if a display or log call was
  removed. Found by Space Bunny Free in #25.

- **0x440940** (loading progress): the counter starts at 100 and adds 100
  before each division, so entry i reports `100 * (i + 2) / count`, starting
  one step ahead and passing 100 near the end; a final store of 100 hides it.
  Entries skipped for a zero field still count in the divisor. Found by Space
  Bunny Free in #34.

- **0x446310**: allocates an "AVAILABLE MODES" buffer (`count << 8` bytes at
  `obj+0x14`), zeroes its first byte and never reads or frees it, so it leaks.
  Found by DeepSeek V4.1 Flash in #126.
- **0x446e90**: tests `players[i].type != 4` after an inlined check has
  already limited that byte to 1, 2 or 3, so the test is dead. Found by
  DeepSeek V4.1 Flash in #126.
- **0x45b150**: a `deep = 1` store at the end of the loop body is dead, since
  the branch reaching it has already tested `deep` as 1. Found by Space Bunny
  Free in #143.

- **0x445c70** stores the same value to unit+0xa3 twice, the second time
  through a fresh lookup of the local player's unit (0x445d08, 0x445d36); its
  sibling 0x445d60 stores once. Found by Space Bunny Free in #125.

- **0x452570** searches the ten player entries for the same id twice; the
  second search's result is thrown away. Found by DeepSeek V4.1 Flash in #139.

- **0x471340** caps a list with `if (v.size() > 400)` before pushing, so the
  list reaches 401 entries before one is evicted; an off-by-one if 400 was
  meant as the maximum. Found by DeepSeek V4.1 Flash in #201.

- **0x41f0a0** scans the 25 mission flags at g_game+0x391cf for the first
  'U' after filling the missions list and never uses the index, perhaps a
  lost "select the first unplayed mission" step. **0x41ec50** calls
  FUN_004ab0a0(gadget) twice in a row in its load and save branches. Found by
  Claude Opus 5.5 in #211.

## Possible leaks and unchecked inputs

- **0x413470** (an order handler), state 3 (likely): after two misses it
  allocates a `Class_0044e2d0` waypoint and sets its speed with
  `FUN_0044e730(0x80)`, then only ORs 0x110e8 into the order flags and returns
  2. Every other branch hands the waypoint to the order through
  `FUN_004388d0`; this one never does, so the object leaks and the waypoint
  is lost. Found by Claude Opus 5.5 in #98. **0x4111b0** (VTOL transport,
  state 4) does the same with its pickup waypoint (#96).
- **0x4384a0** (possible): when `operator new(0x56)` returns 0 it skips the
  constructor and then reads `[eax + 0x42]` with eax still 0 (0x438537), a
  null read on allocation failure. Found by Space Bunny Free in #31.
- **0x41d7b0** (possible): calls `strlen(ext)` with no null check, where its
  sibling 0x4290f0 tests `if (ext != 0)` first, so a null extension would
  crash here. Found by DeepSeek V4.1 Flash in #21.
- **0x402010** (possible): the unit type's countdown is a three-bit field
  (0 to 7) but indexes a six-entry sound array with no bound, so values 6 and
  7 would read past it. Whether the data ever holds those values is unknown.
  Found by Codex / GPT-6 in #7.
- **0x420e50** (possible): builds a spawn structure on the stack whose flags
  dword at +0x28 is only partly set. It clears bits 4 and 5 and sets bits 1 to
  3, so bit 0 and bits 6 to 31 keep stack garbage, and the whole structure is
  then copied into the spawned object (FUN_00421620's inlined `rep movsd`).
  Harmless if nothing reads those bits. Found by DeepSeek V4.1 Flash in #22.
- **0x43de30** (possible): its chunked read `FUN_004b4c80(buf, 0x23)` is not
  checked, so a short read copies a partly uninitialised record into the unit
  type; its sibling loader 0x44d930 does check. Found by DeepSeek V4.1 Flash
  in #33.
- **0x45bbf0** (possible): reads `e->list` before the null check of
  FUN_004a0200's result (0x45bc07, then `test eax, eax` at 0x45bc0d), so a
  missing "VIDSLDR" entry would be dereferenced. Found by Space Bunny Free in
  #143.
- **0x44b140** and **0x44b230** (possible): never check `fopen`'s result, so a
  missing or unwritable file passes a null `FILE*` to `fread`/`fwrite` and
  `fclose`; the sibling writer 0x4bc290 does check. Found by DeepSeek V4.1
  Flash in #127.
- **0x44e5b0** (possible): in the `flags & 4` branch it reads
  `target->heading` with no null check; `target` is only tested when
  `flags & 1` is set. Found by DeepSeek V4.1 Flash in #136.
- **0x44c0d0** (likely): its null check tests the address of the unit type's
  name array (`lea eax, [esi+0x20]; test eax, eax` at 0x44c154) instead of the
  type pointer, so it can never fail, and a null type is then read at
  `[esi+0x245]`. Found by DeepSeek V4.1 Flash in #128.
- **0x44fc10** (possible): the send-side encrypt-and-checksum loop runs
  `for (i = 3; i < total - 3; i++)` with `total = payload size + 3`, so the
  last three payload bytes of every outgoing packet are neither XORed nor
  added to the checksum. Harmless if the receiver skips the same bytes, which
  is not checked yet. Found by Space Bunny Free in #137.
- **0x4523e0** (likely): when `to` is -1, its inlined player search returns
  10 and the function writes the new group through `players[10].data`, one
  past the ten-player table (a pointer read from g_game+0x2878); the other
  users of the same search (0x44fed0, 0x452800, 0x4526c0) check for 10 first.
  Found by Claude Opus 5.5 in #228.
- **0x4523e0** (possible): when all ten group slots are in use (possible when
  `to` is not an active player, such as -1), the loop ends without writing
  the message's value byte at `[esp+0x13]`, so FUN_00451bc0 sends a two-byte
  message whose second byte is uninitialised. Found by DeepSeek V4.1 Flash in
  #139.
- **0x476830** (possible): its "lowercase" loop adds 0x20 to every non-zero
  byte, so `.` becomes `N` and `a` becomes 0x81; fine only if the input is
  always upper case. It also writes one byte past a `count * 30` buffer for a
  30-character name. Found by Space Bunny Free in #207.
- **0x476cd0** (possible): the copy loop tests the next byte rather than the
  current one, so the last input character is never copied, and the
  quoted-newline path overwrites the byte just written with `&`. Found by
  Space Bunny Free in #207.
- **0x417890** (likely): a debug console command formats
  `debugdat\%s.txt` with its argument into a 60-byte stack buffer using
  `sprintf`, with no bound, so a long argument overflows it. Found by ozgb's
  Codex / GPT-6 Astra in #174.
- **0x418310** (possible): the arrow overlay tests the masked flag 4, but its
  colour switch uses the unmasked flags and sets the colour only for 1 and 2,
  so flag values 5 and 6 draw with a stale stack byte (loaded at 0x4186f8).
  Found by ozgb's Codex / GPT-6 Astra in #174.
- **0x4861d0** (possible): when a spawn record's id is 0 the unit pointer is
  null, but `unit->field_a6` is still read (`cmp word ptr [esi+0xa6], 0` right
  after `xor esi, esi`), then written, and the null pointer is passed to every
  callee; only the loop inlined from FUN_00485e90 checks for null. Harmless if
  id 0 never occurs. Found by Claude Opus 5.5 in #333.
