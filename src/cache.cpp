#include "cache.h"
#include "data.h"
void Cache::handle_write_back_load_hit()
{
    stats.cycles += CACHE_DELAY;
}
void Cache::handle_write_through_load_hit()
{
    stats.cycles += CACHE_DELAY;
}
void Cache::handle_write_back_load_miss(int set_num, std::set<int> &s)
{
    if (utils.is_full(s))
    {
        int to_evict = evictor->get_to_evict(set_num);
        if (dirty_blocks.find(to_evict) != dirty_blocks.end())
        {
            dirty_blocks.erase(dirty_blocks.find(to_evict));
            stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay() * 2;
        }
        else
        {

            stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay();
        }
    }
    else
    {
        stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay();
    }
}
void Cache::handle_write_through_load_miss()
{
    stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay();
}
void Cache::handle_write_back_save_hit(int block)
{
    stats.cycles += 2 * CACHE_DELAY;
    dirty_blocks.insert(block);
}
void Cache::handle_write_through_save_hit()
{
    stats.cycles += CACHE_DELAY;
}
void Cache::handle_write_through_save_miss()
{
    if (utils.alloc_on_write())
    {
        stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay() * 2;
    }
    else
    {
        stats.cycles += CACHE_DELAY + utils.get_mem_delay();
    }
}
void Cache::handle_write_back_save_miss(int block, int set_num, std::set<int> &s)
{
    /* stats.cycles += CACHE_DELAY; */
    if (utils.alloc_on_write())
    {
        if (utils.is_full(s))
        {
            int to_evict = evictor->get_to_evict(set_num);
            if (dirty_blocks.find(to_evict) != dirty_blocks.end())
            {
                dirty_blocks.erase(dirty_blocks.find(to_evict));
                stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay() * 2;
            }
            else
            {
                stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay();
            }
        }
        else
        {
            stats.cycles += CACHE_DELAY * 2 + utils.get_mem_delay();
        }
    }
    else
    {
        stats.cycles += CACHE_DELAY + utils.get_mem_delay();
    }
    dirty_blocks.insert(block);
}
void Cache::handle_load(int address)
{
    stats.loads++;
    int block = utils.get_block_num(address);
    int set_num = utils.get_set_num(address);
    std::set<int> &s = cache_matrix[set_num];
    if (s.find(block) != s.end())
    {
        stats.load_hits++;
        switch (utils.get_write_type())
        {
        case WriteBack: {
            handle_write_back_load_hit();
            break;
        }
        case WriteThrough: {
            handle_write_through_load_hit();
            break;
        }
        }
        evictor->touch(set_num, block);
    }
    else
    {
        stats.load_misses++;
        switch (utils.get_write_type())
        {
        case WriteBack: {
            handle_write_back_load_miss(set_num, s);
            break;
        }
        case WriteThrough: {
            handle_write_through_load_miss();
            break;
        }
        }
        insert(s, set_num, block);
    }
}
void Cache::insert(std::set<int> &s, int set_num, int block)
{
    if (utils.is_full(s))
    {
        int replaced = evictor->get_to_evict(set_num);
        evictor->erase(set_num);
        s.erase(s.find(replaced));
    }
    // Otherwise just bring whatever you want
    s.insert(block);
    evictor->insert(set_num, block);
}
void Cache::handle_store(int address)
{
    stats.stores++;
    int block = utils.get_block_num(address);
    int set_num = utils.get_set_num(address);
    std::set<int> &s = cache_matrix[set_num];
    if (s.find(block) != s.end())
    {
        stats.store_hits++;
        switch (utils.get_write_type())
        {
        case WriteThrough: {
            handle_write_through_save_hit();
            break;
        }
        case WriteBack: {
            handle_write_back_save_hit(block);
            break;
        }
        }
        evictor->touch(set_num, block);
    }
    else
    {
        stats.store_misses++;
        switch (utils.get_write_type())
        {
        case WriteThrough: {
            handle_write_through_save_miss();
            break;
        }
        case WriteBack: {
            handle_write_back_save_miss(block, set_num, s);
            break;
        }
        }
        if (utils.alloc_on_write())
        {
            insert(s, set_num, block);
        }
    }
}
Cache::Cache(CacheParameters params) : utils{params}
{
    if (params.evict == LRU)
    {
        evictor = std::make_unique<LRUEvictionHandler>(params.sets);
    }
    else
    {
        evictor = std::make_unique<FIFOEvictionHandler>(params.sets);
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
