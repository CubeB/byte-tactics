// Decompiled by Opus. Names are provisional.
// Probably std::pair<map::iterator, bool>::pair(const iterator&, const bool&),
// out of line: its one caller (0x46d2e0) is an inlined map::insert that
// builds the result from the node found (or inserted) and whether it is new.

struct Node_0046fad0;

class Class_0046fad0 {
public:
    Node_0046fad0* first;              // +0x0 (the iterator's node)
    bool second;                       // +0x4

    Class_0046fad0(Node_0046fad0* const& f, const bool& s);
};

// FUNCTION: 0x46fad0
Class_0046fad0::Class_0046fad0(Node_0046fad0* const& f, const bool& s)
    : first(f), second(s)
{
}
