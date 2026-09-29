// Decompiled by GPT-6. Names are provisional.
// Partial (99.5%): the move opcode uses different temporary stack slots,
// and opcode 0x10059000 reverses the XOR operand registers. Size is exact.
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
class Class_004b08c0
{
  public:
    int FUN_004b08c0(int);
};
int __stdcall FUN_004b6c30(int);
class Class_004b0da0
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
    virtual void FUN_00480c50(int, int, int) = 0;
    virtual void FUN_00480ce0(int, int, int) = 0;
    virtual void FUN_00480d50(int, int) = 0;
    virtual void FUN_00480db0(int, int) = 0;
    virtual void FUN_00480df0(int, int) = 0;
    virtual int FUN_00480c30(int, int) = 0;
    virtual int FUN_00480cb0(int, int) = 0;
    virtual int FUN_004b1e50(int);
    virtual int FUN_004b1e60(int);
    virtual int FUN_004b1e70(int);
    virtual void FUN_004b1e80(int, int, int);
    virtual void FUN_004b1e90(int);
    virtual void FUN_004b1ea0(int, int);
    virtual void FUN_004b1eb0(int, int);
    virtual void FUN_004b0650(int, int, int);
    virtual void FUN_004b0660(int);
    virtual void FUN_004b0670(int, int);
    virtual int FUN_004b0680(int, int, int, int, int);
    virtual int FUN_004b0690(int);
    virtual int FUN_004b06a0();
    void FUN_004b0da0(unsigned int channel, int elapsed);
    void Wake(unsigned int index)
    {
        for (int i = 0; i < 8; i++)
            if ((channels[i].state & 0xfff00000) == 0x2800000 && channels[i].waiting == index)
                channels[i].state = 0x1000000;
    }
};

// FUNCTION: 0x4b0da0
void Class_004b0da0::FUN_004b0da0(unsigned int channel, int elapsed)
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
            unsigned int opcode = table->code[c->pc];
            switch (opcode & 0x100ff000)
            {
            case 0x10001000: {
                Piece *p;
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].move[axis] = c->Pop();
                pieces[piece].moveSpeed[axis] = c->Pop() / scale;
                p = pieces;
                if (p[piece].move[axis] < FUN_00480c30(piece, axis))
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
                Piece *p = pieces;
                int delta = p[piece].turn[axis] - FUN_00480cb0(piece, axis);
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
                FUN_00480d50(table->code[c->pc + 1], 1);
                c->pc += 2;
                break;
            }
            case 0x10006000: {
                FUN_00480d50(table->code[c->pc + 1], 0);
                c->pc += 2;
                break;
            }
            case 0x10007000: {
                FUN_00480db0(table->code[c->pc + 1], 1);
                c->pc += 2;
                break;
            }
            case 0x10008000: {
                FUN_00480db0(table->code[c->pc + 1], 0);
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
                FUN_00480c50(piece, axis, pieces[piece].move[axis]);
                c->pc += 3;
                break;
            }
            case 0x1000c000: {
                int piece = table->code[c->pc + 1];
                int axis = table->code[c->pc + 2];
                pieces[piece].turn[axis] = c->Pop() & 0xffff;
                pieces[piece].acceleration[axis] = 0;
                pieces[piece].turnSpeed[axis] = 0;
                FUN_00480ce0(piece, axis, pieces[piece].turn[axis]);
                c->pc += 3;
                break;
            }
            case 0x1000d000: {
                FUN_00480df0(table->code[c->pc + 1], 1);
                c->pc += 2;
                break;
            }
            case 0x1000e000: {
                FUN_00480df0(table->code[c->pc + 1], 0);
                c->pc += 2;
                break;
            }
            case 0x1000f000: {
                int a = c->Pop();
                FUN_004b1ea0(table->code[c->pc + 1], a);
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
                c->Push(FUN_004b6c30(a - b + 1) + b);
                c->pc++;
                break;
            }
            case 0x10042000: {
                int a = c->Pop();
                c->Push(FUN_004b0680(a, 0, 0, 0, 0));
                c->pc++;
                break;
            }
            case 0x10043000: {
                int a = c->Pop();
                int b = c->Pop();
                int d = c->Pop();
                int e = c->Pop();
                int f = c->Pop();
                c->Push(FUN_004b0680(f, e, d, b, a));
                c->pc++;
                break;
            }
            case 0x10044000: {
                int a = c->Pop();
                c->Push(FUN_004b0690(a));
                c->pc++;
                break;
            }
            case 0x10045000: {
                c->Push(FUN_004b06a0());
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
                int a = c->Pop();
                c->Push(a ^ c->Pop());
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
                int child = ((Class_004b08c0 *)this)->FUN_004b08c0(table->code[c->pc + 1]);
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
                int child = ((Class_004b08c0 *)this)->FUN_004b08c0(table->code[c->pc + 1]);
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
                FUN_004b1eb0(table->code[c->pc + 1], a);
                c->pc += 2;
                break;
            }
            case 0x10082000: {
                int a = c->Pop();
                int b = c->Pop();
                FUN_004b0670(b, a);
                c->pc++;
                break;
            }
            case 0x10083000: {
                int a = c->Pop();
                int b = c->Pop();
                int d = c->Pop();
                FUN_004b0650(d, b, a);
                c->pc++;
                break;
            }
            case 0x10084000: {
                FUN_004b0660(c->Pop());
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
