// Decompiled by Sonnet, Haiku, Opus, deepseek-v4.1-flash, GPT-6, space-bunny-free, deepseek-v4.1, GPT-5.6-Terra, LongCat 2.5 Preview Free, GPT-6.1-sol, mimo-v2.6-pro, claude-sonnet-5-5 and Space Bunny Free. Names are provisional.
// CobScript: the interpreter of a unit's COB script, an abstract base class
// of 0x540 bytes (vtable 0x4fdb00, 21 slots). Slots 0-6 are pure; each is
// named after the one override that fills it, in the vtable of the only
// derived class, UnitScript (src/units/unit_script.cpp). Slot 20 is the
// virtual destructor (the vtable holds its scalar deleting destructor).

#include <string.h>
#include <stdlib.h>

// The compiled COB script of a unit type.
struct ScriptTable {
    int unknown_0;                     // +0x00
    int count;                         // +0x04, of the scripts
    int pieceCount;                    // +0x08
    int unknown_c;                     // +0x0c
    int staticCount;                   // +0x10
    char unknown_14[0x18 - 0x14];
    int* entries;                      // +0x18, each script's code offset
    char** names;                      // +0x1c
    char unknown_20[0x24 - 0x20];
    int* code;                         // +0x24
};

// Told when a script started with one finishes.
struct Callback
{
    virtual void Complete(int) = 0;
};

// One script thread.
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

// 0x4b0c40's view of a channel. Its channel array starts at offset 0, so its
// fields sit 0x1c after the real Channel's: `count` is the real `sp`, `values`
// the real `stack` and `unknown_3c` the real `callback`. The function does not
// use activeCount, so the view's offset does not matter.
struct Channel_004b0c40 {
    char unknown_0[0x24];
    int count;                         // +0x24
    char unknown_28[0x3c - 0x28];
    int unknown_3c;                    // +0x3c
    int values[5];                     // +0x40
    char unknown_54[0xa4 - 0x54];
};

// The script's state of one piece: where it is moving and turning to.
// `e` is 0x4b1c00's view of the same record, e[0..5] being move, moveSpeed,
// turn, turnSpeed, spin and acceleration.
struct Piece
{
    int active;
    union {
        struct {
            int move[3];
            int moveSpeed[3];
            int turn[3];
            int turnSpeed[3];
            int spin[3];
            int acceleration[3];
        };
        int e[6][3];
    };
};

// The saved state of a script (0x528 bytes) and of one piece (0x6c bytes).
struct SavedScript {
    int checksum;
    Channel channels[8];
    int activeCount;
};

struct SavedPiece {
    int move[3];
    int moveSpeed[3];
    int turn[3];
    int turnSpeed[3];
    int spin[3];
    int acceleration[3];
    int translation[3];
    int rotation[3];
    int visible;
    int cached;
    int shaded;
};

#include "../util/hapi_bank.h"

extern int GetTickRate();
void __cdecl GameFreeThunk(void* p);
int __stdcall GetCobChecksum(void* param);
void* __cdecl GameReallocTagged(void* param_1, const char* name, unsigned int param_3);
char* __cdecl GameAllocIgnoreTag(const char* text, int value);
int __stdcall RandomInt(int);

class CobScript
{
  public:
    int scale;                         // +0x004
    ScriptTable *table;                // +0x008
    int unknown_c;                     // +0x00c, the script's checksum
    int *statics;                      // +0x010
    Piece *pieces;                     // +0x014
    int changed;                       // +0x018, a piece is still moving
    Channel channels[8];               // +0x01c
    int activeCount;                   // +0x53c

    CobScript();

    virtual void SetPieceTranslation(int, int, int) = 0;  // slot 0
    virtual void SetPieceRotation(int, int, int) = 0;  // slot 1
    virtual void SetPieceVisible(int, int) = 0;       // slot 2
    virtual void SetPieceCached(int, int) = 0;        // slot 3
    virtual void SetPieceShaded(int, int) = 0;        // slot 4
    virtual int GetPieceTranslation(int, int) = 0;    // slot 5
    virtual int GetPieceRotation(int, int) = 0;       // slot 6
    virtual int IsPieceVisible(int);                  // slot 7
    virtual int IsPieceCached(int);                   // slot 8
    virtual int IsPieceShaded(int);                   // slot 9
    virtual void ExplodeLegacy(int, int, int);        // slot 10
    virtual void PlaySoundNoop(int);                  // slot 11
    virtual void EmitSfx(int, int);                   // slot 12
    virtual void ExplodePiece(int, unsigned int);     // slot 13
    virtual void AttachUnit(unsigned short, int, int); // slot 14
    virtual void DropUnit(unsigned short);            // slot 15
    virtual void SetUnitValue(int, int);              // slot 16
    virtual int GetUnitValue(int, int, int, int, int); // slot 17
    virtual int IsCarryingUnit(int);                  // slot 18
    virtual int GetTransporterId();                   // slot 19
    virtual ~CobScript();                             // slot 20

    void SetCob(ScriptTable* data);
    ScriptTable* GetCob();
    int FUN_004b07b0(int, int);
    int FindScript(const char* name);
    int StartThreadByName(const char* name);
    int StartThread(int id);
    int StartScript(const char* name, Callback* callback, int update);
    int StartScriptByIndex(int id, Callback* callback, int update);
    int StartScriptWithArgs(char* name, Callback* callback, int update, int count, int a, int b, int c, int d);
    int StartScriptWithArgsByIndex(int index, Callback* callback, int update, int count, int a, int b, int c, int d);
    int QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5);
    // 0x4b0c40: it matches only through the Channel_004b0c40 view, addressing
    // the channels from `this + i * 0xa4`.
    int QueryScriptByIndex(int index, int* p2, int* p3, int* p4, int* p5);
    void RemoveCallback(Callback* callback);
    void RunScripts(int param_1);
    void RunThread(unsigned int channel, int elapsed);
    void Wake(unsigned int index)
    {
        for (int i = 0; i < 8; i++)
            if ((channels[i].state & 0xfff00000) == 0x2800000 && channels[i].waiting == index)
                channels[i].state = 0x1000000;
    }
    // 0x4b1c00: it matches only indexing the piece record through Piece's
    // e[6][3] view (see the note at its definition).
    void AnimatePieces(int param_1);
    void SaveScriptState(HapiBank* file);
    int LoadScriptState(HapiBank* file);
};

// FUNCTION: 0x4b0610
CobScript::CobScript()
{
    table = 0;
    pieces = 0;
    statics = 0;
    for (int i = 0; i < 8; i++) {
        channels[i].state = 0;
    }
    activeCount = 0;
    scale = GetTickRate();
}

// FUNCTION: 0x4b0650
void CobScript::AttachUnit(unsigned short, int, int)
{
}

// FUNCTION: 0x4b0660
void CobScript::DropUnit(unsigned short)
{
}

// FUNCTION: 0x4b0670
void CobScript::SetUnitValue(int, int)
{
}

// FUNCTION: 0x4b0680
int CobScript::GetUnitValue(int, int, int, int, int)
{
    return 0;
}

// FUNCTION: 0x4b0690
int CobScript::IsCarryingUnit(int)
{
    return 0;
}

// FUNCTION: 0x4b06a0
int CobScript::GetTransporterId()
{
    return 0;
}

// The compiler-generated scalar deleting destructor (0x4b06b0, vtable slot
// 20) comes from the destructor definition below, with its body inlined.
// FUNCTION: 0x4b06b0 ??_GCobScript@@UAEPAXI@Z
// FUNCTION: 0x4b06f0
CobScript::~CobScript()
{
    if (pieces) {
        GameFreeThunk((int*)pieces);
    }
    if (statics) {
        GameFreeThunk((int*)statics);
    }
}

// SetCob attaches the
// object's state data: it stores the data block, looks up its runtime state,
// reallocates the "Object States" and "Static Varibles" tables and clears the
// state table. Called from 0x485d40 with unit->type->field_18e.
// FUNCTION: 0x4b0720
void CobScript::SetCob(ScriptTable* data)
{
    table = data;
    if (data != 0) {
        unknown_c = GetCobChecksum(data);
        pieces = (Piece*)GameReallocTagged(pieces, "Object States", data->pieceCount * 0x4c);
        statics = (int*)GameReallocTagged(statics, "Static Varibles", data->staticCount * 4);
        memset(pieces, 0, data->pieceCount * 0x4c);
    }
}

// FUNCTION: 0x4b07a0
ScriptTable* CobScript::GetCob()
{
    return table;
}

// FUNCTION: 0x4b07b0
int CobScript::FUN_004b07b0(int, int)
{
    return 0;
}

// Looks a name up in the table at +8 and claims a channel slot for its index
// (StartThread, see 0x4b0a10.cpp); -1 when there is no table.
// The name lookup is defined as a member here, not a static inline helper, so it is inlined.
// FUNCTION: 0x4b07c0
int CobScript::FindScript(const char* name)
{
    for (int i = 0; i < table->count; i++) {
        if (strcmp(name, table->names[i]) == 0) {
            return i;
        }
    }
    return -1;
}

// FUNCTION: 0x4b0830
int CobScript::StartThreadByName(const char* name)
{
    if (table == 0) {
        return -1;
    }
    return StartThread(FindScript(name));
}

// Claims the first free one of 8 channel slots for entry `id` of the table
// at +8 and returns its index; -1 when `id` is out of range or no slot is
// free (see 0x4b0830.cpp and 0x4b0a10.cpp).
// FUNCTION: 0x4b08c0
int CobScript::StartThread(int id)
{
    if (id < 0 || id >= table->count)
        return -1;
    for (int i = 0; i < 8; i++) {
        if (channels[i].state == 0) {
            channels[i].state = 0x1000000;
            channels[i].pc = table->entries[id];
            channels[i].sp = -1;
            channels[i].callback = 0;
            channels[i].signal = 1;
            activeCount++;
            return i;
        }
    }
    return -1;
}

// Looks a name up in the table at +8 (the lookup at 0x4b07c0, inlined here),
// claims a channel slot for its index with 0x4b08c0, stores the value and
// refreshes the channels when asked. Compare 0x4b0a10, its index-taking twin.
// FUNCTION: 0x4b0940
int CobScript::StartScript(const char* name, Callback* callback, int update)
{
    int i = StartThread(FindScript(name));
    if (i < 0)
        return 0;
    channels[i].callback = callback;
    if (update) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                RunThread(j, 0);
        }
        AnimatePieces(0);
    }
    return 1;
}

// StartThread claims one of the 8 channel slots for an id and returns its
// index (or -1); this one then stores a value in the new slot.
// FUNCTION: 0x4b0a10
int CobScript::StartScriptByIndex(int id, Callback* callback, int update)
{
    int i = StartThread(id);
    if (i < 0)
        return 0;
    channels[i].callback = callback;
    if (update) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                RunThread(j, 0);
        }
        AnimatePieces(0);
    }
    return 1;
}

// FUNCTION: 0x4b0a70
int CobScript::StartScriptWithArgs(char* name, Callback* callback, int update, int count, int a, int b, int c, int d)
{
    return StartScriptWithArgsByIndex(FindScript(name), callback, update, count, a, b, c, d);
}

// Index-taking twin of 0x4b0a70 (the name lookup version): StartThread claims
// one of the 8 channel slots, this stores the value plus a four-deep call
// frame on the slot's own stack, truncates the slot's stack pointer to
// param_4 - 1 and refreshes the channels when asked. Compare 0x4b0a10 (same
// shape without the frame) and 0x4b0c40.
//
// The channel array is viewed from `this + i * 0xa4` with the fields at
// +0x24 (stack pointer, starts at -1), +0x3c (value) and +0x40 (stack). The
// real 0x1c-byte channel header sits 0x1c before this view, which is why the
// array is declared at offset 0 here; the offsets are what the code reads.
// FUNCTION: 0x4b0b00
int CobScript::StartScriptWithArgsByIndex(int index, Callback* callback, int update,
                                           int count, int a, int b, int c, int d)
{
    int i = StartThread(index);
    if (i < 0) {
        if (callback)
            callback->Complete(0);
        return 0;
    }
    Channel* ch = &channels[i];
    ch->callback = callback;
    ch->stack[++ch->sp] = a;
    ch->stack[++ch->sp] = b;
    ch->stack[++ch->sp] = c;
    ch->stack[++ch->sp] = d;
    // Spelled through channels[i] (not ch) so MSVC keeps the fourth push's
    // increment instead of folding it into the array store.
    channels[i].sp = count - 1;
    if (update) {
        if (activeCount) {
            for (int j = 0; j < 8; j++)
                RunThread(j, 0);
        }
        AnimatePieces(0);
    }
    return 1;
}

// FUNCTION: 0x4b0bc0
int CobScript::QueryScript(char* name, int* param_2, int* param_3, int* param_4, int* param_5)
{
    return QueryScriptByIndex(FindScript(name), param_2, param_3, param_4, param_5);
}

// Index-taking core of 0x4b0bc0: claims a channel for `index` (0x4b08c0),
// pushes the four pointed-to values (0 when the pointer is null) onto the
// channel's value list, processes the channel with 0x4b0da0, then writes the
// four processed values back through the pointers. Compare 0x4b0bc0, its
// name-taking wrapper.
//
// The channel is viewed from `this + index * 0xa4` with the fields at +0x24
// (the real `sp`, starts at -1), +0x3c (the real `callback`) and +0x40 (the
// real `stack`), the offsets 0x1c past the real Channel.
// FUNCTION: 0x4b0c40
int CobScript::QueryScriptByIndex(int index, int* p2, int* p3, int* p4, int* p5)
{
    int i = this->StartThread(index);
    if (i < 0)
        return 0;
    // Not the real channels[i]: that folds the 0x1c base into every field offset.
    Channel_004b0c40* c = (Channel_004b0c40*)((char*)this + i * 0xa4);
    c->unknown_3c = 0;
    // The reference keeps the count store in the fourth push; ++c->count drops it.
    int& cnt = c->count;
    c->values[++cnt] = p2 ? *p2 : 0;
    c->values[++cnt] = p3 ? *p3 : 0;
    c->values[++cnt] = p4 ? *p4 : 0;
    c->values[++cnt] = p5 ? *p5 : 0;
    c->count = 3;
    this->RunThread(i, 0);
    if (p2) *p2 = c->values[0];
    if (p3) *p3 = c->values[1];
    if (p4) *p4 = c->values[2];
    if (p5) *p5 = c->values[3];
    return 1;
}

// FUNCTION: 0x4b0d20
void CobScript::RemoveCallback(Callback* callback)
{
    if (activeCount != 0) {
        for (int i = 0; i < 8; i++) {
            if ((channels[i].state & 0xff000000) != 0 && channels[i].callback == callback) {
                channels[i].callback = 0;
            }
        }
    }
}

// Updates every channel with the value (compare the tail of 0x4b0a10).
// FUNCTION: 0x4b0d60
void CobScript::RunScripts(int param_1)
{
    if (activeCount) {
        for (int j = 0; j < 8; j++)
            RunThread(j, param_1);
    }
    AnimatePieces(param_1);
}

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
            // One shared p, declared here and used by both the move and turn cases.
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
                ExplodeLegacy(table->code[c->pc + 1], b, a);
                c->pc += 2;
                break;
            }
            case 0x1000a000: {
                PlaySoundNoop(table->code[c->pc + 1]);
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
                // Through the inline XorOp(): the inline boundary sets the pop registers.
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
                int child = this->StartThread(table->code[c->pc + 1]);
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
                int child = this->StartThread(table->code[c->pc + 1]);
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

// Advance every moving piece by param_1 (a percentage) of its per-axis speed:
// e[1] is the translation speed toward the e[0] limit, e[5] the turn speed with the
// e[4] limit, e[3] the current angle, e[2] the wanted angle (-1 means none). Each
// element that still moves leaves its record's active flag set, and any such flag
// keeps `changed` (the "something is still animating" flag) at 1.
// FUNCTION: 0x4b1c00
void CobScript::AnimatePieces(int param_1)
{
    if (param_1 == 0)
        return;
    if (changed == 0)
        return;
    changed = 0;
    // Piece records are indexed through the e[6][3] view, not the named arrays.
    for (int i = 0; i < table->pieceCount; i++) {
        if (pieces[i].active != 0) {
            pieces[i].active = 0;
            for (int j = 0; j <= 2; j++) {
                if (pieces[i].e[1][j] != 0) {
                    int v = GetPieceTranslation(i, j);
                    v += param_1 * pieces[i].e[1][j];
                    if (pieces[i].e[1][j] > 0) {
                        if (v >= pieces[i].e[0][j]) {
                            v = pieces[i].e[0][j];
                            pieces[i].e[1][j] = 0;
                        } else
                            pieces[i].active = 1;
                    } else {
                        if (v <= pieces[i].e[0][j]) {
                            v = pieces[i].e[0][j];
                            pieces[i].e[1][j] = 0;
                        } else
                            pieces[i].active = 1;
                    }
                    SetPieceTranslation(i, j, v);
                }
                if (pieces[i].e[5][j] != 0) {
                    pieces[i].e[3][j] += pieces[i].e[5][j];
                    if (pieces[i].e[5][j] > 0) {
                        if (pieces[i].e[3][j] >= pieces[i].e[4][j]) {
                            pieces[i].e[3][j] = pieces[i].e[4][j];
                            pieces[i].e[5][j] = 0;
                        }
                    } else {
                        if (pieces[i].e[3][j] <= pieces[i].e[4][j]) {
                            pieces[i].e[3][j] = pieces[i].e[4][j];
                            pieces[i].e[5][j] = 0;
                        }
                    }
                }
                if (pieces[i].e[3][j] != 0) {
                    int r = GetPieceRotation(i, j);
                    int cur = r;
                    int step = param_1 * pieces[i].e[3][j];
                    r += step;
                    int want = pieces[i].e[2][j];
                    if (want != -1) {
                        if (pieces[i].e[3][j] > 0) {
                            if ((want - cur + 0x10000) % 0x10000 <= step) {
                                r = want;
                                pieces[i].e[3][j] = 0;
                            } else
                                pieces[i].active = 1;
                        } else {
                            if ((cur - want + 0x10000) % 0x10000 <= -step) {
                                r = want;
                                pieces[i].e[3][j] = 0;
                            } else
                                pieces[i].active = 1;
                        }
                    } else
                        pieces[i].active = 1;
                    SetPieceRotation(i, j, r & 0xffff);
                }
            }
            if (pieces[i].active != 0)
                changed = 1;
        }
    }
}

// Vtable slots 7-13 of CobScript.
// FUNCTION: 0x4b1e50
int CobScript::IsPieceVisible(int)
{
    return 0;
}

// FUNCTION: 0x4b1e60
int CobScript::IsPieceCached(int)
{
    return 0;
}

// FUNCTION: 0x4b1e70
int CobScript::IsPieceShaded(int)
{
    return 0;
}

// FUNCTION: 0x4b1e80
void CobScript::ExplodeLegacy(int, int, int)
{
}

// FUNCTION: 0x4b1e90
void CobScript::PlaySoundNoop(int)
{
}

// FUNCTION: 0x4b1ea0
void CobScript::EmitSfx(int, int)
{
}

// FUNCTION: 0x4b1eb0
void CobScript::ExplodePiece(int, unsigned int)
{
}

// Save method of CobScript (the object at Unit+0x9a), the counterpart of
// the loader 0x4b2040. Writes the eight 0xa4-byte records at +0x1c, the block
// at ptr10, then one 0x6c-byte record per element of the array at ptr14.
// FUNCTION: 0x4b1ec0
void CobScript::SaveScriptState(HapiBank* file)
{
    file->SeekBox(0);

    SavedScript big;
    big.checksum = unknown_c;
    for (int n = 0; n < 8; n++) {
        big.channels[n] = channels[n];
        big.channels[n].callback = 0;
    }
    big.activeCount = activeCount;
    file->WriteBox(&big, 0x528);
    file->WriteBox(statics, table->staticCount * 4);

    for (int i = 0; i < table->pieceCount; i++) {
        SavedPiece rec;
        rec.visible = IsPieceVisible(i);
        rec.cached = IsPieceCached(i);
        rec.shaded = IsPieceShaded(i);
        for (int j = 0; j <= 2; j++) {
            rec.move[j] = pieces[i].move[j];
            rec.moveSpeed[j] = pieces[i].moveSpeed[j];
            rec.turn[j] = pieces[i].turn[j];
            rec.turnSpeed[j] = pieces[i].turnSpeed[j];
            rec.spin[j] = pieces[i].spin[j];
            rec.acceleration[j] = pieces[i].acceleration[j];
            rec.translation[j] = GetPieceTranslation(i, j);
            rec.rotation[j] = GetPieceRotation(i, j);
        }
        file->WriteBox(&rec, 0x6c);
    }
}

// The record is the plain 0x6c struct (int e[6][3] at 0, a[3] at 0x48, b[3] at
// 0x54, h[3] at 0x60). The three virtual calls before the j loop read the
// record's h[3] at 0x60; the two in the j loop read a[j] at 0x48 and b[j] at 0x54.
// FUNCTION: 0x4b2040
int CobScript::LoadScriptState(HapiBank* file)
{
    int size = table->staticCount * 4;
    int bytes = table->pieceCount * 0x6c;
    if (file->GetBoxSize() != bytes + 0x528 + size) {
        return 0;
    }
    file->SeekBox(0);
    SavedScript big;
    if (file->ReadBox(&big, 0x528) != 0x528) {
        return 0;
    }
    if (unknown_c != big.checksum) {
        return 0;
    }
    for (int n = 0; n < 8; n++) {
        channels[n] = big.channels[n];
        channels[n].callback = 0;
    }
    activeCount = big.activeCount;
    if (file->ReadBox(statics, size) != size) {
        return 0;
    }
    SavedPiece* buffer = (SavedPiece*)GameAllocIgnoreTag("Piece States", bytes);
    if (file->ReadBox(buffer, bytes) != bytes) {
        return 0;
    }
    // Walk the buffer by index (buffer[i]), not with a moving pointer.
    for (int i = 0; i < table->pieceCount; i++) {
        pieces[i].active = 1;
        SetPieceVisible(i, buffer[i].visible);
        SetPieceCached(i, buffer[i].cached);
        SetPieceShaded(i, buffer[i].shaded);
        for (int j = 0; j <= 2; j++) {
            pieces[i].move[j] = buffer[i].move[j];
            pieces[i].moveSpeed[j] = buffer[i].moveSpeed[j];
            pieces[i].turn[j] = buffer[i].turn[j];
            pieces[i].turnSpeed[j] = buffer[i].turnSpeed[j];
            pieces[i].spin[j] = buffer[i].spin[j];
            pieces[i].acceleration[j] = buffer[i].acceleration[j];
            SetPieceTranslation(i, j, buffer[i].translation[j]);
            SetPieceRotation(i, j, buffer[i].rotation[j]);
        }
    }
    changed = 1;
    GameFreeThunk(buffer);
    return 1;
}
