// Box: an axis-aligned box, the two corners lo and hi. The one declaration of
// the box for the files that read its corners. It includes vec3.h, since a
// by-value member needs the full type. A view whose Vec3 gives x, y or z a
// fractional half, or adds methods, does not fit this plain Vec3 and keeps its
// own Vec3 and Box (unit_orders.cpp, vtol_orders.cpp).
#ifndef BOX_H
#define BOX_H

#include "../util/vec3.h"

struct Box {
    Vec3 lo;
    Vec3 hi;
};

#endif
