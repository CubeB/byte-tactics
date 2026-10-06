# Thaldren's names

Names from Thaldren's Ghidra database of `TotalA.exe`, built by hand over years
of Total Annihilation modding. He has agreed to it being used however this
project likes. These files are evidence for naming, not names to copy.

| File | Rows | What |
| --- | --- | --- |
| `functions.csv` | 3,888 | `address,namespace,name,signature,comment`: every function he named |
| `globals.csv` | 810 | `address,name,type,comment`: named data |
| `structs.csv` | 2,937 | `struct,size,offset,field,type,comment`: fields of his 300 structs |

Addresses use the repository's form (`0x401000`). Ghidra's own placeholders
(`FUN_`, `DAT_`, `LAB_`) are left out.

## How far to trust it

- **Strong:** game structs and object layouts (`GameState`, `UnitInstance`,
  `UnitDef`, `MapInfo`, `PlayerState`, `TdfFile`). It caught swapped
  `SpendMetal`/`SpendEnergy` and showed that the 0xec4 "network" object is the
  mission (`.ota`) info.
- **Weak:** the debug library, where stack-trace records are called "string
  constructors".
- **Hierarchies are flattened:** `CobScript` and `UnitScript` are one type there.
- **Signatures:** a `__fastcall (int param_1)` entry is usually a method that
  takes only `this`. Parameter types are often `undefined4`.

## Mapping to this repository's names

His style (`Prefix_Verb`, `UnitInstance`, `AiSearch`) is not this repository's
(`docs/tidy-up.md`, "Conventions"). Use his name as evidence for what something
is, then name it in the repository's style, and map his types onto the names
already established here:

| His | Here |
| --- | --- |
| `UnitInstance` | `Unit` |
| `GameState` | `Game` |
| `PlayerState` | `Player` |
| `AiSearch` | `Pathfinder` |
| `MapInfo` (0xec4) | `Mission` |
| `AudioEngine` | `Sound` |
| `TdfSection` | `TdfRecord` |

When a placeholder has a name here, cite it in the pull request's evidence
column as `Thaldren: <name>`, with what else supports it (strings, callers).

## Regenerating

`TotalA.exe.gzf` is not in the repository. With it in the repository root:

```sh
toolchain/ghidra/support/analyzeHeadless /tmp/thal Thal -import TotalA.exe.gzf \
    -noanalysis -scriptPath tools/ghidra -postScript BtExportNames.java <out> \
    -deleteProject
```

then rewrite the addresses as `0x<hex without leading zeros>` and drop the
structs under `/DOS/` and the other Windows header categories.
