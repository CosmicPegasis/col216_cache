#include "cache_util.h"
#include "data.h"
#include "eviction_handler.h"
#include "lru.h"
#include <queue>
#include <stdexcept>
#include <vector>

#pragma once
class LRUEvictionHandler : public EvictionHandler
{
  private:
    std::vector<LRUCache> l;

  public:
    LRUEvictionHandler(long long num_sets) : EvictionHandler(LRU)
    {
        l.resize(num_sets);
    }

    int get_size(int index)
    { // Override declaration
        return l[index].get_size();
    }

    int get_to_evict(int index)
    {
        return l[index].get_lru();
    }

    void insert(int index, int addr)
    {
        l[index].insert(addr);
    }

    void erase(int index)
    {
        l[index].erase();
    }

    void touch(int index, int addr)
    {
        l[index].insert(addr);
    }
};
class FIFOEvictionHandler : public EvictionHandler
{
  private:
    std::vector<std::queue<int>> q;

  public:
    FIFOEvictionHandler(long long num_sets) : EvictionHandler(FIFO)
    {
        q.resize(num_sets);
    }

    int get_size(int index)
    {
        return q[index].size();
    }

    int get_to_evict(int index)
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

    void insert(int index, int addr)
    {
        q[index].push(addr);
    }

    void erase(int index)
    {
        q[index].pop();
    }

    void touch(int index, int addr)
    {
        (void)index;
        (void)addr;
    } // No-op for FIFO
};

class Cache
{
    CacheStatistics stats;
    CacheUtil utils;
    EvictionHandler evictor;
    std::set<int> dirty_blocks;
    void handle_load(int address);
    void handle_store(int address);
    void insert(std::set<int> &s, int set_num, int block);
    bool has_block(int block);
    std::vector<std::set<int>> cache_matrix;

  public:
    Cache(CacheParameters params);
    void proc_request(MemReq req);
    CacheStatistics get_statistics();
};
