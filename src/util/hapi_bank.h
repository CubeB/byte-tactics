// HapiBank: a bank of named accounts, each holding named items (integers,
// doubles and strings) and safe-deposit boxes, saved to and loaded from a
// HAPIBANK file. The one declaration of the class, for hapi_bank.cpp and every
// file that calls it; the types behind the pointers stay private to
// hapi_bank.cpp.
#ifndef HAPI_BANK_H
#define HAPI_BANK_H

struct AccountList;
struct StringPool;
struct FileHandle;
struct _iobuf;
typedef struct _iobuf FILE;

class HapiBank {
public:
    AccountList* bank;               // +0x00

    HapiBank* InitBank();
    void CloseBank();
    void NewBank();
    int OpenBank(char* filename, char* name, char* account);
    int SaveBank(char* filename, char* name, int compress, int audit);
    void SaveAccount(int index, FILE* file, StringPool* pool, int compress);
    void LoadAccount(FileHandle* file, int* image, char* name);
    int OpenAccount(char* name);
    int SetIntegerItem(const char* name, int value);
    int SetDoubleItem(const char* name, double value);
    int SetStringItem(const char* name, char* value);
    int GetIntegerItem(char* name, int def);
    double GetDoubleItem(char* name, double def);
    char* GetStringItem(char* name, char* def);
    int HasItem(const char* name);
    int FindItem(const char* name, int create);
    int FindNumberedBox(int number, int create);
    int FindNamedBox(char* name, int create);
    int OpenNumberedBox(int number);
    int OpenNamedBox(char* name);
    int GetBoxSize();
    void SeekBox(int pos);
    int SeekBoxEnd();
    int ReadBox(void* dst, int len);
    int WriteBox(void* src, int len);
    void WriteAuditFile(char* filename);
};

#endif
