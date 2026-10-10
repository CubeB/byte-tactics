# Files that stay split from their module

Phase 3 of `docs/cleanup-roadmap.md` gathers each module into one file. The
table below is the record of the `<module>_<address>.cpp` files that still
cannot join their module file: each merge was retried (issue #6252) and the
reason is what stopped it. A module whose row list is empty is one file.

| Module | File | Reason |
| --- | --- | --- |
| graphics/particles | src/graphics/particles_470c10.cpp | Grow needs the file's hand-written `<vector>` view of the pool; the real `<vector>` compiles it differently. |
| graphics/particles | src/graphics/particles_471160.cpp | The list's inlined add needs the `std::vector<Elem_00473500>` helper names, which the module's `std::vector<ParticleSystem*>` view cannot produce. |
| graphics/particles | src/graphics/particles_471820.cpp | Same list view as particles_471160.cpp. |
| graphics/particles | src/graphics/particles_471a50.cpp | Same list view as particles_471160.cpp. |
| graphics/particles | src/graphics/particles_471de0.cpp | Its SIB byte follows the file's symbol total (`docs/c2-regalloc.md`); merged, one store's operand order flips. |
| graphics/particles | src/graphics/particles_472630.cpp | With `ParticleSystem`'s constructor, destructor and operator new defined in the module, `/Ob2` inlines them into the smoke constructors and the emission entry points. |
| graphics/particles | src/graphics/particles_4732e0.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| graphics/particles | src/graphics/particles_473a00.cpp | In the module the fog arm's cell address picks the other SIB base. |
| graphics/particles | src/graphics/particles_473d50.cpp | `NanoParticles::Emit` needs the file's hand-written `<vector>` view of the records. |
| graphics/particles | src/graphics/particles_474b80.cpp | In the module the fog arm's cell address picks the other SIB base and drops the fog pointer from a register. |
| graphics/particles | src/graphics/particles_4750f0.cpp | It returns bool, where 0x475600 tests the result as an int. |
| graphics/particles | src/graphics/particles_475470.cpp | In the module the fog-culled loop walks from the wrong field. |
| graphics/particles | src/graphics/particles_475700.cpp | In the module the inlined draw rotates its temporaries differently. |
| graphics/particles | src/graphics/particles_4758c0.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| graphics/particles | src/graphics/particles_475bd0.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| graphics/particles | src/graphics/particles_475ef0.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| graphics/particles | src/graphics/particles_476210.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| graphics/particles | src/graphics/particles_476490.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| map/terrain | src/map/terrain_47cc30.cpp | The symbol count moves the owner index multiply onto the other operand. |
| map/terrain | src/map/terrain_47d0e0.cpp | The count clears bit 14 of g_game's id, which decides how the cell index multiply folds. |
| map/terrain | src/map/terrain_47d820.cpp | The count changes the operand order of the footprint mask load. |
| map/terrain | src/map/terrain_47e2d0.cpp | The symbol ids move the def load and the feature lookup's operand order. |
| map/los_tables | src/map/los_tables_433130.cpp | It needs a hand-written `std::vector` so that insert and erase stay out of line, which the module's real `<vector>` would redefine. |
| map/los_tables | src/map/los_tables_433270.cpp | It needs its `Wrap_00433270` element view so that the innermost `_Destroy`/`deallocate` calls stay out of line. |
| map/los_tables | src/map/los_tables_433a80.cpp | `std::_Destroy` and `std::_Construct` overloads must call the out-of-line `DestroyPoint` and `StoreDwordIfDst`; the module's plain template cannot. |
| map/los_tables | src/map/los_tables_434360.cpp | It needs the same `_Destroy` name for the element type to call `DestroyLine`. |
| util/tdf | src/util/tdf_4c2f60.cpp | It needs its own `TdfRecord` view: each path that deletes the root section destroys its entries a different way. |
| util/tdf | src/util/tdf_4c3120.cpp | Same `TdfRecord` view as tdf_4c2f60.cpp. |
| util/tdf | src/util/tdf_4c51b0.cpp | Same `TdfRecord` view as tdf_4c2f60.cpp, for the destructor and `??_GTdfField`. |
| util/tdf | src/util/tdf_4c54f0.cpp | A unit of its own: it calls the TDF methods (not inlined) and each file sees `TdfField` its own way. |
| util/tdf | src/util/tdf_4c59d0.cpp | With the real `<vector>` (tdf_4c54f0.cpp) the helpers are inlined into this insert. |
| util/hpi | src/util/hpi_4bb4e0.cpp | Gap code (`tools/gapcheck.py` sizes a region by the next annotation in the file, so it cannot share a file). |
| util/hpi | src/util/hpi_4bc800.cpp | Gap code, same as hpi_4bb4e0.cpp. |
| util/hpi | src/util/hpi_4bd830.cpp | Joined, its register allocation changes (the entry-name strlen takes another register) and the bytes only match with this file's declarations. |
| util/hpi | src/util/hpi_4be320.cpp | `HAPI_ClearShadowFlags` would be inlined into its recursive call, where the original calls it out of line. |
| util/hpi | src/util/hpi_4be6c0.cpp | The module's hand-written `std::vector` keeps this insert out of line; the real `<vector>` would inline it. |
| network/net_game | src/network/net_game_453d40.cpp | HandleNetPackets' base/index orders and registers follow the symbol ids of a file that includes `<windows.h>` and `<memory.h>` before g_game; merged, it stays at 80 to 82% (swapped base/index pairs) for every count of real declarations before it (0 to 700) and before g_game (0 to 65536). |
| network/unit_sync | src/network/unit_sync_46ca60.cpp | FinishUnitSync calls the out-of-line `_Destroy` of the `Elem_0046faf0` vector (0x46e870), where the real `<vector>` inlines its empty body (79.5% merged); it needs the file's hand-written `std::vector`. |
| network/unit_sync | src/network/unit_sync_46cc10.cpp | SendSequenced inlines `vector::insert` but calls the out-of-line `_Destroy`, `_Ucopy` and `_Ufill`, which the real `<vector>` inlines too (832 bytes against 678); it needs the file's hand-written `std::vector`. |
| network/unit_sync | src/network/unit_sync_46d1a0.cpp | `~UnitSync` needs real `std::map`, `std::list` and `std::vector` members; the module's `UnitSync` holds the hand-written `UnitSyncMap` and element views, and the destructor merged is 168 bytes against 312. |
| network/unit_sync | src/network/unit_sync_46dad0.cpp | `ProcessSync` needs `UnitSync`'s player list as a `vector<SyncPlayerRecord>` and the real `std::map`, and redefines `SyncTaggedVector`, `SyncTempTaggedVector` and `SyncPlayerRecord` as the module has them. |
| network/unit_sync | src/network/unit_sync_46e640.cpp | `// FLAGS: /Gi`, which the merged file cannot carry (89.6% without it). |
| network/unit_sync | src/network/unit_sync_46eba0.cpp | `// FLAGS: /Gi`, which the merged file cannot carry. |
| network/unit_sync | src/network/unit_sync_46f7a0.cpp | `// FLAGS: /Gi`, which the merged file cannot carry; it also needs a hand-written `std::vector`. |
| orders/vtol_orders | src/orders/vtol_orders_40f790.cpp | The two reference arguments take the other registers; it matches only with 79 more symbol ids before it (one count in 0 to 127), and those move 0x413d80 and 0x415250 off their windows. |
| orders/vtol_orders | src/orders/vtol_orders_410c70.cpp | Merged, its owner load takes the other base and index at every count from 0 to 79 more ids, and its inlined vector code moves 0x413bc0 off its window. |
| ai/ai_player | src/ai/ai_player_407d40.cpp | The constructor's vtable stores need the plain view of SpatialTimer apart from the module's virtual one. |
| ai/ai_player | src/ai/ai_player_407e70.cpp | SpatialTimer's slot 0 and scalar deleting destructor need the derived view of SpatialTimer apart from the module's. |
| ai/ai_player | src/ai/ai_player_408090.cpp | The joined file folds the cell index's width load into the imul, so the function is two bytes short. |
| ai/ai_player | src/ai/ai_player_408100.cpp | Its symbol count comes from ta_types.h. |
| ai/ai_player | src/ai/ai_player_408620.cpp | In the joined file its table access takes the opposite SIB base. |
| ai/ai_player | src/ai/ai_player_408f30.cpp | The vector::insert is built with /Gi, which the merged file cannot carry. |
| ai/ai_player | src/ai/ai_player_409730.cpp | ComputeBaseWeights needs that file's cut-down `<vector>` and its small symbol count. |
| ai/ai_player | src/ai/ai_player_40b1c0.cpp | In the joined file the compiler encodes its table access as `[pointer + id]` instead of the original's `[id + pointer]`. |
| ai/ai_player | src/ai/ai_player_40b530.cpp | Its `std::_Construct` hook, declared before `<vector>`, changes every `vector<Unit*>` copy in the file. |
| ai/ai_player | src/ai/ai_player_40c530.cpp | The out-of-line vector members' register allocation follows the emissions in that file; in the joined file four fall out of their windows and splitting just those four out does not help. |
| debug/debug_lib | src/debug/debug_lib_4d8310.cpp | Gap region: its `int 3` inline assembly, which the merged game file cannot carry. |
| debug/debug_lib | src/debug/debug_lib_4d8870.cpp | Gap region: the `TraceRecord` constructor's inline stack-walk assembly. |
| debug/debug_lib | src/debug/debug_lib_4d8d70.cpp | Gap region: the inline `mov top, esp`. |
| debug/debug_lib | src/debug/debug_lib_4d9ab0.cpp | Gap region: its `int 3` inline assembly and `__except` handler. |
| debug/debug_lib | src/debug/debug_lib_4da120.cpp | Gap region compiled `/Od`; every value goes through its local. |
| debug/debug_lib | src/debug/debug_lib_4da2c0.cpp | Gap region compiled `/Od`; the debug thread started by 0x4da1d0. |
| debug/debug_lib | src/debug/debug_lib_4da3f0.cpp | Joined, the `text + (size - len) - 1` destination is computed with its operands in the other order. |
| debug/debug_lib | src/debug/debug_lib_4dacf0.cpp | It inlines `FreeBlockIter`'s begin/end from the file's own view of the free-block classes. |
| debug/debug_lib | src/debug/debug_lib_4db610.cpp | Its constructor builds the `std::map` member from the file's own view of the class. |
| debug/debug_lib | src/debug/debug_lib_4db7d0.cpp | It inlines the FreeBlockMap `upper_bound` stub from the file's own view of the class. |
| debug/debug_lib | src/debug/debug_lib_4dfd10.cpp | The singleton's atexit term function is the file's first static; joined, 0x4dfd50 loses the `_$E2` name the placement build knows. |
| debug/debug_lib | src/debug/debug_lib_4dfd50.cpp | It is the `_$E2` term function the static in 0x4dfd10 generates; joined it is not the file's first static. |
| debug/debug_lib | src/debug/debug_lib_4e16b0.cpp | Gap region. |
| debug/debug_lib | src/debug/debug_lib_4e1990.cpp | Merged, `/Ob2` inlines `NameKey::LessThan` at the 0x4e1a30 call site, which the original calls out of line. |
| debug/debug_lib | src/debug/debug_lib_4e1e50.cpp | Gap region (with 0x4e20a0). |
| debug/debug_lib | src/debug/debug_lib_4e21f0.cpp | Its 0.0 and 5.0 constants sit in a different constant pool from the memory status dialog's 0.0. |
| debug/debug_lib | src/debug/debug_lib_4e2580.cpp | Its `NameMapTree` is keyed by `const char*` and returns its own iterator, not the `NameKey`-keyed view the tree methods use. |
| debug/debug_lib | src/debug/debug_lib_4e2620.cpp | Its `NameMapTree` carries the tree fields and its own rotations, where 0x4e2250's inherits them from the XTREE chain. |
| debug/debug_lib | src/debug/debug_lib_4e35b0.cpp | Gap region. |
| debug/debug_lib | src/debug/debug_lib_4e3750.cpp | Joined, its register allocation lands differently, and its inlined GDPERF calls come out differently. |
| map/features | src/map/features_421f20.cpp | The module is built with `/Gi`, and in the joined file the loop address's base and index swap (`lea [edx+ebx+4]` against the original's `[ebx+edx+4]`). |
| map/features | src/map/features_4224b0.cpp | Merged, the symbol ids schedule the `name` load after the strncpy setup, and adding its prototypes moves 0x425210 off its window. |
| map/features | src/map/features_422ea0.cpp | `/Gi` changes the base and index of the feature-table address and of the dead, burnt and reclamate stores; merged it falls to 81.4%. |
| map/features | src/map/features_424c00.cpp | It matches only with `include/ta_types.h` at its exact size and with the 3D loop's `c` numbered past 65536; `/Gi` alone leaves it at 89.6%. |
| network/online | src/network/online_45b250.cpp | Gap code: `tools/gapcheck.py` sizes a gap region by the file it is in, so it cannot share online.cpp. |
| network/online | src/network/online_45b490.cpp | Gap code, same as online_45b250.cpp. |
| network/online | src/network/online_45b670.cpp | Gap code, same as online_45b250.cpp. |
| graphics/draw | src/graphics/draw_4c0330.cpp | Merged, its dy local keeps a register where the original spills it, which takes 4 bytes off the frame and moves the block codegen. |
| graphics/model_render | src/graphics/model_render_4581e0.cpp | Its summing loop needs to be the first function after the module's own types; even there the merged context puts the store's lea before the fadd (99.8%). |
| graphics/model_render | src/graphics/model_render_4589c0.cpp | It calls BuildObjectPicture and DrawPieces on CMemoryCache, so it needs the class to derive from UnitTable, where the module makes CMemoryCache the base. |
| graphics/model_render | src/graphics/model_render_458fa0.cpp | Merged, one of its copy loop's two pointer loads is scheduled before the other (99.3%). |
