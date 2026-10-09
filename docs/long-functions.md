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

**`0x43f0e0 GetOrderType`** (4420, `src/orders/order_dispatch_43f0e0.cpp:542-749`).
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
