# Bugs in the original game

Matching means reproducing Cavedog's code exactly, mistakes included. This file
records the mistakes found along the way: code that compiles to the original
bytes but almost certainly does not do what its author meant. The source keeps
the bug (it has to, to match) with a comment; the entry here says what goes
wrong and how sure we are.

Confidence: **likely** means the code is clearly wrong on its face; **possible**
means it looks wrong but the intent is not certain from the code seen so far.

`g_game->players` has eleven 0x14b-byte slots (+0x1b63 to +0x299c, where the
next field starts), not ten, so loops over slots 0 to 10 are not overruns and
an index of 10 is the spare last slot. An earlier entry here that called those
loops (0x4453a0, 0x445450) an overrun was withdrawn in #413. An entry that
called a stack slot in 0x43cd20 uninitialised (#714) was withdrawn too: it
misread the push depth, and the writes it named go to two different slots,
each written before it is read. An entry on 0x4a4170's 16-bit `sub ax` / `add eax` was withdrawn in #5063: the sum is stored with `mov word ptr [ebx+0x140], ax` (0x4a427b), so the high half never matters.

An independent review (#5772) led to these changes, each checked against the
disassembly. Withdrawn: the "inverted" test on the piece flag in 0x45b030 (the
word at piece +0x26 is cleared by every change to the piece, at 0x480c85,
0x480d17, 0x480d8f and, for the root, 0x45ab7a, and nothing in the exe stores a
nonzero value to it, so 0x45b030 and 0x45b150 share the same normal test and
displaced vertices are restored); `field_152` cleared with `field_156` in
0x42db90 (both stores sit inside the `field_156 != 0` branch, `je 0x42dc66`
skips both); the two "CHOICE2" names in the surrender dialog 0x460680 (the
stores fill the layer's Enter and Escape default-gadget fields, the Yes and No
gadgets keep their own names, and both defaults pick No, as the dialog's initial
selection does); the position pointer passed to `FUN_004b7123` by 0x49c9c0 (the
argument slot is reused as scratch and holds the computed speed there; the
source already matched 600 of 600 bytes with that reading); the slider rounding
in 0x45d7c0 and 0x45b9b0 (the fraction test works on a copy, the position is
`ceil(value * (steps - 1) / 64)`); and the inverted fade return in 0x4c6890
(slot 5 is the DirectDraw `Blt` colour fill, whose `DD_OK` of 0 the wrapper turns
into 1); the "never runs" scroll-down test in 0x4a99c0 (the source reads
`sel < last + step && sel >= last`, an interval that is nonempty for any
positive step, and the disassembly agrees); the footprint scan in 0x47db70 (the
index is `y * width + x` with a stride of `width - footprint width`, and the
second argument is a 16-bit owner id that is compared but never dereferenced);
the saved `esi` overwrite in 0x45ffb0 (the saved register sits at the lowest
stack slot, below the 0x40 bytes of locals, so the store at 0x46011a stays
inside them and `pop esi` restores it); and the unstored `field_4` in 0x4743a0
(the 0x3c-byte record holds it at +0x38, stored at 0x4744d7, and the frame-count
store goes to +0x28, not into the third position). Corrected in place: 0x445b70,
0x445c70, 0x440940, 0x4a1b40, 0x4af320, 0x425b80, 0x4ab1b0, 0x4373a0, 0x4a36a0,
0x4ba9d0, 0x464060, 0x498da0 and 0x476cd0.

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

**0x46d530**. The send helper `UnitSync::Send(target, packet)` (inlined)
reads `target->id` when the object is in "direct" mode. 0x46d530 calls it with
no target, so in direct mode it reads address 0 (`mov eax, [0]` in the
original) and would crash. Either direct mode is never on when this runs, or
it is a latent crash. Its sibling 0x46d630 passes a real target.
The play tests for #2662 point to the first: the null read in the inlined
helper only happens when `direct` is set, but the call sits in a branch that
runs only when it is not, and `direct` is fixed when the object is created.
Five network games with the read pointed back at address 0 never faulted.

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
`&target->pos` straight after `Class_004071f0::FindNearestEnemyUnit`, which returns 0
when it finds no enemy unit (see 0x4071f0.cpp), so with no enemy the group is
sent towards address 0x6a. Found by Claude Opus 5.5 in #54.

## Group centre may drift after moving a unit (possible)

**0x407560**. After `SetUnitSquad(*best, kind)` moves the farthest unit to the
other group, the code re-reads `*best` to subtract that unit's position from
the running sums. If SetUnitSquad erases the unit from this group's vector,
`*best` then names the next unit and the centre drifts. Not verified until
SetUnitSquad is decompiled. Found by Claude Opus 5.5 in #54.

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
a GAF entry (compared with 10 and 1, passed to InitGafSequence and stored). With
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

## The unit limit is stored twice (likely, harmless)

**0x445b70** stores the unit limit into the local player's lobby record
(+0xa5) twice, the second time through a fresh lookup of the record (0x445c25,
0x445c51). FindHostSlot returns the host's slot (the first active slot whose
record has bit 0 of +0x97, the host flag, set), so when another player hosts,
the value shown and stored is the host's limit: a joining client follows the
host's setting. An earlier reading of this entry as a watching bug that
overwrites your own limit was withdrawn in #5772. Found by Space Bunny Free in
#125.

## Segment vertices overflow a 25-entry stack buffer (likely)

**0x45a610** copies `seg->count` 12-byte vertices into a `Vertex tmp[25]` on
its stack and passes that count on to FillFlatPolygon, with no bound; a segment
with more than 25 vertices overruns `tmp` into the vertex array above it.
Found by Space Bunny Free in #142.

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

## Missing `break` draws 128-wide texture spans twice (likely)

**0x4c7a20**, a span renderer that switches on the texture width (the jump
table at 0x4c7f88/0x4c7fa0). The unmasked case for width 0x80 calls
BlitSpan128 (0x4c7df3) and then runs straight on into the width 0x40 case,
which calls BlitSpan64 (0x4c7e0e) with the same arguments; nothing branches
between them. Both helpers fill the same destination span, the first with a
128-byte texel row (`shl ebx, 7`) and the second with a 64-byte one
(`shl ebx, 6`), so every unmasked span of a 128-wide texture is drawn
correctly and then overwritten with the wrong stride. The masked path handles
0x80 with its own loop. The function is matched, so the missing `break` is in
the source. Found by ozgb's Codex / GPT-6 in #2052.

## Gadget navigation clears only a quarter of its table (likely)

**0x4a7960**, which moves the GUI selection with the arrow keys, keeps an
`int used[200]` table on its stack (0x320 bytes from `[esp+0x28]`) but clears
it with `mov ecx, 0x32; rep stosd` (0x4a798b), 50 dwords: an element count
where the byte count 200 was meant, or the other way round. The
column-snapping loop fills `used[1..count]` in order and, for each gadget,
scans the entries already filled until it meets a 0, relying on the next slot
still being clear; from the 50th gadget on that slot holds stack garbage, so
the scan snaps to a stale value or reads on past the filled entries. Only GUIs
with 50 or more gadgets are affected. Found by ozgb's OpenCode /
deepseek-v4.1 in #2151.

## Skirmish starts after the missing-CD warning (likely)

**0x47ae60**, the skirmish menu handler. When the Start check for the
multiplayer CD fails (FindGameCdDrive(1) returns 0, so `jne 0x47af2e` at 0x47aef8
is not taken), it shows "Please insert the Multiplayer CD (Disc 1) and try
again" (0x5030c0, through 0x4abd90 at 0x47af19) and resets the button
(0x4ab0a0 at 0x47af29), then falls straight into the Start validation at
0x47af2e instead of returning. The other failures in the same block (terrain,
player count, too many players, a single allied group) all end in the shared
tail at 0x47b0de, which shows the message and returns. So without the CD the
warning appears and the game starts anyway if the other checks pass. Found by
ozgb's Codex / GPT-6 in #2020.

## A footprint's second height class is computed and never used (possible)

**0x47d820** walks a unit's footprint mask over the map cells. For cells with
mask bit 3 it keeps the lowest `field_6`, and for cells with bit 4 it keeps the
highest `field_5` in a byte local (`[esp+0x13]`, written only at 0x47d901), but
nothing reads that local: the result (0x47d92a to 0x47d94a) is the bit-3
minimum if any bit-3 cell was seen, otherwise the water level minus the type's
`+0x22c`. So bit-4 cells never affect the result; whether they were meant to
is not known. Found by CubeB's OpenCode / deepseek-v4.1-flash in #1846 and
Space Bunny Free in #2123.

## The "Ally" sound ignores the other player's flag (likely)

**0x447b10**, the ALLY button of the multiplayer setup screen. It picks the
sound from `ally2[i] << 1 == 3 | ally[i]`, which C parses as
`((ally2[i] << 1) == 3) | ally[i]`: the original loads the byte at +0x113,
`shl edx, 1`, `cmp edx, 3`, `sete al`, then ORs in the byte at +0x108
(0x447fc5 to 0x447fe5). A value shifted left by one is even and never equals 3,
so the test reduces to our own flag, and the other player's alliance state
never selects the "Ally" sound. Most likely meant `((ally2[i] << 1) | ally[i])
== 3`, both sides allied. Found by Claude Code / Opus 5.5 in #4830.

## A team search's "not found" falls back to entry 0 (possible)

**0x4a6ae0**, with the search inlined. The loop (0x4a6fc0 to 0x4a6fd2) walks the
0x15b-byte GUI entries for one with type 4 and the wanted group, and jumps to
0x4a6fd6 with the index in esi when it finds one; when none matches it falls
through to `xor esi, esi` (0x4a6fd4). The caller then tests `cmp esi, -1`
(0x4a6fd6), which can never be true, so a missing entry is not skipped but
treated as entry 0. Possible rather than likely: entry 0 may always be a valid
fallback in practice. Found by Claude Code / Fable 5.1 in #4854.

## A selected row without a cell is highlighted through a null pointer (likely)

**0x4a1b40**, cell mode of a list gadget. Each row's cell pointer is tested
(`test edi, edi; je 0x4a2216` at 0x4a2124) and the draw is skipped when it is
null, but the jump lands on the selected-row highlight, which reads the cell's
width and height with no second test: `mov cx, word ptr [edi]` (0x4a2233) and
`mov ax, word ptr [edi + 2]` (0x4a224c) with edi still 0. A selected row with
no cell would read address 0 and fault. It may never happen in practice if
every selectable row has a cell. Found by Claude Code / Opus 5.5 in #4857.

## A flag test that is always true (likely)

**0x4a3780**, the list layout. `flags & 0x20 | 0x80` parses as
`(flags & 0x20) | 0x80`: the original computes `and ecx, 0x20`, `or cl, 0x80`,
`test cl, cl` and `je 0x4a3cd3` (0x4a3c29 to 0x4a3c33), and the `je` can never
be taken. So a list with neither flag still runs the variable-row walk. Most
likely meant `flags & (0x20 | 0x80)`. Noted in earlier attempts and confirmed
by Claude Code / Opus 5.5 in #4856.

## A position is used before its null test (possible)

**0x43f0e0**, case 12 of the order chooser. It passes `pos` to GetMapCellAtPosition
(push and call at 0x43f50a, 0x43f50b), which reads `[pos]` and `[pos + 8]` with
no check (0x4815a5, 0x4815a7), and only later tests `pos` for null
(`test esi, esi` at 0x43f58c) before the RECLAIM branch. So the code treats
`pos` as possibly null after it has already dereferenced it; a null `pos` would
fault in GetMapCellAtPosition. Possible rather than likely: case 12 may never be reached
without a position. Found by Claude Code / Opus 5.5 in #4914.

## The hotkey underline is placed by the wrong string's width (possible)

**0x4a5f40**, the flags 0x20 path of a list-gadget entry (centred text with an
underlined hotkey letter). After drawing the text before the hotkey
(`call 0x4a50e0` at 0x4a6817), it advances the underline's x by the width of
`[esp+0x4c]` (0x4a681e onwards), which is the entry's first string, not the
prefix it just drew. When field_136 selects a later string, the underline lands
in the wrong place. The flags 2 branch measures its truncated copy instead.
Possible: entries with a hotkey may never select a later string. Found by
Claude Code / Opus 5.5 in #5148.

## An order button reads an unassigned entry pointer (likely)

**0x419be0**, which draws the order buttons. The entry whose state the buttons
test is only assigned when `entries[button->index].type == 1`. Otherwise the
code reads the variable without assigning it: at 0x419c15 to 0x419c1c the
original does `mov ebp, ecx` / `cmp byte ptr [ecx], 1` / `je` /
`mov ebp, [esp+0x34]`, and nothing in the function stores to `[esp+0x34]`.
MSVC put the unassigned variable's home in the dead stack slot of the `button`
argument (`button` itself lives in edi from 0x419bec), so the read gets the
button pointer and the later tests read the button's bytes as the entry's
state. It works by accident. Found by Claude Code / Opus 5.5 in #5276.

## A long command line makes the tokenizer write past its object (likely)

**0x4b7440** (`Tokenize`), which splits a console line or a line of an AI
script into arguments. It copies each token into the 0x7e-byte text buffer at
+0x50 and stops when that buffer is full (`lea edx, [ebp+0xce]` /
`cmp ebx, edx` / `jae` at 0x4b74c3), but it leaves the text pointer on the
character it did not copy. The outer loop then finds that same character
again, stores another terminating zero one byte further on and starts a new
token there, so it never reaches the end of the text: every pass writes one
more zero past the buffer, over the argument count at +0xd0 and on past the
end of the object, until something faults. A line needs more than about 126
bytes of token text to get there; the console's input line and the game's own
scripts evidently stay below that. Found by Claude Code / Opus 5.5 while
naming the util folder.

## Harmless oddities

Things that look wrong in the original but have no effect, kept for the record.

- **0x4b71a7, 0x4ccd1c, 0x4ccd85** (hand-written assembly in the gap regions
  0x4b70a0 and 0x4cbbe0): after calling a routine they drop its arguments with
  a 16-bit `add sp, N` (0x4b71cb, 0x4ccd39 and others), a habit from 16-bit
  code. It changes only the low half of esp, so it would leave esp 64 KB off
  if the pushed arguments straddled a 64 KB boundary. These routines run at a
  steady stack depth, where that evidently never happens. Found by Claude
  Code / Opus 5.5 in #2662.
- **0x4d9ab0** (the fatal error handler, a gap region): it tests CreateFileA's
  result against 0 (`test esi, esi` at 0x4d9b91) where failure is
  INVALID_HANDLE_VALUE (-1), so when ErrorLog.txt cannot be opened it calls
  SetFilePointer, WriteFile and CloseHandle on -1, which is the current
  process's pseudo-handle: the calls fail or do nothing. Found by Claude
  Code / Opus 5.5 in #2662.
- **0x4cca33** (a 16-bit line drawer in the gap region 0x4cbbe0): it calls the
  clipping routine 0x4cc650 with four arguments and no surface, so 0x4cc650
  takes x0 as the surface, and it reads `word ptr [edi]` from an edi nothing
  sets. Its only caller is 0x4ccd85, which nothing calls: it looks like an
  unfinished routine. Found by Claude Code / Opus 5.5 in #2662.

- **0x435320** (briefing text loader): it frees the buffer at `+0xc14`
  (0x435330) and stores the new one only when the file size is non-zero; a
  zero size takes `je 0x435395` at 0x435368 and returns with the freed pointer
  still in the field. An empty name stores 0 correctly (0x435354). Harmless as
  called: its only caller, 0x435da0, clears that buffer first, so nothing is
  freed. Found by Claude Code / Opus 5.5 in #5494.
- **0x453360** (a send-to-players routine): its `int result` is only assigned
  from each send's return value and never initialised, so when the loop sends
  nothing the function returns whatever its home holds. MSVC put that home in
  the dead slot of the `text` argument (`mov eax, [esp+0x14]` at 0x4533d0 and
  0x453439; `text` itself is in esi from 0x45336e), so the "result" is the
  text pointer. The only caller, 0x463e50, ignores it. Found by Claude Code /
  Opus 5.5 in #5471.
- **0x4851c0** (a dead stepping routine): the x and z differences are divided
  by the step count (the two `idiv esi` at 0x485215 and 0x48521e), but the y
  difference stored at 0x4851de, the step's middle field, is never divided, so
  each pass adds the whole `b.y - a.y`. a.y is not read in the loop or
  returned, and the function has no callers and no pointer to it in the exe.
  Found by Claude Code / Opus 5.5 in #5463.
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
  garbage, and ExpandNeighbour adds a node's +0xc into its cost (0x40db0c).
  Probably harmless, since the start node is popped and closed on the first
  expansion before anything reads it. Found by Claude Opus 5.5 in #81.
- **0x40eb70** (the per-tick path scheduler): the `r < 3` case and the final
  `else` set the same value, and its second call to 0x40ef20 can never run.
  Found by Claude Opus 5.5 in #81.
- **0x41d7b0** (builds a path on the CD): its format string at 0x502914 is
  `%c\\%s\%s`, two backslashes after the drive letter and one before the file
  name, so paths come out as `D:\\dir\file`. Windows accepts the doubled
  separator. Found by DeepSeek V4.1 Flash in #21.

- **0x428d10** (a script tokenizer): the number branch tests `c != '-'`, but
  the punctuation branch above it already returns on any `-`, so that test
  can never fail. Found by Space Bunny Free in #25.
- **0x428e90** and **0x428f60**: format a parse error into a 256-byte local
  buffer with `sprintf` and never use it, as if a display or log call was
  removed. Found by Space Bunny Free in #25.

- **0x440940** (loading progress): the counter starts at 100 and adds 100
  before each division, so entry i reports `100 * (i + 2) / count`, starting
  one step ahead and passing 100 near the end; a final store of 100 hides it.
  Entries with a zero first field are skipped by the counting pass and the
  build pass alike, so they are not in the divisor. Found by Space Bunny Free
  in #34.

- **0x446310**: allocates an "AVAILABLE MODES" buffer (`count << 8` bytes at
  `obj+0x14`), zeroes its first byte and never reads or frees it, so it leaks.
  Found by DeepSeek V4.1 Flash in #126.
- **0x446e90**: tests `players[i].type != 4` after an inlined check has
  already limited that byte to 1, 2 or 3, so the test is dead. Found by
  DeepSeek V4.1 Flash in #126.
- **0x45b150**: a `deep = 1` store at the end of the loop body is dead, since
  the branch reaching it has already tested `deep` as 1. Found by Space Bunny
  Free in #143.

- **0x445c70** stores the same value to +0xa3 (the starting metal, in
  hundreds) of the local player's lobby record twice, the second time through
  a fresh lookup of that record (0x445d08, 0x445d36); its sibling 0x445d60
  (energy, +0xa1) stores once. The record is the player-setup record, not a
  battle unit. Found by Space Bunny Free in #125.

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

- **0x405980**: in the first reclaim branch SetAttachedFx(0) is called twice
  (0x405bce and 0x405c0e), in the other three branches once; with argument 0 it
  only releases the order's +0x52 attachment, which the first call has already
  cleared. Found by ozgb's Cline / deepseek-v4.1 in #2167.

- **0x435da0**: its error format at 0x504d9c reads "Hey, joker!  There is no
  mission defintion for this mission: %s". Found by ozgb's OpenCode /
  deepseek-v4.1-flash in #2274.

- **0x43e490** (case 2, 0x43ea36 to 0x43ea7c): calls the predicate
  CanRepair and returns 6 if it holds and `target->field_104 != 0`, then
  calls it again and returns 6 if it holds, so the first test adds nothing; the
  predicate only reads. Found by ozgb's Cline / deepseek-v4.1 in #2142.

- **0x44c7e0**: `if (item->name)` on each unit type's 0x166-byte name array
  (`add esi, 0x20; test esi, esi` at 0x44ca6f) is always true, as at 0x44c0d0;
  probably `name[0]` was meant, to skip unused slots. Found by ozgb's
  deepseek-v4.1-flash in #2593.

- **0x462f30**: the inlined ring pop reads through a null entry in its empty
  arm (`xor eax, eax; mov edx, [eax+8]` at 0x462fb5 and 0x463523), but both
  pops are only reached after a peek has returned an entry. Found by CubeB's
  OpenCode / deepseek-v4.1-flash in #1837.

- **0x464f80**: the player guard at 0x464fe1 to 0x464ffb (active, type 1 to 3,
  +0x146 not 10) is repeated verbatim at 0x465001 to 0x465024 with nothing in
  between. Found by ozgb's Cline / deepseek-v4.1 in #2157.

- **0x46a860**: the veteran line tests `kills > 4` (0x46b30d) and then
  `kills == 1` (0x46b313) to choose "kill" or "kills"; the singular can never
  be chosen there. Found by ozgb's Codex / GPT-6 in #1991.

- **0x46d2e0**: copies the never-written `y` of a local (`[esp+0x3c]`, read at
  0x46d328) into the value it inserts, then overwrites the inserted entry's y
  with 0 (0x46d483). Found by Space Bunny Free (CubeB) in #2132.

- **0x47ae60**: the Difficulty arm (0x47b92f) plays the sound "SKirmish"
  (0x502a6c) where the other thirteen arms use "Skirmish" (0x507ccc); 0x41ee8b
  uses the same misspelt string, and PlaySoundByName looks it up with
  `_strcmpi`, so both find the same sound. Found by ozgb's deepseek-v4.1-flash
  in #2576.

- **0x482130**: tests its `changed` local (`mov eax, [esp+0x10]` at 0x48214d)
  before anything writes it; it is set only when an expired entry is found, so
  a stale non-zero value only runs the compaction pass, which then finds
  nothing. Found by ozgb's Cline / deepseek-v4.1 in #2245.

- **0x497f40**: in the `flags & 2` exit path a ten-iteration loop copies each
  active player's control byte (+0x73) into one stack byte (0x4984c6), and the
  function then returns. Found by CubeB's OpenCode / deepseek-v4.1-flash in
  #2284 and ozgb's deepseek-v4.1-flash in #2494.

- **0x4a1b40**: both arms of `if (holder->field_20 == param_2)` (0x4a1fb3) call
  FadeRectangle with the same surface, rectangle and colour 0x1e (0x4a1fb8,
  0x4a1fcb). Only the selected row reaches them (the other rows just set their
  text colours), so the selection is shaded the same whether or not this
  gadget is the holder's current one; a different colour for the unfocused
  case was probably meant. Found by ozgb's OpenCode / deepseek-v4.1 in #2152.

- **0x4a7960**: the tail that refreshes a type-3 (text) selection is emitted
  twice in a row (0x4a7c76 to 0x4a7d81, then 0x4a7d86 to 0x4a7e98), so its
  colour and language setup, the FUN_004ab6c0 redraw and ClearKeyQueue run
  twice. Found by ozgb's deepseek-v4.1-flash in #2606.

- **0x4a9fd0** (hit testing): entry 0 is tested by adding the window origin to
  its own position, which doubles its offsets when its type is not 0
  (`add eax, eax; add ecx, ecx` at 0x4aa0f5); the normal type-0 header has a
  local position of 0, so this only shows in a GUI whose first gadget is not a
  window. Found by ozgb's Codex / GPT-6 in #1945.

- **0x4b91b0**: allocates `4*w*h + 0x18` bytes (`lea eax, [ebx+ebx+0x18]` at
  0x4b91d0, with `ebx = 2*w*h`) for a buffer of `w*h` 16-bit cells, twice what
  it needs, as if the byte pitch were doubled again. Its only caller passes 22
  by 22. Found by CubeB's Codex / GPT-6.1-sol in #1949.

- **0x4bd160**: writes the copyright line with `fprintf(f, buf)` (0x4bd373),
  the buffer as the format string; the text is the literal "Copyright 0000
  Cavedog Entertainment" with the year's digits patched in, so no `%` can reach
  it, but `fputs` was meant. Found by Space Bunny Free (CubeB) in #2067.

- **0x4bfe10**: on the caller-supplied-surface path it returns a local that is
  never set (C4700 on the matching source), which is the dead `rect` argument
  slot (`mov esi, [esp+0x50]` at 0x4bff0d), so it returns the rect pointer; all
  five callers ignore the result. Found by CubeB's Codex / GPT-6.1-sol in
  #1899, #2296 and #2549.

- **0x4d1670**: tests `DAT_00526ff4 == 0` and prints "Hey! The window buffer
  ptr is not pointing to anything!" (0x4d1798), although the pointer was
  allocated and null-checked at the top and nothing in between clears it;
  0x4d0f60 has the same dead check. Found by ozgb's Cline / deepseek-v4.1 in
  #2242.

- **0x4d89b0**: `out[0] = 0;` (0x4d89cd) just before `strcpy(out, "\n")`.
  Found by ozgb's Cline / deepseek-v4.1 in #2242.
- **0x50b9e0** (likely): the table of SQUASHERR_ names that
  `SquashErrorString` (0x4d1c60) indexes is missing a comma, so its sixth
  string is "SQUASHERR_BADPACKTYPESQUASHERR_BADPARAMS" and the seventh entry,
  which the function still accepts (`cmp eax, 7` at 0x4d1c64), is null. Code 5
  (output buffer too small) would print the merged name and code 6 (bad
  parameters) a null string, but only unpack errors (1 to 4) are ever printed.
  Found by Claude Code / Opus 5.5 while naming the util folder.
- **0x4b7540** (likely, dead code): `ShiftArgs`, which drops the first n
  arguments of a command, has its test the wrong way round (`cmp eax, edi` /
  `jl` at 0x4b754b): with at least n arguments it clears them all, and with
  fewer it copies from `&args[n]` until it meets `&args[count]`, which lies
  below the start, so the copy runs on through memory. Nothing in the exe calls
  it. Found by Claude Code / Opus 5.5 while naming the util folder.
- **0x4b39c0**: `SaveBank` keeps the packer's result in a local, and the path
  where the compression buffer could not be allocated reads that local without
  setting it (`mov eax, [esp+0x1c]` at 0x4b3bb2, which holds a leftover spill
  from 0x4b3aba). The result is only tested when the buffer exists, and without
  it the bank is written unpacked, so nothing goes wrong. Noted in
  0x4b39c0.cpp by its decompilers.
- **0x4bc120** (possible, dead code): `HAPI_LoadFileInto` reads a file into a
  buffer the caller passes in, but when the read returns no bytes it frees
  that buffer (0x4bc26c) as if it had allocated it, as its sibling
  `HAPI_LoadFile` does with its own block. Nothing in the exe calls it. Found
  by Claude Code / Opus 5.5 while naming the util folder.

- **0x440940** (`BuildAllPassMaps`): the load progress starts at 100 and
  adds 100 before each store of `progress / count`, so after movement class k
  of n it shows (k + 1) * 100 / n: one class ahead, and past 100 on the last
  class until the final store of 100.

## Possible leaks and unchecked inputs

- **0x42a8d0** (the unit type loader, a gap region; likely): when an FBI file
  has no UNITINFO section it returns 0 at once (`je 0x42b1bc` from 0x42ac78),
  freeing that file's buffer but not closing the file (the normal path calls
  HAPI_CloseFile at 0x42b173) and not freeing the weapon TDF table at
  DAT_005122a0 (only the normal end does, from 0x42b20a). Found by a Claude
  Code / Opus 5.5 subagent in #2662.
- **0x413470** (an order handler), state 3 (likely): after two misses it
  allocates a `Class_0044e2d0` waypoint and sets its speed with
  `SetApproachRadius(0x80)`, then only ORs 0x110e8 into the order flags and returns
  2. Every other branch hands the waypoint to the order through
  `SetAttachedFx`; this one never does, so the object leaks and the waypoint
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
  then copied into the spawned object (StartExplodePiece's inlined `rep movsd`).
  Harmless if nothing reads those bits. Found by DeepSeek V4.1 Flash in #22.
- **0x43de30** (possible): its chunked read `ReadBox(buf, 0x23)` is not
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
- **0x4af320** (not a bug): the `mode == 1` test controls only whether
  directories are listed first; the file pass that follows always runs and
  continues the same list. The callers rely on that: the save-game, campaign
  and multiplayer lists pass mode 0 and read only files, the directory browser
  (0x4af5b0) passes 1. An earlier reading of this as an ignored flag was
  withdrawn in #5772. Found by Space Bunny Free (CubeB) in #861.
- **0x47bf70** (possible): passes a 16-bit field (`mov cx, word ptr
  [eax+0x370]` at 0x47c111) to smackw32.dll ordinal 5 with `push ecx`, and the
  upper half of `ecx` still holds a pointer from 0x47c0ea, so the call gets
  garbage in its high 16 bits; harmless only if the callee reads a 16-bit
  value. Found by Space Bunny Free in #819.
- **0x4394e0** (likely): of four `idiv` sites, the division by the word at
  +0x2c is floored to 1 (`cmp; jae; mov ..., 1`), but the very next one, by
  the zero-extended word at +0x00, has no guard, so a record with 0 there
  faults with a divide error. Read from the disassembly of a partial match.
  Found by Space Bunny Free in #797.
- **0x425b80** (possible, probably harmless): the main-menu spark animation
  (UpdateMenuSparks, installed as the layer's per-tick callback at 0x426440)
  works on the 640x480 screen surface and a second surface it restores from.
  A spark spawns only on, and keeps moving only over, pixels whose low nibble
  is above 12, and it paints index 0xaa (low nibble 10), so a spark dies on a
  pixel another spark has painted and none can spawn there. The pixel is put
  back from the second surface when the spark moves on. An earlier reading of
  the cells as burnt battlefield features was withdrawn in #5772. Found by
  Space Bunny Free (CubeB) in #399.
- **0x4ab1b0** (possible, no known trigger): AddTextGadget copies its name
  argument into the entry's name field with an unbounded `strcpy`; a name of
  18 or more characters would spill into the field at +0x13. The caller at
  0x4abe6b (OpenMessageBox) passes the literal "TEXT" (0x5029f8), so that call
  is safe; an earlier note that it passed `"Player%dController"` misread the
  string. Found by DeepSeek V4.1 Flash in #366.
- **0x4a5d50** (likely): the loop that shortens a label until it fits
  re-measures the empty string forever when the entry's height is below 6,
  since `w <= height - 6` can then never hold for `w == 0`. Found by DeepSeek
  V4.1 Flash in #364.
- **0x4aedd0** (likely): when the table has no entry 0x49 it subtracts a
  local that is never written (`mov ebx, [esp+0x10]` at 0x4aee78) from every
  frame's +6 word; the compiler warns C4700 on the matching source. Found by
  DeepSeek V4.1 Flash in #368.
- **0x4b0160** (possible): both callers (0x4b0320, 0x4b0498) push three colour
  bytes, but the function reads only two, so the third, apparently meant for
  a highlight, is ignored. Found by DeepSeek V4.1 Flash in #369.
- **0x4b9d70** (possible): clamps the horizontal copy count to the
  destination width without subtracting the destination column, so a copy
  with a column offset can write past the end of the row (the vertical clip
  does subtract the row). Read from the disassembly of a partial match. Found
  by DeepSeek V4.1 Flash in #374.
  The count can also go negative: `n = src->width - srcCol` (0x4b9dd2) is
  only clamped from above, and the copy loop is guarded by `test ebx, ebx; je`
  and counts down with `dec ebx; jne` (0x4b9e26 to 0x4b9e45), so when `srcCol`
  exceeds the source width it runs about 2^32 times and faults, where the
  vertical clip exits. Found by CubeB's OpenCode / deepseek-v4.1-flash in
  #1841 and Space Bunny Free in #2130.
- **0x47c530** (likely): the movie summary prints "Total Playback Time" as
  `1000 * totalTime / totalTime`, always 1000, the line above it copied and
  not edited; both divisions are unguarded, so a summary with zero total time
  faults. Found by Space Bunny Free in #327.
- **0x435110** (likely): formats a message with the campaign name into a
  0x80-byte stack buffer, while the name comes from a 0x100-byte field, so a
  long name overruns the frame; 0x435da0 formats the same message into 0x100
  bytes. Found by Space Bunny Free (CubeB) in #401.
- **0x4373a0** (likely): the map checksum for the lobby (it reads the map's
  .tnt header, not a campaign file). It ignores the result of the 0x40-byte
  header read (0x43742f), so a map file cut off after its first four bytes
  still passes the 0x2000 test and the checksum folds in the uninitialised
  rest of the header; the later reads' results are ignored too, the two
  allocations are not null-tested, and the header's offsets are not checked
  against the file size. It also returns without closing the file when the
  magic is wrong (0x437439). Found by Space Bunny Free (CubeB) in #401.
- **0x4a7f70** (possible): the loops that reset every frame's size and pick
  the frame nearest the object run only on the CHECKBOX, `stagebuttn%d` and
  BUTTONS0 fallbacks; a successful name lookup jumps past them (0x4a7fbf,
  0x4a7fca, 0x4a7fdf). Found by Space Bunny Free in #559.
- **0x4ba9d0** (likely): when no palette entry falls in the brightness band,
  the fallback uses the loop counter after the loop. When the scan runs off
  the end (the target is more than 40 brighter than every entry) that is 256,
  which truncates to 0 in an `unsigned char`, so it returns `order[0]`, the
  darkest entry. When the scan stops early at the first entry above the band,
  the counter is that position and it returns `order[i]`, the first entry
  above the band, which is probably intended. Found by Space Bunny Free in
  #572.
- **0x4baf30** (possible): the blue clamp tests `(blue >> 1) + 0x3c > 0xff` but
  stores `(blue >> 1) + 0x32`; the sum can never pass 0xff, so the clamp is
  dead and the two constants disagree. Found by DeepSeek V4.1 Flash in #375.
- **0x4a2480** (likely): skips drawing a null first glyph but still adds its
  width (`mov dx, word ptr [ecx]` with ecx 0, at 0x4a24f8). Found by DeepSeek
  V4.1 Flash in #363.
- **0x4a31c0** (possible): allocates the SCROLLITEMS scratch block with
  FUN_004d83b0 and never frees it, where the siblings 0x41eaa0 and 0x41eb60
  free the same kind of block with FUN_004d85a0. Found by DeepSeek V4.1 Flash
  in #363.
- **0x4a36a0** and **0x4a35a0** (not a bug): when the layout entry is not
  found they call FatalError (0x4b6290) with "Error in GUI layout" and then
  fall into stores through a null entry (0x4a3716 onwards), but FatalError
  shows a message box and calls `exit(1)` (0x4e4600, ending in
  `ExitProcess`), so it never returns and the null stores never run. Found by
  DeepSeek V4.1 Flash in #363.
- **0x4b7620** (likely): finds the insertion point with a case-insensitive
  compare (`_strcmpi` at 0x4b7656) but tests for an existing key with a
  case-sensitive one (inlined `strcmp` at 0x4b769b), so a key differing only
  in case ("ABC", then "abc") is inserted as a duplicate instead of updating
  the entry. Read from the disassembly of a partial match. Found by DeepSeek
  V4.1 Flash in #373.
- **0x464060** (possible, latent): in mode 1, the arm for team 2 jumps past
  the only store to the `show` flag (0x46413d), so it keeps the previous
  entry's value, or an uninitialised one on the first pass. The only store to
  the mode (g_game+0x37efe) found in the exe is the 3 written at 0x491316, so
  mode 1 is never selected by any path found. `y += start` (0x46423e) adds the
  font height read once after SetFont (0x4640b7), a constant line spacing, not
  a bug. Read from the disassembly of a partial match. Found by Space Bunny
  Free in #413.
- **0x497ce0** (likely): while it still waits for players (network flag
  bit 3 clear), it divides 620 by the count of player records that are
  present, on team 1 to 3 and not kind 10 (`idiv dword ptr [esp+0x10]` at
  0x497dc3), with no test for 0; the early return guards only the other
  path. Read from the disassembly of a partial match. Found by Space Bunny
  Free in #502.
- **0x498da0** (possible): keeps bit 2 of the byte at g_game+0x2cc6 as a cache
  of bits 0 and 1 (`b2 = b0 || b1`), which nothing else writes. The two
  branches test different regions, not the same one: the radar rect at
  +0x142bb (scaled by world size over screen size, bit 0) and the main view
  rect at +0x37e27 (clamped and translated by the camera, bit 1), so bit 2
  means the cursor is over either. 0x469e70 reads it with mask 6. It also
  passes `GetMapCell`'s result, which is 0 for a cell off the map, to
  `GetCellFeature` (0x498f49), which reads its +8 without a null test;
  whether a cursor position can reach that is not settled. Found by Space
  Bunny Free in #502.
- **0x4bf4d0** (likely): locks the screen surface itself (0x4bf4eb), but
  its three failure exits (null low map, null high map, null computed map, at
  0x4bf589, 0x4bf5a9 and 0x4bf5c0) return 0 without the unlock call that only
  the shared tail at 0x4bf5ff makes, so the surface stays locked. It also
  indexes a 256-byte table with a sign-extended pixel byte (`movsx` at
  0x4bf5e8), reading up to 128 bytes before it for values above 127. Read
  from the disassembly. Found by Space Bunny Free in #378.
- **0x4bee60** (likely): its fallback test is on the address of the local
  `screen` (`lea ecx, [esp+8]; test ecx, ecx; jne`), which is never 0, so the
  second lock and the block at 0x4beea4 to 0x4bef1a never run and the game
  never draws to the fallback surface. Found by Space Bunny Free in #378.
- **0x48b710** (possible): reads the unit's link at +0x96 for a 16-bit id
  subtraction after testing only the owner and its vtable slot 7, while
  0x48b200, which writes the same field, tests the link for null first, so a
  unit with no link is read through a null pointer. Found by Space Bunny Free
  in #498.
- **0x488310** (possible): passes the network count, an `int`, straight to a
  `vector(n, value)` constructor; a negative count is clamped only for the
  allocation, so `_Ufill` would then write far past a zero-byte block. Harmless
  if the count is never negative. Read from the disassembly of a partial
  match. Found by Space Bunny Free in #498.
- **0x45f8c0** (likely): when a line value does not start with '|', the
  split scans from `value + 1` for the next '|' with no test for '\0', so a
  value with no second '|' runs off the end of the 0x80-byte buffer. When it
  does start with '|', the two `SkipTextLines(value, 0/1)` calls read
  uninitialised stack at `value + 0x10` and `+0x14`. Read from the
  disassembly of a partial match. Found by Space Bunny Free in #411.
- **0x4565a0** (possible): the same player search returns 10 when the id is
  -1 or not found, and this net-message handler uses it unchecked, so it reads
  and writes the spare eleventh slot (`players[10]`) instead of skipping the
  message. Found by Space Bunny Free in #408.
- **0x4523e0** (possible): when `to` is -1, its inlined player search returns
  10 and the function writes the new group through the spare eleventh slot's
  `players[10].data` (a pointer read from g_game+0x2878); the other users of
  the same search (0x44fed0, 0x452800, 0x4526c0) check for 10 first. Found by
  Claude Opus 5.5 in #228.
- **0x4523e0** (possible): when all ten group slots are in use (possible when
  `to` is not an active player, such as -1), the loop ends without writing
  the message's value byte at `[esp+0x13]`, so SendPacketToPlayer sends a two-byte
  message whose second byte is uninitialised. Found by DeepSeek V4.1 Flash in
  #139.
- **0x451df0** (possible): BroadcastPacket's per-group loop reads its "group
  already sent" table DAT_00512b90 (eleven ints) at the player's group index
  (+0xc, `mov ecx, [eax*4+0x512b90]` at 0x451f5f) before anything checks the
  index; only the store after the send (0x451f90) tests `0 <= group < 10`. A
  group outside 0 to 10 reads past the table. Harmless if the group is always
  a slot index. Found by Claude Opus 5.5 while naming the network module.
- **0x476830** (possible): its "lowercase" loop adds 0x20 to every non-zero
  byte, so `.` becomes `N` and `a` becomes 0x81; fine only if the input is
  always upper case. It also writes one byte past a `count * 30` buffer for a
  30-character name. Found by Space Bunny Free in #207.
- **0x476cd0** (possible): inside a colour run (an odd number of `&` seen) a
  `\n` overwrites the byte just written with `&`, writes `\r\n`, then reopens
  the run with `&` and the colour byte. After a CRLF the overwritten byte is
  the `\r`, which is written again, so nothing is lost; after a bare `\n` the
  preceding visible character is replaced by `&`. The copy loop copies the
  current byte before testing the next, so the last input character is kept.
  Found by Space Bunny Free in #207.
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
  callee; only the loop inlined from InitUnit checks for null. Harmless if
  id 0 never occurs. Found by Claude Opus 5.5 in #333.
- **0x421700** (possible): increments the debris counter at g_game+0x1491b
  and writes the entry's position before searching the 300-slot object pool;
  if the pool is full it returns with that entry claimed, its object pointer
  0, and its refs and velocity left from the slot's previous use, so a stale
  animation could be drawn. Found by Claude Opus 5.5 in #273.
- **0x497080** (possible): applies the running maximums of each slot's +0xc
  and +0x10 values inside the same loop, so each player gets the maximum of
  the slots up to its own and only the last playing player gets the true
  maximum. May be intended. Found by Claude Opus 5.5 in #431.
- **0x4bf8c0** (likely): when `surface != 0` the shared tail returns the
  stack slot at `[esp+0x7c]` (`mov ebx, [esp+0x7c]`), which was last written
  with `r->bottom` by the right edge's inlined segment code, so the function
  returns `r->bottom` instead of 1. Found by CubeB's OpenCode /
  deepseek-v4.1-flash in #1460.
- **0x4a3ef0** (possible): in the `0x20` arm, when `e->field_c0 <= 0` the
  `jle` at 0x4a40b2 skips the block that sets up the divisor, and 0x4a40d1
  then does `idiv ecx` with the register still zero, a divide by zero. The
  `0x80` arm tests both divisors first, so the check looks forgotten. Found by
  CubeB's OpenCode / deepseek-v4.1-flash in #1316.
- **0x44c0d0** (harmless): `if (defs[type].name)` tests the address of an
  array member (`lea eax, [esi+0x20]; test eax, eax`), which can never be null,
  so the check is always true and its false arm is dead. Probably meant to
  test the first character. Found by CubeB's OpenCode / deepseek-v4.1-flash
  in #1690.
- **0x4a2580** (likely): the same slip as the recorded 0x4a2480, twice. In the
  vertical branch the first arrow glyph from GetGafFrame is tested
  (`test eax, eax` at 0x4a26ec) and a null result skips only the draw
  (`je 0x4a2701`), after which `mov cx, word ptr [eax+2]` at 0x4a2705 reads its
  height through the null pointer; the horizontal branch does the same with
  the width (`je` at 0x4a2900, `mov cx, word ptr [eax]` at 0x4a2917). The
  later glyph lookups (0x4a271a, 0x4a292b) are not tested at all. A font
  missing that glyph crashes the scrollbar draw. Found by ozgb's Codex / GPT-6
  in #1968 (the horizontal site was found while checking it).
- **0x4aa8f0** (likely): when HAPI_FileLengthByName reports the GUI file missing or
  empty (returns 0), `je 0x4aac2d` at 0x4aaa17 skips both stores of the
  `layer` local (0x4aaa3b and 0x4aaa87), so the code at 0x4aac2d loads that
  stack slot uninitialised, writes its `+4`, `+0x1c`, `+0x24` and `+0x3b`, links
  it into the menu (when flag 0x200 is clear) and copies the name to address 2
  (`ebp` is still 0). A missing GUI file corrupts memory instead of failing.
  Unconfirmed: when the parser 0x4aeac0 fails, the layer freed at 0x4aac23
  seems to be written and linked the same way. Found by ozgb's OpenCode /
  deepseek-v4.1 in #2183.
- **0x4a2e40** (possible): the inlined search for the list's scrollbar
  (type 4) returns 0 when there is none (`xor ecx, ecx` at 0x4a3037), and the
  rescale then reads entry 0's `+0x136` and `+0x140` and may write `+0x140`
  (0x4a3099); entry 0 is the window's own header entry. The same inlined search
  in 0x4a6ae0 is followed by `cmp esi, -1` (0x4a6fd6), which can never be
  true, so the author evidently expected -1; on a miss 0x4a6ae0 also redraws
  entry 0 and calls entry 0's `+0x144` pointer if it is set
  (0x4a703c to 0x4a7052). Found by ozgb's Codex / GPT-6 in #1919.
- **0x4b5070** (likely): the DirectX version check compares the installed
  version with the wanted one field by field, but when the first field differs
  from `want0` (argument 1, tested at 0x4b51fd) it returns `majhi >= want1`
  (`cmp ebx, [esp+0xe4]` at 0x4b5233, argument 2), so a version whose first
  field differs is judged against the second wanted field. Noted in a comment
  in 0x4b5070.cpp by ozgb's OpenCode / Space Bunny Free run in #970 but never
  reported; found again while checking #1940 and confirmed from the
  disassembly.
- **0x4c0820** (possible): a polygon fill that starts its extremum search
  with sentinels (999999 and -999999 at 0x4c083c). With a vertex count of 0 or
  less the scan is skipped, the degenerate-polygon test (0x4c089a) sees two
  unequal sentinels and does not return, and the edge walks index the points
  with the never-written `iymin` (0x4c08b7) and `iymax` (0x4c0962). Its only
  caller (0x459128) passes a stored count plus 1, so this needs a negative
  stored count. Found by ozgb's Codex / GPT-6 in #1916.
- **0x4d1970** (possible): an HPI chunk decoder. Its length variable lives in
  the dead `data` argument slot and is written only by the method 1 and 2 arms
  (0x4d1a79, 0x4d1a63); methods 0 and 3 pass the `method >= 4` check but fall
  straight to the length test at 0x4d1a7d, which then compares `header.size`
  with the data pointer, so such chunks always fail with 3 and nothing is
  written. Harmless if HPI files only use methods 1 and 2. Found by ozgb's
  Cline / deepseek-v4.1 in #2242.
- **0x4d1820** (possible): `SquashPack`, the packer that writes those chunks,
  has the same hole. Its length variable also lives in the dead `data`
  argument slot and only the method 1 and 2 arms write it, so methods 0 and 3
  pass the `method >= 4` check (0x4d1873) and compute `length + 0x13`
  (0x4d18a8) from the data pointer, which is far larger than any output buffer:
  such packs fail with 5 (output too small). Harmless if only methods 1 and 2
  are asked for: SaveBank and SaveAccount pass 1, HAPI_WriteArchiveData the
  archive's compression type. Noted in 0x4d1820.cpp by deepseek-v4.1-flash.
- **0x4da8d0** (possible, out of memory only): with no out-of-memory handler
  installed (0x5289bc), a failed GlobalAlloc in the inlined pool carve returns
  0 (`xor eax, eax` at 0x4da987), and the new node is then written through it
  (`mov [eax+0x3c], ebx` at 0x4da9b0, then `+4`, `+0` and `+8`). The head-node
  allocation at 0x4da920 is not tested either, and the pool allocator 0x4ddce0
  has the same unchecked write (0x4ddd24): the allocator returns 0 where the
  STL code expects an exception. Found by ozgb's Cline / deepseek-v4.1 in
  #2242.
- **0x4ded60** (likely, harmless in effect): the crash-report header builder
  tests CreateFileA's result against 0 (`test esi, esi; je` at 0x4deee1), but
  CreateFileA fails with INVALID_HANDLE_VALUE (-1), which then reaches
  GetFileSize (0x4deeec), GetFileTime and CloseHandle. Since -1 is also the
  current-process pseudo-handle, those calls just fail and the "Executable is
  ... bytes" line is skipped. Found by ozgb's Codex / GPT-6 in #1982 and
  CubeB's OpenCode / deepseek-v4.1-flash in #2287.
- **0x42f9a0** (likely): reading "NumSkirmishPlayers" from the registry (key
  0x5044d4), the range test `value > 1 && value <= 10` (0x4307c1 to 0x4307c7)
  picks between two branches that store the same value to g_game+0x38d81
  (0x4307cf, 0x4307dd), so any value is accepted; the else branch presumably
  meant to store a default. Only an edited registry value can be out of range;
  whether the later loops that run to this count (0x430c06 onwards) then
  overrun was not checked. Found in ozgb's OpenCode run in #2244, which gives
  the address of the neighbouring Gamma block (0x4301b7).
- **0x43a420** (possible): the unit type table DAT_00512344 is a
  `std::vector` (0x512348 is its end), and the name lookup treats it that way
  (`cmp esi, eax; je` at 0x43a51f), but the fallback scan for records without
  a name (0x43a556 to 0x43a598) loops while `p <= end` (`jbe` at 0x43a58d),
  reading the flag byte of the element past the end, and when no kind matches
  it leaves with an index two past the last valid one. That index is not
  rejected (only the name path returns 0) and is used as
  `DAT_00512344[kind].name` (0x43a66d to 0x43a679) in a `strcmp`. Only a record
  whose kind is beyond the table (from another version's save, say) reaches
  it. Found by ozgb's Codex / GPT-6 in #1986 and OpenCode / deepseek-v4.1 in
  #2179.
- **0x450a10** (possible): copies the DirectPlay player's names from
  GetPlayerName's DPNAME with unbounded inlined `strcpy`s, the short name into
  the 42-byte field at +0x49 (0x450b7e) and the long name into the 30-byte
  field at +0x2b (0x450ba3), so a long name of 30 characters or more runs into
  the +0x49 field and one of 72 or more reaches the type byte at +0x73. The
  names come from the network; whether TA's own UI limits their length was not
  checked. Found by Space Bunny Free (CubeB) in #2102.
- **0x453d40** (likely), the network message handler, two findings. The range
  check at 0x4547f5 (`cmp al, 1; ja; cmp al, 0x2d; jb`) sends the error reply
  `RejectPlayer(sender, 6)` only when `cmd <= 1 && cmd >= 45`, which can never
  hold, so `||` was surely meant; out-of-range commands are still dropped by
  the switch bound, but silently, after the raw byte has indexed the
  g_packetModes mask table (0x454758). (The same block also repeats the status
  test of 0x4547cc at 0x45480e.) And command 20 (0x455649) looks up the player
  named at packet+3; when there is none the lookup gives 10, the code sets the
  player pointer to 0 (0x4556ef to 0x4556f5) and then reads `[eax]` at
  0x45575a, so a type-20 packet naming a player who has left crashes the
  receiver (when the named unit exists and has flag 0x10000000). Found by
  ozgb's Codex / GPT-6 in #2002 and deepseek-v4.1-flash in #2458.
- **0x476ef0** (possible): the coloured-run copy (0x4772bb to 0x4772c9) runs
  until a `&` or 127 bytes and never tests for the terminator, while the outer
  loop stops at 0, 0xff and newlines. The write is bounded by the 128-byte
  buffer, but a run with no closing `&` draws up to 127 following bytes in
  colour, across line ends, and past the end of the text if it ends sooner.
  Found by ozgb's Codex / GPT-6 in #1997.
- **0x482c20** (possible, out of memory only): `operator new(10)` is not
  tested; a null result is stored to g_game+0x142b7 (0x482c4e) and written
  through (`mov dword ptr [eax+2], 0x1f` at 0x482c60). The grid buffers
  allocated at 0x482cc5 and 0x482f62 are not tested either: a null only skips
  the clearing loop, and the border and fill loops then write through it.
  Found by ozgb's Codex / GPT-6 in #2035.
- **0x491ec0** (possible), the load-game preview, two findings. The
  "Difficulty" value read from the save (0x4b4800 at 0x492259) indexes a
  three-entry stack table of "Easy", "Medium" and "Hard" with no range check
  (`mov edx, [esp+eax*4+0x48]` at 0x49225e) and goes to `sprintf("%s")`, so a
  damaged save passes a neighbouring stack dword as a string. And the result of
  `FUN_004a0280(..., "RADAR")`, which is 0 after it logs "Error in GUI layout",
  is kept in `esi` and never tested, so `mov dword ptr [esi+0xc2], 0x51e6f8`
  at 0x491fd1 writes to address 0xc2 when the gadget is missing; the `je`
  before it tests the radar image, not the gadget. Six of the twelve callers of
  FUN_004a0280 do test the result. Found by ozgb's Codex / GPT-6 in #1950 and
  CubeB's OpenCode / deepseek-v4.1-flash in #2286.
- **0x49b090** (possible): a cell's feature id is checked against the feature
  count on the direct path (`cmp edx, [g+0x14253]; jl` at 0x49b2d4), but for
  the 0xfffe marker the code steps back to the feature's origin cell
  (0x49b2e7 to 0x49b304), reads its id, checks only that it is below the 0xfffb
  sentinels (0x49b30a) and indexes the mapping table with it (0x49b31b)
  without the count check. Found by ozgb's Codex / GPT-6 in #1898.
- **0x49be60** (likely): for a kind-3 projectile it passes a stack local
  (`lea ecx, [esp+0x38]` at 0x49c252) as the rotation argument of
  DrawModel3doProjected, which hands it to RotateByAngles for every vertex, and that reads
  three angles from it (0x4b6cd6, 0x4b6cf6, 0x4b6d1b); nothing in 0x49be60
  writes those bytes, so the model is rotated by whatever the stack held. Kind
  1 builds its angles from the shot's +0x34 to +0x38 (0x49c0fd) and another arm
  passes `&p->field_34` (0x49c482). Found by ozgb's OpenCode / deepseek-v4.1
  in #2152.
- **0x46d6c0** (possible): the sender lookup (0x46d6e8 to 0x46d71d) walks a
  vector for the player's id and leaves the pointer at `end()` when it is not
  found (or the vector is empty), and the switch that follows uses it
  unchecked: case 1 stores to `[end+0x24]` (0x46d73b) and case 4 to
  `[end+0x2c]` (0x46d7b3). Harmless if messages only come from tracked players.
  Found while checking the other 0x46d6c0 claim in #2132 (Space Bunny Free,
  CubeB), which did not hold.
- **0x481140** (`UnitScript::ExplodePiece`, possible): builds the debris
  record for StartExplodePiece on the stack and updates its flags dword at +0x28
  with read-modify-writes (`h.bits & ~0x30` and the four merges after it)
  that are never preceded by a plain store, so bits 6 to 31 keep whatever the
  stack held and are copied into the debris object, as in 0x420e50 above. The
  dword at +0x2c is never written either. Harmless if nothing reads those
  bits.
- **0x4b2040** (`CobScript::LoadScriptState`, possible): allocates the
  "Piece States" buffer and returns 0 without freeing it when the read that
  fills it comes back short (`cmp eax, edi; je` at 0x4b2161). The total size
  is checked against the record's length first, so only a failed read can
  get there.
- **0x4390a0** (`DrawUnitRangeRings`, likely): the weapon3 ring tests
  `slots[0].flags` (unit+0x1f) but takes the range from `slots[2].weapon`
  (unit+0x48); weapon3's own flags are `slots[2].flags` at unit+0x57. So the
  third ring follows weapon1's flag bit. Thaldren's note for 0x439740 records
  the same. Found by OpenCode / deepseek-v4.1-flash in #6054.
- **0x491200** (`InitGame`, possible): calls `SetMissionType(0)` and then
  tests `g_game->field_391e9->GetGameType() == 3`, but 0x435100 returns the
  Mission's type at +0x0, which that call has just made 0, so the two `= 10`
  stores (`field_38a4b`, `field_38a4d`) never run. Harmless dead code unless
  the test was meant to come before the reset. Found by OpenCode /
  deepseek-v4.1-flash in #6051.
- **0x474760** (`WakeParticles::FUN_00474760`, possible): divides 2^32 by
  twice the length of the direction between its two points with no test for
  zero, so two identical points raise a divide exception at 0x474811
  (`_alldiv`). Harmless if callers never pass coincident points. Found by
  OpenCode / deepseek-v4.1-flash in #6082.
- **0x499ab0** (possible): the weapon packet is a 0x24-byte stack local and
  its byte at +0x1a is a one-bit field; only bit 0 is written
  (`packet.flag = unit->def->f111 >> 30`), so the other seven bits go over the
  network as whatever the stack held. Harmless if receivers only test bit 0.
  Found by OpenCode / deepseek-v4.1-flash in #6085.
- **0x4a03f0** (possible): when a multi-state button's group entry is not
  found, the search leaves `found` at 0, but the recursive call is guarded by
  `found != -1` (0x4a0491 sets the local to 0, 0x4a0493 compares it with -1),
  so the guard never fails and the function recurses with index 0 instead of
  skipping. An initial `found = -1` would match the evident intent. Found by
  OpenCode / deepseek-v4.1-flash in #6089.
- **0x440d70, 0x4437c0** (possible): both look up the PASSWORD entry with
  FUN_004a0010 and use `entry + 0xb6` without testing the result, though
  0x4437c0's own UPDATE path (0x443895) does test it (`cmp eax, edi; je`). A
  NEWMULTI or SELGAME dialog without a PASSWORD entry would read through
  address 0xb6. Harmless while the shipped GUI files have the entry. Found by
  OpenCode / deepseek-v4.1-flash in #6093.
- **0x4abd90** (possible): the max-line-width loop measures entries
  `count+1` to `count+lines` (0x4abeb8 walks forward from `count+1`, one
  0x15b step per line), but the text entries it has just added are
  `count-lines+1` to `count`, so it measures one entry past the last line it
  added. Found by OpenCode / deepseek-v4.1-flash in #6104.
- **0x4acae0** (likely): the first `memcpy` copies 0x400 bytes into a
  768-byte stack buffer (the frame reserves 0x320 bytes), overrunning it by
  0x100 bytes into the rest of the frame. Found by OpenCode /
  deepseek-v4.1-flash in #6104.
- **0x49d580** (possible): on the aiming path for weapons with flag bit 1,
  the heading is stored into argument one's home slot (the shooter pointer),
  and AimWithinTolerance at 0x49d681 then reads that slot as its first angle
  (`mov edx, [esp+0x58]`) while edi still holds the real shooter, so the angle
  checked is a pointer's low 16 bits. The target read back from the same slot
  at 0x49d745 feeds `target->f_a8` (0x49d812) and FireLineOfSightProjectile
  (0x49d77d). Found by OpenCode / deepseek-v4.1-flash in #6105.
- **0x444ea0** (possible, unreachable): calls `Mission::RefreshMapList`
  (0x435d30, which ends in `ret 4`) without pushing its argument, leaving the
  stack 4 bytes short. The only caller (0x4488ea) reaches it only when
  `HasMissionName()` is false, which does not happen in practice. Found by
  OpenCode / deepseek-v4.1-flash in #6111.
- **0x4a0e00** (possible): the entry lookup searches
  `holder->unknown_0->entries`, but the type 3 write goes through
  `holder->entries[index]`, so the text can be stored into a different array
  from the one searched; the function walks both chains. Found by OpenCode /
  deepseek-v4.1-flash in #6113.
- **0x43a0c0** (possible): loads the order record's default flags from +0x11
  (`mov eax, [eax + edx + 0x11]`), but the order table keeps that dword at
  +0x10 (unit_orders.cpp), so the flags the constructor copies are shifted by
  8 bits (the ready order's 0xf reads as 0). Found by OpenCode /
  deepseek-v4.1-flash in #6125.
- **0x4b5980** (possible, harmless): fetches the work area with
  `SystemParametersInfoA(SPI_GETWORKAREA)` into the rectangle at +0xec, which
  overlaps the flag word at +0xf0 (the rectangle's `top`); the flag word is
  overwritten three instructions later, so the fetch has no lasting effect.
  Found by OpenCode / deepseek-v4.1-flash in #6132.
- **0x459200** (`DrawObjectPicture`, possible): the far-sprite test is
  `field_a6 != 0 || dx >= field_1427f`, so the sprite is drawn when the unit
  is off the ground or in view range, where the intent reads as both; both
  halves of the function have it. Found by OpenCode / deepseek-v4.1-flash in
  #6143.
- **0x47e2d0** (possible): the visibility index's row term shifts
  `footprint.x` by 2, where the row extent is `footprint.y` (the footprint is a
  Point), so non-square footprints index the wrong row. Found by OpenCode /
  deepseek-v4.1-flash in #6146.
- **0x4cb9e0** (`SaveBmp`, possible, harmless): the seek offset is
  `(bmp.height - n) * stride + dataOffset`, where `n` is the height `Open` has
  just stored in `bmp.height`, so the difference is always 0 and the seek
  always lands on `dataOffset`. Found by OpenCode / deepseek-v4.1-flash in
  #6147.
- **0x45c950** (`LoadSavedAudioSettings`, likely): restores the per-track
  type table by copying `g_optionsBackupTrackTypes` to +0x215
  (`CopyTrackTypeTable`, 0x4ce3e0), but `SaveGameSettings` (0x45cde0) saved it
  from +0x214 (`GetCategoryOfTrack(i)` for i = 0 to 99, 0x4ce7e0), so every
  restored track takes the previous track's type and the last one is dropped.
  Found by OpenCode / deepseek-v4.1-flash in #6156.
- **0x499ba0** (`BroadcastWeaponFire`, possible, unreachable): its 0x24-byte
  weapon fire packet initialises only +0x00 to +0x19, but the receiver
  0x49d270 reads the heading, pitch, target, owner and weapon slot from +0x1b
  to +0x23 for every non-meteor weapon, so those bytes would be stack garbage.
  The function has no callers in the exe. Found by OpenCode /
  deepseek-v4.1-flash in #6175.
