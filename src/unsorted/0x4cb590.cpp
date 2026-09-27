// Decompiled by Opus. Names are provisional.
// Mirrors an object tree: negates x and z of every vertex and two fields of
// each node, recursing into the first child and then the next sibling (the
// sibling call became a loop).

struct Vertex_004cb590 {
    int x;                             // +0x0
    int y;                             // +0x4
    int z;                             // +0x8
};

struct Object_004cb590 {
    int unknown_0;
    int vertexCount;                   // +0x4
    char unknown_8[8];
    int field_10;                      // +0x10
    int unknown_14;
    int field_18;                      // +0x18
    char unknown_1c[8];
    Vertex_004cb590* vertices;         // +0x24
    int unknown_28;
    Object_004cb590* child;            // +0x2c
    Object_004cb590* sibling;          // +0x30
};

// FUNCTION: 0x4cb590
void __stdcall FUN_004cb590(Object_004cb590* obj)
{
    for (int i = 0; i < obj->vertexCount; i++) {
        obj->vertices[i].x = -obj->vertices[i].x;
        obj->vertices[i].z = -obj->vertices[i].z;
    }
    obj->field_10 = -obj->field_10;
    obj->field_18 = -obj->field_18;
    if (obj->child)
        FUN_004cb590(obj->child);
    if (obj->sibling)
        FUN_004cb590(obj->sibling);
}
