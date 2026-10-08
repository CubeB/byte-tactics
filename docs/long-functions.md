# The 20 longest functions: inlined helpers or genuinely long

Phase 7 of `docs/cleanup-roadmap.md` asks which of the biggest functions are
really helpers the compiler inlined and which are genuinely long, and queues
the extractions worth doing. This audits the 20 largest game functions: the
addresses and sizes are from `data/functions.csv`, the names from
`data/symbols.csv`, and the line numbers are at the commit that added this
file.

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

**`0x47ae60 HandleSkirmishClick`** (2947, `src/frontend/skirmish_menu_479660.cpp:905-1180`).
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
