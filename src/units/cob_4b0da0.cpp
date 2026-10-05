// Decompiled by GPT-6, finished by space-bunny-free, edited by deepseek-v4.1, finished by deepseek-v4.1-flash. Names are provisional.
// Body started by GPT-6, continued by space-bunny-free, edited by deepseek-v4.1.
// deepseek-v4.1-flash #2072: MATCH. The last hunk in case 0x10001000 was the
// packer swapping the pieces base and the move[] offset between [esp+0x10] and
// [esp+0x1c]. The fix is that `pieces` is one shared pointer variable declared
// in the do-block and reused by both 0x10001000 and 0x10002000 (`p = pieces;`
// then `p[piece]` in each), so the base is a single lifetime and lands in
// [esp+0x10]; before that, case 0x10001000 had its own `Piece *p;` inside the
// case block and the base ended up sharing the index's [esp+0x18]. Nothing else
// changed. Every earlier note below is history: the 99.6%/99.5% hunks and the
// shapes that failed are kept for reference, do not re-try them.
// deepseek-v4.1-flash: 99.6%. The 0x10059000 hunk is FIXED by moving the
// expression into an inline Channel::XorOp() (the inline boundary flips the
// pop registers to ecx/edx). One hunk remains, in case 0x10001000: the stack
// packer gives the move[] byte offset [esp+0x10] and reuses [esp+0x18] for the
// piece base, while the original gives the base [esp+0x10] and the offset
// [esp+0x1c] (the dword index piece*19+axis keeps [esp+0x18]). The instruction
// sequence is otherwise byte-identical, so it is a slot pick, not a code shape.
// Tried with no change: p only in the `if` (current), p declared first in the
// block, p at function scope, no p at all, an `int&` reference, wrapping the
// whole body in an inline method, and building the real function 0x4b0d60
// above this one. p before the stores and p for the stores both cost ~80%.
// The note below is the earlier attempt; its hunk 2 parts are now fixed.
// Partial was 99.5%. Two hunks remain (checked again in #1641 and #2072):
//   1. case 0x10001000 (0x4b0ebf, 0x4b0eec, 0x4b0ef3). The original keeps three
//      frame slots live across the call: [esp+0x10] = the piece array base,
//      [esp+0x18] = the dword index piece*19+axis, [esp+0x1c] = the move[]
//      byte offset. We reuse the now-dead index slot [esp+0x18] for the piece
//      array, so only two slots appear and the two live values swap offsets.
//      Declaring the `p` pointer local at function scope instead of inside the
//      case block (so its slot is reserved early) changes nothing: MSVC5 gives
//      an enregistered local its spill slot lazily, at its first spill point,
//      so declaration scope does not move it. Note [esp+0x14] is never touched
//      by the original even though case 0x10002000 needs the same three slots
//      in the same order, so the free-slot search skips it.
//   2. case 0x10059000 (0x4b1866). The original loads the first pop into ecx
//      and the second into edx before `xor ecx,edx`; we load them the other
//      way round. `Push(Pop() ^ Pop())` compiles identically to
//      `int a=Pop(); Push(a ^ Pop())`, so MSVC5 evaluates the right operand
//      of ^ first and hands it ecx; the xor destination is always the left
//      operand's register. Untried: two named locals with `Push(a ^ b)`
//      (both pops hoisted out of the expression), which is the only spelling
//      left that can give the first pop ecx. Tested in #2072 and it cannot:
//      `int a = c->Pop(); c->Push(a ^ c->Pop());` and the two-local spellings
//      `int a = c->Pop(); int b = c->Pop(); c->Push(b ^ a);` / `(a ^ b)` all
//      compile to the identical edx/ecx pair, so the pick is a function-wide
//      register allocation decision, not something the local source can steer.
//      Hunk 1 tested in #2072 as well: the frame slot pick survives moving the
//      `pieces[piece].move[axis] = c->Pop();` lines before `p = pieces;` (that
//      costs a lot, 99.5% -> 81.4%) and swapping the `Piece *p;` declaration
//      order (no change).
//      Also tried in #2072 (all kept 99.5% with the same two hunks, or worse):
//      `int a = Pop() ^ Pop(); Push(a);`, `int a = Pop(); a ^= Pop(); Push(a);`,
//      `int a = Pop(); int b = Pop(); a = a ^ b; Push(a);`,
//      `int a = Pop(); Push(Pop() ^ a);` for hunk 2; and `p = pieces;` moved
//      after the second Pop (99.2%) or `p[piece].moveSpeed[...]` (80.5%) for
//      hunk 1. Using the shared function-scope `value` as the XOR temp does
//      give the original's ecx-first pop order, but it re-colours the whole
//      function (86.2%), proof the pick is global allocation. A fresh
//      function-scope temp (`int t;` next to `value`) keeps 99.5% but does
//      not flip the order either.
// deepseek-v4.1-flash #2072 second pass (17 check.py runs, best stays 99.6%): the
// remaining hunk is only the packer swapping base and offset slots, and it is
// not reachable from the case body. Same 99.6% with the same hunk: no `p` at
// all (direct pieces[piece] in the if), `Piece *p = pieces;` at the assignment
// point instead of a forward declaration, an unused `int index = piece*19+axis;`
// local, the two *declarations* swapped (axis first), forward declarations with
// the assignments kept in place, `p` declared in the do-block before the
// switch, and `(p = pieces)[piece].move[axis]` inside the condition. Worse:
// `Piece *p = pieces;` before the stores (99.3%, base gets the third slot
// [esp+0x1c] and the packer stores it early), `int& movep =`
// pieces[piece].move[axis] (82.3%), `int* movep = &pieces[piece].move[axis]`
// (82.3%), `Piece *p = pieces + piece;` with p-> (81.4%), p used for both
// stores (81.6%), `Piece *p = pieces;` between the two stores (99.4%).
// The emitted instruction stream is identical apart from the slot numbers, so
// the pick is made by a global pass, not by the shape of this case.
#include <stdlib.h>

struct ScriptTable
{
    char pad[0x24];
    int *code;
};
struct Callback
{
    virtual void Complete(int) = 0;
};
struct Channel
{
    unsigned int state;
    int pc;
    int sp;
    int delay;
    int piece;
    int axis;
    int waiting;
    int signal;
    Callback *callback;
    int stack[32];
    int Pop()
    {
        return stack[sp--];
    }
    void Push(int value)
    {
        stack[++sp] = value;
    }
    void XorOp()
    {
        int a = Pop();
        Push(a ^ Pop());
    }
};
struct Piece
{
    int active;
    int move[3];
    int moveSpeed[3];
    int turn[3];
    int turnSpeed[3];
    int spin[3];
    int acceleration[3];
};
int __stdcall RandomInt(int);
class CobScript
{
  public:
    int scale;
    ScriptTable *table;
    int unknown_c;
    int *statics;
    Piece *pieces;
    int changed;
    Channel channels[8];
    int activeCount;
    virtual void SetPieceTranslation(int, int, int) = 0;
    virtual void SetPieceRotation(int, int, int) = 0;
    virtual void SetPieceVisible(int, int) = 0;
    virtual void SetPieceCached(int, int) = 0;
    virtual void SetPieceShaded(int, int) = 0;
    virtual int GetPieceTranslation(int, int) = 0;
    virtual int GetPieceRotation(int, int) = 0;
    virtual int IsPieceVisible(int);
    virtual int IsPieceCached(int);
    virtual int IsPieceShaded(int);
    virtual void FUN_004b1e80(int, int, int);
    virtual void FUN_004b1e90(int);
    virtual void EmitSfx(int, int);
    virtual void ExplodePiece(int, unsigned int);
    virtual void AttachUnit(unsigned short, int, int);
    virtual void DropUnit(unsigned short);
    virtual void SetUnitValue(int, int);
    virtual int GetUnitValue(int, int, int, int, int);
    virtual int IsCarryingUnit(int);
    virtual int GetTransporterId();
    void RunThread(unsigned int channel, int elapsed);
    void Wake(unsigned int index)
    {
        for (int i = 0; i < 8; i++)
            if ((channels[i].state & 0xfff00000) == 0x2800000 && channels[i].waiting == index)
                channels[i].state = 0x1000000;
    }
    int StartThread(int);
};

// FUNCTION: 0x4b0da0
void CobScript::RunThread(unsigned int channel, int elapsed)
{
    Channel *c = &channels[channel];
    if ((c->state & 0xff000000) == 0)
        return;
    if ((c->state & 0xff000000) == 0x2000000)
    {
        switch (c->state & 0xf00000)
        {
        case 0x100000:
            if (pieces[c->piece].turnSpeed[c->axis] == 0)
                c->state = 0x1000000;
            break;
        case 0x200000:
            if (pieces[c->piece].moveSpeed[c->axis] == 0)
                c->state = 0x1000000;
            break;
        case 0x400000:
            c->delay -= elapsed;
            if (c->delay <= 0)
                c->state = 0x1000000;
            break;
        }
    }
    if ((c->state & 0xff000000) == 0x1000000)
    {
        int running = 1;
        int value;
        int arguments[4];
        do
        {
            Piece *p;
            unsigned int opcode = table->code[c->pc];
            switch (opcode & 0x100ff000)
            {
            case 0x10001000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].move[axis] = c->Pop();
                pieces[piece].moveSpeed[axis] = c->Pop() / scale;
                p = pieces;
                if (p[piece].move[axis] < GetPieceTranslation(piece, axis))
                    p[piece].moveSpeed[axis] = -p[piece].moveSpeed[axis];
                pieces[piece].active = 1;
                changed = 1;
                c->pc += 3;
                break;
            }
            case 0x10002000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].turn[axis] = c->Pop() & 0xffff;
                pieces[piece].acceleration[axis] = 0;
                pieces[piece].turnSpeed[axis] = c->Pop() / scale;
                p = pieces;
                int delta = p[piece].turn[axis] - GetPieceRotation(piece, axis);
                if (delta == 0)
                    p[piece].turnSpeed[axis] = 0;
                else if ((abs(delta) > 0x8000) ^ (delta < 0))
                    p[piece].turnSpeed[axis] = -p[piece].turnSpeed[axis];
                pieces[piece].active = 1;
                changed = 1;
                c->pc += 3;
                break;
            }
            case 0x10003000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].turn[axis] = -1;
                pieces[piece].spin[axis] = c->Pop() / scale;
                pieces[piece].acceleration[axis] = c->Pop() / scale;
                if (pieces[piece].acceleration[axis] == 0)
                    pieces[piece].turnSpeed[axis] = pieces[piece].spin[axis];
                pieces[piece].active = 1;
                changed = 1;
                c->pc += 3;
                break;
            }
            case 0x10004000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].spin[axis] = 0;
                pieces[piece].acceleration[axis] = -(c->Pop() / scale);
                if (pieces[piece].acceleration[axis] == 0)
                    pieces[piece].turnSpeed[axis] = 0;
                c->pc += 3;
                break;
            }
            case 0x10005000: {
                SetPieceVisible(table->code[c->pc + 1], 1);
                c->pc += 2;
                break;
            }
            case 0x10006000: {
                SetPieceVisible(table->code[c->pc + 1], 0);
                c->pc += 2;
                break;
            }
            case 0x10007000: {
                SetPieceCached(table->code[c->pc + 1], 1);
                c->pc += 2;
                break;
            }
            case 0x10008000: {
                SetPieceCached(table->code[c->pc + 1], 0);
                c->pc += 2;
                break;
            }
            case 0x10009000: {
                int a = c->Pop();
                int b = c->Pop();
                FUN_004b1e80(table->code[c->pc + 1], b, a);
                c->pc += 2;
                break;
            }
            case 0x1000a000: {
                FUN_004b1e90(table->code[c->pc + 1]);
                c->pc += 2;
                break;
            }
            case 0x1000b000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].move[axis] = c->Pop();
                pieces[piece].moveSpeed[axis] = 0;
                SetPieceTranslation(piece, axis, pieces[piece].move[axis]);
                c->pc += 3;
                break;
            }
            case 0x1000c000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].turn[axis] = c->Pop() & 0xffff;
                pieces[piece].acceleration[axis] = 0;
                pieces[piece].turnSpeed[axis] = 0;
                SetPieceRotation(piece, axis, pieces[piece].turn[axis]);
                c->pc += 3;
                break;
            }
            case 0x1000d000: {
                SetPieceShaded(table->code[c->pc + 1], 1);
                c->pc += 2;
                break;
            }
            case 0x1000e000: {
                SetPieceShaded(table->code[c->pc + 1], 0);
                c->pc += 2;
                break;
            }
            case 0x1000f000: {
                int a = c->Pop();
                EmitSfx(table->code[c->pc + 1], a);
                c->pc += 2;
                break;
            }
            case 0x10011000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                c->state = 0x2100000;
                c->axis = axis;
                c->piece = piece;
                c->pc += 3;
                running = 0;
                break;
            }
            case 0x10012000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                c->state = 0x2200000;
                c->axis = axis;
                c->piece = piece;
                running = 0;
                c->pc += 3;
                break;
            }
            case 0x10013000: {
                c->state = 0x2400000;
                running = 0;
                c->delay = scale * c->Pop() / 1000;
                c->pc++;
                break;
            }
            case 0x10021000: {
                switch (opcode & 7)
                {
                case 1:
                    value = table->code[c->pc + 1];
                    break;
                case 2:
                    value = c->stack[table->code[c->pc + 1]];
                    break;
                case 4:
                    value = statics[table->code[c->pc + 1]];
                    break;
                }
                c->Push(value);
                c->pc += 2;
                break;
            }
            case 0x10022000: {
                c->sp++;
                c->pc++;
                break;
            }
            case 0x10023000: {
                switch (opcode & 7)
                {
                case 2: {
                    int a = c->Pop();
                    c->stack[table->code[c->pc + 1]] = a;
                    break;
                }
                case 4: {
                    int a = c->Pop();
                    statics[table->code[c->pc + 1]] = a;
                    break;
                }
                }
                c->pc += 2;
                break;
            }
            case 0x10024000: {
                c->sp--;
                c->pc++;
                break;
            }
            case 0x10031000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b + a);
                c->pc++;
                break;
            }
            case 0x10032000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b - a);
                c->pc++;
                break;
            }
            case 0x10033000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b * a);
                c->pc++;
                break;
            }
            case 0x10034000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b / a);
                c->pc++;
                break;
            }
            case 0x10035000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b & a);
                c->pc++;
                break;
            }
            case 0x10036000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b | a);
                c->pc++;
                break;
            }
            case 0x10037000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b ^ a);
                c->pc++;
                break;
            }
            case 0x10038000: {
                int a = c->Pop();
                c->Push(~a);
                c->pc++;
                break;
            }
            case 0x10041000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(RandomInt(a - b + 1) + b);
                c->pc++;
                break;
            }
            case 0x10042000: {
                int a = c->Pop();
                c->Push(GetUnitValue(a, 0, 0, 0, 0));
                c->pc++;
                break;
            }
            case 0x10043000: {
                int a = c->Pop();
                int b = c->Pop();
                int d = c->Pop();
                int e = c->Pop();
                int f = c->Pop();
                c->Push(GetUnitValue(f, e, d, b, a));
                c->pc++;
                break;
            }
            case 0x10044000: {
                int a = c->Pop();
                c->Push(IsCarryingUnit(a));
                c->pc++;
                break;
            }
            case 0x10045000: {
                c->Push(GetTransporterId());
                c->pc++;
                break;
            }
            case 0x10051000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b < a);
                c->pc++;
                break;
            }
            case 0x10052000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b <= a);
                c->pc++;
                break;
            }
            case 0x10053000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b > a);
                c->pc++;
                break;
            }
            case 0x10054000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b >= a);
                c->pc++;
                break;
            }
            case 0x10055000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b == a);
                c->pc++;
                break;
            }
            case 0x10056000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b != a);
                c->pc++;
                break;
            }
            case 0x10057000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b && a);
                c->pc++;
                break;
            }
            case 0x10058000: {
                int a = c->Pop();
                int b = c->Pop();
                c->Push(b || a);
                c->pc++;
                break;
            }
            case 0x10059000: {
                c->XorOp();
                c->pc++;
                break;
            }
            case 0x1005a000: {
                int a = c->Pop();
                c->Push(!a);
                c->pc++;
                break;
            }
            case 0x10061000: {
                int count = table->code[c->pc + 2];
                int child = ((CobScript *)this)->StartThread(table->code[c->pc + 1]);
                if (child >= 0)
                {
                    if (count > 0)
                    {
                        int *dest = &channels[child].stack[count];
                        int i = count;
                        do
                        {
                            --dest;
                            int a = c->Pop();
                            i--;
                            *dest = a;
                        } while (i);
                    }
                    channels[child].signal = c->signal;
                }
                c->pc += 3;
                break;
            }
            case 0x10062000: {
                int count = table->code[c->pc + 2];
                int child = ((CobScript *)this)->StartThread(table->code[c->pc + 1]);
                if (child >= 0)
                {
                    if (count > 0)
                    {
                        int *dest = &channels[child].stack[count];
                        int i = count;
                        do
                        {
                            --dest;
                            int a = c->Pop();
                            i--;
                            *dest = a;
                        } while (i);
                    }
                    channels[child].signal = c->signal;
                }
                c->waiting = child;
                c->state = 0x2800000;
                c->pc += 3;
                running = 0;
                break;
            }
            case 0x10063000: {
                int count = table->code[c->pc + 2];
                for (int i = count - 1; i >= 0; i--)
                    arguments[i] = c->Pop();
                c->pc += 3;
                break;
            }
            case 0x10064000: {
                c->pc = table->code[c->pc + 1];
                break;
            }
            case 0x10065000: {
                if (c->callback)
                    c->callback->Complete(c->Pop());
                c->state = 0;
                activeCount--;
                Wake(channel);
                running = 0;
                break;
            }
            case 0x10066000: {
                int a = c->Pop();
                if (a == 0)
                    c->pc = table->code[c->pc + 1];
                else
                    c->pc += 2;
                break;
            }
            case 0x10067000: {
                int mask = c->Pop();
                for (int i = 0; i < 8; i++)
                {
                    if (channels[i].state && (mask & channels[i].signal))
                    {
                        channels[i].state = 0;
                        activeCount--;
                        Wake(i);
                        if (i == channel)
                            running = 0;
                    }
                }
                c->pc++;
                break;
            }
            case 0x10068000: {
                c->signal = c->Pop();
                c->pc++;
                break;
            }
            case 0x10071000: {
                int a = c->Pop();
                ExplodePiece(table->code[c->pc + 1], a);
                c->pc += 2;
                break;
            }
            case 0x10082000: {
                int a = c->Pop();
                int b = c->Pop();
                SetUnitValue(b, a);
                c->pc++;
                break;
            }
            case 0x10083000: {
                int a = c->Pop();
                int b = c->Pop();
                int d = c->Pop();
                AttachUnit(d, b, a);
                c->pc++;
                break;
            }
            case 0x10084000: {
                DropUnit(c->Pop());
                c->pc++;
                break;
            }
            default:
                c->state = 0;
                running = 0;
                activeCount--;
                break;
            }
        } while (running);
    }
}
