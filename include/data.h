#pragma once
enum Instruction
{
    Load,
    Save
};

enum EvicitionType
{
    LRU,
    FIFO
};
enum WriteType
{
    WriteThrough,
    WriteAllocate
};

struct CacheParameters
{
    int sets;
    int blocks;
    int bytes_per_block;
    bool allocate_on_write;
    WriteType writeType;
    EvicitionType evict;
};

struct MemReq
{
    Instruction ins;
    int address;
};
