// How far a shot carries at each of eleven elevations, in percent of the
// weapon's range: 0x43cc20 takes the unit's pitch in steps of 2048, clamps it
// to -5..5 and reads entry pitch + 5, which it reaches as DAT_00505205[pitch].

// GLOBAL: 0x505200
signed char g_rangeByPitch[11] = {25, 55, 70, 85, 100, 100, 75, 50, 25, 20, 15};
