// Decompiled by Opus. Names are provisional.
// Moves the cursor to the child of the current node (or of the root when
// there is none) whose name matches case-insensitively; returns whether
// one was found.
#include <string.h>

struct Node_004c3410 {
    char* name;                         // +0x0
    int unknown_4;
    Node_004c3410** first;              // +0x8
    Node_004c3410** last;               // +0xc

    Node_004c3410* FindChild(char* key)
    {
        for (Node_004c3410** p = first; p < last; p++) {
            if (_strcmpi((*p)->name, key) == 0)
                return *p;
        }
        return 0;
    }
};

class Class_004c3410 {
public:
    Node_004c3410* root;                // +0x0
    Node_004c3410* current;             // +0x4

    int SelectRecord(char* name);
};

// FUNCTION: 0x4c3410
int Class_004c3410::SelectRecord(char* name)
{
    Node_004c3410* node = current;
    if (node == 0)
        node = root;
    current = node->FindChild(name);
    return current != 0;
}
