// Decompiled by GPT-5.6-Terra. Names are provisional.
// Partial, 81.6%: the remaining mismatch is the original's 0x18-byte local
// frame (ours is 0x10) and its ESI/EDI allocation in the bulk erase loop.
#include <yvals.h>

struct Node_004b2540 {
    Node_004b2540* left;
    Node_004b2540* parent;
    Node_004b2540* right;
    int key;
};

extern Node_004b2540* DAT_0051fbbc;
extern int DAT_0051fbcc;
extern Node_004b2540* __stdcall FUN_004b3370(Node_004b2540* x);

class Class_004b3490 {
public:
    Node_004b2540* FUN_004b3490(const int& key);
};

class Class_004b3430 {
public:
    Node_004b2540* FUN_004b3430(const int& key);
};

class Class_004b3590 {
public:
    Node_004b2540* ptr;
    void FUN_004b3590();
};

class Class_004b2fb0 {
public:
    struct iterator {
        Node_004b2540* ptr;
        iterator() {}
        iterator(Node_004b2540* p) : ptr(p) {}
        bool operator==(const iterator& x) const { return ptr == x.ptr; }
        bool operator!=(const iterator& x) const { return !(*this == x); }
        iterator& operator++()
        {
            std::_Lockit lock;
            if (ptr->right != DAT_0051fbbc)
                ptr = FUN_004b3370(ptr->right);
            else {
                Node_004b2540* parent = ptr->parent;
                if (ptr == parent->right) {
                    do {
                        ptr = parent;
                        parent = parent->parent;
                    } while (ptr == parent->right);
                }
                if (ptr->right != parent)
                    ptr = parent;
            }
            return *this;
        }
        iterator operator++(int)
        {
            iterator tmp = *this;
            ((Class_004b3590*)&ptr)->FUN_004b3590();
            return tmp;
        }
    };

    char allocator;
    char compare;
    Node_004b2540* head;
    char multi;
    int size;

    iterator begin() { return head->left; }
    iterator end() { return head; }
    iterator FUN_004b2ac0(iterator p);
    void FUN_004b2fb0(Node_004b2540* p);

    iterator erase(iterator first, iterator last)
    {
        if (size == 0 || first != begin() || last != end()) {
            while (first != last)
                FUN_004b2ac0(first++);
            return first;
        }
        else {
            std::_Lockit lock1;
            Node_004b2540* node = head->parent;
            {
                std::_Lockit lock2;
                while (node != DAT_0051fbbc) {
                    FUN_004b2fb0(node->right);
                    Node_004b2540* next = node->left;
                    operator delete(node);
                    node = next;
                }
            }
            head->parent = DAT_0051fbbc;
            size = 0;
            head->left = head;
            head->right = head;
            return begin();
        }
    }

    int erase(const int& key)
    {
        struct Range {
            iterator first;
            iterator last;
        } range;
        range.last = ((Class_004b3490*)this)->FUN_004b3490(key);
        range.first = ((Class_004b3430*)this)->FUN_004b3430(key);
        iterator last = range.last;
        iterator first = range.first;
        int count = 0;
        for (iterator current = first; current != last; ++current)
            ++count;
        erase(first, last);
        return count;
    }
};

static Class_004b2fb0 DAT_0051fbc0;
extern void __cdecl FUN_004d85a0(void* x);

// FUNCTION: 0x4b2540
void __stdcall FUN_004b2540(int key)
{
    if (key != 0) {
        int local_key = key;
        DAT_0051fbc0.erase(local_key);
        FUN_004d85a0((void*)key);
    }
}
