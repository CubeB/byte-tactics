// Decompiled by Opus. Names are provisional.
// Memory cache initialisation: frees any previous block (as 0x437a00 does),
// allocates a "CMemoryCache CCH" block of the given size and makes it one
// free chunk covering the whole block.

void FUN_004d85a0(int* param_1);
void* FUN_004d83b0(char* name, unsigned int size);

struct Chunk_004379b0 {
    Chunk_004379b0* next;              // +0x0
    unsigned int size;                 // +0x4
};

class Class_004379b0 {
public:
    unsigned int size;                 // +0x0
    int* block;                        // +0x4
    Chunk_004379b0* free;              // +0x8

    int FUN_004379b0(unsigned int size);
};

// FUNCTION: 0x4379b0
int Class_004379b0::FUN_004379b0(unsigned int newSize)
{
    if (block != 0) {
        FUN_004d85a0(block);
        block = 0;
    }
    Chunk_004379b0* chunk = (Chunk_004379b0*)FUN_004d83b0("CMemoryCache CCH", newSize);
    block = (int*)chunk;
    size = newSize;
    free = chunk;
    chunk->next = 0;
    free->size = size;
    return 1;
}
