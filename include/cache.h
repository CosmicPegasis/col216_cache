#include "cache_util.h"
#include "data.h"
#include "eviction_handler.h"
#include "lru.h"
#include <memory>
#include <queue>
#include <stdexcept>
#include <vector>

#pragma once
class LRUEvictionHandler : public EvictionHandler
{
  private:
    std::vector<LRUCache> l;

  public:
    LRUEvictionHandler(long long num_sets)
    {
        l.resize(num_sets);
    }

    long long get_size(long long index)
    { // Override declaration
        return l[index].get_size();
    }

    long long get_to_evict(long long index)
    {
        return l[index].get_lru();
    }

    void insert(long long index, long long addr)
    {
        l[index].insert(addr);
    }

    void erase(long long index)
    {
        l[index].erase();
    }

    void touch(long long index, long long addr)
    {
        l[index].insert(addr);
    }
};
class FIFOEvictionHandler : public EvictionHandler
{
  private:
    std::vector<std::queue<long long>> q;

  public:
    FIFOEvictionHandler(long long num_sets)
    {
        q.resize(num_sets);
    }

    long long get_size(long long index)
    {
        return q[index].size();
    }

    long long get_to_evict(long long index)
    {
        if (get_size(index))
        {
            return q[index].front();
        }
        else
        {
            throw std::runtime_error("FIFO is empty");
        }
    }

    void insert(long long index, long long addr)
    {
        q[index].push(addr);
    }

    void erase(long long index)
    {
        q[index].pop();
    }

    void touch(long long index, long long addr)
    {
        (void)index;
        (void)addr;
    } // No-op for FIFO
};

class Cache
{
    CacheStatistics stats;
    CacheUtil utils;
    std::unique_ptr<EvictionHandler> evictor;
    std::set<long long> dirty_blocks;
    void handle_load(long long address);
    void handle_store(long long address);
    void insert(std::set<long long> &s, long long set_num, long long block);
    bool has_block(long long block);
    void handle_write_back_load_miss(long long set_num, std::set<long long> &s);
    void handle_write_through_load_miss();
    void handle_write_back_save_miss(long long block, long long set_num, std::set<long long> &s);
    void handle_write_through_save_miss();
    void handle_write_back_load_hit();
    void handle_write_through_load_hit();
    void handle_write_back_save_hit(long long block);
    void handle_write_through_save_hit();

  public:
    Cache(CacheParameters params);
    void proc_request(MemReq req);
    CacheStatistics get_statistics();
    std::vector<std::set<long long>> cache_matrix;
};
