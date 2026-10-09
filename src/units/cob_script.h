// CobScript: the interpreter of a unit's COB script, an abstract base class of
// 0x540 bytes (vtable 0x4fdb00, 21 slots) reached through Unit+0x9a. Slots
// 0-6 are pure and filled by the one derived class, UnitScript; slot 20 is the
// virtual destructor. The one declaration of the class, for cob.cpp, which
// defines the methods, and every file that starts or queries a script. The
// script table, the piece records and the callback stay private to cob.cpp.
#ifndef COB_SCRIPT_H
#define COB_SCRIPT_H

struct ScriptTable;
struct Piece;
struct Callback;
class HapiBank;

// One script thread (0xa4 bytes): an operand stack of 32 words.
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

#endif
