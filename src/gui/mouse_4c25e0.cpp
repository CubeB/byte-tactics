// Decompiled by Sonnet 5.5, finished by space-bunny-free. Names are provisional.
// Redraws the software mouse cursor: reads the cursor position, computes the
// cursor's top left (position minus the sprite's origin), rebuilds the saved
// background and the composed image in the three off-screen surface
// descriptors at +0x1be, +0x1c2 and +0x1c6 (each resized to the sprite),
// restores what the old cursor covered, draws the sprite, and hands the union
// of the old and new cursor rectangles to UnlockPrimary to be shown.
#include <windows.h>

#pragma pack(push, 1)
struct Sprite_004c25e0 {
    unsigned short width;              // +0x0
    unsigned short height;             // +0x2
    short dx;                          // +0x4
    short dy;                          // +0x6
};

struct Desc_004c25e0 {
    int width;                         // +0x0
    int height;                        // +0x4
    int pitch;                         // +0x8
};

struct Rect_004c25e0 {
    int left;
    int top;
    int right;
    int bottom;
};

struct App_004c25e0 {
    char unknown_0[0x196];
    int cursorX;                       // +0x196
    int cursorY;                       // +0x19a
    char unknown_19e[0x1b2 - 0x19e];
    Sprite_004c25e0* sprite;           // +0x1b2
    int savedX;                        // +0x1b6
    int savedY;                        // +0x1ba
    Desc_004c25e0* saved;              // +0x1be
    Desc_004c25e0* under;              // +0x1c2
    Desc_004c25e0* work;               // +0x1c6
    char unknown_1ca[0x1d2 - 0x1ca];
    int enabled;                       // +0x1d2
};
#pragma pack(pop)

struct Info_004c25e0 {
    int data[12];
};

int __stdcall LockPrimary(Info_004c25e0* info);
void __cdecl BlitSurface(void* a, void* b, int x, int y);
void __stdcall ResetClipRect(Desc_004c25e0* d);
void __stdcall DrawFrame(Desc_004c25e0* d, Sprite_004c25e0* s, int x, int y);
int __stdcall UnlockPrimary(void* out, Rect_004c25e0* a, Rect_004c25e0* b);

// FUNCTION: 0x4c25e0
void __stdcall RedrawMouseCursor(App_004c25e0* app)
{
    // info doubles as the first rect for BlitSurface: no separate rect local.
    Info_004c25e0 info;
    Rect_004c25e0 r1;
    Rect_004c25e0 r2;
    POINT pt;
    if (app->enabled == 0)
        return;
    if (LockPrimary(&info) == 0)
        return;
    GetCursorPos(&pt);
    int x = pt.x;
    int y = pt.y;
    app->cursorX = x;
    app->cursorY = y;
    x -= app->sprite->dx;
    y -= app->sprite->dy;
    app->under->width = app->sprite->width;
    app->under->height = app->sprite->height;
    app->under->pitch = app->sprite->width;
    app->work->width = app->sprite->width;
    app->work->height = app->sprite->height;
    app->work->pitch = app->sprite->width;
    BlitSurface(app->under, &info, -x, -y);
    BlitSurface(app->under, app->saved, app->savedX - x, app->savedY - y);
    BlitSurface(app->work, app->under, 0, 0);
    ResetClipRect(app->work);
    DrawFrame(app->work, app->sprite, app->sprite->dx, app->sprite->dy);
    BlitSurface(app->saved, app->work, x - app->savedX, y - app->savedY);
    r1.left = app->savedX;
    r1.top = app->savedY;
    // Right and bottom computed from the just-stored left and top fields.
    r1.right = r1.left + app->saved->width;
    r1.bottom = app->saved->height + r1.top;
    r2.left = x;
    r2.top = y;
    r2.right = x + app->sprite->width;
    r2.bottom = y + app->sprite->height;
    BlitSurface(&info, app->saved, app->savedX, app->savedY);
    BlitSurface(&info, app->work, x, y);
    app->saved->width = app->sprite->width;
    app->saved->height = app->sprite->height;
    app->saved->pitch = app->sprite->width;
    BlitSurface(app->saved, app->under, 0, 0);
    app->savedX = x;
    app->savedY = y;
    UnlockPrimary(&info, &r1, &r2);
}
