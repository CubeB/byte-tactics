# The longest functions: inlined helpers or genuinely long

Phase 7 of `docs/cleanup-roadmap.md` asks which of the biggest functions are
really helpers the compiler inlined and which are genuinely long, and queues
the extractions worth doing. Two audits cover the 40 largest game functions:
the first (#6250) below, the second (#6408) under "The next 20". The
addresses and sizes are from `data/functions.csv`, the names from
`data/symbols.csv`, and the line numbers are at the commit that added each
audit.

The functions are already matched, so an extraction only helps if the source
still compiles to the same bytes. The method was to read each function, look
for blocks that do one thing with different inputs (the same TDF read for
each key, the same draw, the same decode), and check whether a block repeats
the body of a function that is out of line elsewhere in the exe. A scratch
script (`build/scratch/6250/repeats.py`, not committed) also reported
repeated statement sequences per function, which is what surfaced the
LoadSettings and LoadUnitFbi runs.

## Verdicts

Eight functions are inlined helpers that can be pulled out; twelve are
genuinely long. The five strongest extractions are queued as issues.

| Address | Bytes | Function | Verdict |
| --- | ---: | --- | --- |
| 0x453d40 | 8944 | `HandleNetPackets` | genuinely long (packet-type switch) |
| 0x426e80 | 6310 | `RunFrontendStateMachine` | genuinely long (state machine) |
| 0x468cf0 | 5904 | `DrawBattleFrame` | genuinely long (small 2-site candidates) |
| 0x42f9a0 | 5472 | `LoadSettings` | inlined helpers (queued) |
| 0x4a81e0 | 5248 | `RenderLayer` | inlined helpers (queued) |
| 0x42bf40 | 4772 | `LoadUnitFbi` | inlined helpers (queued) |
| 0x43f0e0 | 4420 | `GetOrderType` | genuinely long (order switch) |
| 0x447b10 | 4306 | `HandleBattleRoomClick` | genuinely long (gadget dispatch) |
| 0x46a860 | 4247 | `DrawUnitInfoPanel` | genuinely long (small candidates) |
| 0x42e440 | 3923 | `LoadWeaponType` | inlined helpers (lower priority) |
| 0x448c70 | 3902 | `RefreshBattleRoomRows` | inlined helpers (lower priority) |
| 0x4b0da0 | 3666 | `CobScript::RunThread` | genuinely long (bytecode switch) |
| 0x41f7f0 | 3580 | `RunEndGameState` | genuinely long (state machine) |
| 0x497f40 | 3463 | `LoadingScreenFrame` | inlined helpers (queued) |
| 0x43e490 | 3152 | `GetOrderCursor` | genuinely long (order switch) |
| 0x47ae60 | 2947 | `HandleSkirmishClick` | inlined helpers (lower priority) |
| 0x497180 | 2797 | `LoadMatch` | genuinely long (one candidate) |
| 0x449bb0 | 2756 | `OpenBattleRoom` | genuinely long |
| 0x435da0 | 2740 | `Mission::LoadMission` | genuinely long (one candidate) |
| 0x431a60 | 2737 | `LoadSideData` | inlined helpers, out-of-line match (queued, proved) |

## The functions

**`0x453d40 HandleNetPackets`** (8944 bytes, `src/network/net_game_453d40.cpp:423-878`).
A switch on the packet type. The file already has 18 `static inline` lookups
and predicates for it (lines 249-419: `GetPlayerId`, `FindPlayerIndex`,
`PlayerById`, `InGame`, `DropPlayer`, `CommandAllowed`, ...), so what is left
is dispatch, not repetition. The repeated `RejectPlayer(GetPlayerId(target),
8)` pairs inside one case (lines 493-500) are per-case, not a helper.

**`0x426e80 RunFrontendStateMachine`** (6310, `src/frontend/frontend.cpp:1123-1587`).
Nested switches on the state and substate; `SetState`, `SetSubState`,
`SetStateLogged` and friends are already out of line (lines 1041-1109). Cases
4 and 5 (lines 1208-1236) differ only in the movie, and cases 1 and 3
(1198-1206) are `PlayMovie` plus `SetState`, but each arm is only two or
three lines and the register use differs. Not worth a helper.

**`0x468cf0 DrawBattleFrame`** (5904, `src/ingame/info_panel_468cf0.cpp:220-568`).
`ProfileMark`, `Approach` and `ShowSelectBox` are already inline (lines 88,
114, 209). Two 2-site blocks remain: the feature blit (`IsFootprintVisible`
then `BlitFeatureGaf`, lines 356-365 and 395-404) and the unit selection draw
(`DrawSelectionBox`, `DrawUnit`, lines 384-389 and 417-423). Both guards
differ between their two sites, so they are candidates only if that can be
kept; not queued.

**`0x42f9a0 LoadSettings`** (5472, `src/game/settings.cpp:245-685`).
61 `ReadRegistryDword` calls and 31 `WriteRegistryDword` calls. The same
read, default and write-back block repeats in six groups: lines 253-337
(twelve), 373-414 (six), 415-430 (`Gamma`, `SwitchAlt`), 546-610 (ten multi
and skirmish options), 621-658 (the six reads in the per-player loop, no
write back), and lines 459-545 (sixteen reads with a default but no write
back). This is the largest repetition in the audit. Queued as #6271.

**`0x4a81e0 RenderLayer`** (5248, `src/gui/gui.cpp:5775-6275`).
A per-entry-type loader switch. The file is already used to inline helpers
(`FindEntry`, `entry_at`, ...), and one verbatim block is still spelled out
four times: the loop that clears every frame's offsets (lines 5876-5880
`"BackTile"`, 5896-5900 `"SLIDERS"`, 5957-5961 `"TEXTINPUT"`, 5972-5976
`"LISTBOX"`). Queued as #6274.

**`0x42bf40 LoadUnitFbi`** (4772, `src/units/unit_types.cpp:569-951`).
Between lines 656 and 806 there are 44 `unitdef->flags1/flags2 =`
assignments, nearly all the same masked field insert:

```cpp
value = ((TdfRecord*)parser.current)->GetFieldInt("key", 0);
unitdef->flags1 = (value & 1) << N | unitdef->flags1 & MASK;
```

Queued as #6272, with the xor forms (`standingmoveorder`,
`mobilestandorders`) and the `value2` sites called out as the ones to keep.

**`0x43f0e0 GetOrderType`** (4420, `src/orders/order_dispatch.cpp:542-749`).
A switch on the order mode. `Pick`, `FEATURE_CHECK`, `Lookup` and `Visible`
are already helpers (lines 504-540). The two arms of case 1 (lines 707-746)
write out near-duplicate tails, but the comment at line 709 records that a
shared goto breaks the register use.

**`0x447b10 HandleBattleRoomClick`** (4306, `src/frontend/multi.cpp:3700-4015`).
A per-player loop that checks seven gadgets by `sprintf("TAG%d", i)`
(lines 3720-3861), each doing different work, then a chain of option toggles
(lines 3892-3936) that share `PlaySoundByName` plus assign plus
`BroadcastPlayerInfo` plus `UpdateNetGameInfo` plus `dirty` but assign through
different expressions. No single repeated block; the `IsLocal_*` predicates
are already out of line.

**`0x46a860 DrawUnitInfoPanel`** (4247, `src/ingame/info_panel_46a860.cpp:78-356`).
`DrawBar_0046a860` and `Positive_0046a860` are already helpers (lines 53-74).
The remaining repeats are the icon loop twice (91-98 and 202-211) and the
`GetTextWidth`/`DrawString` centred-text pair three times (288-291, 299-302,
311-315). Each is small and the panel is one long draw sequence; not queued.

**`0x42e440 LoadWeaponType`** (3923, `src/weapons/weapon_types.cpp:241-421`).
One long run of TDF reads. Repeats worth naming: the sound read, load and
default three times (lines 376-390), the explosion-GAF pair twice (347-353
and 356-374), and the `GetFieldDouble * factor` conversions (248-273). The
casts are load-bearing (the comment at line 274 records one that must not be
added), so this is lower priority and is not queued.

**`0x448c70 RefreshBattleRoomRows`** (3902, `src/frontend/multi.cpp:4101-4355`).
The unused-player branch hides nine gadgets with the same
`sprintf(name, "TAG%d", n)`, `e = FindGadgetOrNull(entries, name)`,
`if (e) e->visible = 0` sequence (roughly lines 4205-4254), and the playing
branch repeats a visible-and-refresh version. `FindGadgetOrNull` and the
`Is*_00448c70` predicates already exist; the two arms differ in lookup and
flags, so lower priority and not queued.

**`0x4b0da0 CobScript::RunThread`** (3666, `src/units/cob.cpp:466-981`).
A bytecode interpreter: one switch over roughly forty opcode cases. The
child-thread argument copy appears in two cases (lines 851-872 and 873-897),
but the pop loop is part of the dispatch.

**`0x41f7f0 RunEndGameState`** (3580, `src/frontend/endgame.cpp:1067-1228`).
A switch on `g_game->state`; `HasNextMission`, `StatsComplete` and the
`ENABLE_BARS` macro are already helpers (lines 874, 1047, 1055). The
end-mission calls `OpenEndMissionScreen(); FillEndGameStatistics();
EnableEndMissionButtons();` plus two menu calls appear at lines 1154-1156 and
1173-1175 (and partially at 1185-1186). Two full copies is borderline; the
arms order the calls differently.

**`0x497f40 LoadingScreenFrame`** (3463, `src/game/game_load.cpp:873-1149`).
Six progress bars, each about fifteen lines (lines 1053-1139), differing in
the label, y, and which per-bar previous-percent and alpha storage they use.
Queued as #6273.

**`0x43e490 GetOrderCursor`** (3152, `src/units/unit_scripts.cpp:584-704`).
A switch on the order mode. `Selectable`, `RECLAIM_CHECK`, `Visible` and
`GetFeature` are already helpers (lines 532-579), and the comment at line 628
records that an inline helper there blows the budget.

**`0x47ae60 HandleSkirmishClick`** (2947, `src/frontend/skirmish_menu.cpp:905-1180`).
Four copies of the loop that counts table players (`active == 2` at lines
932-941, 952-961; `active == 1` at 964-971, 990-1008), the Energy and Metal
branches (1059-1106) reduce to an adjust-and-refresh pair each, and the
option toggles (1107-1158) share a shape. The count loops are the cleanest
candidate; lower priority than the queued five, so not queued.

**`0x497180 LoadMatch`** (2797, `src/game/game_load.cpp:536-785`).
Mostly one-shot setup, with three small repeats: the HAPI bank
`OpenAccount("summary")`/`HasItem("BetweenMissions")` pair (lines 606-609 and
742-745), the in-game player-record filter three times (694-704, 709-719,
724-734), and the player-flag reads (591-599 and 638-644). The filter is the
only helper candidate, and the long comment at lines 786-803 records how the
registers were won, so it needs care; not queued.

**`0x449bb0 OpenBattleRoom`** (2756, `src/frontend/multi_449bb0.cpp:257-424`).
Setup code; the only repeats are two small `for (p = DAT; *p; p++)` gadget
loops (lines 355-357 and 403-407) that do different work.

**`0x435da0 Mission::LoadMission`** (2740, `src/map/map_list.cpp:925-1101`).
One long TDF read. The comment at lines 920-924 records that 0x435320
(`LoadBriefing`) and 0x4356c0 (`GetName`) are inlined here by design. The
only repeat is the free-if-set triplet for units, rules and features (lines
948-963).

**`0x431a60 LoadSideData`** (2737, `src/game/side_data.cpp:164-391`).
Eleven hand-written x1/y1/x2/y2 blocks (lines 215-358) are the body of the
out-of-line `ReadSideRect` (0x431950, same file, lines 120-135): same
message, same four `GetFieldInt` reads, same save and restore of the current
record. The other sixteen sections already call it (lines 360-375). This is
the only block in the audit that is verbatim the body of an out-of-line
function elsewhere in the exe. Queued as #6270 and proved below.

## Proof: the LoadSideData extraction

First attempt: replace the eleven blocks with calls to the out-of-line
`ReadSideRect`, making all 27 sections calls. That does not match:
`check.py 0x431a60` reports 46.6%, ours 1213 bytes against the original's
2737. MSVC inlines neither the declaration nor the calls, so the eleven
hand-written copies are not code the compiler would regenerate from calls.
Reverted.

Second attempt, the proof the issue asks for: add

```cpp
// The x1/y1/x2/y2 block of one section; msg is the caller's buffer, which
// keeps each inlined expansion's string on its own stack slot.
static inline void LoadSideRect(TdfFile* parser, int* out, char* name, char* side, char* msg)
{
    int saved = parser->GetCurrentRecord();
    if (!parser->SelectRecord(name)) {
        sprintf(msg, "No [%s] in GAMEDATA/SIDEDATA.TDF for side:%s", name, side);
        FatalError(msg);
    } else {
        out[0] = parser->current->GetFieldInt("x1", 0);
        out[1] = parser->current->GetFieldInt("y1", 0);
        out[2] = parser->current->GetFieldInt("x2", 0);
        out[3] = parser->current->GetFieldInt("y2", 0);
    }
    parser->SetCurrentRecord(saved);
}
```

call it at the eleven sites with the existing `msg*` buffer as the last
argument (`LoadSideRect(&parser, &s->logo.x1, "LOGO", s->name, msgLogo);`),
and drop the loop's now unused `int saved;`. Result:
`check.py 0x431a60` prints MATCH (original 2737 bytes, ours 2737 bytes) and
`place.py` rebuilds the exe to MD5
`8e74a1dffa1f5988624c52048f5b20cd`. The trial was reverted before the audit
commit; #6270 is to land it.

The result says a repeated block can be pulled into a `static inline` helper
without moving the frame, as long as the helper's own storage (here the
message buffer) still comes from the call site. It does not say every
candidate passes: a helper costs symbol ids and can move the register
allocation (`docs/c2-regalloc.md`), which is why each queued issue names the
group to try first and leaves the special sites alone.

## Queued extractions

| Issue | Function | Repeated block |
| --- | --- | --- |
| #6270 | `LoadSideData` 0x431a60 | eleven inline copies of `ReadSideRect`, lines 215-358 (proved) |
| #6271 | `LoadSettings` 0x42f9a0 | 61 registry reads in six groups, lines 253-658 |
| #6272 | `LoadUnitFbi` 0x42bf40 | 44 flag-field inserts, lines 656-806 |
| #6273 | `LoadingScreenFrame` 0x497f40 | six loading bars, lines 1053-1139 |
| #6274 | `RenderLayer` 0x4a81e0 | four glyph-offset resets, lines 5876-5976 |

The remaining inlined-helper candidates stay here for a later pass:
`LoadWeaponType` (sound reads, lines 376-390), `RefreshBattleRoomRows`
(hide-gadget blocks, lines 4205-4254), `HandleSkirmishClick` (player-count
loops, lines 932-1008), `DrawBattleFrame` (feature blit and unit draw, lines
356-423), `DrawUnitInfoPanel` (icon loop and centred text, lines 91-315), and
`LoadMatch` (player-record filter, lines 694-734).

## The next 20 (issue #6408)

The second audit covers the next 20 game functions by size, with the same
method: read each function, look for blocks that do one thing with different
inputs and for code that repeats an out-of-line function's body, then build
the most promising extraction and check it. Six functions hold repeated
blocks that could come out as inline helpers; fourteen are genuinely long.
Three extractions are queued as #6446, #6447 and #6448, and the first was
proved.

### Verdicts

| Address | Bytes | Function | Verdict |
| --- | ---: | --- | --- |
| 0x4a5f40 | 2700 | `DrawButton` | genuinely long (one small text-colour pair) |
| 0x4d8e60 | 2644 | `ReportException` | genuinely long (log writer; the append spelling is load-bearing) |
| 0x48e010 | 2542 | `MissionConditions::RegisterConditions` | inlined helpers (queued, proved) |
| 0x464f80 | 2392 | `UpdatePlayers` | genuinely long (player state machine; the duplication is load-bearing) |
| 0x42b370 | 2375 | `UnitDef::operator=` | genuinely long (compiler-generated member copy) |
| 0x44a680 | 2340 | `UpdateBattleRoom` | genuinely long (screen update sequence) |
| 0x4224b0 | 2324 | `LoadFeatureType` | inlined helpers (queued; both helper shapes moved the frame) |
| 0x495e90 | 2292 | `HandleGameKey` | genuinely long (key switch) |
| 0x49be60 | 2272 | `DrawProjectiles` | genuinely long (projectile-type chain) |
| 0x401360 | 2239 | `UpdatePlayerEconomy` | genuinely long (small accumulate and clamp blocks) |
| 0x418310 | 2203 | `DrawMapDebugOverlay` | genuinely long (mode chain; small candidates) |
| 0x42d2e0 | 2173 | `LoadUnitTypes` | genuinely long (load pass) |
| 0x4a9fd0 | 2164 | `UpdateMenu` | genuinely long (gadget-type switch) |
| 0x4a1b40 | 2160 | `DrawListBox` | inlined helpers, lower priority (fade run) |
| 0x4e0b90 | 2150 | `MemoryStatusDialog::HandleMemoryStatusMessage` | genuinely long (message switch; one inlined formatter copy) |
| 0x430f00 | 2112 | `SaveSettings` | genuinely long (one write per setting) |
| 0x459c70 | 2047 | `UnitTable::DrawLitPieces` | inlined helpers, lower priority (first-face setup three times) |
| 0x411f50 | 1980 | `AirStrikeOrder` | inlined helpers (queued; the audit shape reached 96.3%) |
| 0x40fbe0 | 1976 | `VtolFollowOrder` | inlined helpers, lower priority (two move sites share a shape) |
| 0x483610 | 1975 | `LoadTntMap` | genuinely long (TNT loader, already in five regions) |

### The functions

**`0x4a5f40 DrawButton`** (2700, `src/gui/gui.cpp:4841-5058`).
The text-colour if/else (`SetTextColors(menu->colours[field_138 != 0 ? 0 : me->colours], GetTextKeyColor())`) appears three times (4950-4953, 4998-5001, 5013-5016). The hotkey paths (flags 2 and 0x20) draw the letter and the underline differently enough that no larger block repeats, and the `do { ... } while (pass--)` runs once because `pass` starts at 0. The pair is two lines; not queued.

**`0x4d8e60 ReportException`** (2644, `src/debug/debug_lib.cpp:828-978`).
More than forty `sprintf` appends, most of the form `{ char* d = log + strlen(log); sprintf(d, ...); }`, plus the grouped register (906-917), Dr (940-945) and FPU (948-955) dumps. The comments record that each site's destination form (`d`, `L`, or `strlen` inline) is what sets the push order, so the repetition is deliberate and a helper would unify the forms. Not queued.

**`0x48e010 MissionConditions::RegisterConditions`** (2542, `src/game/victory.cpp:482-606`).
Eighteen registration blocks and the two "if none registered" defaults (492-603) all end `victory[victoryCount] = ...; victoryCount++;` (or `defeat`). Queued as #6446 and proved below.

**`0x464f80 UpdatePlayers`** (2392, `src/game/players_464290.cpp:863-1190`).
A per-player loop around a state machine. The filter at the top is written twice with a fresh pointer (881-898), the two starting-resource blocks (1029-1056) differ only in `resourceSlot`/`field_d4` and `energy`/`metal`, and the end-game countdown block appears four times. Comments at 938-943, 967 and 1021-1028 record that the duplication is what makes MSVC reload or hoist the way the original does. Not queued.

**`0x42b370 UnitDef::operator=`** (2375, `src/units/unit_types.cpp:406-488`).
Compiler-generated: 13 element loops plus about 180 member copies. The issue names it as the example of genuinely long. Not queued.

**`0x44a680 UpdateBattleRoom`** (2340, `src/frontend/multi_44a680.cpp:247-394`).
The battle-room screen update. `IsScreenNamed("LOUNGE2.GUI")` is tested four times; the two host branches (285-306) both load the host's mission but differ in screen and follow-up calls, and the per-player version loop draws one row per player. No block with one shape. Not queued.

**`0x4224b0 LoadFeatureType`** (2324, `src/map/features_4224b0.cpp:144-291`).
Eight `seqname*` field reads (189-246): six save the sequence and clear its kind, two only save it, and the reads also fill the same `ok` local used earlier for the "object" read. Queued as #6448; the audit tried two helper shapes, both below MATCH.

**`0x495e90 HandleGameKey`** (2292, `src/ingame/keys.cpp:490-817`).
A switch on the key code whose case order is fixed by the jump table. The two game-speed guards (787-797, 799-809) share their player-flag test and differ only in the limit and sign. Not queued.

**`0x49be60 DrawProjectiles`** (2272, `src/weapons/weapons_49be60.cpp:176-387`).
A chain on `type->field_10c` with eight arms. The to-screen projection (`sp.x/y/z = pos... - scroll`) appears four times (249-251, 277-279, 288-290, 321-323) and the `sx`/`sy` pairs recur in most arms, but each arm then draws differently and one arm returns early. Not queued.

**`0x401360 UpdatePlayerEconomy`** (2239, `src/game/economy_401360.cpp:307-439`).
The four-field accumulation over one resource appears four times (368-375 and 378-385) and the storage clamp twice (422-429). The file header records that the separate `float[2]` arrays and their declaration order are the frame layout, so the blocks stay lower priority. Not queued.

**`0x418310 DrawMapDebugOverlay`** (2203, `src/game/console_commands.cpp:1549-1681`).
The four corners of a tile are loaded by walking `tile`, `x` and `y` (1573-1588), and the mode 2 and 3 height-colour pair is the same four lines twice (1625-1631, 1650-1656). The walk threads the indices through the stores, which is why the unrolled form is the one that matches. Not queued.

**`0x42d2e0 LoadUnitTypes`** (2173, `src/units/unit_types.cpp:955-1176`).
The unit load pass: movement classes, the unit table, the type sort, the per-unit FBI, 3DO and GUI loads, then the canbuild lists. The GUI-suffix search (1107-1126) and the canbuild read (1144-1166) are already source loops whose guard shapes carry comments. Not queued.

**`0x4a9fd0 UpdateMenu`** (2164, `src/gui/gui.cpp:6751-6955`).
A switch on the gadget type (6855-6932); each case calls one input handler, with the label case doing the selection work and the button and sprite cases ticking their colours. No block repeats. Not queued.

**`0x4a1b40 DrawListBox`** (2160, `src/gui/gui.cpp:1555-1771`).
The text path fades a highlighted row with four consecutive `FadeRectangle` colours (1647-1650), and the selected-row if/else calls with 0x1e in both arms (1652-1655, already in `docs/bugs.md`). A loop or helper for the fade run is plausible but lower priority; the cell path below (from 1671) has its own draw. Not queued.

**`0x4e0b90 MemoryStatusDialog::HandleMemoryStatusMessage`** (2150, `src/debug/memory_status_dialog.cpp:119-280`).
A message switch whose `WM_CTLCOLORSTATIC` case refreshes the dialog. It calls the file's `static __inline fmt_004e0b90` nine times (198-215) and then spells that formatter's body out once for `g_liveAllocCount` (218-247), with a comment that a tenth call would exceed the inline budget. Genuinely long; not queued.

**`0x430f00 SaveSettings`** (2112, `src/game/settings.cpp:554-653`).
One `WriteRegistryDword` or `WriteRegistryString` call per setting (560-648), plus a six-field per-skirmish-player loop. The calls take different keys and expressions, so there is no block to pull out. Not queued.

**`0x459c70 UnitTable::DrawLitPieces`** (2047, `src/graphics/model_render_4581e0.cpp:166-371`).
The first-face setup (`if (info->firstFace != -1) { face = info->faces + 1; fi = 1; } else { face = info->faces; fi = 0; }`) is written three times before three different face walks (240-246, 266-272, 295-301). It returns two values through caller locals, so the helper shape is the same risk as `LoadFeatureType`; lower priority and not queued.

**`0x411f50 AirStrikeOrder`** (1980, `src/orders/vtol_orders_411f50.cpp:159-284`).
Four arms build the same move effect: `new Class_0044e2d0(order, dest)`, `SetApproachRadius`, `SetAttachedFx` (198-201, 216-219, 254-257, 263-266; state 4 at 238-244 picks one of two classes). Queued as #6447; the audit shape reached 96.3%.

**`0x40fbe0 VtolFollowOrder`** (1976, `src/orders/vtol_orders_40fbe0.cpp:67-188`).
Move effects at 77-80 and 176-179 share their three statements (radius 128); the middle one (95-98) attaches an altitude instead, and the first site sets the flags before `SetAttachedFx`, not after. Two full sites is borderline; lower priority and not queued.

**`0x483610 LoadTntMap`** (1975, `src/map/line_of_sight.cpp:1014-1275`).
The TNT loader, already carried through the splitting workflow with five `// REGION` markers. The two version cases (1030-1065) assign the same fields from different offsets (`attr_a`/`attr_b` and the sea values swap), but each case is a straight run of assignments. Not queued.

### Proof: the RegisterConditions extraction

Every append site in `RegisterConditions` becomes a call to one helper, declared before the `MissionConditions` methods:

```cpp
// Appends a condition to one of the two lists, at the caller's count.
static inline void AddCondition(MissionCondition** list, int* count, MissionCondition* value)
{
    list[*count] = value;
    (*count)++;
}
```

with `AddCondition(victory, &victoryCount, new VictoryKillEnemyCommander);` and so on at the 20 sites, and `defeat`/`defeatCount` for the rest. `check.py 0x48e010` prints MATCH (original 2542 bytes, ours 2542 bytes). The change was reverted before this commit; #6446 is to land it.

Two other candidates were tried and reverted:

- `LoadFeatureType`: an out-parameter helper (`LoadFeatureSeq(TdfRecord*, char* key, char* seqname, Gaf_004224b0*, Seq_004224b0** out)`, 8 sites) reaches 91.8%, ours 2408 bytes against the original's 2324; a return-value helper (`Seq_004224b0* LoadFeatureSeq(...)`) reaches 47.3%, ours 2239, with the helper not inlined. Both move the frame. #6448 records this.
- `AirStrikeOrder`: the `AttachMove(order, dest, radius)` helper at the four clean sites reaches 96.3% with the same 1980 bytes; the inlined code is right, but one register pair in state 6's health block swaps. A void variant and leaving the state 6 site out stayed at 96.3%, so the swap is whole-function allocation. #6447 records this.

The RegisterConditions helper passed only the list, its count and the new condition, and its body sits in the caller's statement flow; the other two read and write caller state (`ok`, the sequence fields, the case-local `obj`) through parameters, which is where MSVC 5's whole-function register allocation moved (`docs/c2-regalloc.md`).

### Queued extractions

| Issue | Function | Repeated block |
| --- | --- | --- |
| #6446 | `MissionConditions::RegisterConditions` 0x48e010 | 18 registration and 2 default blocks, lines 492-603 (proved) |
| #6447 | `AirStrikeOrder` 0x411f50 | four attach-move blocks, lines 198-266 (audit shape 96.3%) |
| #6448 | `LoadFeatureType` 0x4224b0 | eight sequence reads, lines 189-246 (both audit shapes moved the frame) |

The remaining inlined-helper candidates stay here for a later pass:
`DrawLitPieces` (first-face setup, lines 240-301), `VtolFollowOrder` (two move
sites, lines 77-179), `UpdatePlayerEconomy` (accumulate and clamp blocks,
lines 368-429), `DrawMapDebugOverlay` (height-colour pair, lines 1625-1656),
`DrawListBox` (fade run, lines 1647-1650) and `DrawButton` (text-colour pair,
lines 4950-5016).

## The third 20 (issue #6493)

The third audit covers the next 20 game functions by size (1655 to 1964
bytes), read the same way as the first two: look for blocks that do one thing
with different inputs, and for code that repeats the body of a function that
is out of line elsewhere in the exe. Unlike the first two audits it is a
reading pass only: no extraction was built or checked, so none of the
candidates below is proved, and no follow-up issues are opened yet. Ten
functions hold repeated blocks that could come out as inline helpers; ten are
genuinely long. The line numbers are at the commit that added this section.

What stands out is that the GUI functions in this group inline three small
functions that are out of line elsewhere in `src/gui/gui.cpp`:

- `GetGadgetRect` (0x4a1630, line 1228): the `type == 0` origin test plus
  right and bottom from width and height. Written out in `LoadGuiLayer`,
  `HandleButtonInput` and twice in `DrawLabel`.
- `IsPointInRect` (0x4a1920, line 1349): the four-way bounds test. Written
  out nine times in `HandleButtonInput` and four times in `HandleListBoxInput`.
- `SelectFontForEntry` (0x4a1810, line 1311): the loop that finds the n-th
  type 7 entry and sets the font. Written out in `HandleListBoxInput`,
  `DrawLabel` and `LoadGuiLayer`. The file already holds a
  `static inline` copy for `HandleTextInput` (`SelectFontForEntry_inlined`,
  line 5141), which is the precedent for the shape of a helper.

### Verdicts

| Address | Bytes | Function | Verdict |
| --- | ---: | --- | --- |
| 0x4866d0 | 1964 | `ApplyUnitDeath` | genuinely long (death sequence; small two-site candidates) |
| 0x492360 | 1964 | `LoadGameScreenHandler` | inlined helpers (list-free block twice) |
| 0x429870 | 1938 | `LoadGameResources` | genuinely long (one lookup per resource) |
| 0x477ab0 | 1935 | `HandleNewGameClick` | genuinely long (gadget dispatch) |
| 0x4df590 | 1904 | `PerformanceDialog::HandlePerformanceMessage` | genuinely long (message switch) |
| 0x441460 | 1874 | `ConnectToGame` | inlined helpers (ten column appends) |
| 0x413470 | 1864 | `AirToGroundHoverOrder` | inlined helpers (move effect, shares the #6447 shape) |
| 0x49b720 | 1853 | `UpdateProjectiles` | genuinely long (projectile-type chain) |
| 0x4c8020 | 1845 | `DrawLitTexturedSpan` | inlined helpers (six span loops) |
| 0x412d40 | 1838 | `AirToAirOrder` | genuinely long (state switch; prologue shared with other orders) |
| 0x4a3780 | 1832 | `HandleListBoxInput` | inlined helpers (`IsPointInRect`, `GetGadgetRect` call, font select) |
| 0x487bf0 | 1811 | `RunInitialMission` | inlined helpers (position parse, order add) |
| 0x4aa8f0 | 1762 | `LoadGuiLayer` | inlined helpers (`GetGadgetRect`, name searches) |
| 0x4a6ae0 | 1703 | `HandleButtonInput` | inlined helpers (`GetGadgetRect`, `IsPointInRect` nine times) |
| 0x421700 | 1692 | `BreakPieceIntoDebris` | genuinely long (one debris builder) |
| 0x409730 | 1678 | `PlayerAI::ComputeBaseWeights` | genuinely long (scoring chain) |
| 0x466dc0 | 1662 | `DrawRadarUnits` | inlined helpers (four range circles) |
| 0x4a56b0 | 1662 | `DrawLabel` | inlined helpers (`SelectFontForEntry`, `GetGadgetRect` twice) |
| 0x499200 | 1655 | `BattleFrame` | genuinely long (input and state chain) |
| 0x462f30 | 1653 | `PacketReceiver::ReceiveFrame` | genuinely long (receive state machine) |

### The functions

**`0x4866d0 ApplyUnitDeath`** (1964, `src/units/units_485010.cpp:1161-1345`).
One straight death sequence: detach and unlink the unit, credit the kill by
death kind (switch at 1200-1234), update the rank table, adjust the
parent's metal, spawn the corpse, free the script, state and object. The
repeats are small: the commander-name test `_strcmpi(g_game->names[...].name,
unit->type->name)` twice (1210-1216, 1227-1230), the id-to-`Unit*` lookup
twice (1165-1168, 1175-1178), and the two rank loops over ten players
(1248-1266, 1270-1275). Comments at 1247, 1252, 1290, 1313 and 1334 record
that the loop form, compare arms, pointer and flag order are what the
registers need. Not queued.

**`0x492360 LoadGameScreenHandler`** (1964, `src/game/savegame.cpp:387-549`).
The list-free block (`GameFreeThunk` on the file names, descriptions, side
list, then the radar preview, each cleared to 0) is written out twice here
(399-410 and 513-524), and again in `SaveGameScreenHandler` (627-639) and
`game_state_490ac0.cpp:993`. The "back to the menu" tail (`mode = 2; handler
= MenuFrame; SetCloseHandler(LeaveNetGameCallback, 0)`, 534-536 and
510-512) is a second three-line repeat. The load itself (426-541) is one
sequence of summary-bank reads. Candidate: a `static inline` that frees the
four lists, at the two sites here. No out-of-line function matches the block.

**`0x429870 LoadGameResources`** (1938, `src/game/data_files.cpp:405-525`).
About eighty `g_game->x = (int)FindGafEntry(gaf, "name")` lines in five
groups, each preceded by `LoadAnimGaf` and followed by re-reading the field
into `gaf` (the comment at 411 says that re-read is needed), then a TDF loop
over the sides. `FindGafEntry` is already the helper and the lines differ in
destination and string, so there is no block to pull out. Not queued.

**`0x477ab0 HandleNewGameClick`** (1935, `src/frontend/campaign_menu.cpp:901-1020`).
A chain of `IsCurrentGadgetNamed` tests. The Core and Arm side arms (986-1001
and 1004-1015) mirror each other, but one is reached by `goto ArmSide` and
they differ in order and in the mission-list tail, and the comment at 967
records that the shared exit is what the registers need. Not queued.

**`0x4df590 PerformanceDialog::HandlePerformanceMessage`** (1904, `src/debug/debug_lib.cpp:5105-5305`).
A window-message switch (`WM_COMMAND`, `WM_TIMER`, `WM_INITDIALOG`, hotkey).
The five check-box cases (5162-5186) share `flag = (flag == 0);
SyncPerformanceSettings(0); return 0` but toggle different globals. The
only block that repeats an out-of-line body is the tree-iterator advance at
5199-5211, which is `NextNode` (called at 5249) with its `_Lockit` spelled
out; it occurs once, and the comment at 5200 records why it stays. Genuinely
long; not queued.

**`0x441460 ConnectToGame`** (1874, `src/frontend/multi_441460.cpp:90-243`).
The game-list builder writes eleven text columns, each ending in the same
advance, `p[n] += strlen(p[n]) + 1;` (163-222), and nine of them are built
by `sprintf(p[n], fmt, value)` just before it (the others use `strncpy`
or `strcpy`). That pair is the repeated block; a helper would take the
column pointer and the finished text. The ten `ConfigureListBoxByName`
calls (228-237) differ only in name and column index. The frame is
sensitive: the comments at 98, 108 and 154 place locals by size and
declaration order, and the column pointers live in the `p[21]` array, so the
helper has to take them by reference or by index. Candidate; moderate risk.

**`0x413470 AirToGroundHoverOrder`** (1864, `src/orders/vtol_orders_413470.cpp:158-266`).
Four arms build a move effect: `new Class_0044e2d0(order, dest)`,
`SetApproachRadius`, `SetAttachedFx` (177-180, 205-207, 214-216; the fourth
at 230-231 builds it but never attaches it, per the comment at 229). This is
the shape #6447 is extracting from `AirStrikeOrder` (`AttachMove`), so the
same helper applies here once #6447 shows whether it holds; the site at
177-180 sets the flags between the radius and the attach, like the first site
in `VtolFollowOrder`. The 22-line prologue (161-182) is identical in
`AirToAirOrder`, and its first two blocks (the `VTOL_SEEKATTACK` queueing,
161-170) recur in `AirToGroundOrder` (`vtol_orders_412710.cpp:181-190`) and
`AirStrikeOrder` (with `order->target.owner`); it is shared code between
functions, not a block repeated inside one. Candidate; wait for #6447.

**`0x49b720 UpdateProjectiles`** (1853, `src/weapons/weapons_49b720.cpp:195-348`).
One loop over the projectiles with a chain on the weapon type flags. The step
`p->pos += p->vel; p->pos += g_game->wind; p->vel.y -= g_game->gravity;
CheckProjectileCollision(type, p);` appears at 307-310, 318-321 and 324-327,
and the plain `pos += vel; CheckProjectileCollision` at 289-290, 293 and 300,
329-332. Each arm differs around it (lifetime tests, smoke, detonation), so
the block is four lines. Genuinely long; not queued.

**`0x4c8020 DrawLitTexturedSpan`** (1845, `src/graphics/surface.cpp:1583-1737`).
Six depth-tested loops for texture widths 128, 64, 32, 16, 8 and the general
case (1615-1704) are the same twelve lines (`value = z >> 16;` compare
against depth; texel fetch; palette lookup with `light >> 16`; store both;
step `v z light dest depth u`). They differ only in the texel index, where the
`v` shift and mask change (`v >> 9 & ~127` down to `v >> 13 & ~7`, and
`v >> 16` times the width for the default). The unlit half already calls
`BlitSpan128`, `BlitSpan64`, `BlitSpan32` and `BlitSpan16` out of line, which
is how the original wrote the unlit loops; the lit ones are the ones left
spelled out. Candidate: a `static inline` loop taking the shift as a
parameter, which must fold to a constant at each call. The comment at 1609
(one shared temporary keeps the loop counters in place) says the frame is
sensitive. Lower priority than the GUI candidates because the helper cannot
be a plain `inline` if the shift does not fold.

**`0x412d40 AirToAirOrder`** (1838, `src/orders/vtol_orders_412d40.cpp:158-236`).
A switch on the order state with two live cases. The move effect appears once
(177-180) and the `AirManeuverOrder` plus `SetAttachedFx` twice with
different arguments (203-205, 222-223). The prologue noted under
`AirToGroundHoverOrder` is the larger repeat, and it is across functions.
Genuinely long; not queued.

**`0x4a3780 HandleListBoxInput`** (1832, `src/gui/gui.cpp:2831-3017`).
Three blocks repeat bodies of out-of-line functions or each other:

- The bounds test `point.x >= r.x0 && point.x <= r.x1 && point.y >= r.y0 &&
  point.y <= r.y1` at 2872, 2892, 2897 and 2907 is the body of
  `IsPointInRect` (0x4a1920); here the rectangle is the local `Rect_004a3780`
  with `x0`..`y1`.
- The font search (2845-2862) is `SelectFontForEntry` (0x4a1810), which
  differs from it only in the fallback font field (`current` here).
- The separator-row test `s = SkipTextLines(field_c2, n); if
  (strncmp(DAT_00502a20, s, 2) == 0) field_ba = orig_sel;` at 2884-2888,
  2921-2923, 2989-2991 and 3005-3007, and the index clamp at 2877-2883 and
  2912-2919.

The call to `GetGadgetRect` (2844) is already out of line, so the comment
there about the `y1` operands records an earlier near miss. The goto labels
(`skip0`, `above`, `ret1`, `finish`) are load-bearing (comments at 2873,
2974, 3009), so only the first two are candidates. Candidate; high risk.

**`0x487bf0 RunInitialMission`** (1811, `src/units/unit_commands.cpp:89-268`).
A command interpreter, one `switch` on the first letter of each comma-split
token. Two repeats inside it: the position parse `pos.x = (int)(f1 * 65536.0);
pos.y = 0; pos.z = (int)(f2 * 65536.0)` at 130-132, 141-143, 164-166,
177-179 and 203-205, and the pair `GetOrderType(&out.k, id, unit, 0, &pos);
AddOrder(out.k, 1, unit, 0, &pos, ...)` at 133-134, 144-145, 167-169 and
180-181. The `processed`/`selected` stores differ per case. The comment at 93
says loose locals change the frame, and the multiply is written `f * 65536.0`
in some cases and `65536.0 * f` in others, so a shared helper would unify
spellings that the compiler treats differently. Candidate; moderate risk.

**`0x4aa8f0 LoadGuiLayer`** (1762, `src/gui/dialogs.cpp:180-372`).
The fade rectangle at 193-204 is `GetGadgetRect` (0x4a1630) on `cur->entries`
written out. Three search loops (292-302, 304-314, 317-328) find the first
type 1 entry whose name starts with one of two prefixes (`OK` or `NEXT`,
`PREV` or `Cancel`) or equals the focus name; the first two differ only in
the prefixes and the destination, and the comments at 306 and 320 show they
are already tuned individually. The two offset-the-gadgets loops (238-246,
249-256) add different offsets. The final text-focus font search (350-361) is
`SelectFontForEntry` with a different base. Candidates: `GetGadgetRect` (one
site) and the two name searches (two sites).

**`0x4a6ae0 HandleButtonInput`** (1703, `src/gui/gui.cpp:4899-5109`).
The rectangle at 4907-4916 is `GetGadgetRect` written out, and the bounds
test is `IsPointInRect` written out nine times (4922, 4943, 4957, 4968, 4985,
5000, 5019, 5043 and 5050, in both the positive and negated form). With the
mouse-up arms (4938-5014), these account for most of the function. This is
the strongest example in the audit of a block that is verbatim an out-of-line
function elsewhere. Candidate: `IsPointInRect`-shaped `static inline` taking
the local `Rect` and the point; the comment at 5033 says the block order of
the remaining chain is load-bearing, so the helper must not reorder tests.

**`0x421700 BreakPieceIntoDebris`** (1692, `src/weapons/explosions.cpp:828-927`).
One body for one face of the piece: allocate the debris object, copy the
vertices, derive a normal, push the back face along it, centre the vertices,
copy the face data. The repeats are three-field runs: the fixed-to-float
conversion of three vertices (875-883), the three `RandomInt` velocity lines
(856-858) and the three spin lines (859-861), and the three-axis add, divide
and subtract loops (894-913). Each is a few lines and the casts are tuned
(comment at 886). Genuinely long; not queued.

**`0x409730 PlayerAI::ComputeBaseWeights`** (1678, `src/ai/ai_player_409730.cpp:305-363`).
A per-unit-type scoring chain: add weights for flags, then the multipliers,
then the clamps. The two clamps for `e->b` and `e->c` (359, 361) share the
`(char)max(0.0f, min(100.0f, ...))` shape and the energy-use term appears
twice (327, 359), but comments at 329 and 360 record that the method call and
the conditional are what the first resize and `RateWeapons` need. Genuinely
long; not queued.

**`0x466dc0 DrawRadarUnits`** (1662, `src/map/radar.cpp:597-723`).
Two loops (units, then projectiles). The unit loop draws four range circles
with the same line, `if (type->d != 0) DrawCircle(surface, x, y, (int)g_game->
width * type->d / g_game->mapWidth, base[c])`, for the radar, sonar, radar
jam and sonar jam distances (640-655; the first two use `base[0xa]`, the
second two `base[0xc]`). A fifth circle for weapon ranges (662-670) has the
same scale expression with a dashed variant. Candidate: a `static inline` that
takes the distance and colour. The projectile loop (693-715) is two arms with
different drawing. The file already carries `ScaleY_00466dc0` and
`OnRadar_00466dc0` as helpers, so a third in the same style fits.

**`0x4a56b0 DrawLabel`** (1662, `src/gui/gui.cpp:4437-4549`).
The font loop at 4443-4457 is `SelectFontForEntry` (0x4a1810) written out,
and both rectangles (4463-4472 and 4510-4519) are `GetGadgetRect` (0x4a1630)
written out, the second against a re-read `entries2`. The rest is one draw
sequence (fill, shadow, text, then either grey and fade or the hotkey
underline). Candidate; this function and `HandleButtonInput` are the two
best proofs for the `GetGadgetRect` shape, since the inlined `rect` is a
plain local in both.

**`0x499200 BattleFrame`** (1655, `src/ingame/build_placement.cpp:258-412`).
The per-frame input chain: a cursor pick, the order-panel flag dispatch, a
mouse message chain (287-347), the end-game check, and the restart request.
The two restart arms (380-409) share their shutdown and return-to-menu
calls but differ in order and in what sits between them; the comment at 379
records that the compiler merges their stores. The box start and end writes
(331-337, 324-326) are three-field runs. Genuinely long; not queued.

**`0x462f30 PacketReceiver::ReceiveFrame`** (1653, `src/network/packets.cpp:1542-1772`).
A receive state machine over the saved-frame and spare buffers. Repeated
four-line pieces: the copy-out to the caller (`net + 0x4b5`, `0x4b9`,
`memcpy`, `*size`) at 1559-1562 and 1761-1764, the saved-frame swap at
1587-1598 and 1693-1702, and the `Prev`/`Next` gap report pairs (1676-1688).
The comments at 1553, 1563, 1606, 1615, 1644, 1741 and 1746 record that exit
blocks, the `delete`/`new` spellings and the duplicated `Peek` are what the
original code has, so the repetition is deliberate. Genuinely long; not
queued.

### Candidate extractions

None of these has been built. They are listed in the order they look worth
trying; each needs `check.py` on every function it touches, `place.py`, and
the symbol-id note at the top of this document.

| Function | Repeated block | Evidence |
| --- | --- | --- |
| `HandleButtonInput` 0x4a6ae0 | nine `IsPointInRect` tests, one `GetGadgetRect`, lines 4907-5051 | verbatim bodies of 0x4a1920 and 0x4a1630 |
| `DrawLabel` 0x4a56b0 | `SelectFontForEntry` once, `GetGadgetRect` twice, lines 4443-4519 | verbatim bodies of 0x4a1810 and 0x4a1630 |
| `HandleListBoxInput` 0x4a3780 | four `IsPointInRect` tests, the font search, lines 2845-2907 | verbatim bodies of 0x4a1920 and 0x4a1810 |
| `LoadGuiLayer` 0x4aa8f0 | `GetGadgetRect`, two name searches, lines 193-314 | verbatim body of 0x4a1630; two near-identical loops |
| `DrawRadarUnits` 0x466dc0 | four range circles, lines 640-655 | same call four times |
| `AirToGroundHoverOrder` 0x413470 | three move effects, lines 177-216 | same shape as #6447 |
| `ConnectToGame` 0x441460 | ten column advances, lines 161-222 | same two lines ten times |
| `RunInitialMission` 0x487bf0 | five position parses, four order adds, lines 130-181 | same lines in four to five cases |
| `LoadGameScreenHandler` 0x492360 | list-free block twice, lines 399-524 | same twelve lines twice (four in the file) |
| `DrawLitTexturedSpan` 0x4c8020 | six lit span loops, lines 1615-1704 | same twelve lines six times; the shift must fold |

The four GUI functions at the top are one family: a helper of the form
`static inline` for each of the three out-of-line bodies, declared once above
the first use in `gui.cpp` (and in `dialogs.cpp` for `LoadGuiLayer`), is the
smallest first attempt, and `HandleListBoxInput` and `HandleButtonInput` can
reuse the same `IsPointInRect` helper. Calling the out-of-line functions
instead is not an option: `HandleButtonInput` and `LoadGuiLayer` inline them,
and the first audit showed that replacing inlined code with a call changes
the size (`LoadSideData`, 46.6%).
