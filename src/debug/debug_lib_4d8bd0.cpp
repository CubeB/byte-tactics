// Decompiled by Haiku. Names are provisional.

class BlockInfo {
public:
    int field_0;
    int field_4;
    int field_8;

    BlockInfo(void);
};

class TraceRecord {
public:
    char data[0x8c];
    TraceRecord(void);
};

class BlockHistory : public BlockInfo {
public:
    char unknown_c[0x24];
    TraceRecord member_30;
    TraceRecord member_bc;
    char unknown_148[0x1];

    BlockHistory(void);
};

// FUNCTION: 0x4d8bd0
BlockHistory::BlockHistory(void) :
    BlockInfo(),
    member_30(),
    member_bc()
{
    unknown_148[0] = 0;
}
