// Angles16, the three angles of a unit (+0x64 of the unit): bank, heading and
// pitch, 16 bits each (0x10000 is a full turn). The one declaration for the
// files that share it. No constructors, as for Vec3 in vec3.h.
#ifndef ANGLES_H
#define ANGLES_H

struct Angles16 {
    short bank;
    unsigned short heading;
    short pitch;
};

#endif
