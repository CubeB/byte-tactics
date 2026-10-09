// TdfFile and TdfRecord as the rest of the game sees them: a TDF text file is
// loaded into a tree of sections (TdfRecord), and the file keeps a root and a
// current section. Callers select a section and read its `name = value;`
// entries by key. The layout of a section and the code that builds the tree
// stay in tdf.cpp; callers only hold pointers to a
// TdfRecord, so only its methods are declared here.
#ifndef TDF_H
#define TDF_H

class TdfRecord {
public:
    TdfRecord* FindSubRecord(const char* name);
    int GetFieldCount();
    int GetFieldInt(const char* name, int def);
    double GetFieldDouble(const char* name, double def);
    int GetFieldString(char* dst, char* key, unsigned int size, char* def);
    void CopyRecordName(char* dest, unsigned int count);
    int GetSubRecordCount();
    TdfRecord* GetSubRecord(int index);
};

class TdfFile {
public:
    TdfRecord* root;                   // +0x0
    TdfRecord* current;                // +0x4
    int field_8;                       // +0x8

    TdfFile();
    ~TdfFile();
    int LoadFile(char* path);
    void LoadBuffer(char* data, int size, int flag, char* path);
    void Unload();
    int SelectRecord(char* name);
    int SelectRecordAt(int index);
    void ResetCurrentRecord();
    int GetCurrentRecord();
    void SetCurrentRecord(int val);
};

#endif
