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
