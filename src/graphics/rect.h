// Rect, a rectangle of four ints: left, top, right and bottom, inclusive. The
// clip rectangle of a Surface and the argument of its clip methods. The one
// declaration for the files that share it. Pure data (the exe has no member
// function of it), and nothing is included.
#ifndef RECT_H
#define RECT_H

struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

#endif
