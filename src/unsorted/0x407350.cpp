// Decompiled by Haiku. Names are provisional.

class Class_407350
{
public:
    virtual void method1();
    virtual void method2();
    virtual void method3();
    virtual void method4();
    virtual void method5();
    virtual void method6();
    virtual void method7();
    virtual void method8();
    virtual void method9();
    virtual void method10();
    virtual void method11();
    virtual void method12();
    virtual void method13();
    virtual void method14();

    int field_at_4;
    int field_at_8;
    int field_at_c;
    int field_at_10;

    Class_407350(int param1, int param2);
};

// FUNCTION: 0x407350
Class_407350::Class_407350(int param1, int param2)
{
    this->field_at_4 = param1;
    this->field_at_8 = param2;
    this->field_at_c = 0;
    unsigned char byte_val = *(unsigned char*)(param1 + 4);
    this->field_at_10 = byte_val;
}
