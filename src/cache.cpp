#include "cache.h"

#define CACHE_DELAY 1
#define MEM_DELAY 100

// struct CacheParameters
// {
//     int sets;
//     int blocks;
//     int bytes_per_block;
//     bool allocate_on_write;
//     WriteType writeType;
//     EvictionType evict;
// };
void Cache::handle_load(int address)
{
    // Query the cache
    stats.loads++;
    int block = utils.get_block_num(address);
    int set_num = utils.get_set_num(address);
    std::set<int> &s = cache_matrix[set_num];
    // Handle hits
    if (s.find(block) != s.end())
    {
        stats.load_hits++;
        // Delay added would be just the cache access time
        stats.cycles += CACHE_DELAY;
        // On hit just update the access time
        evictor.touch(set_num, block);
    }
    else
    {
        stats.load_misses++;
        // Handle misses
        stats.cycles += CACHE_DELAY;
        // Delay added would be the cache + memory access time
        stats.cycles += utils.get_mem_delay();
        // Fetch the address into the cache
        insert(s, set_num, block);
    }
}
void Cache::insert(std::set<int> &s, int set_num, int block)
{
    if (utils.is_full(s))
    {
        int block = evictor.get_to_evict(set_num);
        evictor.erase(set_num);
        s.erase(s.find(block));
    }
    // Otherwise just bring whatever you want
    s.insert(block);
    evictor.insert(set_num, block);
}
void Cache::handle_store(int address)
{
    (void)address;
}
Cache::Cache(CacheParameters params) : utils{params}
{
    if (params.evict == LRU)
    {
        evictor = LRUEvictionHandler(params.sets);
    }
    else
    {
        evictor = FIFOEvictionHandler(params.sets);
    }
    cache_matrix.resize(params.sets);
}
void Cache::proc_request(MemReq req)
{
    if (req.ins == Load)
        handle_load(req.address);
    else
        handle_store(req.address);
}
CacheStatistics Cache::get_statistics()
{
    return this->stats;
}
