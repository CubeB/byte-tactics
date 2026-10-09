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
