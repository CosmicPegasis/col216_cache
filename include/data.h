#pragma once
#define CACHE_DELAY 1
#define MEM_DELAY 100
#define NUM_TRANSFER_BYTES 4

enum Instruction
{
    Load,
    Save
};

enum EvictionType
{
    LRU,
    FIFO
};
// TODO Fix main param prlong longer
enum WriteType
{
    WriteThrough,
    WriteBack,
};

struct CacheParameters
{
    long long sets;
    long long blocks;
    long long bytes_per_block;
    bool allocate_on_write;
    WriteType writeType;
    EvictionType evict;
};

struct MemReq
{
    Instruction ins;
    long long address;
};

struct CacheStatistics
{
    long long loads = 0;
    long long load_hits = 0;
    long long load_misses = 0;
    long long stores = 0;
    long long store_hits = 0;
    long long store_misses = 0;
    long long cycles = 0;
};
