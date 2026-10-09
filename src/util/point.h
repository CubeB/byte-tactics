// Point, a 2D screen point: two ints, x and y. The one declaration for the
// files that share it. Point16 in vec3.h is the 16-bit map cell; this is the
// 32-bit drawing and hit-test point. No constructors: a user constructor
// changes inlining, as for Vec3 in vec3.h (PR #6278).
#ifndef POINT_H
#define POINT_H

struct Point {
    int x;
    int y;
};

#endif
