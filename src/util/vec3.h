// Vec3, a 3D point: three ints (Thaldren's 0xc-byte Vec3, x/y/z), and Point16,
// a map cell or footprint: two 16-bit coordinates, x and y. The one
// declaration of each for the files that share them. No constructors: a user
// constructor changes the inlining of MoveTowards (PR #6278). A view that
// gives y a fractional half, or adds other members or methods, keeps its own.
#ifndef VEC3_H
#define VEC3_H

struct Vec3 {
    int x;
    int y;
    int z;
};

struct Point16 {
    short x;
    short y;
};

#endif
